#include "../lib/BGM.h"

BGM::BGM(void) {
	// Lists
	ID = 0;
	pathList.resize(4);
	nameList.resize(4);

	pathList[0] = ""; // No music
	pathList[1] = "../assets/musics/Morning Mood";
	pathList[2] = "../assets/musics/Path of The Wind";
	pathList[3] = "../assets/musics/Scatman";
	
	nameList[0] = "None"; // No music
	nameList[1] = "Morning Mood";
	nameList[2] = "Path of The Wind";
	nameList[3] = "Scatman";

	// Set loop
	music.setLoop(true);

	// Set volume
	music.setVolume(50.f);

	// Set initial song
	change(2);
}

void BGM::play(void) {
	if (ID == 0) return;
	music.play();
}

void BGM::stop(void) {
	if (ID == 0) return;
	music.stop();
}

void BGM::change(int id) {
	stop();
	ID = id;
	if (id == 0) return;
	if (!music.openFromFile(pathList[id] + ".mp3")) {
		throw runtime_error("Cannot open music: " + nameList[id] + ".mp3");
	}
	play();
}

void BGM::setVolume(float volume) {
	if (volume < 0.f || volume > 100.f) {
		throw runtime_error("BGM error -> setVolume()");
	}
	music.setVolume(volume);
}

int BGM::getID(void) const {
	return ID;
}

vector<string> BGM::getPathList(void) const {
	return pathList;
}

vector<string> BGM::getNameList(void) const {
	return nameList;
}

int BGM::getVolume(void) {
	return music.getVolume();
}