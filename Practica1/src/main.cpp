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
bool WIDEFRAME = false;
bool paintQuad = false;
bool fade1 = false;
float mixStuff;
float rotacionX,rotacionY = 0.0f;
float gradosRot = 0;
float aumentoRot;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
bool aumentarRotRight, aumentarRotLeft,aumentarUp,aumentarDown;

 
bool camUp, camDown, camLeft, camRight;
vec3 camPosVec, camDirVec, camRightVec, camUpVec;
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

mat4 MyLookAt() {
	mat4 vectors = {
		camRightVec.x, camUpVec.x, camDirVec.x, 0,
		camRightVec.y, camUpVec.y, camDirVec.y, 0,
		camRightVec.z, camUpVec.z, camDirVec.z, 0,
		0, 0, 0, 1
	};

	mat4 positions = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-camPosVec.x, -camPosVec.y, -camPosVec.z, 1
	};

	mat4 look = vectors * positions;
	return look;
}

void DoMovement(GLFWwindow* window) {
	camUp = glfwGetKey(window, 'w');
	camDown = glfwGetKey(window, 's');
	camLeft = glfwGetKey(window, 'a');
	camRight = glfwGetKey(window, 'd');
}

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

	//set function when callback
	glfwSetKeyCallback(window, key_callback);

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

	// Definir el buffer de vertices
	//Reserva de memoria

	//GLfloat traingleVertex[] = {
	//-0.5f,-0.5f,0,
	//0.5f,0.5f,0,
	//-0.5f,0.5f,0,
	//0.5f,-0.5f,0
	//};

	//VBO
	//GLfloat VertexBufferObject[] = {
	//	0.5f,  0.5f, 0.0f,  // Top Right
	//	0.5f, -0.5f, 0.0f,  // Bottom Right
	//	-0.5f, -0.5f, 0.0f,  // Bottom Left
	//	-0.5f,  0.5f, 0.0f   // Top Left 
	//};

	//GLfloat VertexBufferObject[] = {
	//	// Positions          // Colors           // Texture Coords
	//	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
	//	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	//};

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
	camPosVec = vec3(0.f, 0.f, -3.f);
	camDirVec = glm::normalize((vec3(0.f, 0.f, 0.f) - camPosVec));
	camRightVec = glm::normalize(glm::cross(camDirVec, vec3(0, 1, 0)));
	camUpVec = glm::normalize(glm::cross(camDirVec, camRightVec));

	while (!glfwWindowShouldClose(window))
	{


		//mat4 finalMatrix; //Modelo
		mat4 cam; //Vista
		mat4 proj; //Proyeccion
		mat4 view1, view2;

		GLfloat radio = 8.0f;
		GLfloat X = sin(glfwGetTime()) * radio;
		GLfloat Z = cos(glfwGetTime()) * radio;
				
		if (gradosRot > 360 || gradosRot < -360) {
			gradosRot = 0;
		}

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions

		glfwPollEvents();

		//matModelID = glGetUniformLocation(shader.Program, "matrizModel");
		//matProjID = glGetUniformLocation(shader.Program, "matrizProj");
		//matViewID = glGetUniformLocation(shader.Program, "matrizView");
		matrizDefID = glGetUniformLocation(shader.Program, "matrizDefinitiva");

		//Establecer el color de fondo
		//glClear(GL_COLOR_BUFFER_BIT);
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

		
		if (aumentarRotLeft) {
			rotacionY-=aumentoRot;
		}
		else if (aumentarRotRight) {
			rotacionY+= aumentoRot;
		}

		if (aumentarUp) {
			rotacionX -= aumentoRot;
		}
		else if (aumentarDown) {
			rotacionX += aumentoRot;
		}

		if (fade1) {
			if (mixStuff >= 0&&mixStuff<1) {
				mixStuff += 0.01f;
			}
		}
		else {
			if (mixStuff>0.01f) {
				mixStuff -= 0.01f;
			}
		}

		//Cam movement
		if (camUp) {
			camPosVec += camDirVec * camSpeed;
		}

		else if (camDown) {
			camPosVec -= camDirVec * camSpeed;
		}

		if (camRight) {
			camPosVec += camRightVec * camSpeed;
		}

		else if (camLeft) {
			camPosVec -= camRightVec * camSpeed;
		}

		float FOV = 45.0f;

		proj = perspective(FOV, (float)(800/600), 0.1f, 100.0f);

		cam = translate(cam, vec3(camPosVec.x, camPosVec.y, camPosVec.z));

		view1 = glm::lookAt(camPosVec, camPosVec + camDirVec, camUpVec);
		view2 = MyLookAt();

		cout << "------MATRIX GLM--------" << endl;
		cout << view1[0][0] << ", " << view1[1][0] << ", " << view1[2][0] << ", " << view1[3][0] << endl;
		cout << view1[0][1] << ", " << view1[1][1] << ", " << view1[2][1] << ", " << view1[3][1] << endl;
		cout << view1[0][2] << ", " << view1[1][2] << ", " << view1[2][2] << ", " << view1[3][2] << endl;
		cout << view1[0][3] << ", " << view1[1][3] << ", " << view1[2][3] << ", " << view1[3][3] << endl;

		cout << "------MATRIX MyLookAt--------" << endl;
		cout << view2[0][0] << ", " << view2[1][0] << ", " << view2[2][0] << ", " << view2[3][0] << endl;
		cout << view2[0][1] << ", " << view2[1][1] << ", " << view2[2][1] << ", " << view2[3][1] << endl;
		cout << view2[0][2] << ", " << view2[1][2] << ", " << view2[2][2] << ", " << view2[3][2] << endl;
		cout << view2[0][3] << ", " << view2[1][3] << ", " << view2[2][3] << ", " << view2[3][3] << endl;

		cout << " " << endl;
		cout << " " << endl;
		cout << " " << endl;
		cout << " " << endl;
		

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
	}
	

}



