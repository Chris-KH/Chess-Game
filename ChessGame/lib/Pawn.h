#pragma once
#include"../lib/Pieces.h"

class Pawn : public Pieces {
public:
    Pawn(bool isWhite);
    string getType() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board, bool inCheck);
    bool checkPromote();
};
