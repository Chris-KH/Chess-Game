#pragma once
#include"../lib/Pieces.h"
#include<memory>

class Pawn : public Pieces {
public:
    Pawn(bool isWhite, int originCol, int originRow);
    Pawn(const Pawn& pawn) : Pieces(pawn) {
        changeTexture(currentTextureIndex);
    };
    string getType() const override;
    char getTypeKey() const override;
    vector<pair<int, int>> getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board);
    bool checkPromote() const override;
    pair<int, int> enPassant(const vector<vector<unique_ptr<Pieces>>>& board);
    unique_ptr<Pieces> clone() const override;
};


