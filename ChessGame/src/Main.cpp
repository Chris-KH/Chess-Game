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

const int NUM_BOARD = 3;
int boardId;
Texture boardImage[3];

void loadImage(Texture& text, string path);
void switchBoard(Sprite& board);

int main() {
    // Khởi động Stockfish
    string stockfishPath = "stockfish.exe"; // Thay đổi đường dẫn tới stockfish.exe

    try {
        Stockfish stockfish(stockfishPath);
        string response = stockfish.sendCommand("uci");

        // Hiển thị phản hồi
        cout << "Stockfish response:\n" << response << endl;

        // Khởi động cửa sổ SFML
        RenderWindow window(VideoMode(800, 800), "Display Chess Board");

        boardId = 0;
        loadImage(boardImage[0], "../assets/Chess Board/ChessBoard1.png");
        loadImage(boardImage[1], "../assets/Chess Board/ChessBoard2.png");
        loadImage(boardImage[2], "../assets/Chess Board/ChessBoard3.png");

        Sprite board;
        board.setTexture(boardImage[0]);

        Vector2u textureSize = boardImage[0].getSize();

        View view;
        view.setSize(textureSize.x, textureSize.y);
        view.setCenter(textureSize.x / 2, textureSize.y / 2);
        window.setView(view);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
                else if (event.type == Event::Resized) {
                    float r = static_cast<float>(event.size.width) / static_cast<float>(event.size.height);
                    view.setSize(textureSize.x * r, textureSize.y);
                    view.setCenter(textureSize.x / 2, textureSize.y / 2);
                    window.setView(view);
                }
                else if (event.type == Event::KeyPressed) {
                    switchBoard(board);
                }
            }

            window.clear();
            window.draw(board);
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
        cerr << "Khong tim thay " << path;
        exit(1);
    }
}

void switchBoard(Sprite& board) {
    boardId = (boardId + 1) % NUM_BOARD;
    board.setTexture(boardImage[boardId]);
}