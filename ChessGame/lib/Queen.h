#pragma once
#include"../lib/Pieces.h"

class Queen : public Pieces {
public:
    Queen(bool isWhite) : Pieces(isWhite ? "../assets/Cartoon Theme/white-queen.png" : "../assets/Cartoon Theme/black-queen.png") {
        loadTexture(type);
    }
    string getType() const override { return "Queen"; }
};