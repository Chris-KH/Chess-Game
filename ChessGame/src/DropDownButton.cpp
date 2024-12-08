#include "../lib/DropDownButton.h"

// Constructor
DropDownButton::DropDownButton() {
	window = nullptr;
	width = height = posX = posY = 0;
	curBut = 0;
	isClicked = false;
}

DropDownButton::DropDownButton(RenderWindow* window, std::string _name, float _width, float _height, float _posX, float _posY, const vector<std::string>& _list, int _curBut) :
	window(window), name(_name), width(_width), height(_height), posX(_posX), posY(_posY), isClicked(0), curBut(_curBut) {
	// Make the list
	for (const std::string& s : _list) {
		list.push_back(make_unique<Button>());
		list.back()->setTextButton(s, s, "../assets/fonts/ZenOldMincho.ttf", width, height, posX, posY);
	}
}

// Destructor
DropDownButton::~DropDownButton() {}

// User's experience
void DropDownButton::setClick(int val) { isClicked = val; }

void DropDownButton::click(void) {
	isClicked = (isClicked == 0 ? 1 : 0);
}

bool DropDownButton::contain(int x, int y) {
	return list[0]->contain(x, y);
}

bool DropDownButton::contain(float x, float y) {
	return list[0]->contain(x, y);
}

int DropDownButton::eventOption(int x, int y) {
	// -1 : unchanged
	// >= 0 : changed
	if (list[0]->contain(x, y)) return -1;
	for (int i = 0; i < (int)list.size(); i++) {
		bool ok = false;
		list[i]->move(0, (i + 1) * height);
		if (list[i]->contain(x, y)) curBut = i, ok = true;
		list[i]->move(0, -(i + 1) * height);
		if (ok) return i;
	}
	return -1;
}

// Draw
void DropDownButton::draw(RenderWindow &window) {
	if (list.empty()) {
		throw runtime_error("List in a drop down button is empty!");
	}
	list[curBut]->drawText(window);
}

void DropDownButton::drawOption(RenderWindow& window) {
	if (isClicked == true) {
		for (int i = 0; i < (int)list.size(); i++) {
			list[i]->move(0, (i + 1) * height); // (Free area = 20) + (Border = 6 / 2) -> 23
			list[i]->drawText(window);
			list[i]->move(0, -(i + 1) * height);
		}
	}
}

// Get value
std::string DropDownButton::getName(void) const { return name; }
int DropDownButton::getClick(void) const { return isClicked; }
int DropDownButton::getNumOption(void) const { return (int)list.size(); }