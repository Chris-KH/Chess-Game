#pragma once
#include "ChessBoard.h"

class SideBoard {
private:
    RenderWindow* window;
    ChessBoard* chessboard;
    // Buttons
    vector<unique_ptr<Button>> buttonList;
    Button* selectedBut = nullptr;
public:
    // Constructor
    SideBoard() {}
    SideBoard(RenderWindow *window, ChessBoard *chessboard);
    // Update
    void update(Event& event);
    // Handle mouse click
    void handleButtonPress(int mouseX, int mouseY);
    void handleButtonRelease(int mouseX, int mouseY);
    // Draw
    void draw();
};