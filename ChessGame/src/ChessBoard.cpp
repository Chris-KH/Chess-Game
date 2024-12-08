#include"../lib/ChessBoard.h"

// Thiết lập các thứ
ChessBoard::ChessBoard(RenderWindow* win, Stockfish* stockfish, bool isAI) {
    // Khởi tạo
    window = win;
    this->stockfish = stockfish;
    this->isAI = isAI;
    boardFiles = {
        "../assets/Chess Board/ChessBoard1.png",
        "../assets/Chess Board/ChessBoard2.png",
        "../assets/Chess Board/ChessBoard3.png",
        "../assets/Chess Board/ChessBoard4.png",
        "../assets/Chess Board/ChessBoard5.png"
    };
    boardName = {
        "Gray",
        "Blue",
        "Brown",
        "Night owl",
        "Wood"
    };
    pieceName = {
        "Standard",
        "Cartoon",
        "Pixel",
        "Neo",
        "Game Room",
        "Neon"
    };
    soundThemePath = {
        "",
        "../assets/sounds/standard",
        "../assets/sounds/metal",
        "../assets/sounds/space"
    };
    soundThemeName = {
        "None",
        "Standard",
        "Metal",
        "Space"
    };
    soundName = {
        "move-self",
        "move-check",
        "game-win",
        "game-draw",
        "capture",
        "castle",
        "promote"
    };
    this->currentBoardIndex = 0;
    this->currentPieceIndex = 0;
    this->currentSoundIndex = 0;

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

    // Sound
    sounds.resize(7);

    // Player turn
    whiteTurn = true;
    fullMoveNumber = 1;
    haftMoveClock = 0;
    for (int i = 0; i < 4; i++) castlingAvailability[i] = true;
    enPassantTargetSquare = "";

    //Set Stockfish
    this->stockfish->newGame();
    AIDifficulty = 20;
    this->stockfish->setSkillLevel(AIDifficulty);
    this->stockfish->setBoardState(generateFEN());
    this->humanColor = true;
    this->undoPress = false;
    
    // Check
    inCheck[0] = inCheck[1] = false;

    // Game Over
    gameOver = 0;
    stateOver.clear();

    // Repetition State
    repState.clear();
    repState[FENToCheckThreefold()]++;
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

void ChessBoard::setGameOver(int val) { gameOver = val; }
void ChessBoard::setStateOver(string state) { stateOver = state; }
vector<string>& ChessBoard::getBoardList(void) { return boardName; }
vector<string>& ChessBoard::getPieceList(void) { return pieceName; }
vector<string>& ChessBoard::getSoundList(void) { return soundThemeName; }
bool ChessBoard::isWhiteTurn(void) const { return this->whiteTurn; }
int ChessBoard::isOver(void) const { return gameOver; }
string ChessBoard::getStateOver(void) const { return stateOver; }
int ChessBoard::getBoardIndex(void) const { return currentBoardIndex; }
int ChessBoard::getPieceIndex(void) const { return currentPieceIndex; }
int ChessBoard::getSoundIndex(void) const { return currentSoundIndex; }
Sprite ChessBoard::getBoardSprite(void) const { return boardSprite; }

bool ChessBoard::loadBoardTexture(const string& filePath) {
    //Dùng để khởi tạo / thay đổi bàn cờ
    if (!boardTexture.loadFromFile(filePath)) {
        return false; // Không thể load ảnh
    }
    boardSprite.setTexture(boardTexture);
    updateBoardScale(); // Cập nhật tỉ lệ
    return true;
}

// Updates

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
    // Dùng để đổi bàn cờ thành bàn cờ có chỉ số là newIndex
    if (newIndex >= 0 && newIndex < boardFiles.size()) {
        currentBoardIndex = newIndex;
        return loadBoardTexture(boardFiles[currentBoardIndex]);
    }
    return false;
}

bool ChessBoard::changePieces(int newIndex) {
    currentPieceIndex = newIndex;
    if(newIndex >= 0 && newIndex < (int)pieceName.size()) {
        for (auto& pieces : board) for (auto& piece : pieces) {
            if (!piece) continue;
            piece->changeTexture(newIndex);
            piece->setPosition(piece->getCol(), piece->getRow(), cellSize);
        }
        return true;
    }
    return false;
}

bool ChessBoard::changeSound(int newIndex) {
    currentSoundIndex = newIndex;
    // nexIndex == 0 --> No sound
    if (newIndex > 0 && newIndex < (int)soundName.size()) {
        for(int i = 0; i < (int)sounds.size(); i++) {
            if (!sounds[i].first.loadFromFile(soundThemePath[newIndex] + "/" + soundName[i] + ".mp3")) {
                throw runtime_error("Cannot load sound files: " + soundThemePath[newIndex] + "/" + soundName[i] + ".mp3");
            }
            sounds[i].second.setBuffer(sounds[i].first);
        }
    }
}

void ChessBoard::update(const Event& event) {
    // Press left mouse
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;
        handleMousePress(mouseX, mouseY, 1, 0);
    }
    // Release left mouse
    else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;
        handleMouseRelease(mouseX, mouseY, 1, 0);
    }
    // Drag a piece
    if (pieceFollowingMouse) {
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

    for (const auto& tile : highlightTilesJustMove) {
        window->draw(tile);
    }

    for (auto& pieces : board) {
        for (auto& piece : pieces) {
            if (!piece) continue;
            piece->draw(*window);
        }
    }
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
            Nếu đang tới lượt quân cờ này đi, ta xét hết các ô có thể đi
            Nếu không, quân cờ này không thể đi đến ô nào hết
    */

    if (clickedPiece->getColor() != isWhiteTurn()) {
        vpii.clear();
        return;
    }

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

Pieces* ChessBoard::getPieceAtIndex(int row, int col) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return nullptr;

    if (!board[row][col]) return nullptr;

    return board[row][col].get();
}

void ChessBoard::playSound(int id) {
    if (currentSoundIndex == 0) return;
    sounds[id].second.play();
}

void ChessBoard::handleMousePress(int mouseX, int mouseY, bool enableClick, bool enableDrag) {
    int row = int((mouseY - topMargin) / cellSize);
    int col = int((mouseX - leftMargin) / cellSize);
    Pieces* lastPiece = selectedPiece;
    selectedPiece = (0 <= row && row < 8 && 0 <= col && col < 8 ? board[row][col].get() : nullptr);

    if (enableClick) {
        // Giả sử đang tới lượt của quân trắng
        // Đang không chọn quân cờ
        if (lastPiece == nullptr) {
            // Chọn quân hiện tại
            lastPiece = selectedPiece;
        }
        // Đang chọn quân cờ
        else {
            // Có thể di chuyển tới ô click
            vector<pair<int, int>> possibleMoves;
            getPossibleMoves(lastPiece, possibleMoves);
            if (lastPiece->getColor() == isWhiteTurn() && lastPiece->canMoveTo(row, col, possibleMoves)) {
                if (undoPress == true && isAITurn()) {
                    if (lastPiece->getCol() != col || lastPiece->getRow() != row) {
                        undoPress = false;
                        while (redoStack.empty() == false) {
                            draw();
                            window->display();
                            redoMove();
                            Sleep(100);
                        }
                        draw();
                        window->display();
                        Sleep(50);
                    }
                    return;
                }
                else {
                    Move* curMove = nullptr;
                    //Make piece move
                    makeMove(lastPiece->getRow(), lastPiece->getCol(), row, col, possibleMoves, curMove);
                }
                lastPiece = nullptr;
            }
            // Không thể di chuyển tới ô click
            else {
                // Click quân cờ cũ
                if (selectedPiece == lastPiece) {
                    // Bỏ chọn quân cờ
                    lastPiece = nullptr;
                }
                // Click quân cờ mới
                else {
                    // Chọn quân cờ mới
                    lastPiece = selectedPiece;
                }
            }
        }

        // Cập nhật quân cờ hiện tại
        swap(lastPiece, selectedPiece);

        // Highlight
        highlightTiles.clear();
        if (selectedPiece != nullptr) {
            highlightPossibleMove(selectedPiece);
        }
    }
    if (enableDrag) {
        if (lastPiece != nullptr) {
            if (pieceFollowingMouse == lastPiece) {
                lastPiece->unfollowMouse();
                pieceFollowingMouse = nullptr;
                lastPiece = nullptr;
            }
        }
        assert(pieceFollowingMouse == nullptr);
        // Chọn ô được press
        lastPiece = selectedPiece;
        if (lastPiece != nullptr) {
            // Quân cờ này di chuyển theo chuột
            pieceFollowingMouse = lastPiece;
        }
        swap(lastPiece, selectedPiece);

        highlightTiles.clear();
        if (selectedPiece != nullptr) {
            highlightPossibleMove(selectedPiece);
        }
    }
}

void ChessBoard::handleMouseRelease(int mouseX, int mouseY, bool enableClick, bool enableDrag) {
    // Đang thao tác theo kiểu click thì không thao tác theo kiểu drag
    if (enableClick == true) return;

    int row = int((mouseY - topMargin) / cellSize);
    int col = int((mouseX - leftMargin) / cellSize);
    Pieces* lastPiece = selectedPiece;
    selectedPiece = (0 <= row && row < 8 && 0 <= col && col < 8 ? board[row][col].get() : nullptr);

    if (enableDrag) {
        // Đang press quân cờ
        if (lastPiece != nullptr) {
            if (undoPress == true && isAITurn()) {
                highlightTiles.clear();
                lastPiece->unfollowMouse();
                if (lastPiece->getCol() != col || lastPiece->getRow() != row) {
                    undoPress = false;
                    while (redoStack.empty() == false) {
                        draw();
                        window->display();
                        redoMove();
                        Sleep(100);
                    }
                    draw();
                    window->display();
                    Sleep(50);
                }
                pieceFollowingMouse = nullptr;
                lastPiece = nullptr;
                assert(pieceFollowingMouse == nullptr);
                return;
            }
            else if (lastPiece->getColor() == isWhiteTurn()) {
                vector<pair<int, int>> possibleMoves;
                getPossibleMoves(lastPiece, possibleMoves);
                if (lastPiece->canMoveTo(row, col, possibleMoves)) {
                    Move* curMove = nullptr;
                    //Make piece move
                    makeMove(lastPiece->getRow(), lastPiece->getCol(), row, col, possibleMoves, curMove);
                }

            }
            //Undrag piece
            lastPiece->unfollowMouse();
            pieceFollowingMouse = nullptr;
            lastPiece = nullptr;
        }
        assert(pieceFollowingMouse == nullptr);
        highlightTiles.clear();
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
        @Brief : xét xem quân cờ có màu color có bị chiếu hay không?
            Nếu bị chiếu thì tô đỏ quân cờ đang chiếu và quân vua bị chiếu
            Nếu tham số save = 1, mình sẽ cập nhật: inCheck[color], nếu đang bị chiếu thì tô màu quân chiếu và bị chiếu
            Nếu tham số save = 0, mình chỉ trả về: có bị chiếu hay không
        @Idea : Ý tưởng :
            1. Tìm vị trí của con vua
            2. Ta sẽ duyệt hết bàn cờ: 
                Mỗi quân cờ ta xét nếu quân cờ này có thể ăn được con vua thì ta tô màu đỏ cho ô của con cờ này và con vua.
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
        highlightTilesJustMove.pop_back();
        return true;
    }
    return false;
}

bool ChessBoard::cannotMove(void) { 
    // Quân cờ của màu hiện tại không thể đi được thì trả về true
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) {
        if (board[i][j] && board[i][j]->getColor() == whiteTurn) {
            vector<pair<int, int>> moves;
            getPossibleMoves(board[i][j].get(), moves);
            if ((int)moves.size() > 0) {
                return false;
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

    // V. Judge tie when there are 75 consecutive moves with out any capture or pawn move
    if (haftMoveClock >= 150) return true;

    return false;
}

bool ChessBoard::canRequestTie(void) {
    // III. Threefold Repetition
    if (repState[FENToCheckThreefold()] >= 3) return true;
    // IV. 50-Move Rule
    if (haftMoveClock >= 100) return true;
    return false;
}