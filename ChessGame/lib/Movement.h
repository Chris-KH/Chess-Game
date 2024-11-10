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

class Movement {
private:
	int cellSize;
	int boardSize;
	
	
	
	
	
	Vector2f cords[8][8]; // cords[row][col] lấy ra tọa độ góc trái trên của ô hàng row, cột col
public:
	// Constant
	const Vector2f UNDEFINED_POINT = Vector2f(1e9, 1e9);

	// Initialization
	Movement(int cellSize = 0);
	void setCellSize(int cellSize);

	// Call for value
	int getCellSize(void);
	int getBoardSize(void);
	float getBorderThickness();

	// Calculation
	int getRow(float x);
	int getCol(float y);
	float dist(const Vector2f& x, const Vector2f& y);
	Vector2f cordLeftTop(int row, int col);
	Vector2f cordCentre(int row, int col);
};