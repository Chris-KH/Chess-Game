#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <bits/stdc++.h>

using namespace sf;

class ChessBoard {
private:
	const static int NUM = 3;
	int id;
	Texture texture[NUM];
	Sprite sprite;
	Vector2u size;
	View view;
public:
	Sprite getSprite();
	View getView();

	ChessBoard(void);
	void resize (Vector2u size);
	void change (void);
};