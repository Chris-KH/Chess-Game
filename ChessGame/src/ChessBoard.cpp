#include"../lib/ChessBoard.h"

// Thiết lập các thứ
ChessBoard::ChessBoard(RenderWindow* win, int currentBoardIndex) {
    // Khởi tạo
    window = win;
    boardFiles = {
            "../assets/Chess Board/ChessBoard1.png",
            "../assets/Chess Board/ChessBoard2.png",
            "../assets/Chess Board/ChessBoard3.png",
            "../assets/Chess Board/ChessBoard4.png",
            "../assets/Chess Board/ChessBoard5.png"
    };
    currentBoardIndex = currentBoardIndex;

    // Thiết lập bàn cờ
    loadBoardTexture(boardFiles[currentBoardIndex]); // Tải bàn cờ đầu tiên
    cellSize = 100;

    // Tạo bàn cờ
    board.clear();
    board.resize(8);

    for (auto& row : board) {
        row.resize(8); // Mỗi hàng sẽ có 8 cột
    }

    // Tạo quân cờ
    addPiece(make_unique<Rook>(true), 0, 7); // Xe trắng
    addPiece(make_unique<Knight>(true), 1, 7); // Mã trắng
    addPiece(make_unique<Bishop>(true), 2, 7); // Tượng trắng
    addPiece(make_unique<Queen>(true), 3, 7); // Hậu trắng
    addPiece(make_unique<King>(true), 4, 7); // Vua trắng
    addPiece(make_unique<Bishop>(true), 5, 7); // Tượng trắng
    addPiece(make_unique<Knight>(true), 6, 7); // Mã trắng
    addPiece(make_unique<Rook>(true), 7, 7); // Xe trắng
                  
    addPiece(make_unique<Rook>(false), 0, 0); // Xe đen
    addPiece(make_unique<Knight>(false), 1, 0); // Mã đen
    addPiece(make_unique<Bishop>(false), 2, 0); // Tượng đen
    addPiece(make_unique<Queen>(false), 3, 0); // Hậu đen
    addPiece(make_unique<King>(false), 4, 0); // Vua đen
    addPiece(make_unique<Bishop>(false), 5, 0); // Tượng đen
    addPiece(make_unique<Knight>(false), 6, 0); // Mã đen
    addPiece(make_unique<Rook>(false), 7, 0); // Xe đen
                   
    addPiece(make_unique<Pawn>(false), 0, 1);
    addPiece(make_unique<Pawn>(false), 1, 1);
    addPiece(make_unique<Pawn>(false), 2, 1);
    addPiece(make_unique<Pawn>(false), 3, 1);
    addPiece(make_unique<Pawn>(false), 4, 1);
    addPiece(make_unique<Pawn>(false), 5, 1);
    addPiece(make_unique<Pawn>(false), 6, 1);
    addPiece(make_unique<Pawn>(false), 7, 1);
             
    addPiece(make_unique<Pawn>(true), 0, 6);
    addPiece(make_unique<Pawn>(true), 1, 6);
    addPiece(make_unique<Pawn>(true), 2, 6);
    addPiece(make_unique<Pawn>(true), 3, 6);
    addPiece(make_unique<Pawn>(true), 4, 6);
    addPiece(make_unique<Pawn>(true), 5, 6);
    addPiece(make_unique<Pawn>(true), 6, 6);
    addPiece(make_unique<Pawn>(true), 7, 6);

    // Player turn
    whiteTurn = true;

    // Check
    inCheck[0] = inCheck[1] = false;

    // Game Over
    gameOver = false;
}

void ChessBoard::addPiece(unique_ptr<Pieces> piece, int col, int row) {
    /*
        Dùng để khởi tạo quân cờ
    */
    piece->setPosition(col, row, cellSize); // Đặt vị trí quân cờ
    board[row][col].reset();
    board[row][col] = move(piece);
}

bool ChessBoard::loadBoardTexture(const string& filePath) {
    /*
        Dùng để khởi tạo / thay đổi bàn cờ
    */
    if (!boardTexture.loadFromFile(filePath)) {
        return false; // Không thể load ảnh
    }
    boardSprite.setTexture(boardTexture);
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

void ChessBoard::update(const sf::Event& event) {
    /*
        Update các sự kiện xảy ra trên bàn cờ
    */

    // Nhấn số [1..5] để thay đổi bàn cờ
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Num1) {
            changeBoard(0); // Thay đổi sang bàn cờ 1
        }
        else if (event.key.code == sf::Keyboard::Num2) {
            changeBoard(1); // Thay đổi sang bàn cờ 2
        }
        else if (event.key.code == sf::Keyboard::Num3) {
            changeBoard(2); // Thay đổi sang bàn cờ 3
        }
        else if (event.key.code == sf::Keyboard::Num4) {
            this->changeBoard(3); // Thay đổi sang bàn cờ 4
        }
        else if (event.key.code == sf::Keyboard::Num5) {
            this->changeBoard(4); // Thay đổi sang bàn cờ 5
        }
        else if (event.key.code == sf::Keyboard::Num6) {
            this->changePieces(0); 
        }
        else if (event.key.code == sf::Keyboard::Num7) {
            this->changePieces(1); 
        }
        else if (event.key.code == sf::Keyboard::Num8) {
            this->changePieces(2); 
        }
    }
    // Nhấn chuột trái
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;
        handleMousePress(mouseX, mouseY);
    }
    // Nếu thả chuột trái
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;
        handleMouseRelease(mouseX, mouseY);
        if (isCheck(whiteTurn, true)) {
            if (cannotMove()) {
                gameOver = true; // Checkmate
            }
        }
        else {
            if (cannotMove()) {
                gameOver = true; // Stalemate
            }
        }
        isCheck(1 - whiteTurn, true);
    }

    if (pieceFollowingMouse != nullptr) {
        pieceFollowingMouse->followMouse(sf::Mouse::getPosition(*window));
    }
}

void ChessBoard::draw() {
    window->draw(boardSprite);
    //Draw pieces
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
    for (pair<int, int> pii : vpii) {
        // Giả sử ta đặt quân cờ đang xét vào ô hiện tại
        unique_ptr<Pieces> save = move(board[pii.first][pii.second]);
        board[pii.first][pii.second] = move(board[r][c]);
        // Kiểm tra xem có hết bị chiếu hay chưa (tham chiếu save = false vì không cần lưu)
        if (isCheck(color, false) == false) {
            ret.push_back(pii);
        }
        // Sau khi kiểm tra xong thì trả về vị trí ban đầu
        board[r][c] = move(board[pii.first][pii.second]);
        board[pii.first][pii.second] = move(save);
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

    int row = (mouseY - 65) / 100; // Kích thước ô là 100, trừ viền 65px
    int col = (mouseX - 65) / 100; // Kích thước ô là 100, trừ viền 65px

    // Nếu ô được chọn nằm ngoài bàn cờ thì ta không làm gì
    if (col < 0 || col >= 8 || row < 0 || row >= 8) return;

    Pieces* lastPiece = selectedPiece; // lastPiece = quân cờ cũ
    selectedPiece = board[row][col].get(); // Lấy quân cờ ở ô đã click

    // Nếu nước đi hiện tại là đúng luật
    vector<std::pair<int, int>> possibleMoves; 
    getPossibleMoves(lastPiece, possibleMoves);

    if (std::find(possibleMoves.begin(), possibleMoves.end(), make_pair(row, col)) != possibleMoves.end()) {
        // Cập nhật lượt tiếp theo
        alterTurn();

        // Đặt quân cờ từ ô cũ đến ô hiện tại
        {
            board[row][col].reset();
            board[row][col] = move(board[lastPiece->getRow()][lastPiece->getCol()]);
            board[row][col]->setPosition(col, row);
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

        sf::RectangleShape highlightTile(sf::Vector2f(100, 100)); // Kích thước ô là 100x100
        highlightTile.setPosition(65 + targetCol * 100, 65 + targetRow * 100); // Đặt vị trí ô tô màu với viền

        // Nếu ô chứa quân địch, tô màu đỏ; nếu không, tô màu xanh
        if (board[targetRow][targetCol] && board[targetRow][targetCol]->getColor() != selectedPiece->getColor()) {
            highlightTile.setFillColor(sf::Color(255, 99, 71, 100)); // Màu đỏ
        }
        else {
            highlightTile.setFillColor(sf::Color(144, 238, 144, 100)); // Màu xanh
        }

        highlightTiles.push_back(highlightTile); // Thêm ô tô màu vào danh sách
    }
    // Tô cả ô đang đứng
    sf::RectangleShape highlightTile(sf::Vector2f(100, 100)); // Kích thước ô là 100x100
    highlightTile.setPosition(65 + selectedPiece->getCol() * 100, 65 + selectedPiece->getRow() * 100); // Đặt vị trí ô tô màu với viền
    highlightTile.setFillColor(sf::Color(100, 255, 100, 128)); // Màu xanh
    highlightTiles.push_back(highlightTile);
}

// Player turn
void ChessBoard::alterTurn(void) {
    whiteTurn ^= true;
}

// Detect check, checkmate, draw
bool ChessBoard::isCheck(bool color, bool save) {
    /*
        @Brief xét xem quân cờ có màu hiện tại có bị chiếu hay không? True = có, False = không
            Nếu bị chiếu thì tô đỏ quân cờ đang chiếu và quân vua bị chiếu
            Nếu tham số save = 1, mình sẽ cập nhật các thay đổi
            Nếu tham số save = 0, mình chỉ trả về kết quả chứ không cập nhật kết quả
        @Idea Ý tưởng: 
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
    int r, c;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j].get() != nullptr && board[i][j]->getType() == "king" && 
                board[i][j]->getColor() == color) {
                king = board[i][j].get();
                r = i, c = j;
            }
        }
    }

    // Xét xem có quân cờ nào chiếu vào không
    sf::RectangleShape tile(sf::Vector2f(cellSize, cellSize)); // Kích thước ô là 100x100
    tile.setFillColor(sf::Color(255, 99, 71, 100)); // Màu đỏ

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // Nếu quân của đối phương chiếu quân vua của ta thì tô màu đỏ
            if (board[i][j] != nullptr && board[i][j]->getColor() != color) {
                vector<pair<int, int>> moves = board[i][j]->getPossibleMoves(board);
                if (find(moves.begin(), moves.end(), make_pair(r, c)) != 
                    moves.end()) {
                    tile.setPosition(65 + j * cellSize, 65 + i * cellSize); // Đặt vị trí ô tô màu với viền
                    if (save) {
                        checkTiles[color].push_back(tile);
                        inCheck[color] = true;
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

bool ChessBoard::isDraw(void) {
    return false;
}