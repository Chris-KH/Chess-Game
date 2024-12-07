#include "../lib/SideBoard.h"
SideBoard::SideBoard(RenderWindow* window, ChessBoard* chessboard) {
    this->window = window;
    this->chessboard = chessboard;
    // Buttons
    const int buttonW = 55, buttonH = 55;
    addSpriteButton("new", "../assets/Button/new.png", buttonW, buttonH, 960, 20); // 1. New
    addSpriteButton("undo", "../assets/Button/undo.png", buttonW, buttonH, 1035, 20); // 2. Undo
    addSpriteButton("redo", "../assets/Button/redo.png", buttonW, buttonH, 1110, 20); // 3. Redo
    addSpriteButton("surrender", "../assets/Button/surrender.png", buttonW, buttonH, 1185, 20); // 4. Surrender
    addSpriteButton("save", "../assets/Button/save.png", buttonW, buttonH, 960, 90); // 5. Save
    addSpriteButton("load", "../assets/Button/LoadGame.png", buttonW, buttonH, 1035, 90); // 6. Load
    addSpriteButton("setting", "../assets/Button/settings.png", buttonW, buttonH, 1110, 90); // 7. Setting
    addSpriteButton("tie", "../assets/Button/Tie.png", buttonW, buttonH, 1185, 90); // 8. Tie
    spriteButtons.back()->setAvailable(false);
    addSpriteButton("reset", "../assets/Button/reset.png", buttonW, buttonH, 960, 160); // 9. Reset

    addTextButton("turn indicator", "White's turn", "../assets/fonts/CherryBombOne.ttf", 300, 55, 960, 230);

    gameOver = 0;
}

void SideBoard::addSpriteButton(std::string name, std::string path, int sizeX, int sizeY, int posX, int posY) {
    unique_ptr<Button> button = make_unique<Button>();
    button->setSpriteButton(name, path, (float)sizeX, (float)sizeY, (float)posX, (float)posY);
    spriteButtons.push_back(move(button));
}

void SideBoard::addTextButton(std::string name, std::string content, std::string font, int width, int height, int posX, int posY) {
    unique_ptr<Button> button = make_unique<Button>();
    button->setTextButton(name, content, font, (float)width, (float)height, (float)posX, (float)posY);
    textButtons.push_back(move(button));
}

bool SideBoard::update(Event& event) {
    bool openedWindow = false;

    this->gameOver = chessboard->isOver();
    if (chessboard->canRequestTie()) spriteButtons[7]->setAvailable(true);
    else spriteButtons[7]->setAvailable(false);
    
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        handleButtonPress(event.mouseButton.x, event.mouseButton.y);
    }
    else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
        openedWindow |= handleButtonRelease(event.mouseButton.x, event.mouseButton.y);
    }

    if (chessboard->canRequestTie()) spriteButtons[7]->setAvailable(true);
    else spriteButtons[7]->setAvailable(false);
    draw(); // Draw to update

    if (chessboard->isWhiteTurn() == true) textButtons[0]->setTextContent("White's turn");
    else textButtons[0]->setTextContent("Black's turn");

    return openedWindow;
}

void SideBoard::handleButtonPress(int mouseX, int mouseY) {
    selectedBut = nullptr;
    for (unique_ptr<Button>& button : spriteButtons) {
        if (button->contain(mouseX, mouseY)) selectedBut = button.get();
    }
}

bool SideBoard::handleButtonRelease(int mouseX, int mouseY) {
    bool openedWindow = false;

    // Record the last button pressed
    Button* lastBut = selectedBut;
    // Find which button the mouse released
    selectedBut = nullptr;
    for (unique_ptr<Button>& button : spriteButtons) {
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
            chessboard->newGame(false);
            openedWindow = true;
        }
        else if (selectedBut->getName() == "surrender" && gameOver == 0) {
            chessboard->setGameOver(chessboard->isWhiteTurn() + 1);
            chessboard->setStateOver("Surrendered");
        }
        else if (selectedBut->getName() == "setting") {
            GUI::settingChoice(*chessboard);
            openedWindow = true;
        }
        else if (selectedBut->getName() == "load") {
            chessboard->loadGame();
            openedWindow = true;
        }
        else if (selectedBut->getName() == "tie") {
            chessboard->setGameOver(3);
            chessboard->setStateOver("Asked for peace");
        }
        else if (selectedBut->getName() == "reset") {
            chessboard->newGame(true);
        }
    }

    return openedWindow;
}

void SideBoard::draw(void) {
    for (unique_ptr<Button>& button : spriteButtons) {
        button->drawSprite(*window);
    }
    for (unique_ptr<Button>& button : textButtons) {
        button->drawText(*window);
    }
}