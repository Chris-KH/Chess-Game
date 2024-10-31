#include"../lib/Bishop.h"

Bishop::Bishop(bool isWhite) : Pieces(isWhite) {
	vector<string> texturePaths;
	string color = (isWhite ? "white-" : "black-");
	this->type = "bishop";
	texturePaths.push_back("../assets/Standard Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Cartoon Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Pixel Theme/" + color + this->type + ".png");

	loadTexture(texturePaths);
}

string Bishop::getType() const {
	return this->type;
}

vector<pair<int, int>> Bishop::getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board) {
	vector<pair<int, int>> moves;
	// Duyệt theo 4 đường chéo
	for (int i = 1; i < 8; ++i) {
		if (row + i < 8 && col + i < 8 && (!board[row + i][col + i] || board[row + i][col + i].get()->getColor() != isWhite)) moves.emplace_back(row + i, col + i); // Chéo phải dưới
		if (row + i < 8 && col - i >= 0 && (!board[row + i][col - i] || board[row + i][col - i].get()->getColor() != isWhite)) moves.emplace_back(row + i, col - i); // Chéo trái dưới
		if (row - i >= 0 && col + i < 8 && (!board[row - i][col + i] || board[row - i][col + i].get()->getColor() != isWhite)) moves.emplace_back(row - i, col + i); // Chéo phải trên
		if (row - i >= 0 && col - i >= 0 && (!board[row - i][col - i] || board[row - i][col - i].get()->getColor() != isWhite)) moves.emplace_back(row - i, col - i); // Chéo trái trên
	}
	return moves;
}