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

class Pieces {
protected:
    Texture texture; // Texture cho quân cờ
    Sprite sprite;   // Sprite để vẽ quân cờ
    string type;    // Loại quân cờ (vd: "Pawn", "Rook", "Knight", ...)

public:
    Pieces(const string& type) : type(type) {}

    bool loadTexture(const string& filePath);
    void scaleToFitSquare(float squareSize);
    void setPosition(int col, int row, float squareSize);
    void draw(RenderWindow& window);
    virtual string getType() const = 0;
};

