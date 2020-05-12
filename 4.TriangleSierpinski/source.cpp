#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "triangle.h"

#define waitTime 7
int max_profundidad = 8;
//---------------------------------Shader sources
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    in vec3 color;

    out vec3 Color;

    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0f, 1.0);
    }
)glsl";

const GLchar* fragmentSource = R"glsl(
    #version 150 core
    in vec3 Color;
    out vec4 outColor;
    void main()
    {
        outColor = vec4(Color, 1.0);
    }
)glsl";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//setting timer---------------
static double limitFPS = 1.0 / 60.0;
bool flagAnimacion = false;
double lastTime = glfwGetTime(), timer = lastTime;
double deltaTime = 0, nowTime = 0;
std::size_t indice;
unsigned int totalTriangles = calcularNumTriangles(0,max_profundidad)*3;

//------------------------- declarar triangulo y la cola de triangulos
int profundidad = max_profundidad;
Triangle* triangle;
std::vector<Triangle*> triangles;
std::vector<unsigned int> shaders;

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangulo de Sierpinski", NULL, NULL);
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

	//----------------------------creando VAO y VBO
	unsigned int VAO[2], VBO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	
	//------------------------------creando shaders
	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");


	//----------------------------setting timer
	static double limitFPS = 1.0 / 60.0;

	double lastTime = glfwGetTime(), timer = lastTime;
	double deltaTime = 0, nowTime = 0;

	//----------------------------setting triangle shape
	float triangleShape[9] = {
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f,  0.8f, 0.0f
	};
	float triangleShape2[15] = {
		-0.85f, -0.85f, 0.8f, 0.8f, 0.0f,
		0.85f, -0.85f, 0.8f, 0.8f, 0.0f,
		0.0f,  0.9f, 0.8f, 0.8f, 0.0f,
	};

	//--------------------------programa input 
	//std::cout << "ingrese profundidad máxima a calcular: ";
	//std::cin >> max_profundidad;

	//----------------------------variables triangle
	shaders = generateShaders();
	triangle = new Triangle(0.0f, -0.8f, 1.6f, 0.8f, triangleShape, 0);
	triangles.push_back(triangle);
	
	//-----------------------------generar triangles recursivos
	unsigned int numTriangles = 15 * calcularNumTriangles(0,max_profundidad);
	float* vertexes = new float[numTriangles]();
	dividirTriangulo(triangles, vertexes, 0, max_profundidad);
	//std::cout << numTriangles/15  << "-" << triangles.size() << std::endl;

	//---------------------------------give the shape data for triangle1
	//shape setup
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, numTriangles * sizeof(float), vertexes, GL_STATIC_DRAW);
	
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	//---------------------------------give the shape data for triangle2(background)
	//shape setup
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(float), triangleShape2, GL_STATIC_DRAW);
	// Specify the layout of the vertex data
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	
	indice = 0;

	// render loop-----------
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//-----------------------------------------------render background
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		//-----------------------------------------------render triangulos
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, totalTriangles);

		//-----------------------------------------------animacion de triangulos(lo llamas desde el TECLADO(con R)
		if (flagAnimacion) {
			if (indice >= triangles.size()) {
				flagAnimacion = false;
				indice = 0;
			}
			else {
				//Measure time--------------------
				while (deltaTime < waitTime) { // - Only update at x fps
					nowTime = glfwGetTime();
					deltaTime += (nowTime - lastTime) / limitFPS;
					lastTime = nowTime;
				}
				deltaTime = 0;
				glDrawArrays(GL_TRIANGLES, 0, numTriangles);
				triangles[indice]->draw(shaders[(triangles[indice]->profundidad%3)]);
				++indice;
			}	
		}
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(1, VBO);

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		flagAnimacion = false; indice = 0;

		std::cout << "profundidad: " << profundidad << std::endl;
		profundidad += 1;
		if (profundidad > max_profundidad) profundidad = 0;
		totalTriangles = calcularNumTriangles(0, profundidad) * 3;
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		flagAnimacion = true;
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