#include"../lib/Pieces.h"

bool Pieces::loadTexture(const string& filePath) {
    if (!texture.loadFromFile(filePath)) {
        return false; 
    }
    sprite.setTexture(texture);
    return true;
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