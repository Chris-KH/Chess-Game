#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <filesystem>
#include<memory>

using namespace std;
using namespace sf;

class ChessBoard;
class Pieces;

#include "ChessBoard.h"
#include "Pieces.h"
#include "Pawn.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "ScrollableArea.h"
#include "Slider.h"
#include "DropDownButton.h"
#include "BGM.h"


class GUI {
	friend class ChessBoard;
public:
	//Promotion window
	static unique_ptr<Pieces> promoteChoice(unique_ptr<Pieces>& piece);

	//Setting window
	static void settingChoice(ChessBoard &chessBoard, BGM &backgroundMusic);
	static void setPiece(int num, const vector<std::string>& pieceThemePath, const vector<std::string>& piecePath, vector<Texture>& pieceTexture, vector<Sprite>& reviewPiece);
	static void handlePressSetting(Event::MouseButtonEvent mouse, const View& view, 
		Button *&selectedButton, DropDownButton *&selectedDropDownButton, vector<Button*> &buttonList, vector<DropDownButton*> &dropDownButtonList, 
		int initBoard, int initPiece, int initSound, int initMusic, 
		ChessBoard& chessBoard, BGM& backgroundMusic, RenderWindow& window);

	//Game Over window
	static void gameOver(ChessBoard &chessBoard);

	/*
		New game window
		Two option, AI or Human
	*/
	static bool newGame(ChessBoard& chessBoard); // Trả về true nếu người chọn chế độ chơi cho ván mới, nếu người chơi đóng của sổ thì trả về false
	static bool chooseDifficulty(ChessBoard& chessBoard); // Chọn độ khó cho chế độ chơi Player Versus AI
	//Get name of file to save game
	static void saveGame(ChessBoard* chessBoard);

	//Load Game window
	static string loadGame(ChessBoard& chessBoard);

	static bool YesNo();
};