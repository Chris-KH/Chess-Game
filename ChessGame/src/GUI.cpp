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
    * @Brief: Open a new setting choice, no close, resize button or title bar
    * @How to close: Click "Save" button to exit
    */

    // Text
    Font font;
    if (!font.loadFromFile("../assets/fonts/Vogue.ttf")) {
        throw runtime_error("Cannot load text font in GUI::settingChoice!\n");
    }

    // Save button
    Button save;
    save.setSize(100, 40);
    save.setPosition(350, 700);
    save.setText("Save", font);
    save.setName("save");

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

        settingWD.clear(Color(50, 50, 50, 255));
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
    textS.setPosition(windowWidth / 2.0 - textS.getGlobalBounds().width / 2.0, 100);
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
                chessBoard.newtGame();
            }
        }

        gameOverWD.clear(Color(200, 200, 200, 255));
        gameOverWD.draw(textGO);
        gameOverWD.draw(textS);
        gameOverWD.display();
    }
}