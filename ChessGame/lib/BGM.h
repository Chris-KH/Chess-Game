#pragma once
#include <vector>
#include <string>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

class BGM {
private:
	// Lists
	int ID;
	vector<string> pathList;
	vector<string> nameList;

	// Music object
	Music music;
public:
	// Constructor
	BGM(void);

	// Play music track
	void play(void);

	// Stop music track
	void stop(void);

	// Change music track
	void change(int id);

	// Set volume
	void setVolume(float volume);

	// Get
	int getID(void) const;
	vector<string> getPathList(void) const;
	vector<string> getNameList(void) const;
	int getVolume(void);
};