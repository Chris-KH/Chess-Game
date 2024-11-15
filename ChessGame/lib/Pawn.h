#pragma once
#include"../lib/Pieces.h"

class Pawn : public Pieces {
public:
    Pawn(bool isWhite, int originRow, int originCol);
    string getType() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board);
    bool checkPromote() const override;
    pair<int, int> enPassant(const vector<vector<unique_ptr<Pieces>>>& board);
};


