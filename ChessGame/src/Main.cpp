//System include
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
        Stockfish stockfish;
        stockfish.sendCommand("isready\n");
        stockfish.getResponse();
        
        // Khởi động cửa sổ SFML
        RenderWindow window(VideoMode(800, 800), "Display Chess Board");
        ChessBoard chessBoard;
        window.setView(chessBoard.getView());

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) window.close();
                else if (event.type == Event::Resized) {
                    chessBoard.resize(Vector2u(event.size.width, event.size.height));
                    window.setView(chessBoard.getView());
                }
                else if (event.type == Event::KeyPressed) {
                    chessBoard.change();
                }
            }

            window.clear();
            window.draw(chessBoard.getSprite());
            window.display();
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}