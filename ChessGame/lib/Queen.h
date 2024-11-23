#pragma once
#include"../lib/Pieces.h"

class Queen : public Pieces {
public:
    Queen(bool isWhite, int originCol, int originRow);
    Queen(const Queen& queen) : Pieces(queen) {
        changeTexture(currentTextureIndex);
    };
    string getType() const override;
    char getTypeKey() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board);
    bool checkPromote() const override;
    unique_ptr<Pieces> clone() const override;
};