#include"../lib/Pieces.h"

Pieces::Pieces() {
    this->currentTextureIndex = 0;
    this->type = "";
    this->isWhite = true;
    this->chosen = false;
}

Pieces::Pieces(bool isWhite, int originCol, int originRow) {
    this->currentTextureIndex = 0;
    this->isWhite = isWhite;
    this->originRow = originRow;
    this->originCol = originCol;
    this->chosen = false;
}


//isWhite
bool Pieces::getColor() const {
    return this->isWhite;
}

int Pieces::getCol() const {
    return this->col;
}

int Pieces::getRow() const {
    return this->row;
}

size_t Pieces::getCurrentTextureIndex() const {
    return this->currentTextureIndex;
}

bool Pieces::getAlreadyMove(int row, int col) const {
    return (row != this->originRow || col != this->originCol);
}

bool Pieces::loadTexture(const vector<string>& texturePaths) {
    for (const auto& path : texturePaths) {
        Texture texture;
        if (!texture.loadFromFile(path)) {
            throw runtime_error("Unable to load piece texture from: " + path);
        }
        textures.push_back(move(texture)); 
    }
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
    float scale = min(scaleX, scaleY); 
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

void Pieces::setInitialPosition(const Vector2f& position) {
    this->initialPosition = position;
}

// Drag a piece
void Pieces::followMouse(Vector2i mousePos) {
    float x = mousePos.x - sprite.getGlobalBounds().width / 2.0;
    float y = mousePos.y - sprite.getGlobalBounds().height / 2.0;
    sprite.setPosition(x, y);
}

bool Pieces::canMoveTo(const int& row, const int& col, const vector<vector<unique_ptr<Pieces>>>& board) {
    vector<pair<int, int>> possibleMoves = this->getPossibleMoves(board);
    return find(possibleMoves.begin(), possibleMoves.end(), make_pair(row, col)) != possibleMoves.end();
}

bool Pieces::isThreatened(int row, int col, const vector<vector<unique_ptr<Pieces>>>& board) const {
    // Duyệt qua toàn bộ bàn cờ
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            // Bỏ qua ô trống
            if (board[r][c] == nullptr) continue;

            // Lấy quân cờ tại ô đó
            auto piece = board[r][c].get();

            // Kiểm tra nếu quân cờ không là cờ chùng màu
            if (piece->getColor() != getColor()) {
                // Kiểm tra nếu quân cờ có thể di chuyển đến ô đích
                bool curCheck = false;
                if (piece->getType() == "king") { // 1. Vua
                    if (abs(r - getRow()) <= 1 && abs(c - getCol()) <= 1) curCheck = true;
                }
                else if (board[r][c]->getType() == "queen") { // 2. Hậu
                    if (r == getRow() || c == getCol() || 
                        r + c == getRow() + getCol() || r - c == getRow() - getCol()) { // Cùng hàng, cột, đường chéo
                        curCheck = true;
                        int dx = r - getRow(), dy = c - getCol(), d = max(abs(dx), abs(dy));
                        dx /= d, dy /= d;
                        int x = getRow() + dx, y = getCol() + dy;
                        while (x != r || y != c) {
                            if (board[x][y] != nullptr) {
                                curCheck = false;
                                break;
                            }
                            x += dx, y += dy;
                        }
                    }
                }
                else if (board[r][c]->getType() == "bishop") { // 3. Tượng
                    if (r + c == getRow() + getCol() || r - c == getRow() - getCol()) { // cùng đường chéo
                        curCheck = true;
                        int dx = r - getRow(), dy = c - getCol(), d = max(abs(dx), abs(dy));
                        dx /= d, dy /= d;
                        int x = getRow() + dx, y = getCol() + dy;
                        while (x != r || y != c) {
                            if (board[x][y] != nullptr) {
                                curCheck = false;
                                break;
                            }
                            x += dx, y += dy;
                        }
                    }
                }
                else if (board[r][c]->getType() == "knight") { // 4. Mã
                    if ((abs(getRow() - r) == 2 && abs(getCol() - c) == 1) ||
                        (abs(getRow() - r) == 1 && abs(getCol() - c) == 2)) {
                        curCheck = true;
                    }
                }
                else if (board[r][c]->getType() == "rook") { // 5. Xe
                    if (r == getRow() || c == getCol()) { // Cùng cột
                        curCheck = true;
                        int dx = r - getRow(), dy = c - getCol(), d = max(abs(dx), abs(dy));
                        dx /= d, dy /= d;
                        int x = r + dx, y = c + dy;
                        while (x != r || y != c) {
                            if (board[x][y] != nullptr) {
                                curCheck = false;
                                break;
                            }
                            x += dx, y += dy;
                        }
                    }
                }
                else if (board[r][c]->getType() == "pawn") { // 6. Tốt
                    int Dir = isWhite ? 1 : -1;
                    if (getRow() == r + Dir && (getCol() == c - 1 || getCol() == c + 1))
                        curCheck = true;
                }
                
                if (curCheck) return true; // Nếu quân cờ hiện tại có thể ăn thì ta bị đe dọa
            }
        }
    }
    return false; // Ô không bị đe dọa
}