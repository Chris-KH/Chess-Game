#include"../lib/Pieces.h"

Pieces::Pieces() {
    this->currentTextureIndex = 0;
    this->type = "";
    this->isWhite = true;
    this->chosen = false;
}

//Pieces::Pieces(const bool& isWhite) {
//    this->currentTextureIndex = 0;
//    this->type = "";
//    this->isWhite = isWhite;
//}

Pieces::Pieces(bool isWhite) {
    this->currentTextureIndex = 0;
    this->isWhite = isWhite;
    this->chosen = false;
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
void Pieces::followMouse(sf::Vector2i mousePos) {
    float x = mousePos.x - sprite.getGlobalBounds().width / 2.0;
    float y = mousePos.y - sprite.getGlobalBounds().height / 2.0;
    sprite.setPosition(x, y);
}