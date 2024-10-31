#include"../lib/ChessBoard.h"

// Thiết lập các thứ
ChessBoard::ChessBoard(RenderWindow* win, int currentBoardIndex) {
    // Khởi tạo
    this->window = win;
    this->boardFiles = {
            "../assets/Chess Board/ChessBoard1.png",
            "../assets/Chess Board/ChessBoard2.png",
            "../assets/Chess Board/ChessBoard3.png",
            "../assets/Chess Board/ChessBoard4.png",
            "../assets/Chess Board/ChessBoard5.png"
    };
    this->currentBoardIndex = currentBoardIndex;

    // Thiết lập bàn cờ
    loadBoardTexture(boardFiles[currentBoardIndex]); // Tải bàn cờ đầu tiên
    this->cellSize = 100;

    // Tạo bàn cờ
    board.clear();
    board.resize(8);

    for (auto& row : board) {
        row.resize(8); // Mỗi hàng sẽ có 8 cột
    }

    this->addPiece(make_unique<Queen>(true), 4, 4); // Hậu trắng

    // Tạo quân cờ
    this->addPiece(make_unique<Rook>(true), 0, 7); // Xe trắng
    this->addPiece(make_unique<Knight>(true), 1, 7); // Mã trắng
    this->addPiece(make_unique<Bishop>(true), 2, 7); // Tượng trắng
    this->addPiece(make_unique<Queen>(true), 3, 7); // Hậu trắng
    this->addPiece(make_unique<King>(true), 4, 7); // Vua trắng
    this->addPiece(make_unique<Bishop>(true), 5, 7); // Tượng trắng
    this->addPiece(make_unique<Knight>(true), 6, 7); // Mã trắng
    this->addPiece(make_unique<Rook>(true), 7, 7); // Xe trắng
                  
    this->addPiece(make_unique<Rook>(false), 0, 0); // Xe đen
    this->addPiece(make_unique<Knight>(false), 1, 0); // Mã đen
    this->addPiece(make_unique<Bishop>(false), 2, 0); // Tượng đen
    this->addPiece(make_unique<Queen>(false), 3, 0); // Hậu đen
    this->addPiece(make_unique<King>(false), 4, 0); // Vua đen
    this->addPiece(make_unique<Bishop>(false), 5, 0); // Tượng đen
    this->addPiece(make_unique<Knight>(false), 6, 0); // Mã đen
    this->addPiece(make_unique<Rook>(false), 7, 0); // Xe đen
                   
    this->addPiece(make_unique<Pawn>(false), 0, 1);
    this->addPiece(make_unique<Pawn>(false), 1, 1);
    this->addPiece(make_unique<Pawn>(false), 2, 1);
    this->addPiece(make_unique<Pawn>(false), 3, 1);
    this->addPiece(make_unique<Pawn>(false), 4, 1);
    this->addPiece(make_unique<Pawn>(false), 5, 1);
    this->addPiece(make_unique<Pawn>(false), 6, 1);
    this->addPiece(make_unique<Pawn>(false), 7, 1);
             
    this->addPiece(make_unique<Pawn>(true), 0, 6);
    this->addPiece(make_unique<Pawn>(true), 1, 6);
    this->addPiece(make_unique<Pawn>(true), 2, 6);
    this->addPiece(make_unique<Pawn>(true), 3, 6);
    this->addPiece(make_unique<Pawn>(true), 4, 6);
    this->addPiece(make_unique<Pawn>(true), 5, 6);
    this->addPiece(make_unique<Pawn>(true), 6, 6);
    this->addPiece(make_unique<Pawn>(true), 7, 6);


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
    cellSize = (boardSize.x * scale) / 8; // Kích thước ô dựa trên tỉ lệ
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

void ChessBoard::update(const sf::Event& event) {
    /*
        Update các sự kiện xảy ra trên bàn cờ
    */

    // Nhấn phím 1, 2, 3 để thay đổi bàn cờ
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Num1) {
            this->changeBoard(0); // Thay đổi sang bàn cờ 1
        }
        else if (event.key.code == sf::Keyboard::Num2) {
            this->changeBoard(1); // Thay đổi sang bàn cờ 2
        }
        else if (event.key.code == sf::Keyboard::Num3) {
            this->changeBoard(2); // Thay đổi sang bàn cờ 3
        }
    }

    // Thao tác kéo thả các quân cờ
    for (auto& pieces : board) {
        for (auto& p : pieces) {
            if (!p) continue;
            sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;
                this->handleMouseClick(mouseX, mouseY); // Xử lý click
            }

            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                // Nếu nhấn chuột trái
                if (event.type == sf::Event::MouseButtonPressed) {

                    // Chuột đang ở trên quân cờ
                    // thì cập nhật lại là quân cờ p đang bị kéo đi
                    if (p->contain(mousePos)) {
                        p->setDrag(true);
                    }
                }

                // Nếu thả chuột
                else if (event.type == sf::Event::MouseButtonReleased) {

                    // Nếu trước đó đang nắm quân cờ này
                    // thì ta sẽ thả quân cờ vào ô gần với con chuột nhất
                    if (p->getDrag()) {
                        p->toNearestCell(mousePos);
                    }

                    // Cập nhật lại là quân cờ không bị kéo đi
                    p->setDrag(false);
                }
            }

            if (event.type == sf::Event::MouseMoved) {

                // Nếu ta di chuyển con chuột trong khi đang nắm quân cờ p 
                // thì ta quân cờ p sẽ đi theo con chuột
                if (p->getDrag()) {
                    p->moveTo(mousePos);
                }
            }
        }
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

// Highline possible move
void ChessBoard::handleMouseClick(int mouseX, int mouseY) {
    int col = (mouseX - 65) / 100; // Kích thước ô là 100, trừ viền 65px
    int row = (mouseY - 65) / 100; // Kích thước ô là 100, trừ viền 65px

    if (col >= 0 && col < 8 && row >= 0 && row < 8) {
        Pieces* clickedPiece = board[row][col].get(); // Lấy quân cờ ở ô đã click

        // Nếu có quân cờ tại ô click
        if (clickedPiece) {
            // Đặt quân cờ được chọn là clickedPiece
            selectedPiece = clickedPiece;

            // Xóa ô tô màu cũ
            highlightTiles.clear();

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
    }
}