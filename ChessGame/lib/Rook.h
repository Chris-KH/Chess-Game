#pragma once
#include"../lib/Pieces.h"

class Rook : public Pieces {
public:
    Rook(bool isWhite);
    string getType() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<pair<bool, bool>>>& board);
};