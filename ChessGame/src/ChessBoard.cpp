#include"../lib/ChessBoard.h"

ChessBoard::ChessBoard(RenderWindow* win, const vector<string>& boardPaths, int currentBoardIndex) {
    this->window = win;
    this->boardFiles = boardPaths;
    this->currentBoardIndex = currentBoardIndex;
    loadBoardTexture(boardFiles[currentBoardIndex]); // Tải bàn cờ đầu tiên
    squareSize = 800.0f / 8; // Kích thước ô cờ (800x800 / 8)

    // Tải phông chữ
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) { // Thay bằng đường dẫn font có sẵn
        throw runtime_error("Unable to load font");
    }

    border.setSize(Vector2f(800 + 20, 800 + 20)); // Kích thước rìa
    border.setFillColor(Color::White); // Màu rìa
    border.setPosition(-10, -10); // Vị trí của rìa

    // Thiết lập các nhãn cho hàng
    for (int i = 0; i < 8; ++i) {
        rowLabels[i].setFont(font);
        rowLabels[i].setString(to_string(8 - i)); // Số từ 8 đến 1
        rowLabels[i].setCharacterSize(30); // Kích thước chữ
        rowLabels[i].setFillColor(Color::Black); // Màu chữ
        rowLabels[i].setPosition(0, i * squareSize + 10); // Vị trí bên trái (thêm khoảng cách)
    }

    // Thiết lập các nhãn cho cột
    for (int i = 0; i < 8; ++i) {
        colLabels[i].setFont(font);
        colLabels[i].setString(static_cast<char>('A' + i)); // Chữ cái từ A đến H
        colLabels[i].setCharacterSize(30); // Kích thước chữ
        colLabels[i].setFillColor(Color::Black); // Màu chữ
        colLabels[i].setPosition(i * squareSize + 10, 810); // Vị trí phía dưới (thêm khoảng cách)
    }
}

bool ChessBoard::loadBoardTexture(const string& filePath) {
    if (!boardTexture.loadFromFile(filePath)) {
        return false; // Không thể load ảnh
    }
    boardSprite.setTexture(boardTexture);
    updateBoardScale(); // Cập nhật tỉ lệ
    return true;
}

void ChessBoard::updateBoardScale() {
    Vector2u windowSize = window->getSize();
    Vector2u boardSize = boardTexture.getSize();

    // Xác định kích thước mục tiêu cho bàn cờ (ví dụ: 800x800)
    const float targetWidth = 800.0f;
    const float targetHeight = 800.0f;

    // Tính tỉ lệ cho bàn cờ
    float scaleX = targetWidth / static_cast<float>(boardSize.x);
    float scaleY = targetHeight / static_cast<float>(boardSize.y);

    // Chọn tỉ lệ nhỏ hơn để giữ nguyên tỉ lệ của hình ảnh
    float scale = min(scaleX, scaleY);

    boardSprite.setScale(scale, scale); // Áp dụng tỉ lệ

    // Cập nhật kích thước ô
    squareSize = (boardSize.x * scale) / 8; // Kích thước ô dựa trên tỉ lệ
}

bool ChessBoard::changeBoard(int newIndex) {
    if (newIndex >= 0 && newIndex < boardFiles.size()) {
        currentBoardIndex = newIndex;
        return loadBoardTexture(boardFiles[currentBoardIndex]); // Tải bàn cờ mới
    }
    return false; // Chỉ số không hợp lệ
}

void ChessBoard::addPiece(shared_ptr<Pieces> piece, int col, int row) {
    piece->setPosition(col, row, squareSize); // Đặt vị trí quân cờ
    pieces.push_back(piece);
}

void ChessBoard::draw() {
    window->draw(border);
    window->draw(boardSprite);
    for (const auto& piece : pieces) {
        piece->draw(*window);
    }

    // Vẽ nhãn hàng
    for (int i = 0; i < 8; ++i) {
        window->draw(rowLabels[i]);
    }

    // Vẽ nhãn cột
    for (int i = 0; i < 8; ++i) {
        window->draw(colLabels[i]);
    }
}