#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <bits/stdc++.h>

using namespace sf;

#include"Pieces.h"
#include "Pawn.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"

class ChessBoard {
private:
    RenderWindow* window; // Cửa sổ SFML
    Texture boardTexture; // Texture của bàn cờ
    Sprite boardSprite;   // Sprite để vẽ bàn cờ
    vector<string> boardFiles; // Danh sách đường dẫn bàn cờ
    int currentBoardIndex; // Chỉ số bàn cờ hiện tại

    vector<vector<unique_ptr<Pieces>>> board;

    float cellSize = 100; // Kích thước mỗi ô

    Font font; // Phông chữ để vẽ chữ và số
    Text rowLabels[8]; // Mảng chứa các chữ số cho hàng
    Text colLabels[8]; // Mảng chứa các chữ cái cho cột
    RectangleShape border;
public:
    // Constructor
    ChessBoard(RenderWindow* win, int currentBoardIndex = 0);

    // Update các kiểu
    bool loadBoardTexture(const string& filePath);
    void updateBoardScale();
    bool changeBoard(int newIndex);
    void addPiece(unique_ptr<Pieces> piece, int col, int row);

    void update(const sf::Event& ev);
    void draw();

    //Highline possible move
    //void showPossibleMoves(int col, int row);
    //void highlightSquare(int col, int row);
};