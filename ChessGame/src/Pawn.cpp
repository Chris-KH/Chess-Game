#include"../lib/Pawn.h"

Pawn::Pawn(bool isWhite) : Pieces(isWhite) {
	vector<string> texturePaths;
	string color = (isWhite ? "white-" : "black-");
	this->type = "pawn";
	texturePaths.push_back("../assets/Standard Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Cartoon Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Pixel Theme/" + color + this->type + ".png");

	loadTexture(texturePaths);
}

string Pawn::getType() const {
	return this->type;
}

vector<pair<int, int>> Pawn::getPossibleMoves(const vector<vector<pair<bool, bool>>>& board) {
    vector<std::pair<int, int>> moves;
    int direction = (isWhite) ? -1 : 1;

    // Nước đi thẳng
    if (row + direction >= 0 && row + direction < 8 && !board[row + direction][col].first) {
        moves.emplace_back(row + direction, col);
        // Nước đi hai ô khi chưa di chuyển
        if ((isWhite && row == 6) || (!isWhite && row == 1)) {
            if (!board[row + 2 * direction][col].first) {
                moves.emplace_back(row + 2 * direction, col);
            }
        }
    }
    // Nước đi chéo để ăn quân
    if (col - 1 >= 0 && row + direction >= 0 && row + direction < 8 && board[row + direction][col - 1].first && board[row + direction][col - 1].second != isWhite) {
        moves.emplace_back(row + direction, col - 1);
    }
    if (col + 1 < 8 && row + direction >= 0 && row + direction < 8 && board[row + direction][col + 1].first && board[row + direction][col + 1].second != isWhite) {
        moves.emplace_back(row + direction, col + 1);
    }
    return moves;
}