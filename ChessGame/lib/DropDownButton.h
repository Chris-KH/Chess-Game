#pragma once
#include "Button.h"

// Lists of consecutive buttons
class DropDownButton {
private:
	std::string name;
	float curBut, width, height, posX, posY;
	int isClicked; // = 0 if is not clicked, != o if is clicked
	vector<unique_ptr<Button>> list;
public:
	// Constructor and destructor
	DropDownButton() {}
	DropDownButton(std::string _name, float _width, float _height, float _posX, float _posY, const vector<std::string> &_list);
	~DropDownButton() {}

	void setClick(int val) { isClicked = val; }
	void click(void) { isClicked = (isClicked == 0 ? 1 : 0); }

	// Draw
	void draw(RenderWindow &window);
	
	std::string getName(void) const { return name; }
	int getClick(void) const { return isClicked; }
	int getNumOption(void) const { return (int)list.size(); }
	bool contain(int x, int y);
	int eventOption(int x, int y);
};