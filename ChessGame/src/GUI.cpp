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
    if (!icon.loadFromFile("../assets/Icon/PromotionIcon.png")) {
        cerr << "Failed to load icon!" << '\n';
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
        window.requestFocus();
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

    const float wdWidth = 600.f, wdHeight = 800.f;
    const float topSpace = 30.f, botSpace = 30.f, leftSpace = 50.f, rightSpace = 50.f;
    const float lineSpace = 30.f;
    const float dropDownButtonWidth = 225.f;
    const float dropDownButtonSpace = 50.f;

    // Save old data(s)
    const int initBoard = chessBoard.getBoardIndex();
    const int initPiece = chessBoard.getPieceIndex();
    const std::string color = chessBoard.isWhiteTurn() ? "white" : "black";

    // Board preview
    Sprite reviewBoard(chessBoard.getBoardSprite());
    reviewBoard.setPosition(leftSpace, topSpace);
    reviewBoard.setTextureRect(IntRect(65, 65, 500, 200));
    const float previewSpace = topSpace + reviewBoard.getGlobalBounds().height;
    vector<std::string> pieceThemePath = {
        "../assets/Standard Theme/",
        "../assets/Cartoon Theme/",
        "../assets/Pixel Theme/",
        "../assets/Neo Theme/"
    };
    vector<std::string> piecePath = {
        color + "-king.png",
        color + "-queen.png",
        color + "-bishop.png",
        color + "-knight.png",
        color + "-rook.png",
        color + "-pawn.png",
        color + "-pawn.png",
        color + "-pawn.png",
        color + "-pawn.png",
        color + "-pawn.png",
    };
    vector<Texture> pieceTexture(10);
    vector<Sprite> reviewPiece(10);
    for (int i = 0; i < 10; i++) {
        if (i < 5)
            reviewPiece[i].setPosition(leftSpace + i * 100, topSpace);
        else
            reviewPiece[i].setPosition(leftSpace + (i - 5) * 100, topSpace + 100);
    }
    setPiece(initPiece, pieceThemePath, piecePath, pieceTexture, reviewPiece);

    // Change board button
    Font fontBoard;
    if (!fontBoard.loadFromFile("../assets/fonts/ZenOldMincho.ttf")) {
        throw runtime_error("Cannot load ZenOldMincho for GUI::settingChoice");
    }
    Text textBoard("Board", fontBoard, 25);
    textBoard.setFillColor(Color::White);
    textBoard.setPosition(leftSpace, previewSpace + lineSpace - 10);
    DropDownButton board("Board", dropDownButtonWidth, 9, wdWidth - rightSpace - dropDownButtonWidth - 13, previewSpace + lineSpace, chessBoard.getBoardList(), chessBoard.getBoardIndex());
    const float boardSpace = previewSpace + dropDownButtonSpace;

    // Change piece button
    Text textPiece("Pieces", fontBoard, 25);
    textPiece.setFillColor(Color::White);
    textPiece.setPosition(leftSpace, boardSpace + lineSpace - 10);
    DropDownButton piece("Pieces", dropDownButtonWidth, 9, wdWidth - rightSpace - dropDownButtonWidth - 13, boardSpace + lineSpace, chessBoard.getPieceList(), chessBoard.getPieceIndex());
    const float pieceSpace = previewSpace + dropDownButtonSpace * 2;

    // Cancel button
    Button cancel;
    cancel.setTextButton("cancel", "Cancel", "../assets/fonts/Holen Vintage.otf", 115, 12, leftSpace, wdHeight - botSpace - 12 - 13, leftSpace, wdHeight - botSpace - 12 - 13 - 10);

    // Save button
    Button apply;
    apply.setTextButton("apply", "Apply", "../assets/fonts/Holen Vintage.otf", 115, 12, wdWidth - rightSpace - 115 - 13, wdHeight - botSpace - 12 - 13, wdWidth - rightSpace - 115 - 13, wdHeight - botSpace - 12 - 13 - 10);

    // Open setting window
    RenderWindow settingWD(VideoMode(600, 800), "Setting", Style::Close | Style::Titlebar);

    // Set icon for window
    Image icon;
    if (!icon.loadFromFile("../assets/Icon/SettingIcon.png")) {
        cerr << "Failed to load icon!" << '\n';
        return;
    }
    settingWD.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Event event;
    Button* selectedButton = nullptr;
    DropDownButton* selectedDropDownButton = nullptr;
    vector<Sprite*> spriteList = { &reviewBoard };
    for (int i = 0; i < 10; i++) spriteList.push_back(&reviewPiece[i]);
    vector<Text*> textList = { &textBoard, &textPiece };
    vector<Button*> buttonList = { &apply, &cancel };
    vector<DropDownButton*> dropDownButtonList = { &board, &piece };

    while (settingWD.isOpen()) {
        settingWD.requestFocus();
        while (settingWD.pollEvent(event)) {
            if (event.type == Event::Closed) {
                chessBoard.changeBoard(initBoard);
                settingWD.close();
            }
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                int lastPieceIndex = chessBoard.getPieceIndex();
                handlePressSetting(selectedButton, selectedDropDownButton, event.mouseButton, buttonList, dropDownButtonList, chessBoard);
                if(lastPieceIndex != chessBoard.getPieceIndex()) 
                    setPiece(chessBoard.getPieceIndex(), pieceThemePath, piecePath, pieceTexture, reviewPiece);
            }
            else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
                handleReleaseSetting(selectedButton, selectedDropDownButton, event.mouseButton, buttonList, dropDownButtonList, initBoard, chessBoard, settingWD);
            }
        }

        drawSetting(settingWD, selectedDropDownButton, spriteList, textList, buttonList, dropDownButtonList);
    }
}

void GUI::setPiece(int num, const vector<std::string>& pieceThemePath, const vector<std::string>& piecePath, vector<Texture>& pieceTexture, vector<Sprite>& reviewPiece) {
    for (int i = 0; i < 10; i++) {
        if (!pieceTexture[i].loadFromFile(pieceThemePath[num] + piecePath[i])) {
            throw runtime_error("Cannot load piece texture in GUI::setPiece");
        }
        float r = min(100.f / pieceTexture[i].getSize().x, 100.f / pieceTexture[i].getSize().y);
        Sprite newSprite;
        newSprite.setTexture(pieceTexture[i]);
        newSprite.setScale(r, r);
        newSprite.setPosition(reviewPiece[i].getPosition());
        reviewPiece[i] = newSprite;
    }
}

void GUI::handlePressSetting(Button *&selectedButton, DropDownButton *&selectedDropDownButton, Event::MouseButtonEvent &mouse, vector<Button*> &buttonList, vector<DropDownButton*> &dropDownButtonList, ChessBoard& chessBoard) {
    selectedButton = nullptr;
    for (Button*& button : buttonList) {
        if (button->contain(mouse.x, mouse.y)) {
            selectedButton = button;
            break;
        }
    }

    DropDownButton* lastDropDownButton = selectedDropDownButton;
    selectedDropDownButton = nullptr;
    if (lastDropDownButton && lastDropDownButton->getClick()) {
        int optionVal = lastDropDownButton->eventOption(mouse.x, mouse.y);
        if (lastDropDownButton->getName() == "Board") {
            chessBoard.changeBoard(optionVal);
        }
        else if (lastDropDownButton->getName() == "Pieces") {
            chessBoard.changePieces(optionVal);
        }
        lastDropDownButton->setClick(0);
        return;
    }
    for (DropDownButton*& button : dropDownButtonList) {
        if (button->contain(mouse.x, mouse.y)) {
            selectedDropDownButton = button;
        }
    }
    if (lastDropDownButton && lastDropDownButton != selectedDropDownButton) {
        lastDropDownButton->setClick(0);
    }
    if (selectedDropDownButton) {
        selectedDropDownButton->click();
    }
}

void GUI::handleReleaseSetting(Button *&selectedButton, DropDownButton *&selectedDropDownButton, Event::MouseButtonEvent &mouse, vector<Button*> &buttonList, vector<DropDownButton*> &dropDownButtonList, int initBoard, ChessBoard &chessBoard, RenderWindow &window) {
    Button* lastSelectedButton = selectedButton;
    selectedButton = nullptr;
    for (Button*& button : buttonList) {
        if (button->contain(mouse.x, mouse.y)) {
            selectedButton = button;
            break;
        }
    }

    // If click (press and release) on a button then do operations
    if (selectedButton && selectedButton == lastSelectedButton) {
        if (selectedButton->getName() == "apply") {
            window.close();
        }
        else if (selectedButton->getName() == "cancel") {
            chessBoard.changeBoard(initBoard);
            window.close();
        }
    }
}

void GUI::drawSetting(RenderWindow& window, DropDownButton* selectedDropDownButton, vector<Sprite*> &spriteList, vector<Text*> &textList, vector<Button*> &buttonList, vector<DropDownButton*> &dropDownButtonList) {
    RectangleShape background(Vector2f((float)window.getSize().x, 100.0f));
    background.setFillColor(Color::Black);
    background.setPosition(Vector2f(0.0f, window.getSize().y - 100.0f));
    window.clear(Color(60, 60, 60, 255));
    window.draw(background);
    for (Sprite* sprite : spriteList) {
        window.draw(*sprite);
    }
    for (Text* text : textList) {
        window.draw(*text);
    }
    for (Button* button : buttonList) {
        button->drawText(window);
    }
    for(DropDownButton* button : dropDownButtonList) {
        button->draw(window);
    }
    if (selectedDropDownButton && selectedDropDownButton->getClick()) {
        selectedDropDownButton->drawOptions(window);
    }
    window.display();
}

void GUI::gameOver(ChessBoard& chessBoard) {
    /*
    * Display text: "White won", "Black won", or "Tied" to announce
    * that the game was over with the corresponding state.
    * 
    * After turning off this window, game stay but player cannot touch the screen
    */
    const int windowWidth = 800, windowHeight = 400;
    const int topSpace = 25, lineSpace = 10;
    const int textGameOverSize = 50, textStatementSize = 25;
    const int topGameOverSpace = topSpace;
    const int topStatementSpace = topGameOverSpace + textGameOverSize + lineSpace;
    //float rW, rH;

    // Sprite for game over
    Font fontGameOver;
    assert(fontGameOver.loadFromFile("../assets/fonts/Holen Vintage.otf"));
    Text textGameOver;
    textGameOver.setFont(fontGameOver);
    textGameOver.setString("GAME OVER");
    textGameOver.setCharacterSize(textGameOverSize);
    textGameOver.setPosition((windowWidth - textGameOver.getGlobalBounds().width) / 2.0f, topGameOverSpace);
    textGameOver.setFillColor(Color::Black);

    // Text for statement
    Font fontStatement;
    assert(fontStatement.loadFromFile("../assets/fonts/Holen Vintage.otf") == true);
    Text textStatement;
    textStatement.setFont(fontStatement);
    assert(chessBoard.isOver() != 0);
    if (chessBoard.isOver() == 1)
        textStatement.setString("White won!");
    else if (chessBoard.isOver() == 2)
        textStatement.setString("Black won!");
    else if (chessBoard.isOver() == 3)
        textStatement.setString("Tie!");
    textStatement.setCharacterSize(textStatementSize);
    textStatement.setPosition((windowWidth - textStatement.getGlobalBounds().width) / 2.0f, topStatementSpace);
    textStatement.setFillColor(Color::Black);
    
    RenderWindow gameOverWD(VideoMode(windowWidth, windowHeight), "Game Over", Style::Titlebar | Style::Close);
    Image icon;
    if (!icon.loadFromFile("../assets/Icon/GameOVerIcon.png")) {
        cerr << "Failed to load icon!" << '\n';
        return;
    }
    // Set icon for window
    gameOverWD.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Event event;

    while (gameOverWD.isOpen()) {
        gameOverWD.requestFocus();
        while (gameOverWD.pollEvent(event)) {
            if (event.type == Event::Closed) {
                gameOverWD.close();
            }
        }

        gameOverWD.clear(Color(200, 200, 200, 255));
        gameOverWD.draw(textGameOver);
        gameOverWD.draw(textStatement);
        gameOverWD.display();
    }
}

bool GUI::newGame(ChessBoard& chessBoard) {
    Button onePButton, twoPButton;
    onePButton.setSpriteAndTextButton("playerVersusAI", "../assets/Button/playerVersusAI.png", "Player vs. AI", "../assets/fonts/CherryBombOne.ttf", 420, 50, 40, 40, 50, 50, 40, 40, 350, 30, 110, 32);
    twoPButton.setSpriteAndTextButton("playerVersusPlayer", "../assets/Button/playerVersusPlayer.png", "Player vs. Player", "../assets/fonts/CherryBombOne.ttf", 420, 50, 40, 170, 50, 50, 40, 170, 350, 30, 110, 162);

    Image icon;
    if (!icon.loadFromFile("../assets/Icon/NewGameIcon.png")) {
        cerr << "Failed to load icon!" << '\n';
        return false;
    }

    int wdWidth = 500, wdHeight = 270; // can be modified later
    RenderWindow newGameWD(VideoMode(wdWidth, wdHeight), "New Game", Style::Titlebar | Style::Close);
    newGameWD.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Event event;
    Event::MouseButtonEvent mouse;
    Button* selectedButton = nullptr;

    while (newGameWD.isOpen()) {
        newGameWD.requestFocus();
        while (newGameWD.pollEvent(event)) {
            mouse = event.mouseButton;
            if (event.type == Event::Closed) {
                return false;
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

    return true;
}

void GUI::saveGame(ChessBoard* chessBoard) {
    bool isSave = false;

    RenderWindow window(VideoMode(800, 400), "Save Game", Style::Close | Style::Titlebar);
    Image icon;
    if (!icon.loadFromFile("../assets/Icon/saveGameIcon.png")) {
        cerr << "Failed to load icon!" << '\n';
        return;
    }
    // Set icon for window
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    // Font để hiển thị văn bản
    Font font;
    if (!font.loadFromFile("../assets/fonts/Holen Vintage.otf")) {
        throw exception("Failed to load font file.\n");
    }

    // Khai báo biến để nhập tên tệp
    string filename;
    Text filenameText("", font, 30);
    filenameText.setFillColor(Color::Black);
    filenameText.setPosition(110, window.getSize().y / 2.0f + 3);  // Vị trí chữ nhập vào

    // Tạo background cho textbox
    RectangleShape textbox(Vector2f(window.getSize().x - 200.0f, 50.0f));
    textbox.setFillColor(Color::White);  // Màu nền của hộp nhập liệu
    textbox.setOutlineColor(Color(0xA6, 0x8A, 0x64));  // Viền hộp
    textbox.setOutlineThickness(5);  // Độ dày viền
    textbox.setPosition(100, window.getSize().y / 2.0f);  // Vị trí của hộp nhập liệu

    // Con trỏ nhập liệu
    RectangleShape cursor(Vector2f(2, 44));  // Định nghĩa con trỏ nhập liệu
    cursor.setFillColor(Color::Black);
    cursor.setPosition(110, window.getSize().y / 2.0f + 3);  // Vị trí bắt đầu của con trỏ

    // Thông báo yêu cầu nhập tên tệp
    Text instruction("Enter filename and press Enter to save:", font, 25);
    instruction.setFillColor(Color::White);
    instruction.setPosition((window.getSize().x - instruction.getLocalBounds().width) / 2, window.getSize().y / 4.0f);

    Text notification;
    bool notify = false;

    bool cursorVisible = true;  // Để kiểm soát việc hiển thị con trỏ
    Clock clock;  // Dùng để thay đổi trạng thái con trỏ (hiện/ẩn) mỗi giây

    // Giới hạn chiều dài tên tệp (dựa trên chiều rộng của textbox)
    const float maxWidth = textbox.getSize().x - 30; // Khoảng cách giữa viền và văn bản
    const size_t maxChars = 50; // Giới hạn ký tự (có thể thay đổi)

    while (window.isOpen()) {
        Event event;
        window.requestFocus();
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::KeyPressed) {
                notify = false; 
            }

            // Xử lý nhập liệu từ bàn phím
            if (event.type == Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char enteredChar = static_cast<char>(event.text.unicode);
                    if (enteredChar == '\b' && !filename.empty()) {
                        filename.pop_back();  // Xóa ký tự cuối nếu nhấn Backspace
                    }
                    else if (enteredChar != '\r') {
                        // Kiểm tra nếu thêm ký tự không vượt quá giới hạn ký tự
                        if (filename.length() < maxChars) {
                            filename += enteredChar;  // Thêm ký tự vào tên tệp
                        }
                        if (filenameText.getLocalBounds().width > maxWidth) {
                            filename.pop_back();
                        }
                    }
                }
            }

            // Kiểm tra nếu người dùng nhấn Enter
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                notify = true;
                if (isSave) {
                    notification.setString("THIS GAME IS SAVED");
                    notification.setFont(font);
                    notification.setCharacterSize(25);
                    notification.setFillColor(Color::Yellow);
                    notification.setPosition((window.getSize().x - notification.getLocalBounds().width) / 2, 3 * (window.getSize().y / 4.0f));
                }
                else if (!filename.empty()) {
                    if (chessBoard->saveGame(filename)) {
                        //Save successful
                        notification.setString("SAVE GAME SUCCESSFULLY");
                        notification.setFont(font);
                        notification.setCharacterSize(25);
                        notification.setFillColor(Color::Green);
                        notification.setPosition((window.getSize().x - notification.getLocalBounds().width) / 2, 3 * (window.getSize().y / 4.0f));
                        isSave = true;
                    }
                    else {
                        notification.setString("FILE ALREADY EXISTS");
                        notification.setFont(font);
                        notification.setCharacterSize(25);
                        notification.setFillColor(Color::Yellow);
                        notification.setPosition((window.getSize().x - notification.getLocalBounds().width) / 2, 3 * (window.getSize().y / 4.0f));
                    }
                }
                else {
                    notification.setString("EMPTY FILE NAME IS NOT ALLOWED");
                    notification.setFont(font);
                    notification.setCharacterSize(25);
                    notification.setFillColor(Color::Red);
                    notification.setPosition((window.getSize().x - notification.getLocalBounds().width) / 2, 3 * (window.getSize().y / 4.0f));
                }
            }
        }

        // Cập nhật vị trí con trỏ nhập liệu (sau mỗi 0.5 giây, đổi trạng thái hiển thị)
        if (clock.getElapsedTime().asSeconds() > 0.5f) {
            cursorVisible = !cursorVisible;  // Đổi trạng thái con trỏ
            clock.restart();
        }

        // Cập nhật văn bản hiển thị
        filenameText.setString(filename);

        // Cập nhật vị trí con trỏ
        cursor.setPosition(110 + filenameText.getLocalBounds().width, window.getSize().y / 2.0f + 3);  // Vị trí con trỏ ở cuối văn bản

        // Vẽ lên cửa sổ
        window.clear(Color(80, 80, 80, 255));
        window.draw(instruction);
        window.draw(textbox);  // Vẽ background của textbox
        window.draw(filenameText);  // Vẽ tên tệp người dùng nhập vào
        if (cursorVisible) window.draw(cursor);  // Vẽ con trỏ nếu đang visible
        if (notify) {
            window.draw(notification);
        }
        window.display();
    }
}

void GUI::loadGame(ChessBoard& chessBoard) {
    RenderWindow loadWD(VideoMode(800, 400), "Load game", Style::Close | Style::Titlebar);
    Image icon;
    if (!icon.loadFromFile("../assets/Icon/LoadGameIcon.png")) {
        throw runtime_error("Cannot load 'LoadGameIcon.png'!\n");
    }
    loadWD.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Event event;

    while (loadWD.isOpen()) {
        loadWD.requestFocus();
        while (loadWD.pollEvent(event)) {
            if (event.type == Event::Closed) {
                loadWD.close();
            }
        }

        loadWD.clear(Color(80, 80, 80, 255));
        loadWD.display();
    }
}