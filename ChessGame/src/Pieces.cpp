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
        currentTextureIndex = index;
        sprite.setTexture(textures[currentTextureIndex]);
    }
}

void Pieces::scaleToFitCell(float cellSize) {
    this->movement.setCellSize((int)cellSize);
    float scaleX = cellSize / sprite.getGlobalBounds().width;
    float scaleY = cellSize / sprite.getGlobalBounds().height;
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

// Drag a piece
bool Pieces::getDrag(void) {
    return this->drag;
}

void Pieces::setDrag(bool state) {
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

void Pieces::toNearestCell(const sf::Vector2f& point) {
    /*
        Đặt quân cờ vào trong ô gần point nhất
    */
    float cellSize = this->movement.getCellSize();
    float boardSize = this->movement.getBoardSize();
    
    // Lấy ra chỉ số hàng và cột của point
    int r = this->movement.getRow(point.x);
    int c = this->movement.getCol(point.y);

    // Lấy ra tọa độ tâm của các ô gần điểm point
    vector<sf::Vector2f> cell = {
        this->movement.cordCentre(r - 1, c - 1),
        this->movement.cordCentre(r - 1, c),
        this->movement.cordCentre(r, c - 1),
        this->movement.cordCentre(r, c)
    };

    int mn = -1;
    for (int i = 0; i < 4; i++) {
        if (cell[i] == this->movement.UNDEFINED_POINT) {
            continue;
        }
        if (mn == -1 || this->movement.dist(point, cell[mn] ) > this->movement.dist(point, cell[i])) {
            mn = i;
        }
    }

    cell[mn].x -= this->sprite.getGlobalBounds().width / 2;
    cell[mn].y -= this->sprite.getGlobalBounds().height / 2;
    this->sprite.setPosition(cell[mn]);
}