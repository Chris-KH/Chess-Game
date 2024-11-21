#pragma once
#include"../lib/Pieces.h"

class Bishop : public Pieces {
public:
    Bishop(bool isWhite, int originCol, int originRow);
    Bishop(const Bishop& bishop) : Pieces(bishop) {};
    string getType() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board);
    bool checkPromote() const override;
    unique_ptr<Pieces> clone() const override;
};
