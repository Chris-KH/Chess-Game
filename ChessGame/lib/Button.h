#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <bits/stdc++.h>

using namespace std;
using namespace sf;

class Button {
private:
	float width, height, posX, posY;
	Texture texture;
	Sprite sprite;
	Font font;
	Text text;
	RectangleShape rectangle;
	string name;
	string content;
	string imagePath;
	string fontPath;
public:
	Button();
	Button(const Button& other) {
		width = other.width;
		height = other.height;
		posX = other.posX;
		posY = other.posY;
		name = other.name;
		content = other.content;
		imagePath = other.imagePath;
		fontPath = other.fontPath;
	}
	void setSpriteButton(string name, string imagePath, float width, float height, float posX, float posY); // Set sprite with background rectangle
	void setTextButton(string name, string text, string fontPath, float width, float height, float posX, float posY); // Set text with background rectangle
	void setSpriteAndTextButton(string name, string imagePath, string content, string fontPath, float width, float height, float posX, float posY, float spriteWidth, float spriteHeight, float spriteX, float spriteY, float textWidth, float textHeight, float textX, float textY); // Set sprite, text, and a background rectangle
	
	void drawSprite(RenderWindow& window); // Draw sprite
	void drawText(RenderWindow& window); // Draw text
	void drawAll(RenderWindow& window); // Draw sprite, text, and rectangle
	void move(float addX, float addY); // Shift from (posX, posY) to (posX + addX, posY + addY)
	void printPosText(void) { cout << text.getPosition().x << ' ' << text.getPosition().y << '\n'; }
	bool contain(int x, int y); // Check if point(x, y) lie in the button area

	void setWidth(float width);
	void setHeight(float height);
	void setX(float x);
	void setY(float y);

	float getWidth() const { return this->width; }
	float getHeight() const { return this->height; }
	float getX() const { return this->posX; }
	float getY() const { return this->posY; }
	string& getName(void) { return name; }
	string& getContent(void) { return content; }
	string& getImagePath(void) { return imagePath; }
	string& getFontPath(void) { return fontPath; }

	RectangleShape& getRectangle();
};