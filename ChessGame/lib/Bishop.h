#pragma once
#include"../lib/Pieces.h"

class Bishop : public Pieces {
public:
    Bishop(bool isWhite);
    string getType() const override { return "Bishop"; }
};