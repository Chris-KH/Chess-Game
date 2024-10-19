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

class Stockfish {
public:
    Stockfish();
    Stockfish(const string& path);

    ~Stockfish();

    string sendCommand(const string& command);
    void setPosition(const std::string& fen);
    string calculateBestMove(int timeLimit);
    void setSkillLevel(int level);

private:
    FILE* process;
};