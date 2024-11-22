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
	int width, height;
	int posX, posY;
	Texture texture;
	Sprite sprite;
	Font font;
	Text text;
	RectangleShape rectangle;
	std::string name;
public:
	Button() { width = height = posX = posY = 0; }
	void setSpriteButton(std::string name, std::string imagePath, int width, int height, int posX, int posY); // Set sprite with background rectangle
	void setTextButton(std::string name, std::string text, std::string fontPath, int width, int height, int posX, int posY, int textX, int textY); // Set text with background rectangle
	void setSpriteAndTextButton(std::string name, std::string imagePath, std::string content, std::string fontPath, int width, int height, int posX, int posY, int spriteWidth, int spriteHeight, int spriteX, int spriteY, int textWidth, int textHeight, int textX, int textY); // Set sprite, text, and a background rectangle
	std::string getName(void) { return name; }
	void drawSprite(RenderWindow& window); // Draw sprite
	void drawText(RenderWindow& window); // Draw text
	void drawAll(RenderWindow& window);
	bool contain(int x, int y); // Check if point(x, y) lie in the button area
};