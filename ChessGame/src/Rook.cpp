#include"../lib/Rook.h"

Rook::Rook(bool isWhite, int originCol, int originRow) : Pieces(isWhite, originCol, originRow) {
	vector<string> texturePaths;
	string color = (isWhite ? "white-" : "black-");
	this->type = "rook";
	texturePaths.push_back("../assets/Standard Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Cartoon Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Pixel Theme/" + color + this->type + ".png");
    texturePaths.push_back("../assets/Neo Theme/" + color + this->type + ".png");

	loadTexture(texturePaths);
}

string Rook::getType() const {
	return this->type;
}

vector<pair<int, int>> Rook::getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board) {
    vector<pair<int, int>> moves;
    int directions[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} }; // Duyệt các hướng lên, xuống, trái, phải

    for (auto& dir : directions) {
        int newRow = row;
        int newCol = col;

        while (true) {
            newRow += dir[0];
            newCol += dir[1];

            // Kiểm tra xem ô có nằm trong bàn cờ không
            if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) break;

            // Nếu ô trống, thêm vào danh sách
            if (!board[newRow][newCol]) {
                moves.emplace_back(newRow, newCol);
            }
            // Nếu ô chứa quân địch, thêm vào rồi dừng
            else if (board[newRow][newCol]->getColor() != isWhite) {
                moves.emplace_back(newRow, newCol);
                break;
            }
            // Nếu ô chứa quân cùng màu, dừng lại
            else {
                break;
            }
        }
    }
    this->possibleMoves = moves;
	return moves;
}

bool Rook::checkPromote() const {
    return false;
}