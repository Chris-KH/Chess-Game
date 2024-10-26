#pragma once
#include"../lib/Pieces.h"

class Bishop : public Pieces {
public:
    Bishop(bool isWhite) : Pieces(isWhite ? "../assets/Cartoon Theme/white-bishop.png" : "../assets/Cartoon Theme/black-bishop.png") {
        loadTexture(type);
    }
    string getType() const override { return "Bishop"; }
};