#include"../lib/Rook.h"

Rook::Rook(bool isWhite) : Pieces(isWhite) {
	vector<string> texturePaths;
	string color = (isWhite ? "white-" : "black-");
	this->type = "rook";
	texturePaths.push_back("../assets/Standard Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Cartoon Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Pixel Theme/" + color + this->type + ".png");

	loadTexture(texturePaths);
}

string Rook::getType() const {
	return this->type;
}

vector<pair<int, int>> Rook::getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board) {
	vector<pair<int, int>> moves;

	// Di chuyển dọc và ngang
	for (int i = 1; i < 8; ++i) {
		if (row + i < 8 && (!board[row + i][col] || board[row + i][col].get()->getColor() != isWhite)) moves.emplace_back(row + i, col); // Đi xuống
		if (row - i >= 0 && (!board[row - i][col] || board[row - i][col].get()->getColor() != isWhite)) moves.emplace_back(row - i, col); // Đi lên
		if (col + i < 8 && (!board[row][col + i] || board[row][col + i].get()->getColor() != isWhite)) moves.emplace_back(row, col + i); // Đi phải
		if (col - i >= 0 && (!board[row][col - i] || board[row][col - i].get()->getColor() != isWhite)) moves.emplace_back(row, col - i); // Đi trái
	}
	return moves;
}