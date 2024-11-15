#pragma once
#include"../lib/Pieces.h"

class Rook : public Pieces {
public:
    Rook(bool isWhite, int originRow, int originCol);
    string getType() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board);
    bool checkPromote() const override;
};