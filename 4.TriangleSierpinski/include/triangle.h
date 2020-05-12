#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h> 
#include <iostream>
#include <queue>

class Triangle {
private:
	unsigned int VBO;
	unsigned int VAO;

	/*

	  /\      |
	 /  \  -> |  altura 
	/    \    |
	   | <-centro(x,y)
	
	*/
public:
	float x;
	float y;
	float altura;
	float ancho;
	float* shape;

	unsigned int profundidad;

	Triangle(float x, float y, float altura, float ancho, float* shape, unsigned int profundidad);
	~Triangle();
	void draw(unsigned int shaderProgram);
};


//-----------------------------------------------------funciones adicionales
unsigned int calcularNumTriangles(unsigned int profundidad_inicio, unsigned int profundidad_fin);
void agregarShape(float* vertexes, std::vector<Triangle*>& triangles, unsigned int indice, float* shape, int max_profundidad);
void dividirTriangulo(std::vector<Triangle*>& triangles, float* vertexes, unsigned int indice, int max_profundidad);
void draw(std::vector<Triangle*>& triangles, std::vector<unsigned int>& shaders, unsigned int indice, int profundidad, int max_profundidad);
float* generarShape(float x, float y, float altura, float ancho);
std::vector<unsigned int> generateShaders();