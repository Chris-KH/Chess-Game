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
        sf::RenderWindow window(sf::VideoMode(930, 930), "Chess Game", sf::Style::Close | sf::Style::Titlebar);

        // Tạo bàn cờ với các đường dẫn đã cho
        ChessBoard chessBoard(&window, 0);

        // Vòng lặp chính của chương trình
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                // Update bàn cờ
                chessBoard.update(event);
            }

            window.clear();
            chessBoard.draw();  // Vẽ bàn cờ và quân cờ
            window.display();
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}