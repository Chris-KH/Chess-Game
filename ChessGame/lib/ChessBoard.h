#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <bits/stdc++.h>

using namespace sf;

#include"../lib/Pieces.h"

class ChessBoard {
private:
    RenderWindow* window; // Cửa sổ SFML
    Texture boardTexture; // Texture của bàn cờ
    Sprite boardSprite;   // Sprite để vẽ bàn cờ
    vector<shared_ptr<Pieces>> pieces; // Danh sách quân cờ
    vector<std::string> boardFiles; // Danh sách đường dẫn bàn cờ
    int currentBoardIndex; // Chỉ số bàn cờ hiện tại

    float squareSize; // Kích thước mỗi ô
    Font font; // Phông chữ để vẽ chữ và số
    Text rowLabels[8]; // Mảng chứa các chữ số cho hàng
    Text colLabels[8]; // Mảng chứa các chữ cái cho cột
    RectangleShape border;
public:
    // Constructor
    ChessBoard(RenderWindow* win, const vector<string>& boardPaths, int currentBoardIndex = 0);

    // Load ảnh bàn cờ từ file
    bool loadBoardTexture(const string& filePath);
    void updateBoardScale();
    bool changeBoard(int newIndex);
    void addPiece(std::shared_ptr<Pieces> piece, int col, int row);
    void draw();
};