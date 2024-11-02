﻿//System include
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

//Header include
#include "../lib/Game.h"
#include "../lib/ChessBoard.h"

Game GameEngine;

int main() {
    // Khởi động Stockfish
    
    try {
        sf::RenderWindow window(sf::VideoMode(930, 930), "Chess Game", sf::Style::Close | sf::Style::Titlebar);
        GameEngine.startGame(&window);
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}