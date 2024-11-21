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

void GUI::settingChoice(ChessBoard &chessboard) {
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
    RenderWindow settingWD(sf::VideoMode(500, 800), "Setting", Style::None);
    Event event;
    Button* selectedButton = nullptr;

    while (settingWD.isOpen()) {
        while (settingWD.pollEvent(event)) {
            Event::MouseButtonEvent mouse = event.mouseButton;
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
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

        settingWD.clear(Color::Cyan);
        save.drawText(settingWD);
        settingWD.display();
    }

    cout << "Window has been closed!\n";
}

void GUI::gameOver(void) {
    /*
    * Display text: "White won", "Black won", or "Tied" to announce that the game is over with the corresponding state.
    */


}