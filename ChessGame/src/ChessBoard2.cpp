#include"../lib/ChessBoard.h"

string ChessBoard::generateFEN() {
    string fen = "position fen ";

    //Position
    for (int i = 0; i < 8; i++) {
        int freePosition = 0;
        for (int j = 0; j < 8; j++) {
            if (board[i][j]) {
                if (freePosition != 0) fen += to_string(freePosition);
                freePosition = 0;
                fen += board[i][j]->getTypeKey();
            }
            else {
                freePosition++;
            }
        }
        if (freePosition != 0) fen += to_string(freePosition);
        if (i != 7) fen += '/';
    }

    //Side to move
    fen += ' ';
    fen += (whiteTurn ? 'w' : 'b');

    //Castling availability
    fen += ' ';
    string castling = "";
    for (int i = 0; i < 2; i++) {
        if (castlingAvailability[i] == true) {
            castling += (i == 0 ? "KQ" : "kq");
        }
    }
    fen += (castling.empty() ? "-" : castling);

    //En passant target square
    fen += ' ';
    fen += (enPassantTargetSquare.empty() ? "-" : enPassantTargetSquare);

    //Halfmove clock
    fen += ' ';
    fen += to_string(haftMoveClock);

    //Fullmove number
    fen += ' ';
    fen += to_string(fullMoveNumber);

    return fen;
}