#pragma once
#include"../lib/Pieces.h"

class King : public Pieces {
public:
    King(bool isWhite);
    string getType() const override { return "King"; }
};