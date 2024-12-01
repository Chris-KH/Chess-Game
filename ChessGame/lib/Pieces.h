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

#include "Movement.h"

class Pieces {
protected:
    // Texture texture; // Texture cho quân cờ
    Sprite sprite;   // Sprite để vẽ quân cờ
    string type;    // Loại quân cờ (vd: "pawn", "rook", "knight", ...)
    char typeKey;

    // Theme
    size_t currentTextureIndex;
    vector<Texture> textures;
    bool isWhite;
    bool justMove = false;
    
    // Movement
    Vector2f initialPosition; // Vị trí ban đầu của quân cờ
    Movement movement;
    bool chosen;
    int numPress = 0;

    //Original Postion
    int originCol = 0, originRow = 0;
    size_t isAlreadyMove;
    // Current position
    int col = 0, row = 0;
public:
    Pieces();
    //Pieces(const bool& isWhite);
    Pieces(bool isWhite, int originCol, int originRow);
    Pieces(const Pieces& piece);

    virtual ~Pieces() = default;

    bool getColor() const;
    int getCol() const;
    int getRow() const;
    size_t getCurrentTextureIndex() const;
    size_t getAlreadyMove() const;
    void setAlreadyMove(size_t isAlreadyMove);
    bool getJustMove() const { return justMove; }
    void setJustMove(bool justMove) { this->justMove = justMove; }

    bool loadTexture(const vector<string>& texturePaths);
    void changeTexture(size_t index);
    void scaleToFitCell(float cellSize);
    void setPosition(int col, int row, float cellSize = 100);
    void draw(RenderWindow& window);
    virtual string getType() const = 0;
    virtual char getTypeKey() const = 0;

    // Drag/click a piece
    int getNumPress(void) { return numPress; } // return variable numPress
    void press(void) { numPress++;  } // increase variable numPress by 1
    void resetNumPress(void) { numPress = 0; } // set variable numPress = 0
    void followMouse(Vector2i mousePos); // set position of the piece to the mouse's position
    void unfollowMouse(void);

    //Possible move
    virtual vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board) = 0;
    bool canMoveTo(const int& row, const int& col, const vector<pair<int, int>>& possibleMoves);
    bool isThreatened(int row, int col, const vector<vector<unique_ptr<Pieces>>>& board) const;

    //Promotion
    virtual bool checkPromote() const = 0;
    virtual void attemptCastling(vector<vector<unique_ptr<Pieces>>>& board, bool isKingSide) { return; };
    virtual pair<int, int> enPassant(const vector<vector<unique_ptr<Pieces>>>& board) { return {}; };

    virtual unique_ptr<Pieces> clone() const = 0;
};


