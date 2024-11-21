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
//#include "../lib/Game.h"
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
                if (chessBoard.isOver() == 0) {
                    chessBoard.update(event);
                }
                else {
                    GUI::gameOver(chessBoard);
                }
            }

            window.clear();
            chessBoard.draw();
            window.display();
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}