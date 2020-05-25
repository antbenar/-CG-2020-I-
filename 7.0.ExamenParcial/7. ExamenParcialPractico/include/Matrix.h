#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h> 
#include <iostream>
#include <vector>
#include <math.h> 
#include <string>
#include <iomanip>

class OperacionesMatriz {
private:
	unsigned int dimN;	// matriz de vertice Nx1 
						// => matriz de tranformacion MxN (M=N+1)
	unsigned int numVertexes; // numero de vertices a calcular en un array 

	float* mult(float* mat1, float* mat2, int dimI, int dimJ, int dimK);
	float* genVec(float* vertice);
	void TransformacionXVertices(float* transformacion, float* vertices);

public:
	float* matCombinacion;// donde se almacenan varias combinaciones

	OperacionesMatriz(unsigned int dimN, unsigned int numVertexes);
	void restartMatrizCombinacion();

	void TransformarConMatrizCombinacion(float* vertices);

	void Scale(float* vertices, float Sx, float Sy, float Sz, bool combinacion);
	void Traslation(float* vertices, float Sx, float Sy, float Sz, bool combinacion);

	void rotateX(float* vertices, float theta, bool combinacion);
	void rotateY(float* vertices, float theta, bool combinacion);
	void rotateZ(float* vertices, float theta, bool combinacion);

	void mirrorX(float* vertices, bool combinacion);
	void mirrorY(float* vertices, bool combinacion);
	void mirrorZ(float* vertices, bool combinacion);

	void printMatrix(int dimM, int dimN, float* vertices);
	void printMatrix(int dimM, int dimN, float* vertices, std::string matName);

};
