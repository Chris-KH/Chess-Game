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
#include "../lib/SideBoard.h"
#include "../lib/StockFish.h"

int main() {
    
    
    try {
        //Game GameEngine;
        //GameEngine.startGame();
        Stockfish stockfish;
        RenderWindow window(VideoMode(1400, 930), "Chess Game", Style::Close | Style::Titlebar);
        Image icon;
        if (!icon.loadFromFile("../assets/Icon/ChessGameIcon.png")) {
            cerr << "Failed to load icon!" << '\n';
            return -1;
        }
        // Set icon for window
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

        ChessBoard chessBoard(&window, &stockfish, false);
        SideBoard sideBoard(&window, &chessBoard);

        RectangleShape background(Vector2f((float)window.getSize().x - (float)window.getSize().y - 6, 165.0f));
        background.setFillColor(Color::Black);
        background.setOutlineThickness(3);
        background.setOutlineColor(Color(255, 140, 0, 255));
        background.setPosition(Vector2f(933.0f, 3.0f));
        window.draw(background);

        //Main loop
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }
            int gameOver = chessBoard.isOver();
            if (chessBoard.isOver() == 0) {
                chessBoard.update(event);
            }
            sideBoard.update(event);
            if (chessBoard.isOver() != 0 && gameOver == 0) {
                window.clear(Color(60, 60, 60, 255));
                window.draw(background);
                chessBoard.draw();
                sideBoard.draw();
                window.display();
                Sleep(1000);
                GUI::gameOver(chessBoard);
            }

            if (chessBoard.getAI() && chessBoard.isAITurn() && !chessBoard.getUndoPress()) {
                window.clear(Color(60, 60, 60, 255));
                window.draw(background);
                chessBoard.draw();
                sideBoard.draw();
                window.display();

                string bestmove = stockfish.calculateBestMoveWithDepth(10,1000);
                tuple<int, int, int, int> movePos = chessBoard.processStockfishMove(bestmove);
                int lastRow = get<0>(movePos);
                int lastCol = get<1>(movePos);
                int row = get<2>(movePos);
                int col = get<3>(movePos);

                Move* curMove = nullptr;

                vector<pair<int, int>> possibleMoves;
                chessBoard.getPossibleMoves(chessBoard.getPieceAtIndex(lastRow, lastCol), possibleMoves);

                chessBoard.makeMove(lastRow, lastCol, row, col, possibleMoves, curMove);

                Sleep(300);

                if (chessBoard.isOver() != 0 && gameOver == 0) {
                    window.clear(Color(60, 60, 60, 255));
                    window.draw(background);
                    chessBoard.draw();
                    sideBoard.draw();
                    window.display();
                    Sleep(1000);
                    GUI::gameOver(chessBoard);
                }
            }

            window.clear(Color(60, 60, 60, 255));
            window.draw(background);
            chessBoard.draw();
            sideBoard.draw();
            window.display();
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}