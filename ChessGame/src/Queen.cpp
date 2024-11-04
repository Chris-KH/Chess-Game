#include"../lib/Queen.h"

Queen::Queen(bool isWhite) : Pieces(isWhite) {
	vector<string> texturePaths;
	string color = (isWhite ? "white-" : "black-");
	this->type = "queen";
	texturePaths.push_back("../assets/Standard Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Cartoon Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Pixel Theme/" + color + this->type + ".png");

	loadTexture(texturePaths);
}

string Queen::getType() const {
	return this->type;
}

vector<pair<int, int>> Queen::getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board, bool inCheck = false) {
    vector<pair<int, int>> moves;
    int directions[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

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
    this->possibleMoves = moves;
    return moves;
}