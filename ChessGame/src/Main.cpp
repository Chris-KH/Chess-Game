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

const int NUM_CHESS_BOARD = 3;
int chessBoardId;
Texture chessBoardImage[3];
Sprite chessBoardSprite;
Vector2u chessBoardSize;

void loadImage(Texture& texture, string path);
void switchBoard(void);
void chessBoardInit(void);

int main() {
    // Khởi động Stockfish
    string stockfishPath = "stockfish.exe"; // Thay đổi đường dẫn tới stockfish.exe

    try {
        Stockfish stockfish(stockfishPath);
        
        // Khởi động cửa sổ SFML
        RenderWindow window(VideoMode(800, 800), "Display Chess Board");
        
        chessBoardInit();

        View view(FloatRect(0, 0, 800, 800));
        view.setSize(chessBoardSize.x, chessBoardSize.y);
        view.setCenter(chessBoardSize.x / 2, chessBoardSize.y / 2);
        window.setView(view);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
                else if (event.type == Event::Resized) {
                    float r = static_cast<float>(event.size.width) / static_cast<float>(event.size.height);
                    view.setSize(chessBoardSize.x * r, chessBoardSize.y);
                    view.setCenter(chessBoardSize.x / 2, chessBoardSize.y / 2);
                    window.setView(view);
                }
                else if (event.type == Event::KeyPressed) {
                    switchBoard();
                }
            }

            window.clear();
            window.draw(chessBoardSprite);
            window.display();
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}

void loadImage(Texture& texture, string path) {
    if (!texture.loadFromFile(path)) {
        cerr << "Khong tim thay " << path << "!" << endl << endl;
        exit(1);
    }
}

void switchBoard(void) {
    chessBoardId = (chessBoardId + 1) % NUM_CHESS_BOARD;
    chessBoardSprite.setTexture(chessBoardImage[chessBoardId]);
}

void chessBoardInit(void) {
    chessBoardId = 0;
    loadImage(chessBoardImage[0], "../assets/Chess Board/ChessBoard1.png");
    loadImage(chessBoardImage[1], "../assets/Chess Board/ChessBoard2.png");
    loadImage(chessBoardImage[2], "../assets/Chess Board/ChessBoard3.png");

    chessBoardSize = chessBoardImage[0].getSize();
    chessBoardSprite.setTexture(chessBoardImage[0]);
}