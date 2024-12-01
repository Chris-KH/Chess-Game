#include "../lib/Button.h"

Button::Button() : available(true), width(0), height(0), posX(0), posY(0) {
	
}

void Button::setSpriteButton(string name, string imagePath, float width, float height, float posX, float posY) {
	// Update name, width, height, posX, posY
	this->name = name, this->imagePath = imagePath, this->width = width, this->height = height, this->posX = posX, this->posY = posY;
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

void Button::setTextButton(string name, string content, string fontPath, float width, float height, float posX, float posY) {
	this->name = name, this->width = width, this->height = height, this->posX = posX, this->posY = posY;
	this->fontPath = fontPath;
	this->content = content;
	// Set the rectangle
	rectangle.setSize(Vector2f(width, height));
	rectangle.setOutlineThickness(3);
	rectangle.setFillColor(Color::White);
	rectangle.setOutlineColor(Color(0xA6, 0x8A, 0x64));
	rectangle.setPosition(posX, posY);

	// Set text
	if (!font.loadFromFile(fontPath)) {
		throw runtime_error("Cannot load " + fontPath + "!\n");
	}

	text.setFont(font);
	text.setString(content);
	text.setFillColor(Color::Black);
	text.setCharacterSize(height - 10);
	//text.setPosition(rectangle.getPosition().x + (rectangle.getGlobalBounds().width - text.getGlobalBounds().width) / 2.f, rectangle.getPosition().y + 10);
	text.setPosition(rectangle.getPosition().x + 10, rectangle.getPosition().y + 5);
}

void Button::setSpriteAndTextButton(string name, string imagePath, string content, string fontPath, float width, float height, float posX, float posY, float spriteWidth, float spriteHeight, float spriteX, float spriteY, float textWidth, float textHeight, float textX, float textY) {
	// Update variables
	this->name = name, this->width = width, this->height = height, this->posX = posX, this->posY = posY;
	this->imagePath = imagePath;
	this->fontPath = fontPath;
	this->content = content;
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
	// Set the rectangle
	rectangle.setOutlineThickness(3);
	rectangle.setFillColor(Color::White);
	rectangle.setOutlineColor(Color(100, 100, 100, 255));
	rectangle.setSize(Vector2f(width + 20, height + 20));
	rectangle.setPosition(posX - 10, posY - 10);

	text.setFont(font);
	// Text: Text
	text.setCharacterSize(textHeight + 15);
	text.setFillColor(Color::Black);
	text.setString(content);
	// Scale if the text width exceeds the desired width
	if (text.getGlobalBounds().width > textWidth) {
		float r = textWidth / (float)text.getGlobalBounds().width;
		text.setScale(r, r);
	}
	text.setPosition(rectangle.getPosition().x + (rectangle.getGlobalBounds().width - text.getGlobalBounds().width) / 2, rectangle.getPosition().y);
	//text.setPosition(textX, textY);
}

void Button::drawSprite(RenderWindow& window) {
	if (!available) return;
	window.draw(rectangle);
	window.draw(sprite);
}

void Button::drawText(RenderWindow& window) {
	if (!available) return;
	window.draw(rectangle);
	window.draw(text);
}

void Button::drawAll(RenderWindow& window) {
	if (!available) return;
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

void Button::setAvailable(bool available) { this->available = available; }
void Button::setWidth(float width) { this->width = width; }
void Button::setHeight(float height) { this->height = height; }
void Button::setX(float x) { this->posX = x; }
void Button::setY(float y) { this->posY = y; }

bool Button::getAvailable(void) { return this->available; }
float Button::getWidth() const { return this->width; }
float Button::getHeight() const { return this->height; }
float Button::getX() const { return this->posX; }
float Button::getY() const { return this->posY; }
string& Button::getName(void) { return name; }
string& Button::getContent(void) { return content; }
string& Button::getImagePath(void) { return imagePath; }
string& Button::getFontPath(void) { return fontPath; }
RectangleShape& Button::getRectangle(void) { return this->rectangle; }