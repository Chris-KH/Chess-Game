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

vector<pair<int, int>> Queen::getPossibleMoves(const vector<vector<pair<bool, bool>>>& board) {
    vector<pair<int, int>> moves;

    // Duyệt các hướng giống như quân xe và quân tượng
    for (int i = 1; i < 8; ++i) {
        if (row + i < 8 && (!board[row + i][col].first || board[row + i][col].second != isWhite)) moves.emplace_back(row + i, col); // Đi xuống
        if (row - i >= 0 && (!board[row - i][col].first || board[row - i][col].second != isWhite)) moves.emplace_back(row - i, col); // Đi lên
        if (col + i < 8 && (!board[row][col + i].first || board[row][col + i].second != isWhite)) moves.emplace_back(row, col + i); // Đi phải
        if (col - i >= 0 && (!board[row][col - i].first || board[row][col - i].second != isWhite)) moves.emplace_back(row, col - i); // Đi trái
        if (row + i < 8 && col + i < 8 && (!board[row + i][col + i].first || board[row + i][col + i].second != isWhite)) moves.emplace_back(row + i, col + i); // Chéo phải dưới
        if (row - i >= 0 && col + i < 8 && (!board[row - i][col + i].first || board[row - i][col + i].second != isWhite)) moves.emplace_back(row - i, col + i); // Chéo phải trên
        if (row + i < 8 && col - i >= 0 && (!board[row + i][col - i].first || board[row + i][col - i].second != isWhite)) moves.emplace_back(row + i, col - i); // Chéo trái dưới
        if (row - i >= 0 && col - i >= 0 && (!board[row - i][col - i].first || board[row - i][col - i].second != isWhite)) moves.emplace_back(row - i, col - i); // Chéo trái trên
    }
    return moves;
}