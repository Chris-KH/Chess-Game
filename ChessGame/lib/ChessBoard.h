#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <bits/stdc++.h>

#include "GUI.h"
#include "Move.h"
#include "Button.h"

#include"Stockfish.h"

using namespace std;
using namespace sf;

class ChessBoard {
private:
    // Board size
    float cellSize = 100.f; // Size of each square
    float leftMargin = 65.f, topMargin = 65.f; // Size of margin

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

    Font font; // Font of text and number

    Pieces* selectedPiece = nullptr;
    Pieces* pieceFollowingMouse = nullptr;
    vector<RectangleShape> highlightTiles; // List of squares need to highlight
    vector<RectangleShape> highlightTilesJustMove;    

    // Indicate checks
    bool inCheck[2];
    vector<RectangleShape> checkTiles[2];

    // Game over
    // gameOver = 0, 1, 2, and 3 means game is not over, white won, black won, and tie, respectively
    int gameOver;

    // Undo operator
    // Stack store piece move
    vector<Move*> undoStack; //index 0, 1 for start position and 2,3 for destination position
    vector<Move*> redoStack;

    //Chess board state
    bool whiteTurn;
    int fullMoveNumber;
    int haftMoveClock;
    /* 
        [0] for king side white
        [1] for queen side white
        [2] for king side black
        [3] for queen side black
    */
    bool castlingAvailability[4]; 
    string enPassantTargetSquare;
    map<string, int> repState;

    //AI mode
    bool isAI;
    bool humanColor; //true = white, false = black
    bool undoPress;
public:
    // Constructor
    ChessBoard(RenderWindow* win, Stockfish* stockfish, bool isAI = false);
    ~ChessBoard();

    // Updates
    bool loadBoardTexture(const string& filePath);
    void updateBoardScale();
    bool changeBoard(int newIndex);
    bool changePieces(int newIndex);
    void addPiece(unique_ptr<Pieces> piece, int col, int row);
    void setGameOver(int val) { gameOver = val; }
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

    // Handle mouse click or drag
    void getPossibleMoves(Pieces* clickedPiece, vector<pair<int, int>>& vpii);
    void handleMousePress(int mouseX, int mouseY, bool enableClick, bool enableDrag);
    void handleMouseRelease(int mouseX, int mouseY, bool enableClick, bool enableDrag);
    void highlightPossibleMove(Pieces* clickedPiece);

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
    void loadGame(const string& path);

    //Move notation
    string generateLongAlgebraicNotation(Move*& moved);
    string generateMoveNotation(Move*& moved);
    string generateFEN();
    string FENToCheckThreefold();
    void makeMove(const int& lastRow, const int& lastCol, const int& row, const int& col, const vector<pair<int, int>>& possibleMoves, Move*& curMove, char promotionPiece = '0');
    void highLightAfterMove(int lastRow, int lastCol, int row, int col);

    //AI mode
    void setAI(bool isAI);
    bool getAI() const;
    void setHumanColor(bool humanColor);
    bool getHumanColor() const;
    bool getUndoPress() const { return undoPress; };
    bool isAITurn() const;
    tuple<int, int, int ,int, char> processStockfishMove(const string& bestmove);
};
