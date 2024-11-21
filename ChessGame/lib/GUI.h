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

#include "Pieces.h"
#include "Pawn.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Button.h"

class GUI {
	friend class ChessBoard;
public:
	//Promotion window
	static unique_ptr<Pieces> promoteChoice(unique_ptr<Pieces>& piece);

	//Setting window
	static void settingChoice(void);

	/*
		New game window
		Two option, AI or Human
	*/
};

