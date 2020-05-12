#include "game.h"


game::game(unsigned int numNavesEnemigas) {
	navecita = new nave();
	for(unsigned int i = 0; i < numNavesEnemigas; ++i)
		navesEnemigas.push_back(new naveEnemiga());
}

void game::draw(unsigned int shaderProgram) {
	navecita->draw(shaderProgram);

	for (unsigned int i = 0; i < navesEnemigas.size(); ++i)
		navesEnemigas[i]->draw(shaderProgram);
}

void game::moveNavesEnemigas() {
	for (unsigned int i = 0; i < navesEnemigas.size(); ++i)
		navesEnemigas[i]->move();
}