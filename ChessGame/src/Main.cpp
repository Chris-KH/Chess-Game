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
        RenderWindow window(VideoMode(900, 900), "Chess Game", 
            
            Style::Close | Style::Titlebar);
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