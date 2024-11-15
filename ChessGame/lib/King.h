#pragma once
#include"../lib/Pieces.h"

class King : public Pieces {
public:
    King(bool isWhite, int originCol, int originRow);
    string getType() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board);
    bool checkPromote() const override;
    bool canCastleKingside(const vector<vector<unique_ptr<Pieces>>>& board);
    bool canCastleQueenside(const vector<vector<unique_ptr<Pieces>>>& board);
    void performCastling(bool isKingside, vector<vector<unique_ptr<Pieces>>>& board);
    void attemptCastling(int lastRow, int lastCol, int row, int col, vector<vector<unique_ptr<Pieces>>>& board);
};