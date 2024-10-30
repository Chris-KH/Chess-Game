#pragma once
#include"../lib/Pieces.h"

class Queen : public Pieces {
public:
    Queen(bool isWhite);
    string getType() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<pair<bool, bool>>>& board);
};