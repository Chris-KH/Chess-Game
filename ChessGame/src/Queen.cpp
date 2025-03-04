﻿#include"../lib/Queen.h"

Queen::Queen(bool isWhite, int originCol, int originRow) : Pieces(isWhite, originCol, originRow) {
	vector<string> texturePaths;
	string color = (isWhite ? "white-" : "black-");
	this->type = "queen";
    this->typeKey = (isWhite ? 'Q' : 'q');
	texturePaths.push_back("../assets/Standard Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Cartoon Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Pixel Theme/" + color + this->type + ".png");
    texturePaths.push_back("../assets/Neo Theme/" + color + this->type + ".png");
    texturePaths.push_back("../assets/Game Room Theme/" + color + this->type + ".png");
    texturePaths.push_back("../assets/Neon Theme/" + color + this->type + ".png");

	loadTexture(texturePaths);
}

string Queen::getType() const {
	return this->type;
}

char Queen::getTypeKey() const {
    return this->typeKey;
}

vector<pair<int, int>> Queen::getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board) {
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
            else if (board[newRow][newCol]->getColor() != this->getColor()) {
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

bool Queen::checkPromote() const {
    return false;
}

unique_ptr<Pieces> Queen::clone() const {
    return make_unique<Queen>(*this);
}