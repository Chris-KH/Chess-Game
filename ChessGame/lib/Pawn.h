#pragma once
#include"../lib/Pieces.h"

class Pawn : public Pieces {
public:
    Pawn(bool isWhite);
    string getType() const override { return "Pawn"; }
};
