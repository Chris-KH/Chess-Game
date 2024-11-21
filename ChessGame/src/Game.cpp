#include"../lib//Game.h"

Game::Game() {
	this->againstAI = false;
}

Game::Game(bool againstAI, int skillLevel) {
	this->againstAI = againstAI;
	this->stockfish->setSkillLevel(skillLevel);
}

void Game::startGame(void) {

}

void Game::playAgainstAI() {

}

void Game::playAgainstPlayer() {

}

void Game::changeMode(bool mode) {

}