#pragma once
#include "Button.h"

// Lists of consecutive buttons
class DropDownButton {
private:
	RenderWindow* window;
	std::string name;
	float width, height, posX, posY;
	int curBut;
	int isClicked; // = 0 if is not clicked, != o if is clicked
	vector<unique_ptr<Button>> list;
public:
	// Constructor
	DropDownButton();
	DropDownButton(RenderWindow* window, std::string _name, float _width, float _height, float _posX, float _posY, const vector<std::string>& _list, int _curBut);
	
	// Destructor
	~DropDownButton();

	// User's experience
	void setClick(int val);
	void click(void);
	bool contain(int x, int y);
	int eventOption(int x, int y);

	// Draw
	void draw(RenderWindow& window);
	void drawOption(RenderWindow& window);

	// Get value
	std::string getName(void) const;
	int getClick(void) const;
	int getNumOption(void) const;
};