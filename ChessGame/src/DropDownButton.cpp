#include "../lib/DropDownButton.h"

// Constructor
DropDownButton::DropDownButton(std::string _name, float _width, float _height, float _posX, float _posY, const vector<std::string>& _list) :
	curBut(0), name(_name), width(_width), height(_height), posX(_posX), posY(_posY), isClicked(0) {
	// Make the list
	for (const std::string& s : _list) {
		list.push_back(make_unique<Button>());
		list.back()->setTextButton(s, s, "../assets/fonts/ZenOldMincho.ttf", width, height, posX, posY, posX, posY - 5);
	}
}

// Draw
void DropDownButton::draw(RenderWindow &window) {
	if (list.empty()) {
		throw runtime_error("List in a drop down button is empty!");
	}
	list[curBut]->drawText(window);
	if (isClicked) {
		for (int i = 0; i < (int)list.size(); i++) {
			list[i]->move(0, (i + 1) * (height + 26)); // (Free area = 20) + (Border = 6) -> 26
			list[i]->drawText(window);
			list[i]->move(0, -(i + 1) * (height + 26));
		}
	}
}

bool DropDownButton::contain(int x, int y) {
	return list[0]->contain(x, y);
}

int DropDownButton::eventOption(int x, int y) {
	if (list[curBut]->contain(x, y)) return curBut;
	for (int i = 0; i < (int)list.size(); i++) {
		bool ok = false;
		list[i]->move(0, (i + 1) * (height + 26));
		if (list[i]->contain(x, y)) curBut = i, ok = true;
		list[i]->move(0, -(i + 1) * (height + 26));
		if (ok) break;
	}
	return curBut;
}