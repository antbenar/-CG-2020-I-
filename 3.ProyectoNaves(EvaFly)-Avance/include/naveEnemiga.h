#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <time.h> 
#include <iostream>

#define moveSpeed 0.1

class naveEnemiga {
public:
	naveEnemiga();
	~naveEnemiga();
	void draw(unsigned int shaderProgram);//input: shader of the ship
	void move();

private:
	unsigned int VBO;
	unsigned int VAO;

	float shapeNave[9] = {
		-0.1f, 0.9f, 0.0f,
		0.1f, 0.9f, 0.0f,
		0.0f,  0.7f, 0.0f
	};

	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();
};