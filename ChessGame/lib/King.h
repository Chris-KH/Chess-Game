#pragma once
#include"../lib/Pieces.h"

class King : public Pieces {
public:
    King(bool isWhite) : Pieces(isWhite ? "../assets/Cartoon Theme/white-king.png" : "../assets/Cartoon Theme/black-king.png") {
        loadTexture(type);
    }
    string getType() const override { return "King"; }
};