#pragma once
#include"../lib/Pieces.h"

class Rook : public Pieces {
public:
    Rook(bool isWhite) : Pieces(isWhite ? "../assets/Cartoon Theme/white-rook.png" : "../assets/Cartoon Theme/black-rook.png") {
        loadTexture(type);
    }
    string getType() const override { return "Rook"; }
};