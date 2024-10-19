#pragma once
#include"../lib/StockFish.h"

class Game {
private:
	bool againstAI; //True if play with AI
	Stockfish stockfish;
public:
	Game();
	Game(bool againstAI, Stockfish& stockfish, int skillLevel = 0);

	void startGame();
	void playAgainstAI();
	void playAgainstPlayer();
};