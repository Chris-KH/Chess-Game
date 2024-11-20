#include"../lib/Move..h"

Move::Move() : fromX(0), fromY(0), toX(0), toY(0), pieceMoved(nullptr), pieceCaptured(nullptr), promotionPiece(nullptr) {         
    isCastling = false;;
    isPromotion = false;              
    promotionPiece = nullptr;        
    isEnPassant = false;              
    castlingRights[0][0] = false; 
    castlingRights[0][1] = false; 
    castlingRights[1][0] = false;
    castlingRights[1][1] = false;

}

Move::Move(int fromX, int fromY, int toX, int toY)
    : fromX(fromX), fromY(fromY), toX(toX), toY(toY), pieceMoved(nullptr), pieceCaptured(nullptr), promotionPiece(nullptr) {
    isCastling = false;;
    isPromotion = false;
    promotionPiece = nullptr;
    isEnPassant = false;
    castlingRights[0][0] = false;
    castlingRights[0][1] = false;
    castlingRights[1][0] = false;
    castlingRights[1][1] = false;
}

Move::Move(int fromX, int fromY, int toX, int toY, Pieces*& moved, Pieces*& captured, Pieces*& promotionPiece)
    : fromX(fromX), fromY(fromY), toX(toX), toY(toY), pieceMoved(moved), pieceCaptured(captured), promotionPiece(promotionPiece) {
    isCastling = false;;
    isPromotion = false;
    isEnPassant = false;
    castlingRights[0][0] = false;
    castlingRights[0][1] = false;
    castlingRights[1][0] = false;
    castlingRights[1][1] = false;
}


pair<int, int> Move::getFrom() const {
    return make_pair(fromX, fromY);
}

pair<int, int> Move::getTo() const {
    return make_pair(toX, toY);
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

void Move::setPieceMoved(Pieces*& pieceMoved) {
    this->pieceMoved = pieceMoved;
}

void Move::setPieceCaptured(Pieces*& pieceCaptured) {
    this->pieceCaptured = pieceCaptured;
}

void Move::setPromotionPiece(Pieces*& promotionPiece) {
    this->promotionPiece = promotionPiece;
}

Pieces* Move::getPieceMoved() const {
    return this->pieceMoved;
}

Pieces* Move::getPieceCaptured() const {
    return this->pieceCaptured;
}

Pieces* Move::getPromotionPiece() const {
    return this->promotionPiece;
}