#include <iostream>

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;


//static void error_callback(int error, const char* description){
//	fputs(description, stderr);
//}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	//cuando se pulsa una tecla escape cerramos la aplicacion
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main() {

	GLFWwindow* window;
	//glfwSetErrorCallback(error_callback);

	//comprobar que GLFW esta activo
	if (!glfwInit()) {
		cout << "Error al inicializar glfw" << endl;
		exit(EXIT_FAILURE);
	}

	//crear la ventana
	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	int screenWithd, screenHeight;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);

	//que funcion se llama cuando se detecta una pulsacin de tecla en la ventana x
	glfwSetKeyCallback(window, key_callback);

	while (!glfwWindowShouldClose(window)) {
		////definir las dimensiones del buffer
		//int screenWithd, screenHeigtht;
		//float ratio;
		//glfwGetFramebufferSize(window, &screenWithd, &screenHeigtht);
		//ratio = screenWithd / (float)screenHeigtht;

		//origen de la camara, dimensiones de la ventana
		glViewport(0, 0, screenWithd, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT);

		//color de fondo
		glClearColor(0.53, 0.8, 1.0, 1.0); //Azul Cielo

		//definir la matriz de proyeccion
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-10, 10, -10.f, 10.f, -1.0f, 10.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//glRotatef((float)glfwGetTime() * 50.f,1.f, 0.f, 0.f);

		//para volver a dejar la rotacion parada
		//glLoadIdentity();

#if(false)
		//Points
		glPointSize(20.f);
		glColor3f(.0f, .0f, .0f);
		glBegin(GL_POINTS);
		glVertex3f(.0f, .0f, .0f);
		glVertex3f(-.8f, -.8f, .0f);
		glVertex3f(.8f, .8f, .0f);
		glVertex3f(-.8f, .8f, .0f);
		glVertex3f(.8f, -.8f, .0f);
		glEnd();

		//Line
		/*glLineWidth(10.f);
		glBegin(GL_LINES);
		glVertex3f(-5.f, -5.f, .0f);
		glVertex3f(-5.f, 5.f, .0f);
		glEnd();*/

		//Line Strip
		glLineWidth(3.f);
		glBegin(GL_LINE_STRIP);
		glVertex3f(-2.3f, -2.5f, .0f);
		glVertex3f(-2.3f, 2.5f, .0f);
		glVertex3f(2.3f, 2.5f, .0f);
		glVertex3f(2.3f, -2.5f, .0f);
		glEnd();

		//Line Loop
		glLineWidth(3.f);
		glBegin(GL_LINE_LOOP);
		glVertex3f(-1.3f, -1.5f, .0f);
		glVertex3f(-1.3f, 1.5f, .0f);
		glVertex3f(1.3f, 1.5f, .0f);
		glVertex3f(1.3f, -1.5f, .0f);
		glEnd();

		//Line Stipple
		glLineWidth(3.f);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(2, 0x0C0F);
		glBegin(GL_LINE_LOOP);
		glVertex3f(-3.3f, -3.5f, .0f);
		glVertex3f(-3.3f, 3.5f, .0f);
		glVertex3f(3.3f, 3.5f, .0f);
		glVertex3f(3.3f, -3.5f, .0f);
		glEnd();
		glDisable(GL_LINE_STIPPLE);

		//Triangle
		glBegin(GL_TRIANGLES);
		glColor3f(.0f,.0f,1.f);
		glVertex3f(3.f, 7.f, .0f);
		glColor3f(.0f, 1.f, .0f);
		glVertex3f(7.f, 3.f, .0f);
		glColor3f(1.f, .0f, .0f);
		glVertex3f(7.f, 7.f, .0f);
		glEnd();

		//Activar culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW); //Defines the normal facing player

		//Polygon 
		//Need put the vertex in the next order:
		// 2---4
		// -   -
		// 1---3
		glBegin(GL_POLYGON);
		glColor3f(.0f, .0f, .0f);
		glVertex3f(-7.f, 2.f, .0f);
		glVertex3f(-8.f, 1.f, .0f);

		glColor3f(.0f, 1.f, .0f);
		glVertex3f(-8.f, -1.f, .0f);
		glVertex3f(-7.f, -2.f, .0f);

		glColor3f(1.f, .0f, .0f);
		glVertex3f(-5.f, -2.f, .0f);
		glVertex3f(-4.f, -1.f, .0f);

		glColor3f(.0f, .0f, .0f);
		glVertex3f(-4.f, 1.f, .0f);
		glVertex3f(-5.f, 2.f, .0f);
		glEnd();


		glDisable(GL_CULL_FACE);
		//Quad
		//Need put the vertex in CCW order:
		// 4---3
		// -   -
		// 1---2
		glBegin(GL_QUADS);
		glColor3f(.0f, .0f, 1.f);
		glVertex3f(7.f, -2.f, .0f);
		glVertex3f(9.f, -2.f, .0f);
		glColor3f(.0f, .0f, .0f);
		glVertex3f(8.8f, .0f, .0f);
		glVertex3f(7.2f, .0f, .0f);
		glEnd();




#endif
		//Activar culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW); //Defines the normal facing player

		//Smile
		glBegin(GL_POLYGON);
		glColor3f(0.9f, 0.9f, .0f);
		/*
		glVertex3f(-9.0f, 9.0f, .0f);
		glVertex3f(-8.9f, 8.9f, .0f);
		glVertex3f(-8.8f, 8.8f, .0f);
		glVertex3f(-8.7f, 8.7f, .0f);
		glVertex3f(-8.6f, 8.6f, .0f);
		glVertex3f(-8.5f, 8.5f, .0f);
		glVertex3f(-8.4f, 8.4f, .0f);
		glVertex3f(-8.3f, 8.3f, .0f);
		glVertex3f(-8.2f, 8.2f, .0f);
		glVertex3f(-8.1f, 8.1f, .0f);
		glVertex3f(-8.0f, 8.0f, .0f);
		glVertex3f(-7.9f, 8.1f, .0f);
		glVertex3f(-7.8f, 8.2f, .0f);
		glVertex3f(-7.7f, 8.3f, .0f);
		glVertex3f(-7.6f, 8.4f, .0f);
		glVertex3f(-7.5f, 8.5f, .0f);
		glVertex3f(-7.4f, 8.6f, .0f);
		glVertex3f(-7.3f, 8.7f, .0f);
		glVertex3f(-7.2f, 8.8f, .0f);
		glVertex3f(-7.1f, 8.9f, .0f);
		glVertex3f(-7.0f, 9.0f, .0f);//Half
		glVertex3f(-7.1f, 9.1f, .0f);
		glVertex3f(-7.2f, 9.2f, .0f);
		glVertex3f(-7.3f, 9.3f, .0f);
		glVertex3f(-7.4f, 9.4f, .0f);
		glVertex3f(-7.5f, 9.5f, .0f);
		glVertex3f(-7.6f, 9.6f, .0f);
		glVertex3f(-7.7f, 9.7f, .0f);
		glVertex3f(-7.8f, 9.8f, .0f);
		glVertex3f(-7.9f, 9.9f, .0f);
		glVertex3f(-8.0f, 10.0f, .0f);
		glVertex3f(-8.1f, 9.9f, .0f);
		glVertex3f(-8.2f, 9.8f, .0f);
		glVertex3f(-8.3f, 9.7f, .0f);
		glVertex3f(-8.4f, 9.6f, .0f);
		glVertex3f(-8.5f, 9.5f, .0f);
		glVertex3f(-8.6f, 9.4f, .0f);
		glVertex3f(-8.7f, 9.3f, .0f);
		glVertex3f(-8.8f, 9.2f, .0f);
		glVertex3f(-8.9f, 9.1f, .0f);
		*/
		glEnd();

		//Green Floor
		glBegin(GL_QUADS);
		glColor3f(0.0f, 0.5f, 0.0f); //Green Color
		glVertex3f(-10.0f, 0.0f, 0.0f);
		glVertex3f(-10.0f, -10.f, 0.0f);
		glVertex3f(10.0f, -10.0f, 0.0f);
		glVertex3f(10.0f, 0.0f, 0.0f);
		glEnd();

		//Draw Mountains
		glBegin(GL_TRIANGLES);
		glColor3f(0.5f, 0.27f, 0.07f);
		glVertex3f(-12.0f, 0.0f, 0.0f); //Brown Color
		glVertex3f(-5.0f, 0.0f, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f); //White Color
		glVertex3f(-7.0f, 4.2f, 0.0f);
		glEnd();
		glBegin(GL_TRIANGLES);
		glColor3f(0.5f, 0.27f, 0.07f);
		glVertex3f(-7.5f, 0.0f, 0.0f); //Brown Color
		glVertex3f(-2.0f, 0.0f, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f); //White Color
		glVertex3f(-4.0f, 4.0f, 0.0f);
		glEnd();
		glBegin(GL_TRIANGLES);
		glColor3f(0.5f, 0.27f, 0.07f);
		glVertex3f(-3.5f, 0.0f, 0.0f); //Brown Color
		glVertex3f(0.0f, 0.0f, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f); //White Color
		glVertex3f(-1.5f, 4.5f, 0.0f);
		glEnd();


		glDisable(GL_CULL_FACE);

		//intercambia el framebuffer
		glfwSwapBuffers(window);
		//comprueba que algun disparador se halla activado (tales como el teclado, raton, etc)
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}