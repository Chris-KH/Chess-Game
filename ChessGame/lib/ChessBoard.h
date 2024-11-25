#pragma once
#include "GUI.h"
#include "Move.h"
#include "Button.h"

#include"Stockfish.h"

class ChessBoard {
private:
    Stockfish* stockfish;

    RenderWindow* window; // SFML window
    Texture boardTexture; // Texture of chessBoard
    Sprite boardSprite;   // Sprite of chessBoard
    vector<string> boardFiles; // Path list of chessBoard
    vector<string> boardName; // Name of each corresponding board
    vector<string> pieceName; // Name of each corresponding board
    int currentBoardIndex; // board indexing
    int currentPieceIndex; // piece index

    size_t numPieces;
    vector<vector<unique_ptr<Pieces>>> board;
    Pieces* justMovePiece = nullptr;

    float cellSize = 100; // Size of each square

    Font font; // Font of text and number

    Pieces* selectedPiece = nullptr;
    Pieces* pieceFollowingMouse = nullptr;
    vector<RectangleShape> highlightTiles; // List of squares need to highlight

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
    // Stack store piece move
    vector<Move*> undoStack; //index 0, 1 for start position and 2,3 for destination position
    vector<Move*> redoStack;

    // Buttons
    vector<unique_ptr<Button>> buttonList;
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

    //AI mode
    bool isAI;
    bool humanColor; //true = white, false = black
    bool undoPress;
public:
    // Constructor
    ChessBoard(RenderWindow* win, Stockfish* stockfish, int currentBoardIndex = 0, bool isAI = false);
    ~ChessBoard();

    // Update các kiểu
    bool loadBoardTexture(const string& filePath);
    void updateBoardScale();
    bool changeBoard(int newIndex);
    void changePieces(int newIndex);
    void addPiece(unique_ptr<Pieces> piece, int col, int row);
    size_t countPieces();

    // Get
    vector<string>& getBoardList(void) { return boardName; }
    vector<string>& getPieceList(void) { return pieceName; }
    bool isWhiteTurn(void) const { return this->whiteTurn; }
    int isOver(void) const { return gameOver; }
    int getBoardIndex(void) const { return currentBoardIndex; }
    int getPieceIndex(void) const { return currentPieceIndex; }
    Sprite getBoardSprite(void) const { return boardSprite; }
    Pieces* getPieceAtIndex(int row, int col);

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

    //In ChessBoard2.cpp
    // Undo-feature
    void undoMove();
    void redoMove();
    void freeUndoStack();
    void freeRedoStack();

    //Reset game (new game)
    void newGame();

    //Save game
    bool saveGame(const string& path);

    //Load game
    void loadGame(/*const string& path*/);

    //Move notation
    string generateLongAlgebraicNotation(Move*& moved);
    string generateMoveNotation(Move*& moved);
    string generateFEN();
    void makeMove(const int& lastRow, const int& lastCol, const int& row, const int& col, const vector<pair<int, int>>& possibleMoves, Move*& curMove);

    //AI mode
    void setAI(bool isAI);
    bool getAI() const;
    void setHumanColor(bool humanColor);
    bool getHumanColor() const;
    bool getUndoPress() const { return undoPress; };
    bool isAITurn() const;
    tuple<int, int, int ,int> processStockfishMove(const string& bestmove);
};
