#include "nave.h"

nave::nave() {
	// generar la VAO y el VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

nave::~nave(){
	// de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void nave::draw(unsigned int shaderProgram) {
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

void nave::moveUp() {
	if (shapeNave[4] + moveSpeed > 0.2) return;

	for (int i = 1; i < 9; i+=3) {
		shapeNave[i] += moveSpeed;
	}
}

void nave::moveDown() {
	if (shapeNave[1] - moveSpeed < -1) return;

	for (int i = 1; i < 9; i += 3) {
		shapeNave[i] -= moveSpeed;
	}
}

void nave::moveRight() {
	if (shapeNave[6] + moveSpeed > 1) return;

	for (int i = 0; i < 9; i += 3) {
		shapeNave[i] += moveSpeed;
	}
}

void nave::moveLeft() {
	if (shapeNave[0] - moveSpeed < -1) return;

	for (int i = 0; i < 9; i += 3) {
		shapeNave[i] -= moveSpeed;
	}
}