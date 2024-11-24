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
#include "DropDownButton.h"
#include "ChessBoard.h"

class GUI {
	friend class ChessBoard;
public:
	//Promotion window
	static unique_ptr<Pieces> promoteChoice(unique_ptr<Pieces>& piece);

	//Setting window
	static void settingChoice(ChessBoard &chessBoard);
	static void handlePressSetting(Button *&selectedButton, DropDownButton *&selectedDropDownButton, Event::MouseButtonEvent &mouse, vector<Button*> &buttonList, vector<DropDownButton*> &dropDownButtonList, ChessBoard &chessBoard);
	static void handleReleaseSetting(Button*& selectedButton, DropDownButton*& selectedDropDownButton, Event::MouseButtonEvent &mouse, vector<Button*>& buttonList, vector<DropDownButton*>& dropDownButtonList, int initBoard, ChessBoard &chessBoard, RenderWindow &window);
	static void drawSetting(RenderWindow& window, DropDownButton* selectedDropDownButton, vector<Sprite*>& spriteList, vector<Text*>& textList, vector<Button*>& buttonList, vector<DropDownButton*>& dropDownButtonList);

	//Game Over window
	static void gameOver(ChessBoard &chessBoard);

	/*
		New game window
		Two option, AI or Human
	*/
	static void newGame(ChessBoard& chessBoard);

	//Get name of file to save game
	static void saveGame(ChessBoard* chessBoard);
};