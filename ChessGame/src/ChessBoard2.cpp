#include"../lib/ChessBoard.h"

// Undo-feature
void ChessBoard::undoMove() {
    if (undoStack.empty() == true) return;
    Move* move = undoStack.back();
    undoStack.pop_back();
    redoStack.push_back(move);

    highlightTiles.clear();
    window->clear();
    draw();
    window->display();

    if (justMovePiece) justMovePiece->setJustMove(false);
    justMovePiece = nullptr;

    //Processing when press undo
    pair<int, int> fromPosition = move->getFrom();
    pair<int, int> toPosition = move->getTo();

    if (move->getPromotion() == true) {
        board[toPosition.first][toPosition.second].reset();
        if (move->getPieceCaptured()) {
            board[toPosition.first][toPosition.second] = move->getPieceCaptured()->clone();
            board[toPosition.first][toPosition.second]->setPosition(toPosition.second, toPosition.first);
        }

        board[fromPosition.first][fromPosition.second] = move->getPieceMoved()->clone();
        board[fromPosition.first][fromPosition.second]->setPosition(fromPosition.second, fromPosition.first);
    }
    else if (move->getEnPassant() == true) {
        board[fromPosition.first][fromPosition.second] = std::move(board[toPosition.first][toPosition.second]);
        board[fromPosition.first][fromPosition.second]->setPosition(fromPosition.second, fromPosition.first);
        if (board[fromPosition.first][fromPosition.second]->getColor()) {
            board[toPosition.first + 1][toPosition.second] = move->getPieceCaptured()->clone();
            board[toPosition.first + 1][toPosition.second]->setPosition(toPosition.second, toPosition.first + 1);
        }
        else if (!board[fromPosition.first][fromPosition.second]->getColor()) {
            board[toPosition.first - 1][toPosition.second] = move->getPieceCaptured()->clone();
            board[toPosition.first - 1][toPosition.second]->setPosition(toPosition.second, toPosition.first - 1);
        }
    }
    else if (move->getCastling() == true) {
        int castlingRow = (move->getPieceMoved()->getColor() ? 7 : 0);
        if (move->getIsKingSide() == true) {
            //Reset rook move
            board[castlingRow][7] = std::move(board[castlingRow][5]);
            board[castlingRow][7]->setPosition(7, castlingRow);

            //Reset king move
            board[castlingRow][4] = std::move(board[castlingRow][6]);
            board[castlingRow][4]->setPosition(4, castlingRow);
        }
        else {
            //Reset rook move
            board[castlingRow][0] = std::move(board[castlingRow][3]);
            board[castlingRow][0]->setPosition(0, castlingRow);

            //Reset king move
            board[castlingRow][4] = std::move(board[castlingRow][2]);
            board[castlingRow][4]->setPosition(4, castlingRow);
        }
    }
    else {
        board[fromPosition.first][fromPosition.second] = std::move(board[toPosition.first][toPosition.second]);
        board[fromPosition.first][fromPosition.second]->setPosition(fromPosition.second, fromPosition.first);
        if (move->getPieceCaptured()) {
            board[toPosition.first][toPosition.second] = move->getPieceCaptured()->clone();
            board[toPosition.first][toPosition.second]->setPosition(toPosition.second, toPosition.first);
        }
    }

    if (move->getEnPassantLeft() != '0') {
        if (move->getEnPassantLeft() == 'l') {
            justMovePiece = board[fromPosition.first][fromPosition.second - 1].get();
            if (justMovePiece) justMovePiece->setJustMove(true);
        }
        else {
            justMovePiece = board[fromPosition.first][fromPosition.second + 1].get();
            if (justMovePiece) justMovePiece->setJustMove(true);
        }
    }

    //Set turn
    whiteTurn = board[fromPosition.first][fromPosition.second]->getColor();

    this->numPieces = countPieces();

    stockfish->setBoardState(generateFEN());
}

void ChessBoard::redoMove() {
    if (redoStack.empty() == true) return;
    Move* move = redoStack.back();
    redoStack.pop_back();
    undoStack.push_back(move);

    highlightTiles.clear();
    window->clear();
    draw();
    window->display();

    if (justMovePiece) justMovePiece->setJustMove(false);
    justMovePiece = nullptr;

    //Processing when press redo
    pair<int, int> fromPosition = move->getFrom();
    pair<int, int> toPosition = move->getTo();

    //Undo pieceMoved position

    if (move->getPromotion() == true) {
        board[fromPosition.first][fromPosition.second].reset();
        board[toPosition.first][toPosition.second].reset();
        board[toPosition.first][toPosition.second] = move->getPromotionPiece()->clone();
        board[toPosition.first][toPosition.second]->setPosition(toPosition.second, toPosition.first);
    }
    else if (move->getEnPassant() == true) {
        board[toPosition.first][toPosition.second] = std::move(board[fromPosition.first][fromPosition.second]);
        board[toPosition.first][toPosition.second]->setPosition(toPosition.second, toPosition.first);
        if (board[toPosition.first][toPosition.second]->getColor()) {
            board[toPosition.first + 1][toPosition.second].reset();
        }
        if (!board[toPosition.first][toPosition.second]->getColor()) {
            board[toPosition.first - 1][toPosition.second].reset();
        }
    }
    else if (move->getCastling() == true) {
        int castlingRow = (move->getPieceMoved()->getColor() ? 7 : 0);
        if (move->getIsKingSide() == true) {
            //Make rook move
            board[castlingRow][5] = std::move(board[castlingRow][7]);
            board[castlingRow][5]->setPosition(5, castlingRow);

            //Make king move
            board[castlingRow][6] = std::move(board[castlingRow][4]);
            board[castlingRow][6]->setPosition(6, castlingRow);
        }
        else {
            //Make rook move
            board[castlingRow][3] = std::move(board[castlingRow][0]);
            board[castlingRow][3]->setPosition(3, castlingRow);

            //Make king move
            board[castlingRow][2] = std::move(board[castlingRow][4]);
            board[castlingRow][2]->setPosition(2, castlingRow);
        }
    }
    else {
        board[toPosition.first][toPosition.second].reset();
        board[toPosition.first][toPosition.second] = std::move(board[fromPosition.first][fromPosition.second]);
        board[toPosition.first][toPosition.second]->setPosition(toPosition.second, toPosition.first);
    }

    //Set turn
    whiteTurn = !board[toPosition.first][toPosition.second]->getColor();

    this->numPieces = countPieces();

    stockfish->setBoardState(generateFEN());
}

void ChessBoard::freeUndoStack() {
    for (int i = 0; i < (int)undoStack.size(); i++) delete undoStack[i];
    undoStack.clear();
}

void ChessBoard::freeRedoStack() {
    for (int i = 0; i < (int)redoStack.size(); i++) delete redoStack[i];
    redoStack.clear();
}

//Reset game (new game)
void ChessBoard::newGame() {
    if (GUI::newGame(*this) == false) {
        return;
    }

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

    this->stockfish->newGame();
    this->stockfish->setSkillLevel(20);
    this->stockfish->setBoardState(generateFEN());

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
bool ChessBoard::saveGame(const string& path) {
    //Check file existing
    string srcPath = "../save/" + path + ".txt";
    if (filesystem::exists(srcPath)) return false;

    //Create file
    ofstream fout(srcPath);

    //Get FEN (Forsyth-Edwards Notation)
    string FEN = generateFEN();
    fout << FEN << '\n';

    //Get Algebraic Notation
    /*  
        Use undoStack and redoStack to get
        moveNotation (Short algebraic notation)
        LAN (Long algebraic notation)
    */

    return true;
}

//Load game
void ChessBoard::loadGame(/*const string& path*/) {
    GUI::loadGame(*this);
}

string ChessBoard::generateLongAlgebraicNotation(Move*& moved) {
    string LAN = "";
    const char x[9] = "abcdefgh";
    const char y[9] = "87654321";

    pair<int, int> fromPosition = moved->getFrom();
    pair<int, int> toPosition = moved->getTo();

    LAN += x[fromPosition.second];
    LAN += y[fromPosition.first];
    LAN += x[toPosition.second];
    LAN += y[toPosition.first];

    if (moved->getPromotion()) {
        LAN += tolower(moved->getPromotionPiece()->getTypeKey());
    }

    return LAN;
}

string ChessBoard::generateMoveNotation(Move*& moved) {
    string moveNotation = "";
    const char x[9] = "abcdefgh";
    const char y[9] = "12345678";

    pair<int, int> fromPosition = moved->getFrom();
    pair<int, int> toPosition = moved->getTo();

    if (moved->getPromotion()) {
        if (moved->getPieceCaptured()) {
            moveNotation += x[fromPosition.second] + 'x';
        }
        moveNotation += x[toPosition.second] + y[toPosition.first] + '=' + toupper(moved->getPromotionPiece()->getTypeKey());
    }
    else if (moved->getCastling()) {
        if (moved->getIsKingSide()) {
            moveNotation = "O-O";
        }
        else {
            moveNotation = "O-O-O";
        }
    }
    else if (moved->getEnPassant()) {

    }
    else {

    }

    return moveNotation;
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

void ChessBoard::makeMove(const int& lastRow, const int& lastCol, const int& row, const int& col, const vector<pair<int, int>>& possibleMoves, Move*& curMove) {
    //cout << generateFEN() << '\n';
    if (whiteTurn == false) fullMoveNumber++;
    alterTurn();

    // Đặt quân cờ từ ô cũ đến ô hiện tại
    {
        //Move
        curMove = new Move(lastRow, lastCol, row, col);
        unique_ptr<Pieces> deletePiece = move(board[row][col]);

        if (board[lastRow][lastCol]->getType() == "pawn" || deletePiece) haftMoveClock = 0;
        else haftMoveClock++;

        curMove->setPieceMoved(board[lastRow][lastCol]);
        if (deletePiece) curMove->setPieceCaptured(deletePiece);

        board[row][col].reset();
        board[row][col] = move(board[lastRow][lastCol]);
        board[row][col]->setPosition(col, row);

        enPassantTargetSquare = "";
        if (board[row][col]->getType() == "pawn" && abs(row - lastRow) == 2) {
            
            unsigned num = (row + lastRow) / 2;
            enPassantTargetSquare += char('a' + col);
            enPassantTargetSquare += char('8' - num);
        }

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

    stockfish->setBoardState(generateFEN());

    // Bỏ chọn quân cờ này
    board[row][col]->resetNumPress();
    highlightTiles.clear();
    selectedPiece = nullptr;
}

void ChessBoard::setAI(bool isAI) {
    this->isAI = isAI;
}

bool ChessBoard::getAI() const {
    return this->isAI;
}

void ChessBoard::setHumanColor(bool humanColor) {
    this->humanColor = humanColor;
}

bool ChessBoard::getHumanColor() const {
    return this->humanColor;
}

bool ChessBoard::isAITurn() const {
    return this->whiteTurn != this->humanColor;
}

tuple<int, int, int, int> ChessBoard::processStockfishMove(const string& bestmove) {
    if (bestmove.size() < 4) return { -1, -1, -1, -1 };

    int lastCol = bestmove[0] - 'a';
    int lastRow = '8' - bestmove[1];
    int col = bestmove[2] - 'a';
    int row = '8' - bestmove[3];

    return { lastRow, lastCol, row, col };
}