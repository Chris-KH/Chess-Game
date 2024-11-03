#include"../lib//Game.h"

Game::Game() {
	this->againstAI = false;
}

Game::Game(bool againstAI, int skillLevel) {
	this->againstAI = againstAI;
	this->stockfish->setSkillLevel(skillLevel);
}

void Game::startGame(void) {
    sf::RenderWindow menu(sf::VideoMode(930, 930), "Chess Game!", sf::Style::Titlebar | sf::Style::Close);

    sf::Texture bgTexture, onePlayerTexture, twoPlayerTexture;
    bgTexture.loadFromFile("../assets/Menu/tempBG.jpg");
    onePlayerTexture.loadFromFile("../assets/Menu/one-player-button.png");
    twoPlayerTexture.loadFromFile("../assets/Menu/two-player-button.png");

    sf::Sprite bgSprite, onePlayerSprite, twoPlayerSprite;
    bgSprite.setTexture(bgTexture);
    onePlayerSprite.setTexture(onePlayerTexture);
    twoPlayerSprite.setTexture(twoPlayerTexture);

    float rWidth = 930 / bgSprite.getGlobalBounds().width;
    float rHeight = 930 / bgSprite.getGlobalBounds().height;
    float r = min(rWidth, rHeight);
    bgSprite.scale(r, r);

    bgSprite.setPosition((930 - bgSprite.getGlobalBounds().width) / 2, (930 - bgSprite.getGlobalBounds().height) / 2);
    onePlayerSprite.setPosition(390, 340);
    twoPlayerSprite.setPosition(390, 540);

    // Start with a menu with 1-player and 2-player mode
    while (menu.isOpen()) {
        sf::Event event;
        while (menu.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                menu.close();
            }

            sf::Vector2i mousePos = sf::Mouse::getPosition(menu);
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (twoPlayerSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    againstAI = false;
                    menu.close();
                    break;
                }
            }
        }

        menu.clear();
        menu.draw(bgSprite);
        menu.draw(onePlayerSprite);
        menu.draw(twoPlayerSprite);
        menu.display();
    }

	if (this->againstAI) this->playAgainstAI();
	else this->playAgainstPlayer();
}

void Game::playAgainstAI() {

}

void Game::playAgainstPlayer() {
    // Tạo bàn cờ với các đường dẫn đã cho
    sf::RenderWindow window(sf::VideoMode(930, 930), "Chess Game", sf::Style::Titlebar | sf::Style::Close);
    ChessBoard chessBoard(&window, 0);

    // Vòng lặp chính của chương trình
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            chessBoard.update(event);
        }

        window.clear();
        chessBoard.draw();
        window.display();
    }
}