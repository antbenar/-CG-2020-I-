#include "triangle.h"


Triangle::Triangle(float x, float y, float altura, float ancho, float* shape_, unsigned int profundidad)
	:x(x), y(y), altura(altura), ancho(ancho), profundidad(profundidad){
	// generar la VAO y el VBO
	this->shape = shape_;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

Triangle::~Triangle() {
	// de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}


void Triangle::draw(unsigned int shaderProgram) {
	// shape setup
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// give the shape data
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

//-----------------------------------------------------funciones adicionales
unsigned int calcularNumTriangles(unsigned int profundidad_inicio, unsigned int profundidad_fin) {
	unsigned int result = 0;
	for (int i = profundidad_inicio; i <= profundidad_fin; ++i) {
		result += pow(3, i);
	}

	return result;
}

void agregarShape(float* vertexes, std::vector<Triangle*>& triangles, unsigned int indice, float* shape, int max_profundidad) {
	unsigned int indice_vertexes = indice * 15;	//posicion y color de cada triangulo
	int alpha = 1;// triangles[indice]->profundidad / max_profundidad;
	//std::cout << "inidice: " << indice << "(" << triangles[indice]->profundidad << ")" << std::endl;

	for (int i = 0, j = 0; i < 15; ++j) {
		vertexes[indice_vertexes + i] = shape[j]; ++i; ++j;
		vertexes[indice_vertexes + i] = shape[j]; ++i; ++j;

		if (j > 2) {
			vertexes[indice_vertexes + i] = 0.1; ++i;
			vertexes[indice_vertexes + i] = 0.1; ++i;
			vertexes[indice_vertexes + i] = 0.1; ++i;
		}
		else if (triangles[indice]->profundidad % 3 == 0) {
			vertexes[indice_vertexes + i] = 1 * alpha; i+=3;
			
		}
		else if (triangles[indice]->profundidad % 3 == 1) {
			vertexes[indice_vertexes + i] = 0 * alpha; ++i;
			vertexes[indice_vertexes + i] = 1 * alpha; ++i;
			vertexes[indice_vertexes + i] = 0 * alpha; ++i;
		}
		else {
			vertexes[indice_vertexes + i] = 0 * alpha; ++i;
			vertexes[indice_vertexes + i] = 0 * alpha; ++i;
			vertexes[indice_vertexes + i] = 1 * alpha; ++i;
		}
	}
	//std::cout << std::endl;
}


void dividirTriangulo(std::vector<Triangle*>& triangles, float* vertexes, unsigned int indice, int max_profundidad){
	//std::cout << "profundidad: " << triangles.front()->profundidad << std::endl;

	if (indice >= triangles.size() || triangles[indice]->profundidad >= max_profundidad) return;

	Triangle* triangle = triangles[indice];
	agregarShape(vertexes, triangles, indice, triangle->shape, max_profundidad);

	float x;
	float y;
	float altura;
	float ancho;
	float* shape;
	int profundidad = triangle->profundidad + 1;

	altura = triangle->altura / 2;
	ancho = triangle->ancho / 2;

	// crear subtriangulo superior
	y = triangle->y + altura;
	x = triangle->x;
	shape = generarShape(x, y, altura, ancho);
	triangles.push_back(new Triangle(x, y, altura, ancho, shape, profundidad));
	
	// crear subtriangulo izquierdo
	y = triangle->y;
	x = triangle->x - ancho;
	shape = generarShape(x, y, altura, ancho);
	triangles.push_back(new Triangle(x, y, altura, ancho, shape, profundidad));

	// crear subtriangulo derecho
	y = triangle->y;
	x = triangle->x + ancho;
	shape = generarShape(x, y, altura, ancho);
	triangles.push_back(new Triangle(x, y, altura, ancho, shape, profundidad));

	dividirTriangulo(triangles, vertexes, indice+1, max_profundidad);
};

void draw(std::vector<Triangle*>& triangles, std::vector<unsigned int>& shaders, unsigned int indice, int profundidad,  int max_profundidad) {
	if (profundidad > max_profundidad) return;
	Triangle* triangle;

	do{
		if (indice >= triangles.size()) break;
		
		triangle = triangles[indice];

		//if (triangle->profundidad == profundidad) {
			std::cout << "-----profundidad: " << triangle->profundidad << "-" << max_profundidad << std::endl;
			triangle->draw(shaders[profundidad % 3]);
		//}
		indice += 1;

	}while (triangle->profundidad <= profundidad);
	
	profundidad += 1;
};


float* generarShape(float x, float y, float altura, float ancho) {
	float* shape = (float*)malloc(9 * sizeof(float));
	//punto 1
	shape[0] = x - ancho;
	shape[1] = y;
	shape[2] = 0.0f;

	//punto 2
	shape[3] = x + ancho;
	shape[4] = y;
	shape[5] = 0.0f;

	//punto 3
	shape[6] = x;
	shape[7] = y + altura;
	shape[8] = 0.0f;

	return shape;
}


std::vector<unsigned int> generateShaders() {

	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char *fragmentShader1Source = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
		"}\n\0";
	const char *fragmentShader2Source = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
		"}\n\0";
	const char *fragmentShader3Source = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
		"}\n\0";

	// build and compile our shader program
	// ------------------------------------
	// we skipped compile log checks this time for readability (if you do encounter issues, add the compile-checks! see previous code samples)
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderR = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int fragmentShaderG = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int fragmentShaderB = glCreateShader(GL_FRAGMENT_SHADER);

	unsigned int shaderProgramR = glCreateProgram();
	unsigned int shaderProgramG = glCreateProgram();
	unsigned int shaderProgramB = glCreateProgram();

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShaderR, 1, &fragmentShader1Source, NULL);
	glCompileShader(fragmentShaderR);
	glShaderSource(fragmentShaderG, 1, &fragmentShader2Source, NULL);
	glCompileShader(fragmentShaderG);
	glShaderSource(fragmentShaderB, 1, &fragmentShader3Source, NULL);
	glCompileShader(fragmentShaderB);

	// link the first program object
	glAttachShader(shaderProgramR, vertexShader);
	glAttachShader(shaderProgramR, fragmentShaderR);
	glLinkProgram(shaderProgramR);
	// then link the second program object using a different fragment shader (but same vertex shader)
	// this is perfectly allowed since the inputs and outputs of both the vertex and fragment shaders are equally matched.
	glAttachShader(shaderProgramG, vertexShader);
	glAttachShader(shaderProgramG, fragmentShaderG);
	glLinkProgram(shaderProgramG);

	glAttachShader(shaderProgramB, vertexShader);
	glAttachShader(shaderProgramB, fragmentShaderB);
	glLinkProgram(shaderProgramB);


	std::vector<unsigned int> result;
	result.push_back(shaderProgramR);
	result.push_back(shaderProgramG);
	result.push_back(shaderProgramB);

	return result;
}