//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "..\Shader.h"
#include <SOIL.h>



using namespace glm;
using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
GLfloat lastX = 400, lastY = 300;
bool WIDEFRAME = false;
bool paintQuad = false;
bool fade1 = false;
float mixStuff;
float rotacionX,rotacionY = 0.0f;
float gradosRot = 0;
float aumentoRot;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xScroll, double yScroll);
bool aumentarRotRight, aumentarRotLeft,aumentarUp,aumentarDown;
float camSpeed = 0.01f;

void DrawVao(GLuint programID, GLuint VAO) {
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

mat4 GenerateModelMatrix(vec3 aTranslation, vec3 aRotation, vec3 CubesPosition, float aRot) {
	mat4 temp;
	temp = translate(temp, aTranslation);
	temp = translate(temp, CubesPosition);
	temp = rotate(temp, radians(aRot), aRotation);

	return temp;
}

class Camera {
public:
	vec3 cameraPos;
	vec3 cameraFront;
	vec3 cameraRight;
	vec3 cameraUp;
	GLfloat Deltatime;
	GLfloat Lastframe;
	GLfloat LastMx;
	GLfloat LastMy;
	GLfloat Sensitivity;
	GLboolean firstMouse = true;
	GLfloat PITCH = 0.f;
	GLfloat YAW = 0.f;
	GLfloat FOV;
	Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov);
	void MouseMove(GLFWwindow* window, double xpos, double ypos);
	void MouseScroll(GLFWwindow* window, double xScroll, double yScroll);
	mat4 LookAt();
	GLfloat GetFOV();
	void DoMovement(GLFWwindow* window);
};

Camera::Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov) {
	Lastframe = 0;
	cameraPos = position;
	cameraFront = glm::normalize((vec3(0.f, 0.f, 0.f) - cameraPos));
	cameraRight = glm::normalize(glm::cross(cameraFront, vec3(0, 1, 0)));
	cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));

	Sensitivity = sensitivity;
	FOV = fov;

}

void Camera::MouseMove(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xOffset = xpos - lastX;
	GLfloat yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xOffset *= Sensitivity;
	yOffset *= Sensitivity;

	YAW += xOffset;
	PITCH += yOffset;

	PITCH = clamp(PITCH, -89.9f, 89.9f);
	YAW = glm::mod(YAW, 360.f);

	glm::vec3 front;
	front.x = cos(radians(YAW)) * cos(radians(PITCH));
	front.y = sin(radians(PITCH));
	front.z = sin(radians(YAW)) * cos(radians(PITCH));
	cameraFront = normalize(front);
}

void Camera::MouseScroll(GLFWwindow* window, double xScroll, double yScroll) {
	FOV -= yScroll;
	FOV = clamp(FOV, 1.f, 45.f);
}

mat4 Camera::LookAt() {
	mat4 vectors = {
		cameraRight.x, cameraUp.x, cameraFront.x, 0,
		cameraRight.y, cameraUp.y, cameraFront.y, 0,
		cameraRight.z, cameraUp.z, cameraFront.z, 0,
		0, 0, 0, 1
	};

	mat4 positions = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-cameraPos.x, -cameraPos.y, -cameraPos.z, 1
	};

	mat4 look = vectors * positions;
	return look;
}

GLfloat Camera::GetFOV() {
	return FOV;
}

void Camera::DoMovement(GLFWwindow* window) {
	bool camUp, camDown, camLeft, camRight;
	GLfloat actualFrame = glfwGetTime();
	Deltatime = actualFrame - Lastframe;

	float cameraSpeed = 3.f * Deltatime;
	
	camUp = glfwGetKey(window, GLFW_KEY_W);
	camDown = glfwGetKey(window, GLFW_KEY_S);
	camLeft = glfwGetKey(window, GLFW_KEY_A);
	camRight = glfwGetKey(window, GLFW_KEY_D);

	//Cam movement
	if (camUp) cameraPos += cameraFront * cameraSpeed;
	else if (camDown) cameraPos -= cameraFront * cameraSpeed;
	if (camRight) cameraPos += cameraRight * cameraSpeed;
	else if (camLeft) cameraPos -= cameraRight * cameraSpeed;

	Lastframe = actualFrame;
}

Camera myCamera(vec3(0.f, 0.f, -3.f), vec3(0.f, 0.f, 3.f), 0.04f, 45.f);

void main() {
	mixStuff = 0.0f;
	//initGLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
	GLFWwindow* window;

	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	int screenWithd, screenHeight;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//set function when callback
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//set windows and viewport
	glViewport(0, 0, screenWithd, screenHeight);
	glClear(GL_COLOR_BUFFER_BIT);

	//fondo
	glClearColor(0.0, 0.0, 1.0, 1.0);

	//cargamos los shader

	//Shader shader = Shader("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");

	Shader shader = Shader("./src/textureVertex3d.vertexshader", "./src/textureFragment3d.fragmentshader");

	//GLuint programID = shader.Program;

	//GLuint programID = LoadShaders("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");

	GLfloat VertexBufferObject[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vec3 CubesPositionBuffer[] = {
		vec3(0.0f ,  0.0f,  0.0f),
		vec3(2.0f ,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f , -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f , -2.0f, -2.5f),
		vec3(1.5f ,  2.0f, -2.5f),
		vec3(1.5f ,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};

	// Crear los VBO, VAO y EBO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); {

		glGenBuffers(1, &VBO);
		//Se enlaza el buffer para poder usarlo
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//Se pasan los datos
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_DYNAMIC_DRAW);
		
		//Propiedades
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);

		//LIMPIA LOS BUFFERS DE VERTICES
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}glBindVertexArray(0);

	GLuint texture1, texture2;

	GLint matrizDefID;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widthTex, heightTex;
	unsigned char* image = SOIL_load_image("./src/texture.png", &widthTex, &heightTex, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex, heightTex, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widthTex2, heightTex2;

	unsigned char* image2 = SOIL_load_image("./src/texture2.png", &widthTex2, &heightTex2, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex2, heightTex2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	SOIL_free_image_data(image2);

	glBindTexture(GL_TEXTURE_2D, 0);

	aumentoRot = 0.5f;
	
	glEnable(GL_DEPTH_TEST);

	//bucle de dibujado	
	while (!glfwWindowShouldClose(window))
	{
		myCamera.DoMovement(window);
		mat4 model; //Modelo
		mat4 cam; //Vista
		mat4 proj; //Proyeccion

		GLfloat radio = 8.0f;
		GLfloat X = sin(glfwGetTime()) * radio;
		GLfloat Z = cos(glfwGetTime()) * radio;
				
		if (gradosRot > 360 || gradosRot < -360) {
			gradosRot = 0;
		}

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions

		glfwPollEvents();

		matrizDefID = glGetUniformLocation(shader.Program, "matrizDefinitiva");

		//Establecer el color de fondo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		GLint locTex = glGetUniformLocation(shader.Program, "ourTexture");
		GLint locTex2 = glGetUniformLocation(shader.Program, "ourTexture2");
		GLint mixID = glGetUniformLocation(shader.Program, "mixStuff");
		shader.USE();
		
		glUniform1f(mixID, mixStuff);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(locTex,0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(locTex2, 1);
		
		glBindVertexArray(VAO);

		proj = perspective(radians(myCamera.GetFOV()), (float)(800 / 600), 0.1f, 100.0f);

		cam = lookAt(myCamera.cameraPos, myCamera.cameraPos + myCamera.cameraFront, myCamera.cameraUp);
		//cam = myCamera.LookAt();

		//Cube Rotation
		if (aumentarRotLeft) rotacionY -= aumentoRot;
		else if (aumentarRotRight) rotacionY += aumentoRot;
		if (aumentarUp) rotacionX -= aumentoRot;
		else if (aumentarDown) rotacionX += aumentoRot;

		if (fade1) { if (mixStuff >= 0 && mixStuff<1) { mixStuff += 0.01f; } }
		else { if (mixStuff>0.01f) { mixStuff -= 0.01f; } }

		for (int i = 0; i < 10; i++) {
			mat4 matriz;
			if (i == 0) {
				matriz = translate(matriz, CubesPositionBuffer[0]);
				matriz = rotate(matriz, radians(rotacionX), vec3(1,0,0));
				matriz = rotate(matriz, radians(rotacionY),vec3(0,1,0));
			}
			else {
				float rotot = glfwGetTime() * 100;
				rotot = (int)rotot % 360;
				matriz = GenerateModelMatrix(vec3(0.0f), vec3(1, 0.5f, 0), CubesPositionBuffer[i], rotot);
			}
			//glUniformMatrix4fv(matModelID, 1, GL_FALSE, glm::value_ptr(matriz));

			mat4 matrizDefinitiva;

			matrizDefinitiva = proj*cam*matriz;

			glUniformMatrix4fv(matrizDefID, 1, GL_FALSE, glm::value_ptr(matrizDefinitiva));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// liberar la memoria de los VAO, EBO y VBO

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

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

	if (key == GLFW_KEY_UP&&action == GLFW_PRESS) {
		aumentarUp = true;
	}

	if (key == GLFW_KEY_UP&&action == GLFW_RELEASE) {
		aumentarUp = false;
	}

	if (key == GLFW_KEY_DOWN&&action == GLFW_PRESS) {
		aumentarDown = true;
	}

	if (key == GLFW_KEY_DOWN&&action == GLFW_RELEASE) {
		aumentarDown = false;
	}

	if (key == GLFW_KEY_RIGHT&&action == GLFW_PRESS) {
		aumentarRotRight = true;
	}
	else if (key == GLFW_KEY_RIGHT&&action == GLFW_RELEASE) {
		aumentarRotRight = false;
	}

	if (key == GLFW_KEY_LEFT&&action == GLFW_PRESS) {
		aumentarRotLeft = true;
	}
	else if (key == GLFW_KEY_LEFT&&action == GLFW_RELEASE) {
		aumentarRotLeft = false;
	}

	if (key == GLFW_KEY_1&&action == GLFW_PRESS) {
		fade1 = true;
	}
	else if (key == GLFW_KEY_2&&action == GLFW_PRESS) {
		fade1 = false;
	}

	/*
	if (key == GLFW_KEY_W&&action == GLFW_PRESS) {
		camUp = true;
	}
	else if (key == GLFW_KEY_W&&action == GLFW_RELEASE) {
		camUp = false;
	}

	if (key == GLFW_KEY_A&&action == GLFW_PRESS) {
		camLeft = true;
	}
	else if (key == GLFW_KEY_A&&action == GLFW_RELEASE) {
		camLeft = false;
	}

	if (key == GLFW_KEY_S&&action == GLFW_PRESS) {
		camDown = true;
	}
	else if (key == GLFW_KEY_S&&action == GLFW_RELEASE) {
		camDown = false;
	}

	if (key == GLFW_KEY_D&&action == GLFW_PRESS) {
		camRight = true;
	}
	else if (key == GLFW_KEY_D&&action == GLFW_RELEASE) {
		camRight = false;
	}*/
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	myCamera.MouseMove(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xScroll, double yScroll) {
	myCamera.MouseScroll(window, xScroll, yScroll);
}


