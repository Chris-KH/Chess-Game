#pragma once
#include"../lib/Pieces.h"

class Knight : public Pieces {
public:
    Knight(bool isWhite);
    string getType() const override { return "Knight"; }
};