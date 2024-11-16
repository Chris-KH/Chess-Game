﻿#pragma once
#include "GUI.h"

class ChessBoard {
private:
    RenderWindow* window; // Cửa sổ SFML
    Texture boardTexture; // Texture của bàn cờ
    Sprite boardSprite;   // Sprite để vẽ bàn cờ
    vector<string> boardFiles; // Danh sách đường dẫn bàn cờ
    int currentBoardIndex; // Chỉ số bàn cờ hiện tại

    size_t numPieces;
    vector<vector<unique_ptr<Pieces>>> board;
    Pieces* justMovePiece = nullptr;

    float cellSize = 100; // Kích thước mỗi ô

    Font font; // Phông chữ để vẽ chữ và số

    Pieces* selectedPiece = nullptr;
    Pieces* pieceFollowingMouse = nullptr;
    vector<RectangleShape> highlightTiles; // Danh sách ô cần tô màu

    // Player turn
    bool whiteTurn;

    // Indicate checks
    bool inCheck[2];
    vector<RectangleShape> checkTiles[2];

    // Game over
    bool gameOver;

    // Undo operator
    // Stack lưu trạng thái bàn cờ
    vector<vector<int>> undoStack; //index 0, 1 for start position and 2,3 for destination position
    vector<vector<int>> redoStack;
    /*
    RectangleShape undoButton;
    Font undoFont;
    Text undoText;
    */
public:
    // Constructor
    ChessBoard(RenderWindow* win, int currentBoardIndex = 0);

    // Update các kiểu
    bool loadBoardTexture(const string& filePath);
    void updateBoardScale();
    bool changeBoard(int newIndex);
    void changePieces(int newIndex);
    void addPiece(unique_ptr<Pieces> piece, int col, int row);
    size_t countPieces();

    void update(const Event& ev);
    void draw();

    // Handle mouse click
    void getPossibleMoves(Pieces* clickedPiece, vector<pair<int, int>>& vpii);
    void handleMousePress(int mouseX, int mouseY);
    void handleMouseRelease(int mouseX, int mouseY);
    void highlightPossibleMove(Pieces* clickedPiece);

    // Player turn
    bool isWhiteTurn(void) const { return this->whiteTurn; }
    void alterTurn(void); // Đổi lượt

    // Detect check, checkmate, draw
    bool isCheck(bool color, bool save); // To check if current player is in check
    bool cannotMove(void); // To check if current player is in checkmate
    bool isTie(void); // To check if current player is in draw
    bool isOver(void) const { return gameOver; }

    // Undo-feature
    void undoMove();
    void redoMove();
};