#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <bits/stdc++.h>

class Movement {
private:
	int cellSize;
	int boardSize;
	sf::Vector2f cords[8][8];
public:
	// Constant
	const sf::Vector2f UNDEFINED_POINT = sf::Vector2f(1e9, 1e9);

	// Initialization
	Movement(int cellSize = 100);
	void setCellSize(int cellSize);

	// Call for value
	int getCellSize(void);
	int getBoardSize(void);
	float getBorderThickness();

	// Calculation
	int getRow(float x);
	int getCol(float y);
	float dist(const sf::Vector2f& x, const sf::Vector2f& y);
	sf::Vector2f cordLeftTop(int row, int col);
	sf::Vector2f cordCentre(int row, int col);
};