#include "../lib/Button.h"

Button::Button() : width(0), height(0), posX(0), posY(0) {
	
}

void Button::setSpriteButton(std::string name, std::string imagePath, float width, float height, float posX, float posY) {
	// Update name, width, height, posX, posY
	this->name = name, this->width = width, this->height = height, this->posX = posX, this->posY = posY;
	// Load the texture
	if (!texture.loadFromFile(imagePath)) {
		throw runtime_error("Cannot load " + imagePath + "!\n");
	}
	sprite.setTexture(texture);
	// Set sprite
	float r = min(width / (float)sprite.getGlobalBounds().width, 
				height / (float)sprite.getGlobalBounds().height);
	sprite.setScale(r, r);
	sprite.setPosition(posX, posY);
	// Set back rectangle
	rectangle.setOutlineThickness(3);
	rectangle.setFillColor(Color::White);
	rectangle.setOutlineColor(Color(0xA6, 0x8A, 0x64));
	rectangle.setSize(Vector2f(width + 20, height + 20)); // Rectangle's thickness is 20 / 2 = 10
	rectangle.setPosition(posX - 10, posY - 10); // Minus rectangle's thickness: -10
}

void Button::setTextButton(std::string name, std::string content, std::string fontPath, float width, float height, float posX, float posY, float textX, float textY) {
	// Update name, width, height, posX, posY
	this->name = name, this->width = width, this->height = height, this->posX = posX, this->posY = posY;
	//Load the font
	if (!font.loadFromFile(fontPath)) {
		throw runtime_error("Cannot load " + fontPath + "!\n");
	}
	text.setFont(font);
	// Set text
	text.setFillColor(Color::Black);
	text.setString(content);
	text.setCharacterSize(height + 15);
	// Scale the text if the text width exceed the button width
	if (text.getGlobalBounds().width > width) {
		float r = width / (float)text.getGlobalBounds().width;
		text.setScale(r, r);
	}
	text.setPosition(textX, textY);
	// Set back rectangle
	rectangle.setOutlineThickness(3);
	rectangle.setFillColor(Color::White);
	rectangle.setOutlineColor(Color(0xA6, 0x8A, 0x64));
	rectangle.setSize(Vector2f(width + 20, height + 20));
	rectangle.setPosition(posX - 10, posY - 10);
}

void Button::setSpriteAndTextButton(std::string name, std::string imagePath, std::string content, std::string fontPath, float width, float height, float posX, float posY, float spriteWidth, float spriteHeight, float spriteX, float spriteY, float textWidth, float textHeight, float textX, float textY) {
	// Update variables
	this->name = name, this->width = width, this->height = height, this->posX = posX, this->posY = posY;
	// Sprite: texture
	if (!texture.loadFromFile(imagePath)) {
		throw runtime_error("Cannot load the file " + imagePath + "! (Button::setSpriteAndTextButton)\n");
	}
	sprite.setTexture(texture);
	// Sprite: sprite
	float r = min(spriteWidth / (float)sprite.getGlobalBounds().width,
				spriteHeight / (float)sprite.getGlobalBounds().height);
	sprite.setScale(r, r);
	sprite.setPosition(spriteX, spriteY);
	// Text: Font
	if (!font.loadFromFile(fontPath)) {
		throw runtime_error("Cannot load the file " + fontPath + "! (Button::setSpriteAndTextButton)\n");
	}
	text.setFont(font);
	// Text: Text
	text.setFillColor(Color::Black);
	text.setString(content);
	text.setCharacterSize(textHeight + 15);
	// Scale if the text width exceeds the desired width
	if (text.getGlobalBounds().width > textWidth) {
		float r = textWidth / (float)text.getGlobalBounds().width;
		text.setScale(r, r);
	}
	text.setPosition(textX, textY);
	// Set the rectangle
	rectangle.setOutlineThickness(3);
	rectangle.setFillColor(Color::White);
	rectangle.setOutlineColor(Color(100, 100, 100, 255));
	rectangle.setSize(Vector2f(width + 20, height + 20));
	rectangle.setPosition(posX - 10, posY - 10);
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

void Button::move(float addX, float addY) {
	Vector2f rect = rectangle.getPosition();
	rect.x += addX, rect.y += addY;
	rectangle.setPosition(rect);
	Vector2f sp = sprite.getPosition();
	sp.x += addX, sp.y += addY;
	sprite.setPosition(sp);
	Vector2f txt = text.getPosition();
	txt.x += addX, txt.y += addY;
	text.setPosition(txt);
}

bool Button::contain(int x, int y) {
	return rectangle.getGlobalBounds().contains(x, y);
}