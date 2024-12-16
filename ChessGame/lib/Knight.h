#pragma once
#include"../lib/Pieces.h"
#include<memory>

class Knight : public Pieces {
public:
    Knight(bool isWhite, int originCol, int originRow);
    Knight(const Knight& knight) : Pieces(knight) {
        changeTexture(currentTextureIndex);
    };
    string getType() const override;
    char getTypeKey() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board);
    bool checkPromote() const override;
    unique_ptr<Pieces> clone() const override;
};