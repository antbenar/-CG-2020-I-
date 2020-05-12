#include "naveEnemiga.h"


naveEnemiga::naveEnemiga() {
	// generar la VAO y el VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

naveEnemiga::~naveEnemiga() {
	// de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void naveEnemiga::draw(unsigned int shaderProgram) {
	// shape setup
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// give the shape data
	glBufferData(GL_ARRAY_BUFFER, sizeof(shapeNave), shapeNave, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void naveEnemiga::move() {
	int randomPos = rand() % 4;

	if (randomPos == 0) {
		moveUp();
	}
	else if (randomPos == 1) {
		moveDown();
	}
	else if (randomPos == 2) {
		moveRight();
	}
	else if (randomPos == 3) {
		moveLeft();
	}
}

void naveEnemiga::moveUp() {
	if (shapeNave[1] + moveSpeed > 0.9) return;

	for (int i = 1; i < 9; i += 3) {
		shapeNave[i] += moveSpeed;
	}
}

void naveEnemiga::moveDown() {
	if (shapeNave[7] - moveSpeed < 0.2) return;

	for (int i = 1; i < 9; i += 3) {
		shapeNave[i] -= moveSpeed;
	}
}

void naveEnemiga::moveRight() {
	if (shapeNave[3] + moveSpeed > 1) return;

	for (int i = 0; i < 9; i += 3) {
		shapeNave[i] += moveSpeed;
	}
}

void naveEnemiga::moveLeft() {
	if (shapeNave[0] - moveSpeed < -1) return;

	for (int i = 0; i < 9; i += 3) {
		shapeNave[i] -= moveSpeed;
	}
}