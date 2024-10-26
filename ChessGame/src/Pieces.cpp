#include"../lib/Pieces.h"

Pieces::Pieces() {
    this->currentTextureIndex = 0;
    this->type = "";
    this->isWhite = true;
}


//Pieces::Pieces(const bool& isWhite) {
//    this->currentTextureIndex = 0;
//    this->type = "";
//    this->isWhite = isWhite;
//}

Pieces::Pieces(bool isWhite) {
    this->currentTextureIndex = 0;
    this->isWhite = isWhite;
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

void Pieces::scaleToFitSquare(float squareSize) {
    float scaleX = squareSize / sprite.getGlobalBounds().width;
    float scaleY = squareSize / sprite.getGlobalBounds().height;
    float scale = std::min(scaleX, scaleY); // Đảm bảo tỷ lệ đồng nhất
    sprite.setScale(scale, scale);
}

void Pieces::setPosition(int col, int row, float squareSize) {
    scaleToFitSquare(squareSize);
    sprite.setPosition(col * squareSize + (squareSize - sprite.getGlobalBounds().width) / 2,
        row * squareSize + (squareSize - sprite.getGlobalBounds().height) / 2);
}

void Pieces::draw(RenderWindow& window) {
    window.draw(sprite);
}

string Pieces::getType() const {
    return type;
}