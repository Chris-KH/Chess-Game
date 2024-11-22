#pragma once
#include"Pieces.h"

class Move {
private:
    // Old position (x = row, y = col)
    int fromRow, fromCol;
    // New position (x = row, y = col)
    int toRow, toCol;
    unique_ptr<Pieces> pieceMoved;            // Moved piece
    unique_ptr<Pieces> pieceCaptured;         // Captured piece 
    bool isCastling;               // Is a castling move
    bool isPromotion;              // Is a protion move
    unique_ptr<Pieces> promotionPiece;        // Type of piece is promoted
    bool isEnPassant;              // Is an en passant move

    /*
    castlingRights[0][0]: White's king-side castling right (short castling).
    castlingRights[0][1]: White's queen-side castling right (long castling).
    castlingRights[1][0]: Black's king-side castling right (short castling).
    castlingRights[1][1]: Black's queen-side castling right (long castling).
    */
    bool isKingSide;     
public:
    Move();
    Move(int fromRow, int fromCol, int toRow, int toCol);
    Move(const Move& move);
    pair<int, int> getFrom() const;
    pair<int, int> getTo() const;
    void setCastling(bool isCastling);
    void setPromotion(bool isPromotion);
    void setEnPassant(bool isEnPassant);
    void setIsKingSide(bool isKingSide);
    bool getCastling() const;
    bool getPromotion() const;
    bool getEnPassant() const;
    bool getIsKingSide() const;

    void setPieceMoved(unique_ptr<Pieces>& pieceMoved);
    void setPieceCaptured(unique_ptr<Pieces>& pieceCaptured);
    void setPromotionPiece(unique_ptr<Pieces>& promotionPiece);

    Pieces* getPieceMoved() const;
    Pieces* getPieceCaptured() const;
    Pieces* getPromotionPiece() const;
};