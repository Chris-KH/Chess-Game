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
#include "../lib/Pawn.h"
#include "../lib/King.h"
#include "../lib/Queen.h"
#include "../lib/Rook.h"
#include "../lib/Knight.h"
#include "../lib/Bishop.h"

int main() {
    // Khởi động Stockfish
    
    try {
        sf::RenderWindow window(sf::VideoMode(800, 800), "Chess Game");

        // Danh sách các đường dẫn bàn cờ
        vector<string> boardPaths = {
            "../assets/Chess Board/ChessBoard1.png",
            "../assets/Chess Board/ChessBoard2.png",
            "../assets/Chess Board/ChessBoard3.png"
        };

        // Tạo bàn cờ với các đường dẫn đã cho
        ChessBoard chessBoard(&window, boardPaths);

        // Tạo quân cờ
        chessBoard.addPiece(make_shared<Rook>(true), 0, 7); // Xe trắng
        chessBoard.addPiece(make_shared<Knight>(true), 1, 7); // Mã trắng
        chessBoard.addPiece(make_shared<Bishop>(true), 2, 7); // Tượng trắng
        chessBoard.addPiece(make_shared<Queen>(true), 3, 7); // Hậu trắng
        chessBoard.addPiece(make_shared<King>(true), 4, 7); // Vua trắng
        chessBoard.addPiece(make_shared<Bishop>(true), 5, 7); // Tượng trắng
        chessBoard.addPiece(make_shared<Knight>(true), 6, 7); // Mã trắng
        chessBoard.addPiece(make_shared<Rook>(true), 7, 7); // Xe trắng

        chessBoard.addPiece(make_shared<Rook>(false), 0, 0); // Xe đen
        chessBoard.addPiece(make_shared<Knight>(false), 1, 0); // Mã đen
        chessBoard.addPiece(make_shared<Bishop>(false), 2, 0); // Tượng đen
        chessBoard.addPiece(make_shared<Queen>(false), 3, 0); // Hậu đen
        chessBoard.addPiece(make_shared<King>(false), 4, 0); // Vua đen
        chessBoard.addPiece(make_shared<Bishop>(false), 5, 0); // Tượng đen
        chessBoard.addPiece(make_shared<Knight>(false), 6, 0); // Mã đen
        chessBoard.addPiece(make_shared<Rook>(false), 7, 0); // Xe đen

        chessBoard.addPiece(make_shared<Pawn>(false), 0, 1); 
        chessBoard.addPiece(make_shared<Pawn>(false), 1, 1); 
        chessBoard.addPiece(make_shared<Pawn>(false), 2, 1); 
        chessBoard.addPiece(make_shared<Pawn>(false), 3, 1); 
        chessBoard.addPiece(make_shared<Pawn>(false), 4, 1); 
        chessBoard.addPiece(make_shared<Pawn>(false), 5, 1); 
        chessBoard.addPiece(make_shared<Pawn>(false), 6, 1); 
        chessBoard.addPiece(make_shared<Pawn>(false), 7, 1); 

        chessBoard.addPiece(make_shared<Pawn>(true), 0, 6); 
        chessBoard.addPiece(make_shared<Pawn>(true), 1, 6);
        chessBoard.addPiece(make_shared<Pawn>(true), 2, 6);
        chessBoard.addPiece(make_shared<Pawn>(true), 3, 6);
        chessBoard.addPiece(make_shared<Pawn>(true), 4, 6);
        chessBoard.addPiece(make_shared<Pawn>(true), 5, 6);
        chessBoard.addPiece(make_shared<Pawn>(true), 6, 6);
        chessBoard.addPiece(make_shared<Pawn>(true), 7, 6);

        // Vòng lặp chính của chương trình
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                // Chẳng hạn thay đổi bàn cờ khi nhấn phím 1, 2, hoặc 3
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Num1) {
                        chessBoard.changeBoard(0); // Thay đổi sang bàn cờ 1
                    }
                    else if (event.key.code == sf::Keyboard::Num2) {
                        chessBoard.changeBoard(1); // Thay đổi sang bàn cờ 2
                    }
                    else if (event.key.code == sf::Keyboard::Num3) {
                        chessBoard.changeBoard(2); // Thay đổi sang bàn cờ 3
                    }
                }
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