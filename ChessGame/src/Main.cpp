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
        Stockfish stockfish(stockfishPath);
        string response = stockfish.sendCommand("uci");

        // Hiển thị phản hồi
        cout << "Stockfish response:\n" << response << endl;

        // Khởi động cửa sổ SFML
        RenderWindow window(VideoMode(800, 800), "Display Chess Board");

        sf::Texture chessboard_image;
        if (!chessboard_image.loadFromFile("../assets/Chess Board/ChessBoard1.png")) {
            cerr << "Cannot load image" << endl;
            return 0;
        }

        sf::Sprite chessboard;
        chessboard.setTexture(chessboard_image);
        chessboard.setPosition(0, 0);
        chessboard.setScale(0.5, 0.5);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(chessboard);
            window.display();
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}