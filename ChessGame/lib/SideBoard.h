#pragma once
#include "ChessBoard.h"

class SideBoard {
private:
    RenderWindow* window;
    ChessBoard* chessboard;
    BGM* backgroundMusic;
    // Buttons
    vector<unique_ptr<Button>> spriteButtons, textButtons;
    Button* selectedBut = nullptr;

    int gameOver;
public:
    // Constructor
    //SideBoard() {}
    SideBoard(RenderWindow *window, ChessBoard *chessboard, BGM *backgroundMusic);
    // Update
    void addSpriteButton(std::string name, std::string path, int sizeX, int sizeY, int posX, int posY); // Add a new button with name, path to the image file, width, height, position x, and y
    void addTextButton(std::string name, std::string content, std::string font, int width, int height, int posX, int posY); // Add a text button
    bool update(Event& event); // return true if any window is open while running, false if otherwise
    // Handle mouse click
    void handleButtonPress(int mouseX, int mouseY);
    bool handleButtonRelease(int mouseX, int mouseY); // Return true if any window is opened, false otherwise
    // Draw
    void draw();
};