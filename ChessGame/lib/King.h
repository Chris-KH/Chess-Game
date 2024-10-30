#pragma once
#include"../lib/Pieces.h"

class King : public Pieces {
public:
    King(bool isWhite);
    string getType() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<pair<bool, bool>>>& board);
};