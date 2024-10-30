#pragma once
#include"../lib/Pieces.h"

class Bishop : public Pieces {
public:
    Bishop(bool isWhite);
    string getType() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<pair<bool, bool>>>& board);
};