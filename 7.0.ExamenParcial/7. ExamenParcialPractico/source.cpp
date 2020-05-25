#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Matrix.h"
#define PI 3.14159265
#define INFINITE 100000

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
float r = 1.0f, g = 1.0f, b = 1.0f;
// settings
// ------------------------------------------------------------------
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// settings Matriz Transformations
// ------------------------------------------------------------------
OperacionesMatriz* opMat;
OperacionesMatriz* opMatSix;

float velocidadEscala = 0.1;			//usada para medir el ritmo de la escala, oscila entre 0.1 y 0.9
float varEscala = 1 - velocidadEscala;	
float theta = 0.1;						//angulo usado para rotar el cuadrado

float velocidadTraslacion = 0.1;		//usada para la velocidad en la que se mueve el rombo
bool arribaTras = true, izquierdaTras = false, derechaTras = false, abajoTras = false;

// timmer settings
// ------------------------------------------------------------------
double waitTime = 7;
bool pause = false;

// set up vertex data
// ------------------------------------------------------------------
float triangulo[] = {
	-0.3,	-0.3,	0,
	 0.3,	-0.3,	0,
	 0.0,	 0.3,	0
};

float cuadrado[] = {
	//primer triangulo
	0.4,	-0.1,	0,
	0.6,	-0.1,	0,
	0.6,	 0.1,	0,

	//segundo triangulo
	0.4,	-0.1,	0,
	0.6,	 0.1,	0,
	0.4,	 0.1,	0
};

float rombo[] = {
	//primer triangulo
	0.7,	 0.0,	0,
	0.8,	-0.1,	0,
	0.9,	 0.0,	0,

	//segundo triangulo
	0.7,	 0.0,	0,
	0.9,	 0.0,	0,
	0.8,	 0.1,	0,
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

const char *fragmentShaderSource3 = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 Color3;\n"
"void main()\n"
"{\n"
"   FragColor = Color3;\n"
"}\n\0";


void transformObjects() {
	//----------------------------------------------------Triangulo
	if (triangulo[7] > 0.3){
		varEscala = 1 - velocidadEscala;
	}
	else if(triangulo[7] < 0.1) {
		varEscala = 1 + velocidadEscala;
	}
	r -= (1- varEscala);
	g -= (1 - varEscala);
	b -= (1 - varEscala);
	opMat->Scale(triangulo, varEscala, varEscala, varEscala, false);

	//--------------------------------------------------------Cuadrado
	//el funcionamiento es mover el cuadrado al centro de los ejes de coordenadas, 
	//de tal forma que puede rotar sobre su misma posicion en Z con la negativa del angulo, 
	//para luego devolverlo a su posicion original y rotarlo en z nuevamente pero esta vez rotando alrededor del triangulo 
	//y de esta forma el cuadrado siempre permanezca con la misma forma

	float moveX = -0.1 - cuadrado[0];
	float moveY = 0.1 - cuadrado[7];

	opMatSix->Traslation(cuadrado, moveX, moveY, 0, true);
	opMatSix->rotateZ(cuadrado, -theta, true);
	opMatSix->Traslation(cuadrado, -moveX, -moveY, 0, true);
	opMatSix->rotateZ(cuadrado, theta, true);

	opMatSix->TransformarConMatrizCombinacion(cuadrado);
	opMatSix->restartMatrizCombinacion();


	//---------------------------------------------------------Rombo
	if (arribaTras){
		if (rombo[16] + velocidadTraslacion <= 0.9) {
			//-- hacia arriba
			opMatSix->Traslation(rombo, 0, velocidadTraslacion, 0, false);
		}
		else {
			float fill = 0.9 - rombo[16];
			opMatSix->Traslation(rombo, 0, fill, 0, false);
			arribaTras = false;
			izquierdaTras = true;
		}
	}
	else if (izquierdaTras) {
		if (rombo[0] - velocidadTraslacion >= -0.9) {
			//-- hacia izquierda
			opMatSix->Traslation(rombo, -velocidadTraslacion, 0, 0, false);
		}
		else {
			float fill = -0.9 - rombo[0];
			opMatSix->Traslation(rombo, fill, 0, 0, false);
			izquierdaTras = false;
			abajoTras = true;
		}
	}
	else if (abajoTras) {
		if (rombo[4] - velocidadTraslacion >= -0.9) {
			//-- hacia abajo
			opMatSix->Traslation(rombo, 0, -velocidadTraslacion, 0, false);
		}
		else {
			float fill = -0.9 - rombo[4];
			opMatSix->Traslation(rombo, 0, fill, 0, false);
			abajoTras = false;
			derechaTras = true;
		}
	}
	if (derechaTras) {
		if (rombo[6] + velocidadTraslacion <= 0.9) {
			//-- hacia derecha
			opMatSix->Traslation(rombo, velocidadTraslacion, 0, 0, false);
		}
		else {
			float fill = 0.9 - rombo[6];
			opMatSix->Traslation(rombo, fill, 0, 0, false);
			derechaTras = false;
			arribaTras = true;
		}
	}
}


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
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangulo), triangulo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// triangle2 setup
	// --------------------
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cuadrado), cuadrado, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// triangle3 setup
	// --------------------
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rombo), rombo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Crear objeto OperacionesMatriz------
	opMat = new OperacionesMatriz(3, 3);
	opMatSix = new OperacionesMatriz(3, 6);
	
	//Setting timer-----------------------
	static double limitFPS = 1.0 / 60.0;
	double lastTime = glfwGetTime(), timer = lastTime;
	double deltaTime = 0, nowTime = 0;


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		//---------------obtener eventos del teclado
		glfwPollEvents();

		// render
		// -------------------------------------------
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//---------------------------triangulo
		// --enlazar nuevos datos
		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangulo), triangulo, GL_STATIC_DRAW);
		// --dibujar
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "Color1");
		glUniform4f(vertexColorLocation, r, 0, 0, 1.0f);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		//---------------------------cuadrado
		// --enlazar nuevos datos
		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cuadrado), cuadrado, GL_STATIC_DRAW);
		// --dibujar
		int vertexColorLocation2 = glGetUniformLocation(shaderProgram2, "Color2");
		glUniform4f(vertexColorLocation2, 0, g, 0, 1.0f);
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		//---------------------------rombo
		// --enlazar nuevos datos
		glBindVertexArray(VAO[2]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rombo), rombo, GL_STATIC_DRAW);
		// --dibujar
		int vertexColorLocation3 = glGetUniformLocation(shaderProgram3, "Color3");
		glUniform4f(vertexColorLocation3, 0, 0, b, 1.0f);
		glUseProgram(shaderProgram3);
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		//--------------------Controlador del tiempo de la animación, despuéd de una cantidad fija de waitTime, se continua con la animación
		while (deltaTime < waitTime) {
			nowTime = glfwGetTime();
			deltaTime += (nowTime - lastTime) / limitFPS;
			lastTime = nowTime;
		}
		deltaTime = 0;

		//---------------------opcion de pausar la animacion
		if (pause) continue;

		//---------------------transformar las tres figuras
		transformObjects();

		//-----------------Swap buffers
		glfwSwapBuffers(window);
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);

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
		std::cout << "Pause or Dispause" << std::endl;
		pause = !pause;
	}

	//------------velocidad de animaciones
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		if (waitTime > 1)waitTime -= 1;
		std::cout << "Velocidad de animaciones = " << waitTime << "[max = no limits, min = 1]" << std::endl;
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		waitTime += 1;
		std::cout << "Velocidad de animaciones = " << waitTime << "[max = no limits, min = 1]" << std::endl;
	}

	// -----velocidad escala
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		if (velocidadEscala < 0.9) velocidadEscala += 0.1;
		std::cout << "Velocidad escala = " << velocidadEscala << "[max = 0.9, min = 0.1]" << std::endl;
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		if (velocidadEscala > 0.2) velocidadEscala -= 0.1;
		std::cout << "Velocidad escala = " << velocidadEscala << "[max = 0.9, min = 0.1]" << std::endl;
	}

	// -----angulo rotacion
	else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		theta += 0.1;
		std::cout << "Angulo Theta = " << theta  << std::endl;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		theta -= 0.1;
		std::cout << "Angulo Theta = " << theta << std::endl;
	}

	// -----velocidad traslacion
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		if (velocidadTraslacion < 0.9) velocidadTraslacion += 0.1;
		std::cout << "velocidadTraslacion = " << velocidadTraslacion << "[max = 0.9, min = 0.1]" << std::endl;
	}
	else if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		if (velocidadTraslacion > 0.2) velocidadTraslacion -= 0.1;
		std::cout << "velocidadTraslacion = " << velocidadTraslacion << "[max = 0.9, min = 0.1]" << std::endl;
	}
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
