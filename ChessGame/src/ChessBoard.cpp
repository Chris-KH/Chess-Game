#include"../lib/ChessBoard.h"

// Thiết lập các thứ
ChessBoard::ChessBoard(RenderWindow* win, Stockfish* stockfish , int currentBoardIndex) {
    // Khởi tạo
    window = win;
    this->stockfish = stockfish;
    boardFiles = {
            "../assets/Chess Board/ChessBoard1.png",
            "../assets/Chess Board/ChessBoard2.png",
            "../assets/Chess Board/ChessBoard3.png",
            "../assets/Chess Board/ChessBoard4.png",
            "../assets/Chess Board/ChessBoard5.png"
    };
    this->currentBoardIndex = currentBoardIndex;

    // Set texture for chess board
    // Load first texture
    loadBoardTexture(boardFiles[currentBoardIndex]); 
    cellSize = 100;

    // Create a chess board
    board.clear();
    board.resize(8);

    //Each row have 8 column
    for (auto& row : board) {
        row.resize(8); 
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

    // Player turn
    whiteTurn = true;
    fullMoveNumber = 1;
    haftMoveClock = 0;
    for (int i = 0; i < 2; i++) castlingAvailability[i] = true;
    enPassantTargetSquare = "";

    // Check
    inCheck[0] = inCheck[1] = false;

    // Game Over
    gameOver = 0;

    // Buttons
    newBut.setSpriteButton("new", "../assets/Button/new.png", 35, 35, 960, 30);
    undoBut.setSpriteButton("undo", "../assets/Button/undo.png", 35, 35, 1035, 30);
    redoBut.setSpriteButton("redo", "../assets/Button/redo.png", 35, 35, 1110, 30);
    surrenderBut.setSpriteButton("surrender", "../assets/Button/surrender.png", 35, 35, 1185, 30);
    saveBut.setSpriteButton("save", "../assets/Button/save.png", 35, 35, 1260, 30);
    settingBut.setSpriteButton("setting", "../assets/Button/settings.png", 35, 35, 1335, 30);

}

ChessBoard::~ChessBoard() {
    freeUndoStack();
    freeRedoStack();
}

void ChessBoard::addPiece(unique_ptr<Pieces> piece, int col, int row) {
    //Dùng để khởi tạo quân cờ
    piece->setPosition(col, row, cellSize); // Đặt vị trí quân cờ
    board[row][col].reset();
    board[row][col] = move(piece);
}

bool ChessBoard::loadBoardTexture(const string& filePath) {
    //Dùng để khởi tạo / thay đổi bàn cờ
    if (!boardTexture.loadFromFile(filePath)) {
        return false; // Không thể load ảnh
    }
    boardSprite.setTexture(boardTexture);
    boardSprite.setPosition(0, 0);
    updateBoardScale(); // Cập nhật tỉ lệ
    return true;
}

// Update các thứ
void ChessBoard::updateBoardScale() {
    Vector2u windowSize = window->getSize();
    Vector2u boardSize = boardTexture.getSize();

    // Xác định kích thước mục tiêu cho bàn cờ (ví dụ: 800x800)
    const float targetWidth = 930.0f;
    const float targetHeight = 930.0f;

    // Tính tỉ lệ cho bàn cờ
    float scaleX = targetWidth / static_cast<float>(boardSize.x);
    float scaleY = targetHeight / static_cast<float>(boardSize.y);

    // Chọn tỉ lệ nhỏ hơn để giữ nguyên tỉ lệ của hình ảnh
    float scale = min(scaleX, scaleY);

    boardSprite.setScale(scale, scale); // Áp dụng tỉ lệ

    // Cập nhật kích thước ô
    cellSize = 100; // Kích thước ô dựa trên tỉ lệ
}

bool ChessBoard::changeBoard(int newIndex) {
    /*
        @return bool = 
            true: nếu đổi bàn cờ hợp lệ, 
            false: nếu đổi bàn cờ không hợp lệ.

        Dùng để đổi bàn cờ thành bàn cờ có chỉ số là newIndex
    */
    if (newIndex >= 0 && newIndex < boardFiles.size()) {
        currentBoardIndex = newIndex;
        return loadBoardTexture(boardFiles[currentBoardIndex]);
    }
    return false;
}

void ChessBoard::changePieces(int newIndex) {
    for (auto& pieces : board) {
        for (auto& piece : pieces) {
            if (!piece) continue;
            piece->changeTexture(newIndex);
            piece->setPosition(piece->getCol(), piece->getRow(), cellSize);
        }
    }
}

void ChessBoard::update(const Event& event) {
    //Update các sự kiện xảy ra trên bàn cờ
    // Nhấn số [1..5] để thay đổi bàn cờ
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Num1) {
            changeBoard(0); // Thay đổi sang bàn cờ 1
        }
        else if (event.key.code == Keyboard::Num2) {
            changeBoard(1); // Thay đổi sang bàn cờ 2
        }
        else if (event.key.code == Keyboard::Num3) {
            changeBoard(2); // Thay đổi sang bàn cờ 3
        }
        else if (event.key.code == Keyboard::Num4) {
            this->changeBoard(3); // Thay đổi sang bàn cờ 4
        }
        else if (event.key.code == Keyboard::Num5) {
            this->changeBoard(4); // Thay đổi sang bàn cờ 5
        }
        else if (event.key.code == Keyboard::Num6) {
            this->changePieces(0); 
        }
        else if (event.key.code == Keyboard::Num7) {
            this->changePieces(1); 
        }
        else if (event.key.code == Keyboard::Num8) {
            this->changePieces(2); 
        }
        else if (event.key.code == Keyboard::Num9) {
            this->changePieces(3);
        }
    }
    // Nhấn chuột trái
    else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;
        handleMousePress(mouseX, mouseY);
        handleButtonPress(mouseX, mouseY);
    }
    // Nếu thả chuột trái
    else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;
        handleMouseRelease(mouseX, mouseY);
        handleButtonRelease(mouseX, mouseY);
        if (isCheck(whiteTurn, true)) {
            if (cannotMove()) {
                gameOver = whiteTurn + 1; // Checkmate
            }
        }
        else {
            if (cannotMove() || isTie()) {
                gameOver = 3; // Tie state
            }
        }
        isCheck(1 - whiteTurn, true); // Delete old check highlight if it exists
    }

    if (pieceFollowingMouse != nullptr) {
        pieceFollowingMouse->followMouse(Mouse::getPosition(*window));
    }
}

void ChessBoard::draw() {
    window->draw(boardSprite);
    //Pieces
    for (int i = 0; i < 2; i++) for (const auto& tile : checkTiles[i]) {
        window->draw(tile);
    }

    for (const auto& tile : highlightTiles) {
        window->draw(tile);
    }

    for (auto& pieces : board) {
        for (auto& piece : pieces) {
            if (!piece) continue;
            piece->draw(*window);
        }
    }

    // Buttons
    undoBut.drawSprite(*window);
    redoBut.drawSprite(*window);
    saveBut.drawSprite(*window);

    newBut.drawSprite(*window);
    surrenderBut.drawSprite(*window);
    settingBut.drawSprite(*window);
}

size_t ChessBoard::countPieces() {
    size_t count = 0;
    for (auto& pieces : board) {
        for (auto& piece : pieces) {
            if (piece) count++;
        }
    }
    return count;
}

// Handle mouse operators
void ChessBoard::getPossibleMoves(Pieces* clickedPiece, vector<pair<int, int>>& vpii) {
    /*
        @Ý tưởng: 
            Ta sẽ làm trâu bò với mỗi ô tới được, ta cứ đặt quân cờ tại đó:
            Nếu không bị chiếu tướng thì nước đi ấy là hợp lệ
    */

    vpii = clickedPiece->getPossibleMoves(board);
    int color = clickedPiece->getColor();
    int r = clickedPiece->getRow();
    int c = clickedPiece->getCol();

    // Ta xét mỗi nước đi có hợp lệ hay không?
    vector<pair<int, int>> ret;
    for (pair<int, int>& pii : vpii) {
        // Giả sử ta đặt quân cờ đang xét vào ô hiện tại
        unique_ptr<Pieces> tmp = move(board[pii.first][pii.second]);
        board[pii.first][pii.second] = move(board[r][c]);
        // Kiểm tra xem có hết bị chiếu hay chưa (tham chiếu trong isCheck: save = false bởi vì không cần lưu)
        if (isCheck(color, false) == false) {
            ret.push_back(pii);
        }
        // Sau khi kiểm tra xong thì trả về vị trí ban đầu
        board[r][c] = move(board[pii.first][pii.second]);
        board[pii.first][pii.second] = move(tmp);
    }
    swap(ret, vpii);
}

void ChessBoard::handleMousePress(int mouseX, int mouseY) {
    int row = (mouseY - 65) / 100; // Kích thước ô là 100, trừ viền 65px
    int col = (mouseX - 65) / 100; // Kích thước ô là 100, trừ viền 65px

    // Nếu ô được chọn nằm ngoài bàn cờ thì ta không làm gì
    if (col < 0 || col >= 8 || row < 0 || row >= 8) return;

    Pieces* lastPiece = selectedPiece; // lastPiece = quân cờ cũ
    selectedPiece = board[row][col].get(); // Lấy quân cờ ở ô đã click

    // Nếu trước đó có chọn quân cờ
    if (lastPiece != nullptr) {
        highlightTiles.clear();
        // Nếu ta chọn quân cờ khác thì reset biến numPress
        if (selectedPiece != lastPiece) {
            lastPiece->resetNumPress();
        }
    }

    // Nếu ta chọn một quân cờ được đi
    if (selectedPiece != nullptr && selectedPiece->getColor() == whiteTurn) {
        selectedPiece->press();
        highlightPossibleMove(selectedPiece);
        pieceFollowingMouse = selectedPiece;
    }
    // Nếu chọn ô khác mà trước đó ta có chọn một quân cờ được đi
    else if (lastPiece != nullptr && lastPiece->getColor() == whiteTurn) {
        selectedPiece = lastPiece;
    }
}

void ChessBoard::handleMouseRelease(int mouseX, int mouseY) {
    // Nếu trước đó đang giữ quân nào thì thả quân đó ra
    if (pieceFollowingMouse != nullptr) {
        pieceFollowingMouse->setPosition(pieceFollowingMouse->getCol(), pieceFollowingMouse->getRow());
        pieceFollowingMouse = nullptr;
    }

    // Nếu trước đó không chọn quân cờ được phép đi thì ta bỏ qua
    if (selectedPiece == nullptr || selectedPiece->getColor() != whiteTurn) return;

    int lastRow = selectedPiece->getRow();
    int lastCol = selectedPiece->getCol();
    int row = (mouseY - 65) / 100; // Kích thước ô là 100, trừ viền 65px
    int col = (mouseX - 65) / 100; // Kích thước ô là 100, trừ viền 65px

    // Nếu ô được chọn nằm ngoài bàn cờ thì ta không làm gì
    if (col < 0 || col >= 8 || row < 0 || row >= 8) return;

    Pieces* lastPiece = selectedPiece; // lastPiece = quân cờ cũ
    selectedPiece = board[row][col].get(); // Lấy quân cờ ở ô đã click

    // Nếu nước đi hiện tại là đúng luật
    vector<pair<int, int>> possibleMoves; 
    getPossibleMoves(lastPiece, possibleMoves);

    if (find(possibleMoves.begin(), possibleMoves.end(), make_pair(row, col)) != possibleMoves.end()) {
        // Cập nhật lượt tiếp theo
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
    // Nếu nhấn vào ô trước đó
    else if (selectedPiece == lastPiece) {
        // Nếu số lần nhấn vào = 1 thì không làm gì
        if (selectedPiece == lastPiece && selectedPiece->getNumPress() == 1) return;

        // Bỏ chọn quân cờ này
        board[row][col]->resetNumPress();
        highlightTiles.clear();
        selectedPiece = nullptr;
    }
    // Nếu ta đi nước đi không hợp lệ thì trả quân cờ về vị trí ban đầu
    else {
        selectedPiece = lastPiece;
    }
}

void ChessBoard::highlightPossibleMove(Pieces* clickedPiece) {
    // Đặt quân cờ được chọn là clickedPiece
    selectedPiece = clickedPiece;

    // Lấy danh sách các nước đi có thể
    vector<pair<int, int>> possibleMoves;
    getPossibleMoves(clickedPiece, possibleMoves);

    // Tô màu các ô theo danh sách
    for (auto& move : possibleMoves) {
        int targetRow = move.first;
        int targetCol = move.second;

        RectangleShape highlightTile(Vector2f(100, 100)); // Kích thước ô là 100x100
        highlightTile.setPosition(float(65 + targetCol * 100), float(65 + targetRow * 100)); // Đặt vị trí ô tô màu với viền

        // Nếu ô chứa quân địch, tô màu đỏ; nếu không, tô màu xanh
        if (board[targetRow][targetCol] && board[targetRow][targetCol]->getColor() != selectedPiece->getColor()) {
            highlightTile.setFillColor(Color(255, 99, 71, 100)); // Màu đỏ
        }
        else {
            highlightTile.setFillColor(Color(144, 238, 144, 100)); // Màu xanh
        }

        highlightTiles.push_back(highlightTile); // Thêm ô tô màu vào danh sách
    }
    // Tô cả ô đang đứng
    RectangleShape highlightTile(Vector2f(100, 100)); // Kích thước ô là 100x100
    highlightTile.setPosition(float(65 + selectedPiece->getCol() * 100), float(65 + selectedPiece->getRow() * 100)); // Đặt vị trí ô tô màu với viền
    highlightTile.setFillColor(Color(100, 255, 100, 128)); // Màu xanh
    highlightTiles.push_back(highlightTile);
}

void ChessBoard::handleButtonPress(int mouseX, int mouseY) {
    // Find which button did the mouse press
    selectedBut = nullptr;
    if (undoBut.contain(mouseX, mouseY)) selectedBut = &undoBut;
    else if (redoBut.contain(mouseX, mouseY)) selectedBut = &redoBut;
    else if (saveBut.contain(mouseX, mouseY)) selectedBut = &saveBut;
    else if (newBut.contain(mouseX, mouseY)) selectedBut = &newBut;
    else if (surrenderBut.contain(mouseX, mouseY)) selectedBut = &surrenderBut;
    else if (settingBut.contain(mouseX, mouseY)) selectedBut = &settingBut;
}

void ChessBoard::handleButtonRelease(int mouseX, int mouseY) {
    // Record the last button pressed
    Button* lastBut = selectedBut;
    // Find which button did the mouse release
    selectedBut = nullptr;
    if (undoBut.contain(mouseX, mouseY)) selectedBut = &undoBut;
    else if (redoBut.contain(mouseX, mouseY)) selectedBut = &redoBut;
    else if (saveBut.contain(mouseX, mouseY)) selectedBut = &saveBut;
    else if (newBut.contain(mouseX, mouseY)) selectedBut = &newBut;
    else if (surrenderBut.contain(mouseX, mouseY)) selectedBut = &surrenderBut;
    else if (settingBut.contain(mouseX, mouseY)) selectedBut = &settingBut;
    // If the button clicked the same with the button released then do operation(s) on this
    if (selectedBut && selectedBut == lastBut) {
        if (selectedBut->getName() == "undo") {
            undoMove();
        }
        else if (selectedBut->getName() == "redo") {
            redoMove();
        }
        else if (selectedBut->getName() == "save") {
            saveGame();
        }
        else if (selectedBut->getName() == "new") {
            newGame();
        }
        else if (selectedBut->getName() == "surrender") {
            gameOver = whiteTurn + 1;
        }
        else if (selectedBut->getName() == "setting") {
            GUI::settingChoice(*this);
        }
    }
}

// Player turn
void ChessBoard::alterTurn(void) {
    whiteTurn ^= true;
}

// Detect check, checkmate, draw
bool ChessBoard::isCheck(bool color, bool save) {
    /*
        @Return :
            True nếu đang bị chiếu tướng
            False nếu không bị
        @Brief : xét xem quân cờ có màu hiện tại có bị chiếu hay không? True = có, False = không
            Nếu bị chiếu thì tô đỏ quân cờ đang chiếu và quân vua bị chiếu
            Nếu tham số save = 1, mình sẽ cập nhật: inCheck[color], nếu đang bị chiếu thì tô màu quân chiếu và bị chiếu
            Nếu tham số save = 0, mình chỉ trả về: có bị chiếu hay không
        @Idea : Ý tưởng :
            1. Tìm vị trí của con vua
            2. Ta sẽ duyệt hết bàn cờ, mỗi quân cờ ta lấy các nước đi có thể của quân cờ đó.
               Nếu quân cờ này có thể ăn được con vua thì ta tô màu đỏ cho ô của con cờ này và con vua.
    */
    if (save) {
        inCheck[color] = false;
        checkTiles[color].clear();
    }

    // Tìm king
    Pieces* king;
    int r, c; // r là hàng, c là cột của quân cờ vua
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] && board[i][j]->getType() == "king" && board[i][j]->getColor() == color) {
                king = board[i][j].get();
                r = i, c = j;
            }
        }
    }

    // Xét xem có quân cờ nào chiếu vào không
    RectangleShape tile(Vector2f(cellSize, cellSize)); // Kích thước ô là 100x100
    tile.setFillColor(Color(255, 99, 71, 100)); // Màu đỏ
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // Nếu ở ô hiện tại có quân cờ khác màu
            if (board[i][j] && board[i][j]->getColor() != color) {
                bool curCheck = false;
                if (board[i][j]->getType() == "king") {
                    if (abs(i - r) <= 1 && abs(j - c) <= 1) curCheck = true;
                }
                else if (board[i][j]->getType() == "queen") {
                    if (r == i || c == j || r + c == i + j || r - c == i - j) { // Cùng hàng, cột, đường chéo
                        curCheck = true;
                        int dx = i - r, dy = j - c, d = max(abs(dx), abs(dy));
                        dx /= d, dy /= d;
                        int x = r + dx, y = c + dy;
                        while (x != i || y != j) {
                            if (board[x][y] != nullptr) {
                                curCheck = false;
                                break;
                            }
                            x += dx, y += dy;
                        }
                    }
                }
                else if (board[i][j]->getType() == "bishop") {
                    if (r + c == i + j || r - c == i - j) { // cùng đường chéo
                        curCheck = true;
                        int dx = i - r, dy = j - c, d = max(abs(dx), abs(dy));
                        dx /= d, dy /= d;
                        int x = r + dx, y = c + dy;
                        while (x != i || y != j) {
                            if (board[x][y] != nullptr) {
                                curCheck = false;
                                break;
                            }
                            x += dx, y += dy;
                        }
                    }
                }
                else if (board[i][j]->getType() == "knight") {
                    if ((abs(i - r) == 2 && abs(j - c) == 1) ||
                        (abs(i - r) == 1 && abs(j - c) == 2)) {
                        curCheck = true;
                    }
                }
                else if (board[i][j]->getType() == "rook") {
                    if (r == i || c == j) { // Cùng đường chéo
                        curCheck = true;
                        int dx = i - r, dy = j - c, d = max(abs(dx), abs(dy));
                        dx /= d, dy /= d;
                        int x = r + dx, y = c + dy;
                        while (x != i || y != j) {
                            if (board[x][y] != nullptr) {
                                curCheck = false;
                                break;
                            }
                            x += dx, y += dy;
                        }
                    }
                }
                else if (board[i][j]->getType() == "pawn") {
                    int Dir = color ? 1 : -1;
                    if (r == i + Dir && (c == j - 1 || c == j + 1))
                        curCheck = true;
                }
                if (curCheck) {
                    if (save) {
                        inCheck[color] = true;
                        tile.setPosition(Vector2f(float(65 + j * 100), float(65 + i * 100)));
                        checkTiles[color].push_back(tile);
                    }
                    else {
                        return true;
                    }
                }
            }
        }
    }

    // Nếu quân vua bị chiếu thì tô màu ô chứa quân vua màu đỏ
    if (save && inCheck[color]) {
        tile.setPosition(65 + c * cellSize, 65 + r * cellSize);
        checkTiles[color].push_back(tile);
        return true;
    }
    return false;
}

bool ChessBoard::cannotMove(void) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] && board[i][j]->getColor() == whiteTurn) {
                vector<pair<int, int>> moves;
                getPossibleMoves(board[i][j].get(), moves);
                if (moves.size() > 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool ChessBoard::isTie(void) {
    // II. Dead position
    int cnt[2][6], bishop[2][2];
    for (int i = 0; i < 2; i++) for (int j = 0; j < 6; j++) cnt[i][j] = 0;
    // 0 = pawn, 1 = rook, 2 = knight, 3 = bishop, 4 = queen, 5 = king
    
    vector<Pieces*> king, pawn;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j]) {
                string pieceType = board[i][j]->getType();
                int idType;
                if (pieceType == "pawn") {
                    idType = 0;
                    pawn.push_back(board[i][j].get());
                }
                else if (pieceType == "rook") idType = 1;
                else if (pieceType == "knight") idType = 2;
                else if (pieceType == "bishop") {
                    idType = 3;
                    bishop[board[i][j]->getColor()][(i + j) % 2]++;
                }
                else if (pieceType == "queen") idType = 4;
                else {
                    idType = 5;
                    king.push_back(board[i][j].get());
                }
                cnt[board[i][j]->getColor()][idType]++;
            }
        }
    }

    if (king[0]->getColor() == 0) swap(king[0], king[1]);

    // 1. King vs. King
    if (!cnt[0][0] && !cnt[0][1] && !cnt[0][2] && !cnt[0][3] && !cnt[0][4] &&
        !cnt[1][0] && !cnt[1][1] && !cnt[1][2] && !cnt[1][3] && !cnt[1][4]) {
        return true;
    }

    // 2. King vs. Bishop and King
    if ((!cnt[0][0] && !cnt[0][1] && !cnt[0][2] && cnt[0][3] == 1 && !cnt[0][4] &&
         !cnt[1][0] && !cnt[1][1] && !cnt[1][2] && !cnt[1][3] && !cnt[1][4]) ||
        (!cnt[0][0] && !cnt[0][1] && !cnt[0][2] && !cnt[0][3] && !cnt[0][4] &&
         !cnt[1][0] && !cnt[1][1] && !cnt[1][2] && cnt[1][3] == 1 && !cnt[1][4])) {
        return true;
    }

    // 3. King vs. Knight and King
    if ((!cnt[0][0] && !cnt[0][1] && cnt[0][2] == 1 && !cnt[0][3] && !cnt[0][4] &&
         !cnt[1][0] && !cnt[1][1] && !cnt[1][2] && !cnt[1][3] && !cnt[1][4]) ||
        (!cnt[0][0] && !cnt[0][1] && !cnt[0][2] && !cnt[0][3] && !cnt[0][4] &&
         !cnt[1][0] && !cnt[1][1] && cnt[1][2] == 1 && !cnt[1][3] && !cnt[1][4])) {
        return true;
    }

    // 4. King and Bishop vs. King and Bishop (with same cell color with opponent's Bishop)
    if ((!cnt[0][0] && !cnt[0][1] && !cnt[0][2] && cnt[0][3] == 1 && !cnt[0][4] &&
         !cnt[1][0] && !cnt[1][1] && !cnt[1][2] && cnt[1][3] == 1 && !cnt[1][4]) &&
          bishop[0][0] == bishop[1][0] && bishop[0][1] == bishop[1][1]) {
        return true;
    }

    // 5. Blocked Pawn Structures
    /*
        No other pieces exist. Only pawns and kings. Kings are cut off from the other side.
        Impossible to make any progress.
    */
    if (!cnt[0][1] && !cnt[0][2] && !cnt[0][3] && !cnt[0][4] &&
        !cnt[1][1] && !cnt[0][2] && !cnt[0][3] && !cnt[0][4]) {
        // Examine if any pawn can make progress
        bool pawnBlocked = true;
        for (Pieces* p : pawn) {
            if (!p->getPossibleMoves(board).empty()) {
                pawnBlocked = false;
            }
        }

        if (pawnBlocked) {
            // BFS to check if each king can make any progress: capturing other pawns
            int mat[8][8];

            // Examine white king
            for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) mat[i][j] = 0;
            bool whiteBlocked = true;
            for (Pieces* p : pawn) { // Mark cell cannot be visited by white king
                int r = p->getRow();
                int c = p->getCol();
                if (p->getColor()) { // White pawn
                    mat[r][c] = -1;
                }
                else { //  Black pawn
                    mat[r][c] = -2;
                    int row[] = { r + 1, r + 1 };
                    int col[] = { c - 1, c + 1 };
                    for (int i = 0; i < 2; i++) {
                        if (0 < row[i] && row[i] < 8 && 0 < col[i] && col[i] < 8) {
                            mat[row[i]][col[i]] = -1;
                        }
                    }
                }
            }
            { // BFS
                const int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
                const int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
                int num = 0, size = 0;
                pair<int, int> q[65];
                fill(q, q + 65, make_pair(-1, -1));
                mat[king[0]->getRow()][king[0]->getCol()] = 1;
                q[size++] = make_pair(king[0]->getRow(), king[0]->getCol());
                while (num < size) {
                    int r = q[num].first, c = q[num].second;
                    num++;
                    for (int i = 0; i < 8; i++) {
                        int nr = r + dx[i], nc = c + dy[i];
                        if (0 < nr && nr < 8 && 0 < nc && nc < 8) {
                            if (mat[nr][nc] == 0) {
                                mat[nr][nc] = 1;
                                q[size++] = make_pair(nr, nc);
                            }
                            else if (mat[nr][nc] == -2) {
                                whiteBlocked = false;
                            }
                        }
                    }
                }
            }

            // Examine black king
            for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) mat[i][j] = 0;
            bool blackBlocked = true;
            for (Pieces* p : pawn) { // Mark cell cannot be visited by black king
                int r = p->getRow();
                int c = p->getCol();
                if (!p->getColor()) { // Black pawn
                    mat[r][c] = -1;
                }
                else { //  White pawn
                    mat[r][c] = -2;
                    int row[] = { r + 1, r + 1 };
                    int col[] = { c - 1, c + 1 };
                    for (int i = 0; i < 2; i++) {
                        if (0 < row[i] && row[i] < 8 && 0 < col[i] && col[i] < 8) {
                            mat[row[i]][col[i]] = -1;
                        }
                    }
                }
            }
            { // BFS
                const int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
                const int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
                int num = 0, size = 0;
                pair<int, int> q[65];
                fill(q, q + 65, make_pair(-1, -1));
                mat[king[1]->getRow()][king[1]->getCol()] = 1;
                q[size++] = make_pair(king[1]->getRow(), king[1]->getCol());
                while (num < size) {
                    int r = q[num].first, c = q[num].second;
                    num++;
                    for (int i = 0; i < 8; i++) {
                        int nr = r + dx[i], nc = c + dy[i];
                        if (0 < nr && nr < 8 && 0 < nc && nc < 8) {
                            if (mat[nr][nc] == 0) {
                                mat[nr][nc] = 1;
                                q[size++] = make_pair(nr, nc);
                            }
                            else if (mat[nr][nc] == -2) {
                                blackBlocked = false;
                            }
                        }
                    }
                }
            }

            if (whiteBlocked && blackBlocked) {
                return true;
            }
        }
    }

    // III. Threefold Repetition

    // IV. 50-Move Rule

    return false;
}

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
        board[toPosition.first][toPosition.second] = move->getPieceCaptured()->clone();
        board[toPosition.first][toPosition.second]->setPosition(toPosition.second, toPosition.first);

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
    */
}

//Load game
void ChessBoard::loadGame() {

}