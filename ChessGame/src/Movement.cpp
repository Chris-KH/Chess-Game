#include "../lib/Movement.h"

// Initialization
Movement::Movement(int _cellSize) {
	setCellSize(_cellSize);
}

Movement::Movement(const Movement& movement) {
	cellSize = movement.cellSize;
	boardSize = movement.boardSize;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cords[i][j] = movement.cords[i][j];
		}
	}
}

void Movement::setCellSize(int _cellSize) {
	cellSize = _cellSize;
	boardSize = _cellSize * 8;

	for (int r = 0; r < 8; r++) for (int c = 0; c < 8; c++) {
		cords[r][c] = Vector2f(65 + c * _cellSize, 65 + r * _cellSize);
	}
}

// Call for value
int Movement::getCellSize(void) {
	return cellSize;
}

int Movement::getBoardSize(void) {
	return boardSize;
}

// Calculation
int Movement::getRow(float x) {
	int r = (x - 65) / cellSize;
	if (r < 0 || r > 7) return -1;
	return r;
}

int Movement::getCol(float y) {
	int c = (y - 65) / cellSize;
	if (c < 0 || c > 7) return -1;
	return c;
}

float Movement::getBorderThickness() {
	return 65.0f;
}

float Movement::dist(const Vector2f& x, const Vector2f& y) {
	return sqrt((x.x - y.x) * (x.x - y.x) + (x.y - y.y) * (x.y - y.y));
}

Vector2f Movement::cordLeftTop(int row, int col) {
	if (row < 0 || row > 7 || col < 0 || col > 7) return UNDEFINED_POINT;
	return cords[row][col];
}





Vector2f Movement::cordCentre(int row, int col) {
	/*
		Lấy ra tọa độ tâm của ô hàng row, cột col
	*/
	if (row < 0 || row > 7 || col < 0 || col > 7) return UNDEFINED_POINT;
	return Vector2f(cords[row][col].x + cellSize / 2, cords[row][col].y + cellSize / 2);
}