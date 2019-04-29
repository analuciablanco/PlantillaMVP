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
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

//Declaramos apuntador de ventana
GLFWwindow *window;

using namespace std;

Shader *shader;
GLuint posicionID;
GLuint colorID;
GLuint modeloID;
GLuint vistaID;
GLuint proyeccionID;

mat4 vista;
mat4 proyeccion;

Modelo *triangulo;
Modelo *cuadrado;

void inicializarCuadrado() {

	cuadrado = new Modelo();

	Vertice v1 = {
		vec4(-0.8f,0.8f,0.0f,1.0f),
		vec4(0.4f,0.7f,0.1f,1.0f)
	};
	Vertice v2 = {
		vec4(-0.4f,0.8f,0.0f,1.0f),
		vec4(0.4f,0.7f,0.1f,1.0f)
	};
	Vertice v3 = {
		vec4(-0.4f,0.5f,0.0f,1.0f),
		vec4(0.4f,0.7f,0.1f,1.0f)
	};
	Vertice v4 = {
		vec4(-0.8f,0.5f,0.0f,1.0f),
		vec4(0.4f,0.7f,0.1f,1.0f)
	};
	cuadrado->vertices.push_back(v1);
	cuadrado->vertices.push_back(v2);
	cuadrado->vertices.push_back(v3);
	cuadrado->vertices.push_back(v4);
}

void inicializarTriangulo() {

	triangulo = new Modelo();

	Vertice v1 =
	{
		vec4(0.0f,0.3f,0.0f,1.0f),
		vec4(1.0f,0.8f,0.0f,1.0f)
	};
	Vertice v2 =
	{
		vec4(-0.3f,-0.3f,0.0f,1.0f),
		vec4(1.0f,0.8f,0.0f,1.0f)
	};
	Vertice v3 =
	{
		vec4(0.3f,-0.3f,0.0f,1.0f),
		vec4(1.0f,0.8f,0.0f,1.0f)
	};

	triangulo->vertices.push_back(v1);
	triangulo->vertices.push_back(v2);
	triangulo->vertices.push_back(v3);
}

void dibujar() {
	triangulo->dibujar(GL_TRIANGLES);
	cuadrado->dibujar(GL_QUADS);
}

void actualizar() {
	int estadoDerecha = glfwGetKey(window, GLFW_KEY_RIGHT);

	if (estadoDerecha == GLFW_PRESS)
	{
		triangulo->transformaciones = translate(triangulo->transformaciones, vec3(0.001f, 0.0f, 0.0f));
	}

	int estadoIzquierda = glfwGetKey(window, GLFW_KEY_LEFT);

	if (estadoIzquierda == GLFW_PRESS)
	{
		triangulo->transformaciones = rotate(triangulo->transformaciones, 1.0f, vec3(0.0f, 0.0f, 1.0f));
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

	inicializarTriangulo();
	inicializarCuadrado();

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
	transformacionesID =
		glGetUniformLocation(shader->getID(), "transformaciones");

	//Desenlazar el shader
	shader->desenlazar();

	// AQUÍ MANDAMOS LLAMAR LOS SHADERS Y VERTEX ARRAY DE LOS OBJETOS //

		// TRIANGULO
		// Establecer shader al modelo:
		triangulo->shader = shader;
		//Inicializar vertex array:
		triangulo->inicializarVertexArray(posicionID, colorID, transformacionesID);

		// CUADRADO
		// Establecer shader al modelo:
		cuadrado->shader = shader;
		//Inicializar vertex array:
		cuadrado->inicializarVertexArray(posicionID, colorID, transformacionesID);

	// FIN DE: AQUÍ MANDAMOS LLAMAR LOS SHADERS Y VERTEX ARRAY DE LOS OBJETOS //

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

