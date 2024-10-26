#pragma once
#include"../lib/Pieces.h"

class Queen : public Pieces {
public:
    Queen(bool isWhite);
    string getType() const override { return "Queen"; }
};