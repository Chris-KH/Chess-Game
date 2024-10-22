#include"../lib//Game.h"

Game::Game() {
	this->againstAI = true;
}

Game::Game(bool againstAI, Stockfish& stockfish, int skillLevel) {
	this->againstAI = true;
	this->stockfish = stockfish;
	this->stockfish.setSkillLevel(skillLevel);
}

void Game::startGame() {
	if (this->againstAI) this->playAgainstAI();
	else this->playAgainstPlayer();
}

void Game::playAgainstAI() {

}

void Game::playAgainstPlayer() {

}