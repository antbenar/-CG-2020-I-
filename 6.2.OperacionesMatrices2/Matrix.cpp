#include "Matrix.h"


OperacionesMatriz::OperacionesMatriz(unsigned int dimN, unsigned int numVertexes) : dimN(dimN), numVertexes(numVertexes) {
	this->matCombinacion = new float[(dimN + 1) * (dimN + 1)];
	restartMatrizCombinacion();
}

void OperacionesMatriz::restartMatrizCombinacion() {
	// igualar matriz combinacion a la matriz identidad
	memset(matCombinacion, 0, (dimN + 1) * (dimN + 1) * sizeof(float));
	matCombinacion[0] = 1.0f;
	matCombinacion[5] = 1.0f;
	matCombinacion[10] = 1.0f;
	matCombinacion[15] = 1.0f;
}

float* OperacionesMatriz::mult(float* mat1, float* mat2, int dimI, int dimJ, int dimK) {
	// mat1 -> matriz de transofrmacion (ixk)
	// mat2 -> vector (kxj) => kx1
	// el resultado se almacena en matResult (ixj) => ix1

	float* matResult = new float[dimI*dimJ];
	memset(matResult, 0, dimI*dimJ * sizeof(float));
	
	for (unsigned int i = 0; i < dimI; ++i) {
		for (unsigned int j = 0; j < dimJ; ++j) {
			for (unsigned int k = 0; k < dimK; ++k) {
				matResult[i*dimJ + j] += mat1[i*dimK + k] * mat2[k*dimJ + j];
			}
		}
	}

	return matResult;
}

float* OperacionesMatriz::genVec(float* vertice) {//genera matriz de dim = (N+1) x 1
	float* result = new float[dimN + 1];

	float* iter = vertice;
	for (unsigned int i = 0; i < dimN; ++i, iter++) {
		result[i] = *iter;
	}
	result[dimN] = 1;

	return result;
}


void OperacionesMatriz::TransformacionXVertices(float* transformacion, float* vertices) {
	float* iter = vertices;

	for (unsigned int i = 0; i < numVertexes*dimN; i += dimN, iter += dimN) {
		float* tempMat = genVec(vertices + i);

		//printMatrix(1, dimN, vertices + i);//----imprimir
		tempMat = mult(transformacion, tempMat, dimN + 1, 1, dimN + 1);//i = dimN + 1, k = dimN + 1, j = 1

																// copy from tempMat to vertice
		for (unsigned int j = 0; j < dimN; ++j) {
			vertices[i + j] = tempMat[j];
		}
		/*
		std::cout << "--------------------------" << std::endl;//----imprimir
		printMatrix(1, dimN, vertices + i);//----imprimir
		std::cout << "__________________________" << std::endl;//----imprimir
		*/
	}
}

void OperacionesMatriz::TransformarConMatrizCombinacion(float* vertices) {
	float* iter = vertices;

	for (unsigned int i = 0; i < numVertexes*dimN; i += dimN, iter += dimN) {//calcular vertice por vertice
		float* tempMat = genVec(vertices + i);

		tempMat = mult(matCombinacion, tempMat, dimN + 1, 1, dimN + 1);//i = dimN + 1, k = dimN + 1, j = 1
		printMatrix(1, dimN+1, tempMat);//----imprimir el vertice calculado
																	  
		for (unsigned int j = 0; j < dimN; ++j) { // copy from tempMat to vertice
			vertices[i + j] = tempMat[j];
		}

	}
}

void OperacionesMatriz::Scale(float* vertices, float Sx, float Sy, float Sz, bool combinacion) {
	float scaleMat[] = {	Sx, 0, 0, 0,
							0, Sy, 0, 0,
							0, 0, Sz, 0,
							0, 0, 0, 1 };
	if (combinacion) {
		matCombinacion = mult(scaleMat, matCombinacion , dimN + 1, dimN + 1, dimN + 1);
	}
	else {
		TransformacionXVertices(scaleMat, vertices);
	}
	
}


void OperacionesMatriz::Traslation(float* vertices, float Sx, float Sy, float Sz, bool combinacion) {
	float TraslMat[] = {	1, 0, 0, Sx,
							0, 1, 0, Sy,
							0, 0, 1, Sz,
							0, 0, 0, 1 };

	if (combinacion) {
		matCombinacion = mult(TraslMat, matCombinacion, dimN + 1, dimN + 1, dimN + 1);
	}
	else {
		TransformacionXVertices(TraslMat, vertices);
	}
}


void OperacionesMatriz::rotateZ(float* vertices, float theta, bool combinacion) {

	float matRotZ[] = { cos(theta),	-1 * sin(theta),	0, 0,
						sin(theta),	cos(theta)     ,	0, 0,
						0		  , 0		       ,	1, 0,
						0		  , 0			   ,	0, 1};

	if (combinacion) {
		matCombinacion = mult(matRotZ, matCombinacion, dimN + 1, dimN + 1, dimN + 1);
	}
	else {
		TransformacionXVertices(matRotZ, vertices);
	}
}

void OperacionesMatriz::rotateX(float* vertices, float theta, bool combinacion) {

	float matRotX[] = { 1,	0		  ,	0			   , 0,
						0,	cos(theta),	-1 * sin(theta), 0,
						0,	sin(theta),	cos(theta)     , 0,
						0,	0		  ,	0			   , 1};


	if (combinacion) {
		matCombinacion = mult(matRotX, matCombinacion, dimN + 1, dimN + 1, dimN + 1);
	}
	else {
		TransformacionXVertices(matRotX, vertices);
	}


}

void OperacionesMatriz::rotateY(float* vertices, float theta, bool combinacion) {

	float matRotY[] = { cos(theta) ,	0,	sin(theta), 0,
						0          ,	1,	0		  , 0,
					    -sin(theta),	0,	cos(theta), 0,
						0		   ,	0,	0		  , 1};


	if (combinacion) {
		matCombinacion = mult(matRotY, matCombinacion, dimN + 1, dimN + 1, dimN + 1);
	}
	else {
		TransformacionXVertices(matRotY, vertices);
	}

}

void OperacionesMatriz::mirrorX(float* vertices, bool combinacion) {

	float matMirror[] = {	-1, 0, 0, 0,
							 0, 1, 0, 0,
							 0, 0, 1, 0,
							 0, 0, 0, 1 };

	if (combinacion) {
		matCombinacion = mult(matMirror, matCombinacion, dimN + 1, dimN + 1, dimN + 1);
	}
	else {
		TransformacionXVertices(matMirror, vertices);
	}

}

void OperacionesMatriz::mirrorY(float* vertices, bool combinacion) {

	float matMirror[] = {	1,  0, 0, 0,
							0, -1, 0, 0,
							0,  0, 1, 0,
							0,	0, 0, 1};

	if (combinacion) {
		matCombinacion = mult(matMirror, matCombinacion, dimN + 1, dimN + 1, dimN + 1);
	}
	else {
		TransformacionXVertices(matMirror, vertices);
	}

}

void OperacionesMatriz::mirrorZ(float* vertices, bool combinacion) {

	float matMirror[] = {	1, 0,  0, 0,
							0, 1,  0, 0,
							0, 0, -1, 0,
							0, 0,  0, 1};

	if (combinacion) {
		matCombinacion = mult(matMirror, matCombinacion, dimN + 1, dimN + 1, dimN + 1);
	}
	else {
		TransformacionXVertices(matMirror, vertices);
	}

}



void OperacionesMatriz::printMatrix(int dimM, int dimN, float* vertices) {

	for (unsigned int i = 0; i < dimM; ++i) {
		for (unsigned int j = 0; j < dimN; ++j) {
			std::cout << vertices[i*dimN + j] << ", ";
		}
		std::cout << std::endl;
	}
}

void OperacionesMatriz::printMatrix(int dimM, int dimN, float* vertices, std::string matName) {

	std::cout << std::endl << "------------------------------------------------" << std::endl;
	std::cout << "Matriz " << matName << std::endl;

	for (unsigned int i = 0; i < dimM; ++i) {
		for (unsigned int j = 0; j < dimN; ++j) {
			std::cout << std::setprecision(3) << vertices[i*dimN + j] << ",\t";
		}
		std::cout << std::endl;
	}
}

