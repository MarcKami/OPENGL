//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "..\Shader.h"

using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIDEFRAME = false;
bool paintQuad=false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void DrawVao(GLuint programID,GLuint VAO) {
	//establecer el shader
	glUseProgram(programID);

	//pitar el VAO
	glBindVertexArray(VAO);
	if (!paintQuad) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

}
int main() {
	//initGLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);
//TODO

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


	//create a window
//TODO
	GLFWwindow* window;



	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
//TODO

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		return NULL;
	}
	int screenWithd, screenHeight;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);
	//set function when callback
//TODO
		glfwSetKeyCallback(window, key_callback);

	//set windows and viewport
//TODO
		glViewport(0, 0, screenWithd, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT);

	//fondo
		glClearColor(0.0, 0.0, 1.0, 1.0);


//TODO


	//cargamos los shader

		Shader shader = Shader("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
		GLuint programID = shader.Program;

		//GLuint programID = LoadShaders("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");

	// Definir el buffer de vertices
	//Reserva de memoria

	//GLfloat traingleVertex[] = {
	//-0.5f,-0.5f,0,
	//0.5f,0.5f,0,
	//-0.5f,0.5f,0,
	//0.5f,-0.5f,0
	//};

	//VBO
		GLfloat VertexBufferObject[] = {
			0.5f,  0.5f, 0.0f,  // Top Right
			0.5f, -0.5f, 0.0f,  // Bottom Right
			-0.5f, -0.5f, 0.0f,  // Bottom Left
			-0.5f,  0.5f, 0.0f   // Top Left 
		};

	//Numbuffer = cantidad de buffers a generar;

		//Borrado
		//glDeleteBuffers(GLsizei n, const GLuint* ids);

	//-VBO

	//EBO

	//GLuint IndexBufferObject[]{
	//	0,1,2,
	//	1,3,0 };
		GLuint IndexBufferObject[]{
		2,0,3,
		2,1,0

		};
	//-EBO

	int numBuffer = 1;






	// Definir el EBO

	// Crear los VBO, VAO y EBO
	
	GLuint VAO, EBO, VBO;
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO); {

		glGenBuffers(1, &VBO);
		//Se enlaza el buffer para poder usarlo
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//Se pasan los datos
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_DYNAMIC_DRAW);


		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexBufferObject), IndexBufferObject, GL_DYNAMIC_DRAW);
		

		//Propiedades

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);

		//LIMPIA LOS BUFFERS DE VERTICES
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}glBindVertexArray(0);



	//reservar memoria para el VAO, VBO y EBO



	//Establecer el objeto
		//Declarar el VBO y el EBO

		//Enlazar el buffer con openGL

		//Establecer las propiedades de los vertices

		//liberar el buffer
	
	//liberar el buffer de vertices

	//bucle de dibujado

	GLint loc = glGetUniformLocation(shader.Program,"time");

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions

		glUniform1f(loc, glfwGetTime());

		glfwPollEvents();
		//Establecer el color de fondo
		glClear(GL_COLOR_BUFFER_BIT);
		
		//glClearColor(0, 1, 0, 0);

		DrawVao(programID,VAO);
	//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_POLYGON, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		shader.USE();
		//pintar con lineas
		//pintar con triangulos

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// liberar la memoria de los VAO, EBO y VBO

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		paintQuad = !paintQuad;
	}
}
