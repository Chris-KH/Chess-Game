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
	int targetWidth = 50, targetHeight = 50;
	int targetX = 500, targetY = 500;
	Texture texture;
	Sprite sprite;
	Font font;
	Text text;
	RectangleShape rectangle;
	string name;
public:
	Button() {}
	void setSize(int w, int h) { targetWidth = w, targetHeight = h; } // Set size
	void setPosition(int x, int y) { targetX = x, targetY = y; } // Set position
	void loadTexture(string path); // Set texture, sprite, position, and size
	void setName(string s) { name = s; }
	void setText(string s, Font font);
	string getName(void) { return name; }
	void drawSprite(RenderWindow& window); // Draw sprite
	void drawText(RenderWindow& window); // Draw text
	void drawAll(RenderWindow& window);
	bool contain(int x, int y); // Check if point(x, y) lie in the button area
};