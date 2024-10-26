#pragma once
#include"../lib/Pieces.h"

class Rook : public Pieces {
public:
    Rook(bool isWhite);
    string getType() const override { return "Rook"; }
};