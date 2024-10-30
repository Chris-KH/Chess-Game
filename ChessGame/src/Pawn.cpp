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
	return {};
}