#include"../lib/Pieces.h"

Pieces::Pieces() {
    this->currentTextureIndex = 0;
    this->type = "";
    this->isWhite = true;
    this->drag = false;
}

//Pieces::Pieces(const bool& isWhite) {
//    this->currentTextureIndex = 0;
//    this->type = "";
//    this->isWhite = isWhite;
//}

Pieces::Pieces(bool isWhite) {
    this->currentTextureIndex = 0;
    this->isWhite = isWhite;
    this->drag = false;
}

bool Pieces::getColor() const {
    return this->isWhite;
}

int Pieces::getCol() const {
    return this->col;
}

int Pieces::getRow() const {
    return this->row;
}

bool Pieces::loadTexture(const vector<string>& texturePaths) {
    for (const auto& path : texturePaths) {
        Texture texture;
        if (!texture.loadFromFile(path)) {
            throw runtime_error("Unable to load piece texture from: " + path);
        }
        textures.push_back(move(texture)); // Thêm vào vector
    }

    // Đặt texture đầu tiên cho sprite
    sprite.setTexture(textures[currentTextureIndex]);
    return true;
}

void Pieces::changeTexture(size_t index) {
    if (index < textures.size()) {
        this->currentTextureIndex = index;
        Sprite newSprite;
        newSprite.setTexture(textures[currentTextureIndex]);
        this->sprite = newSprite;
    }
}

void Pieces::scaleToFitCell(float cellSize) {
    this->movement.setCellSize((int)cellSize);
    Vector2u textureSize = textures[currentTextureIndex].getSize();
    float scaleX = cellSize / textureSize.x;
    float scaleY = cellSize / textureSize.y;
    float scale = std::min(scaleX, scaleY); // Đảm bảo tỷ lệ đồng nhất
    sprite.setScale(scale, scale);
}

void Pieces::setPosition(int col, int row, float cellSize) {
    this->col = col;
    this->row = row;
    scaleToFitCell(cellSize);
    sprite.setPosition(
        65 + col * cellSize + (cellSize - sprite.getGlobalBounds().width) / 2,
        65 + row * cellSize + (cellSize - sprite.getGlobalBounds().height) / 2
    );
}

void Pieces::draw(RenderWindow& window) {
    window.draw(sprite);
}

string Pieces::getType() const {
    return type;
}

void Pieces::setInitialPosition(const sf::Vector2f& position) {
    this->initialPosition = position;
}

// Drag a piece
bool Pieces::getDrag(void) {
    return this->drag;
}

void Pieces::setDrag(bool state) {
    // Lưu vị trí ban đầu
    this->setInitialPosition(this->sprite.getPosition()); 
    this->drag = state;
}

bool Pieces::contain(const sf::Vector2f& point) {
    return this->sprite.getGlobalBounds().contains(point);
}


// Release a piece
void Pieces::moveTo(const sf::Vector2f& point) {
    sf::Vector2f destination(
        point.x - this->sprite.getGlobalBounds().width / 2,
        point.y - this->sprite.getGlobalBounds().height / 2);
    this->sprite.setPosition(destination);
}

void Pieces::toNearestCell(const sf::Vector2f& point, vector<vector<unique_ptr<Pieces>>>& board) {
    // Xác định chỉ số hàng và cột dựa trên tọa độ `point`
    int nearestRow = static_cast<int>((point.y - this->movement.getBorderThickness()) / this->movement.getCellSize());
    int nearestCol = static_cast<int>((point.x - this->movement.getBorderThickness()) / this->movement.getCellSize());

    // Kiểm tra nếu ô nằm trong phạm vi của bàn cờ
    if (nearestRow < 0 || nearestRow >= board.size() || nearestCol < 0 || nearestCol >= board[0].size()) {
        // Nếu ngoài biên, di chuyển quân cờ về vị trí ban đầu
        this->sprite.setPosition(initialPosition);
        return;
    }

    // Xác định xem ô có nằm trong các nước đi hợp lệ không
    bool isValidMove = false;
    for (const auto& move : possibleMoves) {
        if (move.first == nearestRow && move.second == nearestCol) {
            isValidMove = true;
            break;
        }
    }

    if (isValidMove) {

        // Nếu có quân địch, xóa nó
        if (board[nearestRow][nearestCol] && board[nearestRow][nearestCol]->getColor() != this->getColor()) {
            board[nearestRow][nearestCol].reset();
        }

        // Di chuyển quân cờ đến ô mới và cập nhật `board`
        sf::Vector2f newPosition(
            this->movement.getBorderThickness() + nearestCol * this->movement.getCellSize(),
            this->movement.getBorderThickness() + nearestRow * this->movement.getCellSize()
        );
        this->sprite.setPosition(newPosition);
        
        cout << nearestRow << nearestCol << '\n';
        //board[nearestRow][nearestCol] = std::move(board[this->row][this->col]); // Cập nhật vị trí mới
        //board[this->row][this->col].reset(); // Xóa vị trí cũ

    } else {
        // Nếu nước đi không hợp lệ, quân cờ trở lại vị trí ban đầu
        this->sprite.setPosition(initialPosition);
    }
}