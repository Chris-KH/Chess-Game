#include "../lib/ChessBoard.h"

ChessBoard::ChessBoard() {
    chessBoardId = 0;
    loadImage(chessBoardImage[0], "../assets/Chess Board/ChessBoard1.png");
    loadImage(chessBoardImage[1], "../assets/Chess Board/ChessBoard2.png");
    loadImage(chessBoardImage[2], "../assets/Chess Board/ChessBoard3.png");

    chessBoardSize = chessBoardImage[0].getSize();
    chessBoardSprite.setTexture(chessBoardImage[0]);
}