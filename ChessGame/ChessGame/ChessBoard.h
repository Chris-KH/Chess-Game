#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <bits/stdc++.h>

class ChessBoard {
private:
	const static int NUM_CHESS_BOARD = 3;
	int chessBoardId;
	Texture chessBoardImage[3];
	Sprite chessBoardSprite;
	Vector2u chessBoardSize;
public:
	ChessBoard();
	void loadImage(Texture& texture, string path);
	void switchBoard(void);
};