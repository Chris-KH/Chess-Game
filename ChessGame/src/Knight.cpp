#include"../lib/Knight.h"

Knight::Knight(bool isWhite) : Pieces(isWhite) {
	vector<string> texturePaths;
	string color = (isWhite ? "white-" : "black-");
	this->type = "knight";
	texturePaths.push_back("../assets/Standard Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Cartoon Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Pixel Theme/" + color + this->type + ".png");

	loadTexture(texturePaths);
}

string Knight::getType() const {
	return this->type;
}

vector<pair<int, int>> Knight::getPossibleMoves(const vector<vector<pair<bool, bool>>>& board) {
    vector<pair<int, int>> moves;
    int knightMoves[8][2] = { {1, 2}, {2, 1}, {-1, -2}, {-2, -1}, {1, -2}, {-1, 2}, {2, -1}, {-2, 1} };

    for (auto& move : knightMoves) {
        int newRow = row + move[0];
        int newCol = col + move[1];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            if (!board[newRow][newCol].first || board[newRow][newCol].second != isWhite) {
                moves.emplace_back(newRow, newCol);
            }
        }
    }
    return moves;
}