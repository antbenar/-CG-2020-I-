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
unsigned int currTriangle = 1;
float rColor = 1.0f, gColor = 0.0f, bColor = 0.0f, alpha = 1.0f;
float rColor2 = 1.0f, gColor2 = 1.0f, bColor2 = 0.0f, alpha2 = 1.0f;

// settings Matriz
// ------------------------------------------------------------------
OperacionesMatriz* opMat;
OperacionesMatriz* opMatSquare;

float operacion = 1;// 1 - Scale, 2 - Traslation
float x = 0.0, y = 0.0, z = 0.0;
float theta = (15.0 / 180.0) * PI;

// set up vertex data
// ------------------------------------------------------------------
float triangle[] = {
	0.75, -0.5, 0,
	0.25,0.5,0,
	0,-0.5,0
};

float square[] = {
	-0.5f, -0.8f, 0.0f,  // left
		0.5f, -0.8f, 0.0f,  // right
		0.5f, 0.2f, 0.0f,   // top 
		
		0.5f, 0.2f, 0.0f,
		-0.5f, 0.2f, 0.0f,
		-0.5f, -0.8f, 0.0f
};
float* object = triangle;


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
	"uniform vec4 Color1;\n"
	"void main()\n"
	"{\n"
	"   FragColor = Color1;\n"
	"}\n\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec4 Color2;\n"
	"void main()\n"
	"{\n"
	"   FragColor = Color2;\n"
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
	int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	int shaderProgram = glCreateProgram();
	int shaderProgram2 = glCreateProgram();
	
	
	// compile shaders
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);
	
	
	// link shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);
	
	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	
	// first triangle setup
	// --------------------
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	
	// second triangle setup
	// ---------------------
	glBindVertexArray(VAOs[1]);	
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
	glEnableVertexAttribArray(0);
	
	// Crear objeto OperacionesMatriz
	opMat = new OperacionesMatriz(3,3);
	opMat->restartMatrizCombinacion();
	/*
	opMat->rotateX(triangle, 46, true);
	
	opMat->rotateZ(triangle, 55, true);
	*/
	opMat->Scale(triangle, 1, 1.25, 1.25, true);

	opMat->rotateY(triangle, 17, true);

		
	opMat->Traslation(triangle, 0.16, 0.27, 0.075, true);


	opMat->rotateZ(triangle, 55, true);


	opMat->TransformarConMatrizCombinacion(triangle);
	std::cout << "------------------------" << std::endl;
	opMat->printMatrix(3, 3, triangle);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		
		// render
		// ------
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//---------------------------triangulo1
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "Color1");
		glUniform4f(vertexColorLocation, rColor*alpha, gColor*alpha, bColor*alpha, 1.0f);
		
		// enlazar nuevos datos
		glBindVertexArray(VAOs[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
		// dibujar
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		
		
		//---------------------------triangulo2
		int vertexColorLocation2 = glGetUniformLocation(shaderProgram2, "Color2");
		glUniform4f(vertexColorLocation2, rColor2*alpha2, gColor2*alpha2, bColor2*alpha2, 1.0f);
		
		// enlazar nuevos datos
		glBindVertexArray(VAOs[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
		// dibujar
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		currTriangle = 1;
		object = triangle;
		std::cout << "Triangulo 1 Seleccionado" << std::endl;
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		currTriangle = 2;
		object = square;
		std::cout << "Triangulo 2 Seleccionado" << std::endl;
	}
	
	//--------------------keys to modify x, y, z
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		x = 1; y = 1; z = 1;
		std::cout << "Modificacion x, y, z: x(" << x << "), y(" << y << "), z(" << z << ")" << std::endl;
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		x += 0.05;
		std::cout << "Modificacion x: x(" << x << "), y(" << y << "), z(" << z << ")" << std::endl;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		x -= 0.05;
		std::cout << "Modificacion x: x(" << x << "), y(" << y << "), z(" << z << ")" << std::endl;
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		y += 0.05;
		std::cout << "Modificacion y: x(" << x << "), y(" << y << "), z(" << z << ")" << std::endl;
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		y -= 0.05;
		std::cout << "Modificacion y: x(" << x << "), y(" << y << "), z(" << z << ")" << std::endl;
	}
	else if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) {
		z += 0.05;
		std::cout << "Modificacion z: x(" << x << "), y(" << y << "), z(" << z << ")" << std::endl;
	}
	else if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) {
		z -= 0.05;
		std::cout << "Modificacion z: x(" << x << "), y(" << y << "), z(" << z << ")" << std::endl;
	}
	
	
	// -------Matrix Operations
	// Escala
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		std::cout << "Scala: x(" << x << "), y(" << y << "), z(" << z << ")" << std::endl;
		opMat->Scale(triangle, x, y, z, false);
	}
	else if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		std::cout << "Traslacion: x(" << x << "), y(" << y << "), z(" << z << ")" << std::endl;
		opMat->Traslation(triangle, x, y, z, false);
	}
	else if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		std::cout << "Rotacion en X" << std::endl;
		opMat->rotateX(triangle, theta, false);
	}
	else if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
		std::cout << "Rotacion en Y" << std::endl;
		opMat->rotateY(triangle, theta, false);
	}
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		std::cout << "Rotacion en Z" << std::endl;
		opMat->rotateZ(triangle, theta, false);
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		std::cout << "Reflejo en X" << std::endl;
		opMat->mirrorX(triangle, false);
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		std::cout << "Reflejo en Y" << std::endl;
		opMat->mirrorY(triangle, false);
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		std::cout << "Reflejo en Z" << std::endl;
		opMat->mirrorZ(triangle, false);
	}
	
	// ------------------------colores triangulo
	if (currTriangle == 1) {
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			if (rColor == 1.0) rColor = 0;
			else rColor = 1.0;
		}
		else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
			if (gColor == 1.0) gColor = 0;
			else gColor = 1.0;
		}
		else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
			if (bColor == 1.0) bColor = 0;
			else bColor = 1;
		}
		else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
			if (alpha < 1.0) alpha += 0.1;
		}
		else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
			if (alpha > 0.0) alpha -= 0.1;
		}
	}
	
	else if (currTriangle == 2) {
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			if (rColor2 == 1.0) rColor2 = 0;
			else rColor2 = 1.0;
		}
		else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
			if (gColor2 == 1.0) gColor2 = 0;
			else gColor2 = 1.0;
		}
		else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
			if (bColor2 == 1.0) bColor2 = 0;
			else bColor2 = 1;
		}
		else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
			if(alpha2 < 1.0) alpha2 += 0.1;
		}
		else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
			if (alpha2 > 0.0) alpha2 -= 0.1;
		}
	}	
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
