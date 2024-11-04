#pragma once
#include"../lib/Pieces.h"

class Knight : public Pieces {
public:
    Knight(bool isWhite);
    string getType() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board, bool inCheck);
};