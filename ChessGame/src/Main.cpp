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


int main() {
    // Khởi động Stockfish
    string stockfishPath = "stockfish.exe"; // Thay đổi đường dẫn tới stockfish.exe

    try {
        //Stockfish stockfish(stockfishPath);
        

        // Khởi động cửa sổ SFML
        RenderWindow window(VideoMode(800, 800), "Display Chess Board");

        Texture board_image;
        if (!board_image.loadFromFile("../assets/Chess Board/ChessBoard1.png")) {
            cerr << "Cannot load image" << endl;
            return 0;
        }

        Sprite board;
        board.setTexture(board_image);

        Vector2u textureSize = board_image.getSize();

        View view(FloatRect(0, 0, 800, 800));
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