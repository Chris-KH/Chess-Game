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

    // Theme
    size_t currentTextureIndex;
    vector<Texture> textures;
    bool isWhite;
    
    // Movement
    Vector2f initialPosition; // Vị trí ban đầu của quân cờ
    vector<pair<int, int>> possibleMoves;
    Movement movement;
    bool chosen;
    int numPress = 0;

    // Current position
    int col = 0, row = 0;
public:
    Pieces();
    //Pieces(const bool& isWhite);
    Pieces(bool isWhite);

    bool getColor() const;
    int getCol() const;
    int getRow() const;

    bool loadTexture(const vector<string>& texturePaths);
    void changeTexture(size_t index);
    void scaleToFitCell(float cellSize);
    void setPosition(int col, int row, float cellSize = 100);
    void draw(RenderWindow& window);
    virtual string getType() const = 0;

    // Drag/click a piece
    void setInitialPosition(const sf::Vector2f& position);
    int getNumPress(void) { return numPress; } // return variable numPress
    void press(void) { numPress++;  } // increase variable numPress by 1
    void resetNumPress(void) { numPress = 0; } // set variable numPress = 0
    void followMouse(sf::Vector2i mousePos); // set position of the piece to the mouse's position

    //Possible move
    virtual vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board, bool inCheck = false) = 0;
};

