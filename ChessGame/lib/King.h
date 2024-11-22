#pragma once
#include"../lib/Pieces.h"

class King : public Pieces {
public:
    King(bool isWhite, int originCol, int originRow);
    King(const King& king) : Pieces(king) {
        changeTexture(currentTextureIndex);
    };
    string getType() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board);
    bool checkPromote() const override;
    bool canCastleKingside(const vector<vector<unique_ptr<Pieces>>>& board);
    bool canCastleQueenside(const vector<vector<unique_ptr<Pieces>>>& board);
    void performCastling(bool isKingside, vector<vector<unique_ptr<Pieces>>>& board);
    void attemptCastling(vector<vector<unique_ptr<Pieces>>>& board, bool isKingSide);
    unique_ptr<Pieces> clone() const override;
};