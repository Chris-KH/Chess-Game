#pragma once
#include"Pieces.h"

class Move {
public:
    // Vị trí cũ (x = row, y = col)
    int fromX, fromY;    
    // Vị trí mới (x = row, y = col)
    int toX, toY;                  
    Pieces* pieceMoved;            // Quân cờ được di chuyển
    Pieces* pieceCaptured;         // Quân cờ bị ăn (nếu có)
    bool isCastling;               // Có phải nhập thành không
    bool isPromotion;              // Có phải phong cấp không
    Pieces* promotionPiece;        // Loại quân cờ sau khi phong cấp
    bool isEnPassant;              // Có phải bắt tốt qua đường không
    bool castlingRights[2][2];     // Quyền nhập thành trước nước đi

    Move();
    Move(int fromX, int fromY, int toX, int toY);
    Move(int fromX, int fromY, int toX, int toY, Pieces*& pieceMoved, Pieces*& pieceCaptured);
};