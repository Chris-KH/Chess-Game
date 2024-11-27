#include "../lib/SideBoard.h"
SideBoard::SideBoard(RenderWindow* window, ChessBoard* chessboard) {
    this->window = window;
    this->chessboard = chessboard;
    // Buttons
    unique_ptr<Button> newBut = make_unique<Button>();
    newBut->setSpriteButton("new", "../assets/Button/new.png", 35, 35, 960, 30);
    buttonList.push_back(move(newBut));
    unique_ptr<Button> undoBut = make_unique<Button>();
    undoBut->setSpriteButton("undo", "../assets/Button/undo.png", 35, 35, 1035, 30);
    buttonList.push_back(move(undoBut));
    unique_ptr<Button> redoBut = make_unique<Button>();
    redoBut->setSpriteButton("redo", "../assets/Button/redo.png", 35, 35, 1110, 30);
    buttonList.push_back(move(redoBut));
    unique_ptr<Button> surrenderBut = make_unique<Button>();
    surrenderBut->setSpriteButton("surrender", "../assets/Button/surrender.png", 35, 35, 1185, 30);
    buttonList.push_back(move(surrenderBut));

    unique_ptr<Button> saveBut = make_unique<Button>();
    saveBut->setSpriteButton("save", "../assets/Button/save.png", 35, 35, 960, 100);
    buttonList.push_back(move(saveBut));
    unique_ptr<Button> loadBut = make_unique<Button>();
    loadBut->setSpriteButton("load", "../assets/Button/LoadGame.png", 35, 35, 1035, 100);
    buttonList.push_back(move(loadBut));
    unique_ptr<Button> settingBut = make_unique<Button>();
    settingBut->setSpriteButton("setting", "../assets/Button/settings.png", 35, 35, 1110, 100);
    buttonList.push_back(move(settingBut));

    gameOver = 0;
}

void SideBoard::update(Event& event) {
    this->gameOver = chessboard->isOver();
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        handleButtonPress(event.mouseButton.x, event.mouseButton.y);
    }
    else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
        handleButtonRelease(event.mouseButton.x, event.mouseButton.y);
    }
}

void SideBoard::handleButtonPress(int mouseX, int mouseY) {
    selectedBut = nullptr;
    for (unique_ptr<Button>& button : buttonList) {
        if (button->contain(mouseX, mouseY)) selectedBut = button.get();
    }
}

void SideBoard::handleButtonRelease(int mouseX, int mouseY) {
    // Record the last button pressed
    Button* lastBut = selectedBut;
    // Find which button the mouse released
    selectedBut = nullptr;
    for (unique_ptr<Button>& button : buttonList) {
        if (button->contain(mouseX, mouseY)) selectedBut = button.get();
    }
    // If the button clicked the same with the button released then do operation(s) on this
    if (selectedBut && selectedBut == lastBut) {
        if (selectedBut->getName() == "undo" && gameOver == 0) {
            chessboard->undoMove();
        }
        else if (selectedBut->getName() == "redo" && gameOver == 0) {
            chessboard->redoMove();
        }
        else if (selectedBut->getName() == "save" && gameOver == 0) {
            cout << "Open save game window please!\n";
            GUI::saveGame(chessboard);
        }
        else if (selectedBut->getName() == "new") {
            chessboard->newGame();
        }
        else if (selectedBut->getName() == "surrender" && gameOver == 0) {
            chessboard->setGameOver(chessboard->isWhiteTurn() + 1);
        }
        else if (selectedBut->getName() == "setting") {
            GUI::settingChoice(*chessboard);
        }
        else if (selectedBut->getName() == "load") {
            chessboard->loadGame("");
        }
    }
}

void SideBoard::draw(void) {
    for (unique_ptr<Button>& button : buttonList) {
        button->drawSprite(*window);
    }
}