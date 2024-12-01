﻿#include "../lib/SideBoard.h"
SideBoard::SideBoard(RenderWindow* window, ChessBoard* chessboard) {
    this->window = window;
    this->chessboard = chessboard;
    // Buttons
    const int buttonW = 55, buttonH = 55;
    addButton("new", "../assets/Button/new.png", buttonW, buttonH, 960, 20); // 1. New
    addButton("undo", "../assets/Button/undo.png", buttonW, buttonH, 1035, 20); // 2. Undo
    addButton("redo", "../assets/Button/redo.png", buttonW, buttonH, 1110, 20); // 3. Redo
    addButton("surrender", "../assets/Button/surrender.png", buttonW, buttonH, 1185, 20); // 4. Surrender
    addButton("save", "../assets/Button/save.png", buttonW, buttonH, 960, 90); // 5. Save
    addButton("load", "../assets/Button/LoadGame.png", buttonW, buttonH, 1035, 90); // 6. Load
    addButton("setting", "../assets/Button/settings.png", buttonW, buttonH, 1110, 90); // 7. Setting
    addButton("tie", "../assets/Button/Tie.png", buttonW, buttonH, 1185, 90); // 8. Tie
    buttonList.back()->setAvailable(false);

    gameOver = 0;
}

void SideBoard::addButton(std::string name, std::string path, int sizeX, int sizeY, int posX, int posY) {
    unique_ptr<Button> button = make_unique<Button>();
    button->setSpriteButton(name, path, sizeX, sizeY, posX, posY);
    buttonList.push_back(move(button));
}

bool SideBoard::update(Event& event) {
    bool openedWindow = false;

    this->gameOver = chessboard->isOver();
    if (chessboard->canRequestTie()) buttonList[7]->setAvailable(true);
    else buttonList[7]->setAvailable(false);
    
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        handleButtonPress(event.mouseButton.x, event.mouseButton.y);
    }
    else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
        openedWindow |= handleButtonRelease(event.mouseButton.x, event.mouseButton.y);
    }

    if (chessboard->canRequestTie()) buttonList[7]->setAvailable(true);
    else buttonList[7]->setAvailable(false);
    draw(); // Draw to update

    return openedWindow;
}

void SideBoard::handleButtonPress(int mouseX, int mouseY) {
    selectedBut = nullptr;
    for (unique_ptr<Button>& button : buttonList) {
        if (button->contain(mouseX, mouseY)) selectedBut = button.get();
    }
}

bool SideBoard::handleButtonRelease(int mouseX, int mouseY) {
    bool openedWindow = false;

    // Record the last button pressed
    Button* lastBut = selectedBut;
    // Find which button the mouse released
    selectedBut = nullptr;
    for (unique_ptr<Button>& button : buttonList) {
        if (button->contain(mouseX, mouseY) && button->getAvailable()) selectedBut = button.get();
    }
    // Click and release on the same button
    if (selectedBut && selectedBut == lastBut) {
        if (selectedBut->getName() == "undo" && gameOver == 0) {
            chessboard->undoMove();
        }
        else if (selectedBut->getName() == "redo" && gameOver == 0) {
            chessboard->redoMove();
        }
        else if (selectedBut->getName() == "save" && gameOver == 0) {
            GUI::saveGame(chessboard);
            openedWindow = true;
        }
        else if (selectedBut->getName() == "new") {
            chessboard->newGame();
            openedWindow = true;
        }
        else if (selectedBut->getName() == "surrender" && gameOver == 0) {
            chessboard->setGameOver(chessboard->isWhiteTurn() + 1);
        }
        else if (selectedBut->getName() == "setting") {
            GUI::settingChoice(*chessboard);
            openedWindow = true;
        }
        else if (selectedBut->getName() == "load") {
            chessboard->loadGame("");
            openedWindow = true;
        }
        else if (selectedBut->getName() == "tie") {
            chessboard->setGameOver(3);
        }
    }

    return openedWindow;
}

void SideBoard::draw(void) {
    for (unique_ptr<Button>& button : buttonList) {
        button->drawSprite(*window);
    }
}