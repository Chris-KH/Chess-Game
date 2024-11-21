#include "../lib/Button.h"

void Button::loadTexture(string path) {
	if (!texture.loadFromFile(path)) {
		throw runtime_error("Cannot load " + path + "!\n");
	}
	// Set texture
	sprite.setTexture(texture);
	float ratioWidth = targetWidth / (float)sprite.getGlobalBounds().width;
	float ratioHeight = targetHeight / (float)sprite.getGlobalBounds().height;
	sprite.setScale(ratioWidth, ratioHeight);
	sprite.setPosition(targetX, targetY);

	// Set back rectangle
	rectangle.setOutlineThickness(5);
	rectangle.setFillColor(Color::White);
	rectangle.setOutlineColor(Color(100, 100, 100, 255));
	rectangle.setSize(Vector2f(targetWidth + 20, targetHeight + 20)); // Rectangle's thickness is 20 / 2 = 10
	rectangle.setPosition(targetX - 10, targetY - 10); // Minus rectangle's thickness: -10
}

void Button::setText(string s, Font _font) {
	// Set text with given font, and string
	font = _font;
	text.setString(s);
	text.setFont(font);
	text.setCharacterSize(targetHeight);
	text.setFillColor(Color::Black);
	text.setPosition(targetX, targetY);

	// Set back rectangle
	rectangle.setOutlineThickness(5);
	rectangle.setFillColor(Color::White);
	rectangle.setOutlineColor(Color(100, 100, 100, 255));
	rectangle.setSize(Vector2f(text.getGlobalBounds().width + 20, targetHeight + 20));
	rectangle.setPosition(targetX - 10, targetY - 5);
}

void Button::drawSprite(RenderWindow& window) {
	window.draw(rectangle);
	window.draw(sprite);
}

void Button::drawText(RenderWindow& window) {
	window.draw(rectangle);
	window.draw(text);
}

bool Button::contain(int x, int y) {
	return rectangle.getGlobalBounds().contains(x, y);
}