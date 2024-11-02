#pragma once
#include "../lib/StockFish.h"
#include "../lib/ChessBoard.h"

class Game {
private:
	bool againstAI; //True if play with AI
	Stockfish stockfish;
	sf::RenderWindow* window;

public:
	Game();
	Game(bool againstAI, Stockfish& stockfish, int skillLevel = 0);

	void startGame(sf::RenderWindow* win);
	void playAgainstAI();
	void playAgainstPlayer();
};