#pragma once
#include"../lib/Pieces.h"
#include<memory>

class Rook : public Pieces {
public:
    Rook(bool isWhite, int originCol, int originRow);
    Rook(const Rook& rook) : Pieces(rook) {
        changeTexture(currentTextureIndex);
    };
    string getType() const override;
    char getTypeKey() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board);
    bool checkPromote() const override;
    unique_ptr<Pieces> clone() const override;
};