#include"../lib/King.h"

King::King(bool isWhite, int originCol, int originRow) : Pieces(isWhite, originCol, originRow) {
	vector<string> texturePaths;
	string color = (isWhite ? "white-" : "black-");
	this->type = "king";
    this->typeKey = (isWhite ? 'K' : 'k');
	texturePaths.push_back("../assets/Standard Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Cartoon Theme/" + color + this->type + ".png");
	texturePaths.push_back("../assets/Pixel Theme/" + color + this->type + ".png");
    texturePaths.push_back("../assets/Neo Theme/" + color + this->type + ".png");

	loadTexture(texturePaths);
}

string King::getType() const {
	return this->type;
}

char King::getTypeKey() const {
    return this->typeKey;
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
    if (canCastleKingside(board)) moves.emplace_back(this->getRow(), 6);
    if (canCastleQueenside(board)) moves.emplace_back(this->getRow(), 2);
	return moves;
}

bool King::checkPromote() const {
	return false;
}

bool King::canCastleKingside(const vector<vector<unique_ptr<Pieces>>>& board) {
    int colKing = 4;   // Vị trí cột của Vua ban đầu
    int colRookKingside = 7; // Vị trí cột của Xe phía Kingside
    int row = this->getRow();

    // Kiểm tra điều kiện nhập thành gần
    if (auto& king = board[row][colKing]) {
        if (king->getAlreadyMove()) return false; // Vua đã di chuyển

        if (auto& rook = board[row][colRookKingside]) {
            if (rook->getAlreadyMove()) return false; // Xe đã di chuyển

            // Kiểm tra nếu các ô giữa Vua và Xe trống
            if (board[row][5] != nullptr || board[row][6] != nullptr) return false;

            // Kiểm tra nếu các ô mà Vua đi qua hoặc đích đến không bị đe dọa
            if (isThreatened(row, 4, board) || isThreatened(row, 5, board) || isThreatened(row, 6, board)) return false;

            return true; // Có thể nhập thành gần
        }
    }
    return false;
}

bool King::canCastleQueenside(const vector<vector<unique_ptr<Pieces>>>& board) {
    int colKing = 4;
    int colRookQueenside = 0; // Vị trí cột của Xe phía Queenside
    int row = this->getRow();

    // Kiểm tra điều kiện nhập thành xa
    if (auto& king = board[row][colKing]) {
        if (king->getAlreadyMove()) return false; // Vua đã di chuyển

        if (auto& rook = board[row][colRookQueenside]) {
            if (rook->getAlreadyMove()) return false; // Xe đã di chuyển

            // Kiểm tra nếu các ô giữa Vua và Xe trống
            if (board[row][1] != nullptr || board[row][2] != nullptr || board[row][3] != nullptr) return false;

            // Kiểm tra nếu các ô mà Vua đi qua hoặc đích đến không bị đe dọa
            if (isThreatened(row, 4, board) || isThreatened(row, 3, board) || isThreatened(row, 2, board)) return false;

            return true; // Có thể nhập thành xa
        }
    }
    return false;
}

void King::performCastling(bool isKingside, vector<vector<unique_ptr<Pieces>>>& board) {
    int colKing = 4;
    int row = this->getRow();
    if (isKingside) {
        // Nhập thành gần
        board[row][5] = move(board[row][7]);          // Di chuyển Xe đến f1
        board[row][5]->setPosition(5, row);
        board[row][5]->setAlreadyMove(board[row][5]->getAlreadyMove() + 1);
    }
    else {
        // Nhập thành xa
        board[row][3] = move(board[row][0]);          // Di chuyển Xe đến d1
        board[row][3]->setPosition(3, row);
        board[row][3]->setAlreadyMove(board[row][3]->getAlreadyMove() + 1);
    }
}

void King::attemptCastling(vector<vector<unique_ptr<Pieces>>>& board, bool isKingSide) {
    this->performCastling(isKingSide, board);
}

unique_ptr<Pieces> King::clone() const {
    return make_unique<King>(*this);
}