#include"../lib/Bishop.h"

Bishop::Bishop(bool isWhite, int originCol, int originRow) : Pieces(isWhite, originCol, originRow) {
	vector<string> texturePaths;
	string color = (isWhite ? "white-" : "black-");
	this->type = "bishop";
	texturePaths.push_back("../assets/Standard Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Cartoon Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Pixel Theme/" + color + this->type + ".png");
    texturePaths.push_back("../assets/Neo Theme/" + color + this->type + ".png");

	loadTexture(texturePaths);
}

string Bishop::getType() const {
	return this->type;
}

vector<pair<int, int>> Bishop::getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board) {
    vector<pair<int, int>> moves;
    int directions[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} }; // Duyệt 4 hướng chéo

    for (auto& dir : directions) {
        int newRow = row;
        int newCol = col;

        while (true) {
            newRow += dir[0];
            newCol += dir[1];

            if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) break;

            if (!board[newRow][newCol]) {
                moves.emplace_back(newRow, newCol);
            }
            else if (board[newRow][newCol]->getColor() != isWhite) {
                moves.emplace_back(newRow, newCol);
                break;
            }
            else {
                break;
            }
        }
    }
    return moves;
}

bool Bishop::checkPromote() const {
    return false;
}

unique_ptr<Pieces> Bishop::clone() const {
    return make_unique<Bishop>(*this);
}