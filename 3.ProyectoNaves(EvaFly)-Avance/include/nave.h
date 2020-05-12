#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define moveSpeed 0.1

class nave {
public:
	nave();
	~nave();
	void draw(unsigned int shaderProgram);//input: shader of the ship
	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();
private:
	unsigned int VBO;
	unsigned int VAO;

	float shapeNave[9] = {
		-0.1f, -0.8f, 0.0f,
		0.0f, -0.6f, 0.0f,
		0.1f,  -0.8f, 0.0f
	};

};