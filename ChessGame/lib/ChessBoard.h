#pragma once
#include "GUI.h"
#include "Move.h"
#include "Button.h"

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
    /*
    * gameOver = 0: the game is not over
    * gameOver = 1: white won
    * gameOver = 2: black won
    * gameOver = 3: taiwan
    */
    int gameOver;

    // Undo operator
    // Stack lưu trạng thái bàn cờ
    vector<Move> undoStack; //index 0, 1 for start position and 2,3 for destination position
    vector<Move> redoStack;

    // Buttons
    Button undoBut, redoBut, saveBut, newBut, surrenderBut, settingsBut;
    Button* selectedBut = nullptr;
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
    void handleButtonPress(int mouseX, int mouseY);
    void handleButtonRelease(int mouseX, int mouseY);

    // Player turn
    bool isWhiteTurn(void) const { return this->whiteTurn; }
    void alterTurn(void); // Đổi lượt

    // Detect check, checkmate, draw
    bool isCheck(bool color, bool save); // To check if current player is in check
    bool cannotMove(void); // To check if current player is in checkmate
    bool isTie(void); // To check if current player is in draw
    int isOver(void) const { return gameOver; }

    // Undo-feature
    void undoMove();
    void redoMove();

    //Reset game (new game)
    void newtGame();

    //Save game
    void saveGame();

    //Load game
    void loadGame();
};