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
    string type;    // Loại quân cờ (vd: "Pawn", "Rook", "Knight", ...)

    // Theme
    size_t currentTextureIndex;
    vector<Texture> textures;
    bool isWhite;
    
    // Movement
    Movement movement;
    bool drag;

    // Current position
    int col = 0, row = 0;
public:
    Pieces();
    //Pieces(const bool& isWhite);
    Pieces(bool isWhite);

    bool getColor() const;

    bool loadTexture(const vector<string>& texturePaths);
    void changeTexture(size_t index);
    void scaleToFitCell(float squareSize);
    void setPosition(int col, int row, float squareSize);
    void draw(RenderWindow& window);
    virtual string getType() const = 0;

    // Drag a piece
    bool getDrag(void);
    void setDrag(bool state);
    bool contain(const sf::Vector2f& point);
    void moveTo(const sf::Vector2f& point);

    // Release a piece
    void toNearestCell(const sf::Vector2f& point);

    //Possible move
    virtual vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board) = 0;
};

