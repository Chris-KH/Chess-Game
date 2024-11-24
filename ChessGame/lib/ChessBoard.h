#pragma once
#include "GUI.h"
#include "Move.h"
#include "Button.h"

#include"Stockfish.h"

class ChessBoard {
private:
    Stockfish* stockfish;

    RenderWindow* window; // Cửa sổ SFML
    Texture boardTexture; // Texture của bàn cờ
    Sprite boardSprite;   // Sprite để vẽ bàn cờ
    vector<string> boardFiles; // Danh sách đường dẫn bàn cờ
    vector<string> boardName; // Name of each corresponding board
    int currentBoardIndex; // Chỉ số bàn cờ hiện tại

    size_t numPieces;
    vector<vector<unique_ptr<Pieces>>> board;
    Pieces* justMovePiece = nullptr;

    float cellSize = 100; // Kích thước mỗi ô

    Font font; // Phông chữ để vẽ chữ và số

    Pieces* selectedPiece = nullptr;
    Pieces* pieceFollowingMouse = nullptr;
    vector<RectangleShape> highlightTiles; // Danh sách ô cần tô màu

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
    vector<Move*> undoStack; //index 0, 1 for start position and 2,3 for destination position
    vector<Move*> redoStack;

    // Buttons
    Button undoBut, redoBut, saveBut, newBut, surrenderBut, settingBut;
    Button* selectedBut = nullptr;

    //Chess board state
    bool whiteTurn;
    int fullMoveNumber;
    int haftMoveClock;
    /* 
        [0] for white
        [1] for black
    */
    bool castlingAvailability[2]; 
    string enPassantTargetSquare;

public:
    // Constructor
    ChessBoard(RenderWindow* win, Stockfish* stockfish, int currentBoardIndex = 0);
    ~ChessBoard();

    // Update các kiểu
    bool loadBoardTexture(const string& filePath);
    void updateBoardScale();
    bool changeBoard(int newIndex);
    void changePieces(int newIndex);
    void addPiece(unique_ptr<Pieces> piece, int col, int row);
    size_t countPieces();

    // Get
    vector<std::string>& getBoardList(void) { return boardName; }
    bool isWhiteTurn(void) const { return this->whiteTurn; }
    int isOver(void) const { return gameOver; }

    // Draw
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
    void alterTurn(void); // Đổi lượt

    // Detect check, checkmate, draw
    bool isCheck(bool color, bool save); // To check if current player is in check
    bool cannotMove(void); // To check if current player is in checkmate
    bool isTie(void); // To check if current player is in draw

    // Undo-feature
    void undoMove();
    void redoMove();
    void freeUndoStack();
    void freeRedoStack();


    //In ChessBoard2.cpp
    //Reset game (new game)
    void newGame();

    //Save game
    void saveGame();

    //Load game
    void loadGame();

    //Move notation
    string generateMoveNotation();
    string generateFEN();
    void makeMove(const int& lastRow, const int& lastCol, const int& row, const int& col, const vector<pair<int, int>>& possibleMoves);
};