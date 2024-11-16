#include"../lib/Move..h"

Move::Move() {
    fromX = fromY = 0;              
    toX = toY = 0;                  
    pieceMoved = nullptr;            
    pieceCaptured = nullptr;         
    isCastling = false;;
    isPromotion = false;              
    promotionPiece = nullptr;        
    isEnPassant = false;              
    //castlingRights[2][2];    
}

Move::Move(int fromX, int fromY, int toX, int toY): fromX(fromX), fromY(fromY), toX(toX), toY(toY) {
    pieceMoved = nullptr;
    pieceCaptured = nullptr;
    isCastling = false;;
    isPromotion = false;
    promotionPiece = nullptr;
    isEnPassant = false;
    //castlingRights[2][2];
}

Move::Move(int fromX, int fromY, int toX, int toY, Pieces*& moved, Pieces*& captured)
    : fromX(fromX), fromY(fromY), toX(toX), toY(toY), pieceMoved(moved), pieceCaptured(captured) {
    isCastling = false;;
    isPromotion = false;
    promotionPiece = nullptr;
    isEnPassant = false;
    //castlingRights[2][2];
}