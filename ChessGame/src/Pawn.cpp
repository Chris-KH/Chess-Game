#include"../lib/Pawn.h"

Pawn::Pawn(bool isWhite, int originCol, int originRow) : Pieces(isWhite, originCol, originRow) {
	vector<string> texturePaths;
	string color = (isWhite ? "white-" : "black-");
	this->type = "pawn";
	texturePaths.push_back("../assets/Standard Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Cartoon Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Pixel Theme/" + color + this->type + ".png");
    texturePaths.push_back("../assets/Neo Theme/" + color + this->type + ".png");

	loadTexture(texturePaths);
}

string Pawn::getType() const {
	return this->type;
}

vector<pair<int, int>> Pawn::getPossibleMoves(const vector<vector<unique_ptr<Pieces>>>& board) {
    vector<pair<int, int>> moves;
    int direction = (isWhite) ? -1 : 1;

    // Nước đi thẳng
    if (row + direction >= 0 && row + direction < 8 && !board[row + direction][col]) {
        moves.emplace_back(row + direction, col);
        // Nước đi hai ô khi chưa di chuyển
        if ((isWhite && row == 6) || (!isWhite && row == 1)) {
            if (!board[row + 2 * direction][col]) {
                moves.emplace_back(row + 2 * direction, col);
            }
        }
    }
    // Nước đi chéo để ăn quân
    if (col - 1 >= 0 && row + direction >= 0 && row + direction < 8 && board[row + direction][col - 1] && board[row + direction][col - 1].get()->getColor() != isWhite) {
        moves.emplace_back(row + direction, col - 1);
    }
    if (col + 1 < 8 && row + direction >= 0 && row + direction < 8 && board[row + direction][col + 1] && board[row + direction][col + 1].get()->getColor() != isWhite) {
        moves.emplace_back(row + direction, col + 1);
    }

    pair<int, int> enPassant = this->enPassant(board);
    if (enPassant.first != -1) {
        moves.emplace_back(enPassant);
        //cout << enPassant.first << " " << enPassant.second << "\n";
    }
    return moves;
}

bool Pawn::checkPromote() const {
    if ((this->isWhite && row == 0) || (!this->isWhite && row == 7)) return true;
    return false;
}

pair<int, int> Pawn::enPassant(const vector<vector<unique_ptr<Pieces>>>& board) {
    //cout << this->row << " " << this->col << '\n';
    if (isWhite && this->row == 3) {
        if (col + 1 < 8 && board[row][col + 1] && board[row][col + 1]->getType() == "pawn" && board[row][col + 1]->getColor() == false && board[row][col + 1]->getJustMove() && !board[row - 1][col + 1]) {
            return make_pair(row - 1, col + 1);
        }
        if (col - 1 >= 0 && board[row][col - 1] && board[row][col - 1]->getType() == "pawn" && board[row][col - 1]->getColor() == false && board[row][col - 1]->getJustMove() && !board[row - 1][col - 1]) {
            return make_pair(row - 1, col - 1);
        }
    }

    else if (!isWhite && this->row == 4) {
        if (col + 1 < 8 && board[row][col + 1] && board[row][col + 1]->getType() == "pawn" && board[row][col + 1]->getColor() == true && board[row][col + 1]->getJustMove() && !board[row + 1][col + 1]) {
            return make_pair(row + 1, col + 1);
        }
        if (col - 1 >= 0 && board[row][col - 1] && board[row][col - 1]->getType() == "pawn" && board[row][col - 1]->getColor() == true && board[row][col - 1]->getJustMove() && !board[row + 1][col - 1]) {
            return make_pair(row + 1, col - 1);
        }
    }

    return make_pair(-1, -1);
}