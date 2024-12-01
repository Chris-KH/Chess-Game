#include "../lib/SideBoard.h"
SideBoard::SideBoard(RenderWindow* window, ChessBoard* chessboard) {
    this->window = window;
    this->chessboard = chessboard;
    // Buttons
    addButton("new", "../assets/Button/new.png", 35, 35, 960, 30); // 1. New
    addButton("undo", "../assets/Button/undo.png", 35, 35, 1035, 30); // 2. Undo
    addButton("redo", "../assets/Button/redo.png", 35, 35, 1110, 30); // 3. Redo
    addButton("surrender", "../assets/Button/surrender.png", 35, 35, 1185, 30); // 4. Surrender
    addButton("save", "../assets/Button/save.png", 35, 35, 960, 100); // 5. Save
    addButton("load", "../assets/Button/LoadGame.png", 35, 35, 1035, 100); // 6. Load
    addButton("setting", "../assets/Button/settings.png", 35, 35, 1110, 100); // 7. Setting
    addButton("tie", "../assets/Button/Tie.png", 35, 35, 1185, 100); // 8. Tie
    buttonList.back()->setAvailable(false);

    gameOver = 0;
}

void SideBoard::addButton(std::string name, std::string path, int sizeX, int sizeY, int posX, int posY) {
    unique_ptr<Button> button = make_unique<Button>();
    button->setSpriteButton(name, path, sizeX, sizeY, posX, posY);
    buttonList.push_back(move(button));
}

bool SideBoard::update(Event& event) {
    this->gameOver = chessboard->isOver();
    if (chessboard->canRequestTie()) buttonList[7]->setAvailable(true);
    else buttonList[7]->setAvailable(false);
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        handleButtonPress(event.mouseButton.x, event.mouseButton.y);
    }
    else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
        return handleButtonRelease(event.mouseButton.x, event.mouseButton.y);
    }
    return false;
}

void SideBoard::handleButtonPress(int mouseX, int mouseY) {
    selectedBut = nullptr;
    for (unique_ptr<Button>& button : buttonList) {
        if (button->contain(mouseX, mouseY)) selectedBut = button.get();
    }
}

bool SideBoard::handleButtonRelease(int mouseX, int mouseY) {
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
            // No window was opened
            return false;
        }
        else if (selectedBut->getName() == "redo" && gameOver == 0) {
            chessboard->redoMove();
            // No window was opened
            return false;
        }
        else if (selectedBut->getName() == "save" && gameOver == 0) {
            GUI::saveGame(chessboard);
            // Opened a window
            return true;
        }
        else if (selectedBut->getName() == "new") {
            chessboard->newGame();
            // Opened a window
            return true;
        }
        else if (selectedBut->getName() == "surrender" && gameOver == 0) {
            chessboard->setGameOver(chessboard->isWhiteTurn() + 1);
            // No window was opened
            return false;
        }
        else if (selectedBut->getName() == "setting") {
            GUI::settingChoice(*chessboard);
            // Opened a window
            return true;
        }
        else if (selectedBut->getName() == "load") {
            chessboard->loadGame("");
            // Opened a window
            return true;
        }
        else if (selectedBut->getName() == "tie") {
            chessboard->setGameOver(3);
            // No window was opened
            return false;
        }
    }
    // No window was opened
    return false;
}

void SideBoard::draw(void) {
    for (unique_ptr<Button>& button : buttonList) {
        button->drawSprite(*window);
    }
}