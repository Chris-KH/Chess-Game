#pragma once
#include "ChessBoard.h"

class SideBoard {
private:
    RenderWindow* window;
    ChessBoard* chessboard;
    // Buttons
    vector<unique_ptr<Button>> buttonList;
    Button* selectedBut = nullptr;

    int gameOver;
public:
    // Constructor
    //SideBoard() {}
    SideBoard(RenderWindow *window, ChessBoard *chessboard);
    // Update
    void update(Event& event);
    // Handle mouse click
    void handleButtonPress(int mouseX, int mouseY);
    bool handleButtonRelease(int mouseX, int mouseY); // Return true if any window is opened, false otherwise
    // Draw
    void draw();
};