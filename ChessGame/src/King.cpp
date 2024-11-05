#include"../lib/King.h"

King::King(bool isWhite) : Pieces(isWhite) {
	vector<string> texturePaths;
	string color = (isWhite ? "white-" : "black-");
	this->type = "king";
	texturePaths.push_back("../assets/Standard Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Cartoon Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Pixel Theme/" + color + this->type + ".png");

	loadTexture(texturePaths);
}

string King::getType() const {
	return this->type;
}

vector<pair<int, int>> King::getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board) {
	vector<pair<int, int>> moves;
	int kingMoves[8][2] = { {1, 1}, {-1, -1}, {1, -1}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {-1, 1} };
	// Duyệt theo 4 hướng ngang và dọc
	for (auto& move : kingMoves) {
		int newRow = row + move[0];
		int newCol = col + move[1];
		if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
			if (!board[newRow][newCol] || board[newRow][newCol].get()->getColor() != isWhite) {
				moves.emplace_back(newRow, newCol);
			}
		}
	}
	this->possibleMoves = moves;
	return moves;
}