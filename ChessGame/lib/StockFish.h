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
    Stockfish(const string& path);

    ~Stockfish();

    string sendCommand(const string& command);

private:
    FILE* process = nullptr;
};