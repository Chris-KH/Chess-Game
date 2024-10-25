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

class King : public Pieces {
public:
    King(bool isWhite) : Pieces(isWhite ? "../assets/Cartoon Theme/white-king.png" : "../assets/Cartoon Theme/black-king.png") {
        loadTexture(type);
    }
    string getType() const override { return "King"; }
};

class Queen : public Pieces {
public:
    Queen(bool isWhite) : Pieces(isWhite ? "../assets/Cartoon Theme/white-queen.png" : "../assets/Cartoon Theme/black-queen.png") {
        loadTexture(type);
    }
    string getType() const override { return "Queen"; }
};

class Rook : public Pieces {
public:
    Rook(bool isWhite) : Pieces(isWhite ? "../assets/Cartoon Theme/white-rook.png" : "../assets/Cartoon Theme/black-rook.png") {
        loadTexture(type);
    }
    string getType() const override { return "Rook"; }
};

class Bishop : public Pieces {
public:
    Bishop(bool isWhite) : Pieces(isWhite ? "../assets/Cartoon Theme/white-bishop.png" : "../assets/Cartoon Theme/black-bishop.png") {
        loadTexture(type);
    }
    string getType() const override { return "Bishop"; }
};

class Knight : public Pieces {
public:
    Knight(bool isWhite) : Pieces(isWhite ? "../assets/Cartoon Theme/white-knight.png" : "../assets/Cartoon Theme/black-knight.png") {
        loadTexture(type);
    }
    string getType() const override { return "Knight"; }
};

class Pawn : public Pieces {
public:
    Pawn(bool isWhite) : Pieces(isWhite ? "../assets/Cartoon Theme/white-pawn.png" : "../assets/Cartoon Theme/black-pawn.png") {
        loadTexture(type);
    }
    string getType() const override { return "Pawn"; }
};

