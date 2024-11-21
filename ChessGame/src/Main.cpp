//System include
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <bits/stdc++.h>

using namespace std;
using namespace sf;

//Header include
#include "../lib/Game.h"
#include "../lib/ChessBoard.h"

int main() {
    // Khởi động Stockfish
    
    try {
        //Game GameEngine;
        //GameEngine.startGame();
        RenderWindow window(VideoMode(1400, 930), "Chess Game", Style::Close | Style::Titlebar);
        Image icon;
        if (!icon.loadFromFile("../assets/ChessGameIcon.png")) {
            cerr << "Failed to load icon!" << '\n';
            return -1;
        }

        // Set icon for window
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

        ChessBoard chessBoard(&window, 0);
        cout << "Created a chessboard\n";

        //Stockfish stockfish;

        // Vòng lặp chính của chương trình
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                if (!chessBoard.isOver()) {
                    chessBoard.update(event);
                }
            }

            window.clear();
            chessBoard.draw();
            if (chessBoard.isOver()) {
                Texture GO;
                GO.loadFromFile("../assets/gameover.png");
                RectangleShape colorRect(sf::Vector2f(200, 150));
                colorRect.setFillColor(sf::Color(200, 200, 200, 255)); 
                colorRect.setPosition(365, 385);
                colorRect.setOutlineThickness(10);
                colorRect.setOutlineColor(Color::Black);
                window.draw(colorRect);
                Sprite GameOver;
                GameOver.setTexture(GO);
                GameOver.setPosition(347, 347);
                window.draw(GameOver);
            }
            window.display();
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}