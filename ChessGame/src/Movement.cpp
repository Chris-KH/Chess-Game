#include "../lib/Movement.h"

// Initialization
Movement::Movement(int cellSize) {
	this->setCellSize(cellSize);
}

void Movement::setCellSize(int cellSize) {
	this->cellSize = cellSize;
	this->boardSize = cellSize * 8;

	for (int r = 0; r < 8; r++) for (int c = 0; c < 8; c++) {
		cords[r][c] = sf::Vector2f(65 + r * cellSize, 65 + c * cellSize);
	}
}

// Call for value
int Movement::getCellSize(void) {
	return this->cellSize;
}

int Movement::getBoardSize(void) {
	return this->boardSize;
}

// Calculation
int Movement::getRow(float x) {
	int r = (x - 65) / this->cellSize;
	if (r < 0 || r > 7) return -1;
	return r;
}

int Movement::getCol(float y) {
	int c = (y - 65) / this->cellSize;
	if (c < 0 || c > 7) return -1;
	return c;
}

float Movement::dist(const sf::Vector2f& x, const sf::Vector2f& y) {
	return sqrt((x.x - y.x) * (x.x - y.x) + (x.y - y.y) * (x.y - y.y));
}

sf::Vector2f Movement::cordLeftTop(int row, int col) {
	if (row < 0 || row > 7 || col < 0 || col > 7) return this->UNDEFINED_POINT;
	return this->cords[row][col];
}

sf::Vector2f Movement::cordCentre(int row, int col) {
	if (row < 0 || row > 7 || col < 0 || col > 7) return this->UNDEFINED_POINT;
	return sf::Vector2f(this->cords[row][col].x + cellSize / 2, this->cords[row][col].y + cellSize / 2);
}