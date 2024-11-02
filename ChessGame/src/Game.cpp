#include"../lib//Game.h"

Game::Game() {
	this->againstAI = false;
}

Game::Game(bool againstAI, Stockfish& stockfish, int skillLevel) {
	this->againstAI = againstAI;
	//this->stockfish = stockfish;
	//this->stockfish.setSkillLevel(skillLevel);
}

void Game::startGame(sf::RenderWindow* win) {
    window = win;
	if (this->againstAI) this->playAgainstAI();
	else this->playAgainstPlayer();
}

void Game::playAgainstAI() {

}

void Game::playAgainstPlayer() {
    // Tạo bàn cờ với các đường dẫn đã cho
    ChessBoard chessBoard(window, 0);
    cout << "Created a chessboard\n";

    // Vòng lặp chính của chương trình
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
            chessBoard.update(event);
        }

        window->clear();
        chessBoard.draw();
        window->display();
    }
}