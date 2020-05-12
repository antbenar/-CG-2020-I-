#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "nave.h"
#include "naveEnemiga.h"

class game {
public:
	nave* navecita;
	std::vector<naveEnemiga*> navesEnemigas;

	game(unsigned int numNavesEnemigas);
	void draw(unsigned int shaderProgram);
	void moveNavesEnemigas();
};