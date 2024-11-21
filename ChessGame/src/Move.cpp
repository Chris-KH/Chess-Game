#include"../lib/Move.h"

Move::Move() : fromRow(0), fromCol(0), toRow(0), toCol(0), pieceMoved(nullptr), pieceCaptured(nullptr), promotionPiece(nullptr) {         
    isCastling = false;
    isPromotion = false;                     
    isEnPassant = false;              
    castlingRights[0][0] = false; 
    castlingRights[0][1] = false; 
    castlingRights[1][0] = false;
    castlingRights[1][1] = false;

}

Move::Move(int fromRow, int fromCol, int toRow, int toCol)
    : fromRow(fromRow), fromCol(fromCol), toRow(toRow), toCol(toCol), pieceMoved(nullptr), pieceCaptured(nullptr), promotionPiece(nullptr) {
    isCastling = false;
    isPromotion = false;
    isEnPassant = false;
    castlingRights[0][0] = false;
    castlingRights[0][1] = false;
    castlingRights[1][0] = false;
    castlingRights[1][1] = false;
}

Move::Move(const Move& move)
    : fromRow(move.fromRow), fromCol(move.fromCol), toRow(move.toRow), toCol(move.toCol), isCastling(move.isCastling), isPromotion(move.isPromotion), isEnPassant(move.isEnPassant) {

    castlingRights[0][0] = move.castlingRights[0][0];
    castlingRights[0][1] = move.castlingRights[0][1];
    castlingRights[1][0] = move.castlingRights[1][0];
    castlingRights[1][1] = move.castlingRights[1][1];

    pieceMoved = move.pieceMoved->clone();
    pieceCaptured = move.pieceCaptured->clone();
    promotionPiece = move.promotionPiece->clone();
}

pair<int, int> Move::getFrom() const {
    return make_pair(fromRow, fromCol);
}

pair<int, int> Move::getTo() const {
    return make_pair(toRow, toCol);
}

void Move::setCastling(bool isCastling) {
    this->isCastling = isCastling;
}

void Move::setPromotion(bool isPromotion) {
    this->isPromotion = isPromotion;
}

void Move::setEnPassant(bool isEnPassant) {
    this->isEnPassant = isEnPassant;
}

bool Move::getCastling() const {
    return this->isCastling;
}

bool Move::getPromotion() const {
    return this->isPromotion;
}

bool Move::getEnPassant() const {
    return this->isEnPassant;
}

void Move::setPieceMoved(unique_ptr<Pieces>& pieceMoved) {
    this->pieceMoved = pieceMoved->clone();
}

void Move::setPieceCaptured(unique_ptr<Pieces>& pieceCaptured) {
    this->pieceCaptured = pieceCaptured->clone();
}

void Move::setPromotionPiece(unique_ptr<Pieces>& promotionPiece) {
    this->promotionPiece = promotionPiece->clone();
}

Pieces* Move::getPieceMoved() const {
    return this->pieceMoved.get();
}

Pieces* Move::getPieceCaptured() const {
    return this->pieceCaptured.get();
}

Pieces* Move::getPromotionPiece() const {
    return this->promotionPiece.get();
}
