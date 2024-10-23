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

void loadImage(Texture& text, string path);

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

        Texture boardImage1, boardImage2;
        loadImage(boardImage1, "../assets/Chess Board/ChessBoard1.png");
        loadImage(boardImage2, "../assets/Chess Board/ChessBoard2.png");

        bool setBoard1 = true;
        Sprite board;
        board.setTexture(boardImage1);

        Vector2u textureSize = boardImage1.getSize();

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
                    if (setBoard1) {
                        setBoard1 = false;
                        board.setTexture(boardImage2);
                    }
                    else {
                        setBoard1 = true;
                        board.setTexture(boardImage1);
                    }
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