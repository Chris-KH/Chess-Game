#include"../lib//Game.h"

Game::Game() {
	this->againstAI = false;
}

Game::Game(bool againstAI, int skillLevel) {
	this->againstAI = againstAI;
	this->stockfish->setSkillLevel(skillLevel);
}

void Game::startGame() {
	if (this->againstAI) this->playAgainstAI();
	else this->playAgainstPlayer();
}

void Game::playAgainstAI() {

}

void Game::playAgainstPlayer() {
    // Tạo bàn cờ với các đường dẫn đã cho

}

void Game::changeMode(bool mode) {
    this->againstAI = mode;
}