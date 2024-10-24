#include "../lib/ChessBoard.h"

Sprite ChessBoard::getSprite(void) {
	return this->sprite;
}

View ChessBoard::getView(void) {
	return this->view;
}

ChessBoard::ChessBoard(void) {
	this->id = 0;
	this->texture[0].loadFromFile("../assets/Chess Board/ChessBoard1.png");
	this->texture[1].loadFromFile("../assets/Chess Board/ChessBoard2.png");
	this->texture[2].loadFromFile("../assets/Chess Board/ChessBoard3.png");
	
	this->size = this->texture[0].getSize();
	this->sprite.setTexture(this->texture[0]);

	this->view.setSize(this->size.x, this->size.y);
	this->view.setCenter(this->size.x / 2, this->size.y / 2);
}

void ChessBoard::resize(Vector2u size) {
	float ratio = float(size.x) / size.y;
	this->view.setSize(this->size.x * ratio, this->size.y);
	this->view.setCenter(this->size.x / 2, this->size.y / 2);
}

void ChessBoard::change (void) {
	this->id = (this->id + 1) % this->NUM;
	this->sprite.setTexture(texture[this->id]);
}