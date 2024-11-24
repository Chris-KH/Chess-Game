#include"../lib/ChessBoard.h"

//Reset game (new game)
void ChessBoard::newGame() {
    GUI::newGame(*this);

    // CLear old pieces
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j]) board[i][j].reset();
        }
    }

    this->numPieces = size_t(8 * 4);

    // Create pieces
    addPiece(make_unique<Rook>(true, 0, 7), 0, 7);
    addPiece(make_unique<Knight>(true, 1, 7), 1, 7);
    addPiece(make_unique<Bishop>(true, 2, 7), 2, 7);
    addPiece(make_unique<Queen>(true, 3, 7), 3, 7);
    addPiece(make_unique<King>(true, 4, 7), 4, 7);
    addPiece(make_unique<Bishop>(true, 5, 7), 5, 7);
    addPiece(make_unique<Knight>(true, 6, 7), 6, 7);
    addPiece(make_unique<Rook>(true, 7, 7), 7, 7);

    addPiece(make_unique<Pawn>(true, 0, 6), 0, 6);
    addPiece(make_unique<Pawn>(true, 1, 6), 1, 6);
    addPiece(make_unique<Pawn>(true, 2, 6), 2, 6);
    addPiece(make_unique<Pawn>(true, 3, 6), 3, 6);
    addPiece(make_unique<Pawn>(true, 4, 6), 4, 6);
    addPiece(make_unique<Pawn>(true, 5, 6), 5, 6);
    addPiece(make_unique<Pawn>(true, 6, 6), 6, 6);
    addPiece(make_unique<Pawn>(true, 7, 6), 7, 6);

    addPiece(make_unique<Rook>(false, 0, 0), 0, 0);
    addPiece(make_unique<Knight>(false, 1, 0), 1, 0);
    addPiece(make_unique<Bishop>(false, 2, 0), 2, 0);
    addPiece(make_unique<Queen>(false, 3, 0), 3, 0);
    addPiece(make_unique<King>(false, 4, 0), 4, 0);
    addPiece(make_unique<Bishop>(false, 5, 0), 5, 0);
    addPiece(make_unique<Knight>(false, 6, 0), 6, 0);
    addPiece(make_unique<Rook>(false, 7, 0), 7, 0);

    addPiece(make_unique<Pawn>(false, 0, 1), 0, 1);
    addPiece(make_unique<Pawn>(false, 1, 1), 1, 1);
    addPiece(make_unique<Pawn>(false, 2, 1), 2, 1);
    addPiece(make_unique<Pawn>(false, 3, 1), 3, 1);
    addPiece(make_unique<Pawn>(false, 4, 1), 4, 1);
    addPiece(make_unique<Pawn>(false, 5, 1), 5, 1);
    addPiece(make_unique<Pawn>(false, 6, 1), 6, 1);
    addPiece(make_unique<Pawn>(false, 7, 1), 7, 1);

    selectedPiece = pieceFollowingMouse = nullptr;
    highlightTiles.clear();

    // Player turn
    whiteTurn = true;
    fullMoveNumber = 1;
    haftMoveClock = 0;
    for (int i = 0; i < 2; i++) castlingAvailability[i] = true;
    enPassantTargetSquare = "";

    // Check
    inCheck[0] = inCheck[1] = false;

    // Game Over
    gameOver = false;

    // Clear stack
    freeUndoStack();
    freeRedoStack();

    //
    justMovePiece = nullptr;
    selectedPiece = nullptr;
    pieceFollowingMouse = nullptr;
    highlightTiles.clear();
    selectedBut = nullptr;
}

//Save game
void ChessBoard::saveGame() {
    /*
        This feature i think we will use Algebraic Notation to store the current chess board
        Information about castling, promotion, en passant...

        Reply:
            Yes, digitalize the pieces: 1 = king, 2 = queen, 3 = bishop, 4 = knight, 5 = rook, and 6 = pawn
            Castling, promotion, en passant -> No idea yet
    */
}

//Load game
void ChessBoard::loadGame() {

}

string ChessBoard::generateMoveNotation() {
    return "";
}

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


void ChessBoard::makeMove(const int& lastRow, const int& lastCol, const int& row, const int& col, const vector<pair<int, int>>& possibleMoves) {
    cout << generateFEN() << '\n';
    if (whiteTurn == false) fullMoveNumber++;
    alterTurn();

    // Đặt quân cờ từ ô cũ đến ô hiện tại
    {

        //Move
        Move* curMove = new Move(lastRow, lastCol, row, col);
        unique_ptr<Pieces> deletePiece = move(board[row][col]);

        if (board[lastRow][lastCol]->getType() == "pawn" || deletePiece) haftMoveClock = 0;
        else haftMoveClock++;

        curMove->setPieceMoved(board[lastRow][lastCol]);
        if (deletePiece) curMove->setPieceCaptured(deletePiece);

        board[row][col].reset();
        board[row][col] = move(board[lastRow][lastCol]);
        board[row][col]->setPosition(col, row);

        //Lưu quân tốt này có thể bắt tốt qua đường bắt ở bên nào bên nào không
        if (board[row][col]->getType() == "pawn" && (lastRow == 3 || lastRow == 4)) {
            int direct = (board[row][col]->getColor() ? -1 : 1);
            if ((lastCol - 1) >= 0 && !board[lastRow + direct][lastCol - 1] && find(possibleMoves.begin(), possibleMoves.end(), make_pair(lastRow + direct, lastCol - 1)) != possibleMoves.end()) {
                curMove->setEnPassantLeft('l');
            }
            else if ((lastCol + 1) < 8 && !board[lastRow + direct][lastCol + 1] && find(possibleMoves.begin(), possibleMoves.end(), make_pair(lastRow + direct, lastCol + 1)) != possibleMoves.end()) {
                curMove->setEnPassantLeft('r');
            }
        }

        //Check enPassant
        if (board[row][col]->getType() == "pawn" && abs(col - lastCol) == 1 && !deletePiece) {
            if (board[row][col]->getColor()) {
                curMove->setPieceCaptured(board[row + 1][col]);
                curMove->setEnPassant(true);
                board[row + 1][col].reset();
            }
            if (!board[row][col]->getColor()) {
                curMove->setPieceCaptured(board[row - 1][col]);
                curMove->setEnPassant(true);
                board[row - 1][col].reset();
            }
        }

        //Check Castling
        else if (board[row][col]->getType() == "king" && board[row][col]->getAlreadyMove(lastRow, lastCol) == false) {
            int moveDisplacement = col - lastCol;
            if (moveDisplacement == 2) {
                curMove->setCastling(true);
                curMove->setIsKingSide(true);
                board[row][col]->attemptCastling(board, true);
                castlingAvailability[!board[row][col]->getColor()] = false;
            }
            else if (moveDisplacement == -2) {
                curMove->setCastling(true);
                curMove->setIsKingSide(false);
                board[row][col]->attemptCastling(board, false);
                castlingAvailability[!board[row][col]->getColor()] = false;
            }
        }

        // Check promotion
        else if (board[row][col]->getType() == "pawn" && board[row][col]->checkPromote()) {
            // Vẽ lại giao diện để hiển thị quân mới di chuyển
            highlightTiles.clear();
            window->clear();
            draw();    // Giả sử hàm này vẽ lại toàn bộ bàn cờ và quân cờ
            window->display();
            unique_ptr<Pieces> promotePiece = GUI::promoteChoice(board[row][col]);

            // Nếu không chọn được quân thăng cấp, mặc định là quân Hậu
            if (!promotePiece) promotePiece = make_unique<Queen>(board[row][col]->getColor(), col, row);

            promotePiece->changeTexture(board[row][col]->getCurrentTextureIndex());
            board[row][col].reset();
            board[row][col] = move(promotePiece);
            board[row][col]->setPosition(col, row);

            curMove->setPromotion(true);
            curMove->setPromotionPiece(board[row][col]);
        }

        undoStack.push_back(curMove);
        freeRedoStack();
        if (justMovePiece) justMovePiece->setJustMove(false);
        this->justMovePiece = board[row][col].get();
        if (justMovePiece) justMovePiece->setJustMove(true);
    }

    // Bỏ chọn quân cờ này
    board[row][col]->resetNumPress();
    highlightTiles.clear();
    selectedPiece = nullptr;
}