#include"../lib/GUI.h"

#include <SFML/Graphics.hpp>
#include <stdexcept>

using namespace sf;

unique_ptr<Pieces> GUI::promoteChoice(unique_ptr<Pieces>& piece) {
    bool isWhite = piece->getColor();
    string color = (isWhite ? "white-" : "black-");
    vector<string> paths = {
        "../assets/Standard Theme/",
        "../assets/Cartoon Theme/",
        "../assets/Pixel Theme/",
        "../assets/Neo Theme/"
    };

    string pathFolder = paths[piece->getCurrentTextureIndex()];
    vector<string> texturePaths = {
        pathFolder + color + "queen.png",
        pathFolder + color + "bishop.png",
        pathFolder + color + "knight.png",
        pathFolder + color + "rook.png"
    };

    vector<string> pieceNames = { "Queen", "Bishop", "Knight", "Rook" };

    // Load textures
    vector<Texture> textures;
    for (const auto& path : texturePaths) {
        Texture texture;
        if (!texture.loadFromFile(path)) {
            throw runtime_error("Unable to load piece texture from: " + path);
        }
        textures.push_back(move(texture));
    }

    // Create window and font
    RenderWindow window(VideoMode(600, 200), "Choose Promotion Piece");
    Image icon;
    if (!icon.loadFromFile("../assets/PromotionIcon.png")) {
        std::cerr << "Failed to load icon!" << '\n';
        return nullptr;
    }
    // Set icon for window
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Font font;
    if (!font.loadFromFile("../assets/fonts/Holen Vintage.otf")) {
        throw runtime_error("Unable to load font.");
    }

    vector<Sprite> sprites;
    vector<Text> labels;
    for (size_t i = 0; i < textures.size(); ++i) {
        // Create piece sprite
        Sprite sprite(textures[i]);
        float scaleX = 150.f / sprite.getTexture()->getSize().x;
        float scaleY = 150.f / sprite.getTexture()->getSize().y;
        sprite.setScale(scaleX, scaleY);
        sprite.setPosition(150.f * i, 10.f);
        sprites.push_back(sprite);

        // Create text label
        Text label;
        label.setFont(font);
        label.setString(pieceNames[i]);
        label.setCharacterSize(16);
        label.setFillColor(Color::Black);
        label.setPosition(150.f * i + 50.f, 170.f); // Center text below each piece
        labels.push_back(label);
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return nullptr;
            }
            else if (event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);

                for (size_t i = 0; i < sprites.size(); ++i) {
                    if (sprites[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        switch (i) {
                        case 0: return make_unique<Queen>(isWhite, piece->getRow(), piece->getCol());
                        case 1: return make_unique<Bishop>(isWhite, piece->getRow(), piece->getCol());
                        case 2: return make_unique<Knight>(isWhite, piece->getRow(), piece->getCol());
                        case 3: return make_unique<Rook>(isWhite, piece->getRow(), piece->getCol());
                        }
                    }
                }
            }
        }

        window.clear(Color::White);

        // Draw piece frames, pieces, and labels
        for (size_t i = 0; i < sprites.size(); ++i) {
            // Draw border around each piece
            RectangleShape border(Vector2f(150.f, 150.f));
            border.setPosition(150.f * i, 10.f);
            border.setFillColor(Color::Transparent);
            border.setOutlineColor(Color::Black);
            border.setOutlineThickness(2.f);
            window.draw(border);

            // Draw sprite and label
            window.draw(sprites[i]);
            window.draw(labels[i]);
        }

        window.display();
    }

    return nullptr;
}

void GUI::settingChoice(ChessBoard &chessBoard) {
    /*
    * @Brief: Open a new setting window
    * @How to close: Click "Save" button to exit
    */

    // Change board button
    Font fontBoard;
    if (!fontBoard.loadFromFile("../assets/fonts/ZenOldMincho.ttf")) {
        throw runtime_error("Cannot load ZenOldMincho for GUI::settingChoice");
    }
    Text textBoard;
    textBoard.setFont(fontBoard);
    textBoard.setCharacterSize(25);
    textBoard.setFillColor(Color::White);
    textBoard.setPosition(50, 30);
    textBoard.setString("Board");
    DropDownButton board("Chessboard", 225, 9, 225, 40, chessBoard.getBoardList(), chessBoard.getBoardIndex());

    // Cancel button
    Button cancel;
    cancel.setTextButton("cancel", "Cancel", "../assets/fonts/Holen Vintage.otf", 115, 14, 50, 750, 50, 737.5);

    // Save button
    Button save;
    save.setTextButton("save", "Save", "../assets/fonts/Holen Vintage.otf", 115, 14, 335, 750, 355, 737.5);

    // Open setting window
    RenderWindow settingWD(sf::VideoMode(500, 800), "Setting", Style::Close | Style::Titlebar);
    Image icon;
    if (!icon.loadFromFile("../assets/SettingIcon.png")) {
        std::cerr << "Failed to load icon!" << '\n';
        return;
    }
    // Set icon for window
    settingWD.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Event event;
    Button* selectedButton = nullptr;
    DropDownButton* selectedDDBut = nullptr;

    while (settingWD.isOpen()) {
        while (settingWD.pollEvent(event)) {
            Event::MouseButtonEvent mouse = event.mouseButton;
            if (event.type == Event::Closed) {
                settingWD.close();
                return;
            }
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                // handle press
                selectedButton = nullptr;
                if (save.contain(mouse.x, mouse.y)) selectedButton = &save;
                else {
                    if (board.getClick()) {
                        chessBoard.changeBoard(board.eventOption(mouse.x, mouse.y));
                    }
                }

                DropDownButton* lastDDBut = selectedDDBut;
                selectedDDBut = nullptr;
                if (board.contain(mouse.x, mouse.y)) selectedDDBut = &board;
                if (lastDDBut && lastDDBut != selectedDDBut) {
                    lastDDBut->setClick(0);
                }
                if (selectedDDBut) {
                    selectedDDBut->click();
                }

            }
            else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
                // handle release
                Button* lastSelectedButton = selectedButton;
                selectedButton = nullptr;
                if (save.contain(mouse.x, mouse.y)) selectedButton = &save;

                // If click (press and release) on a button then do operations
                if(selectedButton && selectedButton == lastSelectedButton) {
                    if (selectedButton->getName() == "save") {
                        settingWD.close();
                    }
                }
            }
        }

        RectangleShape background(Vector2f((float)settingWD.getSize().x, 100.0f));
        background.setFillColor(Color::Black);
        background.setPosition(Vector2f(0.0f, settingWD.getSize().y - 100.0f));

        settingWD.clear(Color(60, 60, 60, 255));
        settingWD.draw(background);
        settingWD.draw(textBoard);
        board.draw(settingWD);
        cancel.drawText(settingWD);
        save.drawText(settingWD);
        settingWD.display();
    }
}

void GUI::gameOver(ChessBoard& chessBoard) {
    /*
    * Display text: "White won", "Black won", or "Tied" to announce
    * that the game was over with the corresponding state.
    * 
    * After turning off this window, game reset
    */
    int windowWidth = 300, windowHeight = 200;
    //float rW, rH;

    // Sprite for game over
    Font fontGO;
    if (!fontGO.loadFromFile("../assets/fonts/Vogue.ttf")) {
        throw runtime_error("Font for Game Over cannot be loaded!\n");
    }
    Text textGO;
    textGO.setFont(fontGO);
    textGO.setString("GAME OVER");
    textGO.setCharacterSize(50);
    textGO.setPosition(float(windowWidth / 2.0) - float(textGO.getGlobalBounds().width / 2.0), 25);
    textGO.setFillColor(Color::Black);

    // Text for statement
    Font fontS;
    if (!fontS.loadFromFile("../assets/fonts/Holen Vintage.otf")) {
        throw runtime_error("Font for Game Over Statement cannot be loaded!\n");
    }
    Text textS;
    textS.setFont(fontS);
    switch (chessBoard.isOver()) {
    case 0:
        throw runtime_error("??? The game should have been ended! ???");
        break;
    case 1:
        textS.setString("White won!");
        break;
    case 2:
        textS.setString("Black won!");
        break;
    case 3:
        textS.setString("Tie!");
    }
    textS.setCharacterSize(25);
    textS.setPosition(windowWidth / 2.0f - textS.getGlobalBounds().width / 2.0f, 100);
    textS.setFillColor(Color::Black);
    
    RenderWindow gameOverWD(VideoMode(windowWidth, windowHeight), "Game Over", Style::Titlebar | Style::Close);
    Image icon;
    if (!icon.loadFromFile("../assets/GameOVerIcon.png")) {
        std::cerr << "Failed to load icon!" << '\n';
        return;
    }
    // Set icon for window
    gameOverWD.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Event event;

    while (gameOverWD.isOpen()) {
        while (gameOverWD.pollEvent(event)) {
            if (event.type == Event::Closed) {
                gameOverWD.close();
                chessBoard.newGame();
            }
        }

        gameOverWD.clear(Color(200, 200, 200, 255));
        gameOverWD.draw(textGO);
        gameOverWD.draw(textS);
        gameOverWD.display();
    }
}

void GUI::newGame(ChessBoard& chessBoard) {
    Button onePButton, twoPButton;
    onePButton.setSpriteAndTextButton("playerVersusAI", "../assets/Button/playerVersusAI.png", "Player vs. AI", "../assets/fonts/CherryBombOne.ttf", 420, 50, 40, 40, 50, 50, 40, 40, 350, 30, 110, 32);
    twoPButton.setSpriteAndTextButton("playerVersusPlayer", "../assets/Button/playerVersusPlayer.png", "Player vs. Player", "../assets/fonts/CherryBombOne.ttf", 420, 50, 40, 170, 50, 50, 40, 170, 350, 30, 110, 162);

    Image icon;
    if (!icon.loadFromFile("../assets/NewGameIcon.png")) {
        cerr << "Failed to load icon!" << '\n';
        return;
    }

    int wdWidth = 500, wdHeight = 270; // can be modified later
    RenderWindow newGameWD(VideoMode(wdWidth, wdHeight), "New Game", Style::Titlebar | Style::Close);
    newGameWD.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Event event;
    Event::MouseButtonEvent mouse;
    Button* selectedButton = nullptr;

    while (newGameWD.isOpen()) {
        while (newGameWD.pollEvent(event)) {
            mouse = event.mouseButton;
            if (event.type == Event::Closed) {
                newGameWD.close();
            }
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                selectedButton = nullptr;
                if (onePButton.contain(mouse.x, mouse.y)) selectedButton = &onePButton;
                else if (twoPButton.contain(mouse.x, mouse.y)) selectedButton = &twoPButton;
            }
            else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
                Button* lastSelectedButton = selectedButton;
                selectedButton = nullptr;
                if (onePButton.contain(mouse.x, mouse.y)) selectedButton = &onePButton;
                else if (twoPButton.contain(mouse.x, mouse.y)) selectedButton = &twoPButton;
                if (selectedButton && lastSelectedButton == selectedButton) {
                    if (selectedButton->getName() == "playerVersusAI") {
                        // Start player vs. AI mode
                        newGameWD.close();
                    }
                    else if(selectedButton->getName() == "playerVersusPlayer") {
                        // Start player vs. player mode
                        newGameWD.close();
                    }
                }
            }
        }
        newGameWD.clear();
        onePButton.drawAll(newGameWD);
        twoPButton.drawAll(newGameWD);
        newGameWD.display();
    }
}