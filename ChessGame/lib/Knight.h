#pragma once
#include"../lib/Pieces.h"

class Knight : public Pieces {
public:
    Knight(bool isWhite) : Pieces(isWhite ? "../assets/Cartoon Theme/white-knight.png" : "../assets/Cartoon Theme/black-knight.png") {
        loadTexture(type);
    }
    string getType() const override { return "Knight"; }
};