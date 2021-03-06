// PlantillaOpenGL.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "Shader.h"
#include "Vertice.h"
#include "Modelo.h"
#include "Carro.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

//Declaramos apuntador de ventana
GLFWwindow *window;

GLfloat anchoPantalla = 1024.0f;
GLfloat altoPantalla = 768.0f;

using namespace std;

Shader *shader;
GLuint posicionID;
GLuint colorID;
GLuint modeloID;
GLuint vistaID;
GLuint proyeccionID;

mat4 vista;
mat4 proyeccion;

Carro *carro;

void establecerVista() {
	vec3 posicionCamara = carro->getCoordenadas();
	posicionCamara.z -= 4;
	posicionCamara.y += 2;

	vista = lookAt(
		posicionCamara, //Posición de la cámara.
		carro->getCoordenadas(), //Posición del objetivo.
		vec3(0.0f, 1.0f, 0.0f) //Vector hacia arriba.
	);

	if (carro != NULL)
	{
		carro->vista = vista;
	}
}

void establecerProyección() {
	proyeccion = perspective(
		90.0f, //Campo de visión (FoV).
		anchoPantalla/altoPantalla, //Relación de aspecto.
		0.1f, //Near clipping (desde dónde renderea).
		50.0f //Far clipping (hasta dónde renderea).
	);
}

void dibujar() {
	carro->dibujar(GL_QUADS);
}

void actualizar() {
	int estadoEspacio = glfwGetKey(window, GLFW_KEY_SPACE);
	if (estadoEspacio == GLFW_PRESS)
	{
		//Rotar carro
		//carro->modelo = rotate(carro->modelo, 0.001f, vec3(0.0f, 1.0f, 0.0f));

		//Avanzar carro
		carro->avanzar();
		establecerVista();
	}
}

int main()
{	
	//Si no se pudo iniciar glfw
	//terminamos ejcución
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	//Si se pudo iniciar GLFW
	//entonces inicializamos la ventana
	window =
		glfwCreateWindow(1024, 768, "Ventana", 
			NULL, NULL);
	//Si no podemos iniciar la ventana
	//Entonces terminamos ejecucion
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//Establecemos el contexto
	glfwMakeContextCurrent(window);

	//Una vez establecido  el contexto
	//Activamos funciones modernas
	glewExperimental = true;
	GLenum errorGlew = glewInit();
	if (errorGlew != GLEW_OK) {
		cout << 
			glewGetErrorString(errorGlew);
	}

	const GLubyte *versionGL =
		glGetString(GL_VERSION);
	cout << "Version OpenGL: "
		<< versionGL;

	const char *rutaVertex = 
		"VertexShader.shader";
	const char *rutaFragment = 
		"FragmentShader.shader";

	shader = new Shader(rutaVertex, rutaFragment);

	//Mapeo de atributos
	posicionID =
		glGetAttribLocation(shader->getID(), "posicion");
	colorID =
		glGetAttribLocation(shader->getID(), "color");
	modeloID =
		glGetUniformLocation(shader->getID(), "modelo");
	vistaID =
		glGetUniformLocation(shader->getID(), "vista");
	proyeccionID =
		glGetUniformLocation(shader->getID(), "proyeccion");

	//Desenlazar el shader
	shader->desenlazar();

	establecerProyección();

	// AQUÍ MANDAMOS LLAMAR LOS SHADERS Y VERTEX ARRAY DE LOS OBJETOS //
	carro = new Carro();

	carro->proyeccion = proyeccion;
	
	carro->shader = shader;

	carro->inicializarVertexArray(posicionID, colorID, modeloID, vistaID, proyeccionID);
	// FIN DE: AQUÍ MANDAMOS LLAMAR LOS SHADERS Y VERTEX ARRAY DE LOS OBJETOS //

	carro->vista = vista;
	establecerVista();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//Ciclo de dibujo (Draw loop)
	while (!glfwWindowShouldClose(window)) {
		//Esablecer region de dibujo
		glViewport(0, 0, 1024, 768);
		//Establece el color de borrado
		glClearColor(1, 0.2, 0.5, 1);
		//Borramos
		glClear(
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Rutina de dibujo
		dibujar();
		actualizar();

		//Cambiar los buffers
		glfwSwapBuffers(window);
		//Reconocer si hay entradas
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

