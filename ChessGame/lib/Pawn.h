#pragma once
#include"../lib/Pieces.h"

class Pawn : public Pieces {
public:
    Pawn(bool isWhite) : Pieces(isWhite ? "../assets/Cartoon Theme/white-pawn.png" : "../assets/Cartoon Theme/black-pawn.png") {
        loadTexture(type);
    }
    string getType() const override { return "Pawn"; }
};
