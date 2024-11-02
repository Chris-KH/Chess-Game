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
}

void ChessBoard::addPiece(unique_ptr<Pieces> piece, int col, int row) {
    /*
        Dùng để khởi tạo quân cờ
    */
    piece->setPosition(col, row, cellSize); // Đặt vị trí quân cờ
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
    }
    else {
    }

    if (pieceFollowingMouse != nullptr) {
        pieceFollowingMouse->followMouse(sf::Mouse::getPosition(*window));
    }
}

void ChessBoard::draw() {
    window->draw(boardSprite);
    //Draw pieces
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
    vector<std::pair<int, int>> possibleMoves = lastPiece->getPossibleMoves(board);
    if (std::find(possibleMoves.begin(), possibleMoves.end(), make_pair(row, col)) != possibleMoves.end()) {
        // Cập nhật lượt tiếp theo
        alterTurn();

        // Đặt quân cờ vào ô hiện tại
        board[row][col] = std::move(board[lastPiece->getRow()][lastPiece->getCol()]);
        board[row][col]->setPosition(col, row);

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
    // Nếu không đi đúng luật thì đặt quân cờ về lại vị trí ban đầu
    else {
        // selectedPiece->setPosition(lastPiece->getCol(), lastPiece->getRow());
    }
}

void ChessBoard::highlightPossibleMove(Pieces* clickedPiece) {
    // Đặt quân cờ được chọn là clickedPiece
    selectedPiece = clickedPiece;

    // Lấy danh sách các nước đi có thể
    auto possibleMoves = selectedPiece->getPossibleMoves(board);

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