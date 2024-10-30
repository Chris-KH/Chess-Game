#include"../lib/ChessBoard.h"

// Thiết lập các thứ
ChessBoard::ChessBoard(RenderWindow* win, int currentBoardIndex) {
    // Khởi tạo
    this->window = win;
    this->boardFiles = {
            "../assets/Chess Board/ChessBoard1.png",
            "../assets/Chess Board/ChessBoard2.png",
            "../assets/Chess Board/ChessBoard3.png"
    };
    this->currentBoardIndex = currentBoardIndex;

    // Thiết lập bàn cờ
    loadBoardTexture(boardFiles[currentBoardIndex]); // Tải bàn cờ đầu tiên
    this->cellSize = 100;

    // Tạo quân cờ
    this->addPiece(make_shared<Rook>(true), 0, 7); // Xe trắng
    this->addPiece(make_shared<Knight>(true), 1, 7); // Mã trắng
    this->addPiece(make_shared<Bishop>(true), 2, 7); // Tượng trắng
    this->addPiece(make_shared<Queen>(true), 3, 7); // Hậu trắng
    this->addPiece(make_shared<King>(true), 4, 7); // Vua trắng
    this->addPiece(make_shared<Bishop>(true), 5, 7); // Tượng trắng
    this->addPiece(make_shared<Knight>(true), 6, 7); // Mã trắng
    this->addPiece(make_shared<Rook>(true), 7, 7); // Xe trắng

    this->addPiece(make_shared<Rook>(false), 0, 0); // Xe đen
    this->addPiece(make_shared<Knight>(false), 1, 0); // Mã đen
    this->addPiece(make_shared<Bishop>(false), 2, 0); // Tượng đen
    this->addPiece(make_shared<Queen>(false), 3, 0); // Hậu đen
    this->addPiece(make_shared<King>(false), 4, 0); // Vua đen
    this->addPiece(make_shared<Bishop>(false), 5, 0); // Tượng đen
    this->addPiece(make_shared<Knight>(false), 6, 0); // Mã đen
    this->addPiece(make_shared<Rook>(false), 7, 0); // Xe đen

    this->addPiece(make_shared<Pawn>(false), 0, 1);
    this->addPiece(make_shared<Pawn>(false), 1, 1);
    this->addPiece(make_shared<Pawn>(false), 2, 1);
    this->addPiece(make_shared<Pawn>(false), 3, 1);
    this->addPiece(make_shared<Pawn>(false), 4, 1);
    this->addPiece(make_shared<Pawn>(false), 5, 1);
    this->addPiece(make_shared<Pawn>(false), 6, 1);
    this->addPiece(make_shared<Pawn>(false), 7, 1);

    this->addPiece(make_shared<Pawn>(true), 0, 6);
    this->addPiece(make_shared<Pawn>(true), 1, 6);
    this->addPiece(make_shared<Pawn>(true), 2, 6);
    this->addPiece(make_shared<Pawn>(true), 3, 6);
    this->addPiece(make_shared<Pawn>(true), 4, 6);
    this->addPiece(make_shared<Pawn>(true), 5, 6);
    this->addPiece(make_shared<Pawn>(true), 6, 6);
    this->addPiece(make_shared<Pawn>(true), 7, 6);


    // Thiết lập phông chữ
    //if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) { // Thay bằng đường dẫn font có sẵn
    //    throw runtime_error("Unable to load font");
    //}
}

void ChessBoard::addPiece(shared_ptr<Pieces> piece, int col, int row) {
    /*
        Dùng để khởi tạo quân cờ
    */
    piece->setPosition(col, row, cellSize); // Đặt vị trí quân cờ
    pieces.push_back(piece);
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
    for (shared_ptr<Pieces> p : pieces) {
        sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

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
            else if(event.type == sf::Event::MouseButtonReleased) {

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

void ChessBoard::draw() {
    window->draw(border);
    window->draw(boardSprite);
    for (const auto& piece : pieces) {
        piece->draw(*window);
    }

    // Vẽ nhãn hàng
    /*for (int i = 0; i < 8; ++i) {
        window->draw(rowLabels[i]);
    }*/

    // Vẽ nhãn cột
    //for (int i = 0; i < 8; ++i) {
    //    window->draw(colLabels[i]);
    //}
}


// Highline possible move
//void ChessBoard::showPossibleMoves(int col, int row) {
//    if (pieces[row][col]) {
//        auto moves = pieces[row][col]->getPossibleMoves(pieces);
//        for (auto& move : moves) {
//            int moveRow = move.first;
//            int moveCol = move.second;
//            // Tô sáng các ô có thể di chuyển
//            highlightSquare(moveCol, moveRow);
//        }
//    }
//}
//
//void ChessBoard::highlightSquare(int col, int row) {
//    sf::RectangleShape highlight(sf::Vector2f(cellSize, cellSize));
//    highlight.setFillColor(sf::Color(0, 255, 0, 128)); // Màu tô sáng
//    highlight.setPosition(col * cellSize, row * cellSize);
//    window.draw(highlight);
//}