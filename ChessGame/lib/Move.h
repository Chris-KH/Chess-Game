#pragma once
#include"Pieces.h"

class Move {
private:
    // Old position (x = row, y = col)
    int fromX, fromY;
    // New position (x = row, y = col)
    int toX, toY;
    Pieces* pieceMoved;            // Moved piece
    Pieces* pieceCaptured;         // Captured piece 
    bool isCastling;               // Is a castling move
    bool isPromotion;              // Is a protion move
    Pieces* promotionPiece;        // Type of piece is promoted
    bool isEnPassant;              // Is an en passant move

    /*
    castlingRights[0][0]: White's king-side castling right (short castling).
    castlingRights[0][1]: White's queen-side castling right (long castling).
    castlingRights[1][0]: Black's king-side castling right (short castling).
    castlingRights[1][1]: Black's queen-side castling right (long castling).
    */
    bool castlingRights[2][2];     
public:
    Move();
    Move(int fromX, int fromY, int toX, int toY);
    Move(int fromX, int fromY, int toX, int toY, Pieces*& pieceMoved, Pieces*& pieceCaptured, Pieces*& promotionPiece);
    pair<int, int> getFrom() const;
    pair<int, int> getTo() const;
    void setCastling(bool isCastling);
    void setPromotion(bool isPromotion);
    void setEnPassant(bool isEnPassant);
    bool getCastling() const;
    bool getPromotion() const;
    bool getEnPassant() const;

    void setPieceMoved(Pieces*& pieceMoved);
    void setPieceCaptured(Pieces*& pieceCaptured);
    void setPromotionPiece(Pieces*& promotionPiece);

    Pieces* getPieceMoved() const;
    Pieces* getPieceCaptured() const;
    Pieces* getPromotionPiece() const;
};