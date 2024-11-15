#pragma once
#include"../lib/Pieces.h"

class Bishop : public Pieces {
public:
    Bishop(bool isWhite, int originCol, int originRow);
    string getType() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board);
    bool checkPromote() const override;
};