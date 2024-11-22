#include "../lib/Button.h"

void Button::setSpriteButton(std::string name, std::string imagePath, int width, int height, int posX, int posY) {
	// Update name, width, height, posX, posY
	this->name = name, this->width = width, this->height = height, this->posX = posX, this->posY = posY;
	// Load the texture
	if (!texture.loadFromFile(imagePath)) {
		throw runtime_error("Cannot load " + imagePath + "!\n");
	}
	sprite.setTexture(texture);
	// Set texture
	float ratioWidth = width / (float)sprite.getGlobalBounds().width;
	float ratioHeight = height / (float)sprite.getGlobalBounds().height;
	sprite.setScale(ratioWidth, ratioHeight);
	sprite.setPosition(posX, posY);
	// Set back rectangle
	rectangle.setOutlineThickness(5);
	rectangle.setFillColor(Color::White);
	rectangle.setOutlineColor(Color(0xA6, 0x8A, 0x64));
	rectangle.setSize(Vector2f(width + 20, height + 20)); // Rectangle's thickness is 20 / 2 = 10
	rectangle.setPosition(posX - 10, posY - 10); // Minus rectangle's thickness: -10
}

void Button::setTextButton(std::string name, std::string content, std::string fontPath, int width, int height, int posX, int posY) {
	// Update name, width, height, posX, posY
	this->name = name, this->width = width, this->height = height, this->posX = posX, this->posY = posY;
	//Load the font
	if (!font.loadFromFile(fontPath)) {
		throw runtime_error("Cannot load " + fontPath + "!\n");
	}
	text.setFont(font);
	// Set text
	text.setString(content);
	text.setCharacterSize(height + 7);
	text.setFillColor(Color::Black);
	text.setPosition(posX, posY - 12);
	// Scale the text if the text width exceed the button width
	if (text.getGlobalBounds().width > width) {
		int textWidth = text.getGlobalBounds().width;
		text.scale(width / (float)textWidth, width / (float)textWidth);
	}
	// Set back rectangle
	rectangle.setOutlineThickness(5);
	rectangle.setFillColor(Color::White);
	rectangle.setOutlineColor(Color(0xA6, 0x8A, 0x64));
	rectangle.setSize(Vector2f(width + 20, height + 20));
	rectangle.setPosition(posX + (float)text.getGlobalBounds().width / 2.0 - width / 2.0 - 10,
						  posY + (float)text.getGlobalBounds().height / 2.0 - height / 2.0 - 10);
}

void Button::drawSprite(RenderWindow& window) {
	window.draw(rectangle);
	window.draw(sprite);
}

void Button::drawText(RenderWindow& window) {
	window.draw(rectangle);
	window.draw(text);
}

void Button::drawAll(RenderWindow& window) {
	window.draw(rectangle);
	window.draw(sprite);
	window.draw(text);
}

bool Button::contain(int x, int y) {
	return rectangle.getGlobalBounds().contains(x, y);
}