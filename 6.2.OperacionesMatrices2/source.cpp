#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Matrix.h"
#define PI 3.14159265

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
// ------------------------------------------------------------------
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float rColor = 1.0f, gColor = 0.0f, bColor = 0.0f, alpha = 1.0f;
unsigned int operacion = 0;
bool showTriangulo2 = false, showTriangulo3 = false;

// settings Matriz
// ------------------------------------------------------------------
OperacionesMatriz* opMat;

// set up vertex data
// ------------------------------------------------------------------
float object[] = {
		0.75,	-0.5,	0,
		0.25,	 0.5,	0,
		   0,	-0.5,	0
};

float object2[] = {
		0.75,	-0.5,	0,
		0.25,	 0.5,	0,
		0,		-0.5,	0
};

float object3[] = {
		0.75,	-0.5,	0,
		0.25,	 0.5,	0,
		0,		-0.5,	0
};


// set shader sources
// ------------------------------------------------------------------
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

const char *fragmentShaderSource3 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	
	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	
	// create Shaders
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	int shaderProgram = glCreateProgram();

	int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	int shaderProgram2 = glCreateProgram();

	int fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER);
	int shaderProgram3 = glCreateProgram();
	
	// compile shaders
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);

	glShaderSource(fragmentShader3, 1, &fragmentShaderSource3, NULL);
	glCompileShader(fragmentShader3);
	

	// link shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	glAttachShader(shaderProgram3, vertexShader);
	glAttachShader(shaderProgram3, fragmentShader3);
	glLinkProgram(shaderProgram3);
	
	// Delete Shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);
	glDeleteShader(fragmentShader3);


	unsigned int VBO[3], VAO[3];
	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);
	
	// triangle setup
	// --------------------
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(object), object, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// triangle2 setup
	// --------------------
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(object2), object2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// triangle3 setup
	// --------------------
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(object3), object3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Crear objeto OperacionesMatriz
	opMat = new OperacionesMatriz(3,3);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// render
		// ------
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//---------------------------triangulo 3
		// --enlazar nuevos datos
		glBindVertexArray(VAO[2]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(object3), object3, GL_STATIC_DRAW);
		// --dibujar
		glUseProgram(shaderProgram3);
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		//---------------------------triangulo 1
		// --enlazar nuevos datos
		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(object), object, GL_STATIC_DRAW);
		// --dibujar
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//---------------------------triangulo 2
		if (showTriangulo2) {
			// --enlazar nuevos datos
			glBindVertexArray(VAO[1]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(object2), object2, GL_STATIC_DRAW);
			// --dibujar
			glUseProgram(shaderProgram2);
			glBindVertexArray(VAO[1]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}


		//---------------------------triangulo 3
		if (showTriangulo3) {
			// --enlazar nuevos datos
			glBindVertexArray(VAO[2]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(object3), object3, GL_STATIC_DRAW);
			// --dibujar
			glUseProgram(shaderProgram3);
			glBindVertexArray(VAO[2]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		

		//-----------------Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(1, VBO);
	
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		/*
			Al realizar una transofrmacion y pasarle a la funcion true en el ultimo paramtero, 
			hará que se almacene esa tranformacion en mi matriz de combinacion de transformaciones.

			La funcion restartMatrizCombinacion permite igualar mi matriz combinacion a la matriz identidad
		*/

		if (operacion == 0) {
			std::cout << "__________Matrices de las Transformaciones Basicas____________" << std::endl;

			opMat->rotateX(object, 46, true);
			opMat->printMatrix( 4, 4, opMat->matCombinacion, "Matriz Rotacion en X");
			opMat->restartMatrizCombinacion();

			opMat->rotateY(object, 17, true);
			opMat->printMatrix(4, 4, opMat->matCombinacion, "Matriz Rotacion en Y");
			opMat->restartMatrizCombinacion();

			opMat->rotateZ(object, 55, true);
			opMat->printMatrix(4, 4, opMat->matCombinacion, "Matriz Rotacion en Z");
			opMat->restartMatrizCombinacion();

			opMat->Traslation(object, 0.16, 0.27, 0.075, true);
			opMat->printMatrix(4, 4, opMat->matCombinacion, "Matriz de Traslacion");
			opMat->restartMatrizCombinacion();

			opMat->Scale(object, 1, 1.25, 1.25, true);
			opMat->printMatrix(4, 4, opMat->matCombinacion, "Matriz de Escala");
			opMat->restartMatrizCombinacion();

			++operacion;
		}
		else if (operacion == 1) {
			std::cout << "________________Inicio de Matrices Transformacion1_________________" << std::endl;

			opMat->Scale(object2, 1, 1.25, 1.25, true);
			opMat->rotateY(object2, 17, true);
			opMat->printMatrix(4, 4, opMat->matCombinacion, "S*Ry");

			opMat->Traslation(object2, 0.16, 0.27, 0.075, true);
			opMat->printMatrix(4, 4, opMat->matCombinacion, "T*(S*Ry)");

			opMat->rotateZ(object2, 55, true);
			opMat->printMatrix(4, 4, opMat->matCombinacion, "Rz*(T*(S*Ry))");

			//--------Aplicacion de la transformacion
			std::cout << std::endl << "------------------------------------------------" << std::endl;
			std::cout << "(Aplicacion de Transformacion 2)[coordenadas de los vertices transformados]" << std::endl;
			opMat->TransformarConMatrizCombinacion(object2);//los vertices se imprimen en la funcion TransformarConMatrizCombinacion

			showTriangulo2 = true;
			opMat->restartMatrizCombinacion();
			++operacion;
		}
		else if (operacion == 2) {
			std::cout << "________________Inicio de Matrices Transformacion2_________________" << std::endl;

			opMat->Scale(object3, 1, 1.25, 1.25, true);
			opMat->rotateY(object3, 17, true);
			opMat->printMatrix(4, 4, opMat->matCombinacion, "S*Ry");

			opMat->Traslation(object3, 0.16, 0.27, 0.075, true);
			opMat->printMatrix(4, 4, opMat->matCombinacion, "T*(S*Ry)");

			opMat->rotateX(object3, 46, true);
			opMat->printMatrix(4, 4, opMat->matCombinacion, "Rx*(T*(S*Ry))");

			opMat->rotateZ(object3, 55, true);
			opMat->printMatrix(4, 4, opMat->matCombinacion, "Rz*(Rx*(T*(S*Ry)))");

			opMat->rotateY(object3, 17, true);
			opMat->printMatrix(4, 4, opMat->matCombinacion, "Ry*(Rz*(Rx*(T*(S*Ry))))");

			//--------Aplicacion de la transformacion
			std::cout << std::endl << "------------------------------------------------" << std::endl;
			std::cout << "(Aplicacion de Transformacion 2)[coordenadas de los vertices transformados]" << std::endl;
			opMat->TransformarConMatrizCombinacion(object3);//los vertices se imprimen en la funcion TransformarConMatrizCombinacion

			showTriangulo3 = true;
			opMat->restartMatrizCombinacion();
			++operacion;
		}
	}
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
