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

    // Save old datas
    const int initBoard = chessBoard.getBoardIndex();
    const int initPiece = chessBoard.getPieceIndex();
    const std::string color = chessBoard.isWhiteTurn() ? "white" : "black";

    // Paths
    vector<std::string> pieceThemePath = {
        "../assets/Standard Theme/",
        "../assets/Cartoon Theme/",
        "../assets/Pixel Theme/",
        "../assets/Neo Theme/",
        "../assets/Game Room Theme/",
        "../assets/Neon Theme/"
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
    Font fontTitle;
    if (!fontTitle.loadFromFile("../assets/fonts/ZenOldMincho.ttf")) {
        throw runtime_error("Cannot load font for setting");
    }

    // Sizes and spaces
    const float wdWidth = 550.f, wdHeight = 800.f;
    const float topSpace = 25.f, botSpace = 25.f, leftSpace = 25.f, rightSpace = 25.f;
    const float lineSpace = 30.f;
    const float previewBoardWidth = 500.f, previewBoardHeight = 200.f;
    const float dropDownButtonWidth = 300.f, dropDownButtonHeight = 40.f;
    const float dropDownButtonTitleHeight = 25.f;
    const float buttonWidth = 200.f, buttonHeight = 40.f;
    // Elements' space
    const float previewBoardSpace = topSpace;
    const float changeBoardSpace = previewBoardSpace + previewBoardHeight + lineSpace;
    const float changePieceSpace = changeBoardSpace + dropDownButtonHeight + lineSpace;

    // Open setting window
    RenderWindow settingWD(VideoMode((unsigned)wdWidth, (unsigned)wdHeight), "Setting", Style::Close | Style::Titlebar);

    // Set icon for window
    Image icon;
    if (!icon.loadFromFile("../assets/Icon/SettingIcon.png")) {
        cerr << "Failed to load icon!" << '\n';
        return;
    }
    settingWD.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Board preview
    Sprite previewBoard(chessBoard.getBoardSprite());
    previewBoard.setPosition((wdWidth - previewBoardWidth) / 2.f, previewBoardSpace);
    previewBoard.setTextureRect(IntRect(65, 65, 5 * 100, 2 * 100));
    vector<Texture> pieceTexture(10);
    vector<Sprite> reviewPiece(10);
    for (int i = 0; i < 5; i++) reviewPiece[i].setPosition(leftSpace + i * 100, previewBoardSpace);
    for (int i = 5; i < 10; i++) reviewPiece[i].setPosition(leftSpace + (i - 5) * 100, previewBoardSpace + 100);
    setPiece(initPiece, pieceThemePath, piecePath, pieceTexture, reviewPiece);

    // Change board button
    Text textBoard("Board", fontTitle, (unsigned)dropDownButtonTitleHeight);
    textBoard.setFillColor(Color::White);
    textBoard.setPosition(leftSpace, changeBoardSpace);
    DropDownButton board(&settingWD, "Board", dropDownButtonWidth, dropDownButtonHeight, wdWidth - rightSpace - dropDownButtonWidth, changeBoardSpace, chessBoard.getBoardList(), chessBoard.getBoardIndex());

    // Change piece button
    Text textPiece("Pieces", fontTitle, (unsigned)dropDownButtonTitleHeight);
    textPiece.setFillColor(Color::White);
    textPiece.setPosition(leftSpace, changePieceSpace);
    DropDownButton piece(&settingWD, "Pieces", dropDownButtonWidth, dropDownButtonHeight, wdWidth - rightSpace - dropDownButtonWidth, changePieceSpace, chessBoard.getPieceList(), chessBoard.getPieceIndex());

    // Background
    RectangleShape background(Vector2f((float)settingWD.getSize().x, 100.0f));
    background.setFillColor(Color::Black);
    background.setPosition(Vector2f(0.0f, settingWD.getSize().y - 100.0f));

    // Cancel button
    Button cancel;
    cancel.setTextButton("cancel", "Cancel", "../assets/fonts/Holen Vintage.otf", buttonWidth, buttonHeight, leftSpace, wdHeight - botSpace - buttonHeight);

    // Save button
    Button apply;
    apply.setTextButton("apply", "Apply", "../assets/fonts/Holen Vintage.otf", buttonWidth, buttonHeight, wdWidth - rightSpace - buttonWidth, wdHeight - botSpace - buttonHeight);

    // Scrollable setting
    ScrollableArea scroll(Vector2f(0.f, 0.f), Vector2f(wdWidth, wdHeight * 2), settingWD);

    Event event;
    Button* selectedButton = nullptr;
    DropDownButton* selectedDropDownButton = nullptr;
    
    vector<Button*> buttonList(2, nullptr);
    vector<Sprite*> spriteList(11, nullptr);
    vector<DropDownButton*> dropDownButtonList(2, nullptr);
    vector<Text*> textList(2, nullptr);
    vector<RectangleShape*> rectangleShapeList(1, nullptr);

    spriteList[0] = &previewBoard;
    for (int i = 0; i < 10; i++) spriteList[i + 1] = &reviewPiece[i];
    textList[0] = &textBoard, textList[1] = &textPiece;
    buttonList[0] = &apply, buttonList[1] = &cancel;
    dropDownButtonList[0] = &board, dropDownButtonList[1] = &piece;
    rectangleShapeList[0] = &background;

    for (int i = 0; i < (int)spriteList.size(); i++) {
        scroll.addExternalSprite(spriteList[i]);
    }

    for (int i = 0; i < (int)dropDownButtonList.size(); i++) {
        scroll.addExternalDropDownButton(dropDownButtonList[i]);
    }

    for (int i = 0; i < (int)textList.size(); i++) {
        scroll.addExternalText(textList[i]);
    }

    for (int i = 0; i < (int)rectangleShapeList.size(); i++) {
        scroll.addFixedExternalRectangleShape(rectangleShapeList[i]);
    }

    for (int i = 0; i < (int)buttonList.size(); i++) {
        scroll.addFixedExternalButton(buttonList[i]);
    }

    // Poll events and handle them
    while (settingWD.isOpen()) {
        settingWD.requestFocus();
        while (settingWD.pollEvent(event)) {
            Event::MouseButtonEvent mouse = event.mouseButton;
            if (event.type == Event::Closed) {
                chessBoard.changeBoard(initBoard);
                chessBoard.changePieces(initPiece);
                settingWD.close();
            }
            else if (event.type == Event::MouseButtonPressed && mouse.button == Mouse::Left) {
                int lastPieceIndex = chessBoard.getPieceIndex();
                handlePressSetting(mouse, scroll.getView(), selectedButton, selectedDropDownButton, buttonList, dropDownButtonList, initBoard, initPiece, chessBoard, settingWD);
                if(lastPieceIndex != chessBoard.getPieceIndex()) 
                    setPiece(chessBoard.getPieceIndex(), pieceThemePath, piecePath, pieceTexture, reviewPiece);
            }
            scroll.handleEvent(event, settingWD);
        }

        settingWD.clear(Color(60, 60, 60, 255));
        scroll.draw(settingWD);
        settingWD.display();
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

void GUI::handlePressSetting(Event::MouseButtonEvent mouse, const View& view, Button*& selectedButton, DropDownButton*& selectedDropDownButton, vector<Button*>& buttonList, vector<DropDownButton*>& dropDownButtonList, int initBoard, int initPiece, ChessBoard& chessBoard, RenderWindow& window) {
    Vector2f mouseWindow = window.mapPixelToCoords({ mouse.x, mouse.y }); // Mouse cho phần tử cố định
    Vector2f mouseView = window.mapPixelToCoords({ mouse.x, mouse.y }, view); // Mouse cho phần tử không cố định
    
    selectedButton = nullptr;
    for (Button*& button : buttonList) {
        if (button->contain(mouseWindow.x, mouseWindow.y)) {
            selectedButton = button;
            break;
        }
    }

    if (selectedButton) {
        if (selectedButton->getName() == "apply") {
            window.close();
        }
        else if (selectedButton->getName() == "cancel") {
            chessBoard.changeBoard(initBoard);
            chessBoard.changePieces(initPiece);
            window.close();
        }
    }

    DropDownButton* lastDropDownButton = selectedDropDownButton;
    selectedDropDownButton = nullptr;
    if (lastDropDownButton && lastDropDownButton->getClick()) {
        int optionVal = lastDropDownButton->eventOption(mouseView.x, mouseView.y);
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
        if (button->contain(mouseView.x, mouseView.y)) {
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

void GUI::gameOver(ChessBoard& chessBoard) {
    /*
    * Display text: "White won", "Black won", or "Tied" to announce
    * that the game was over with the corresponding state.
    * 
    * After turning off this window, game stay but player cannot touch the screen
    */
    const int windowWidth = 800, windowHeight = 400;
    const int gameOverSize = 50, statementSize = 35;
    const int notiSize = 35;

    const int topSpace = 25, line = 20;
    const int gameOverY = topSpace;
    const int statementY = gameOverY + gameOverSize + line;
    const int notiY = statementY + statementSize + line;
    //float rW, rH;

    // Sprite for game over
    Font font;
    if (!font.loadFromFile("../assets/fonts/Holen Vintage.otf")) {
        throw runtime_error("Cannot load font game over");
    }
    Text textGameOver;
    textGameOver.setFont(font);
    textGameOver.setString("GAME OVER");
    textGameOver.setCharacterSize(gameOverSize);
    textGameOver.setPosition((windowWidth - textGameOver.getGlobalBounds().width) / 2.0f, gameOverY);
    textGameOver.setFillColor(Color::Black);

    // Text for statement
    Text textStatement;
    textStatement.setFont(font);
    if (chessBoard.isOver() == 0)
        throw runtime_error("The game should have ended\n");
    if (chessBoard.isOver() == 1)
        textStatement.setString("White won!");
    else if (chessBoard.isOver() == 2)
        textStatement.setString("Black won!");
    else if (chessBoard.isOver() == 3)
        textStatement.setString("Tie!");
    textStatement.setCharacterSize(statementSize);
    textStatement.setPosition((windowWidth - textStatement.getGlobalBounds().width) / 2.f, statementY);
    textStatement.setFillColor(Color::Black);

    // Notification : Checkmate or Stalemate
    Text noti;
    noti.setFont(font);
    noti.setString(chessBoard.getStateOver());
    noti.setCharacterSize(notiSize);
    noti.setPosition((windowWidth - noti.getGlobalBounds().width) / 2.f, notiY);
    noti.setOutlineThickness(-1);
    noti.setOutlineColor(Color::Black);
    noti.setFillColor(Color::Red);
    
    RenderWindow gameOverWD(VideoMode(windowWidth, windowHeight), "Game Over", Style::Titlebar | Style::Close);
    Image icon;
    if (!icon.loadFromFile("../assets/Icon/GameOVerIcon.png")) {
        throw runtime_error("Failed to load icon!\n");
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
        gameOverWD.draw(noti);
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
                        chessBoard.setAI(true);
                        newGameWD.close();
                    }
                    else if(selectedButton->getName() == "playerVersusPlayer") {
                        chessBoard.setAI(false);
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

    if (chessBoard.getAI() == true) {
        return chooseDifficulty(chessBoard);
    }

    return true;
}

bool GUI::chooseDifficulty(ChessBoard& chessBoard) {
    Font font;
    if (font.loadFromFile("../assets/fonts/CherryBombOne.ttf") == false) {
        throw runtime_error("Cannot load font for choose difficulty\n");
    }

    const int wdWidth = 500, wdHeight = 270;
    const int insHeight = 35;
    const int slideWidth = 400, slideHeight = 10;
    const int numDiff = 20;
    const int labelHeight = 25;
    const int buttonWidth = 100, buttonHeight = 40;

    const int space = 20;
    const int leftSpace = 50;
    const int botSpace = 20;

    const int insY = 20;
    const int slideY = insY + insHeight + space;
    const int labelY = slideY + slideHeight + space;
    const int applyButtonY = wdHeight - 2 * botSpace - buttonHeight;

    Image icon;
    if (icon.loadFromFile("../assets/Icon/NewGameIcon.png") == false) {
        throw runtime_error("Cannot load the new game icon\n");
    }

    Text instruction;
    instruction.setFont(font);
    instruction.setCharacterSize(insHeight);
    instruction.setString("Slide to choose difficulty");
    instruction.setFillColor(Color(255, 255, 255));
    instruction.setPosition((wdWidth - instruction.getGlobalBounds().width) / 2.f, insY);

    string labels[numDiff];
    for (int i = 0; i < numDiff; i++) {
        labels[i] = to_string(i + 1);
        if (i < 9) labels[i] = '0' + labels[i];
    }
    Slider slider((wdWidth - slideWidth) / 2.f, slideY, slideWidth, slideHeight, numDiff);
    slider.setChangeKnobColor(true);

    Text labelText;
    labelText.setFont(font);
    labelText.setCharacterSize(labelHeight);
    labelText.setFillColor(Color(255, 255, 255));
    labelText.setPosition(leftSpace, labelY);

    Button applyButton;
    applyButton.setTextButton("apply", "Apply", "../assets/fonts/CherryBombOne.ttf", buttonWidth, buttonHeight, (wdWidth - buttonWidth) / 2.f, applyButtonY);
    
    RenderWindow AIDiffWD(VideoMode(wdWidth, wdHeight), "Choose difficulty", Style::Close | Style::Titlebar);
    AIDiffWD.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    while (AIDiffWD.isOpen()) {
        AIDiffWD.requestFocus();

        Event event;
        while (AIDiffWD.pollEvent(event)) {
            if (event.type == Event::Closed) {
                AIDiffWD.close();
                return false;
            }
            else if (event.type == Event::MouseButtonPressed) {
                if (applyButton.contain(event.mouseButton.x, event.mouseButton.y)) {
                    AIDiffWD.close();
                }
            }
            slider.handleEvent(event, AIDiffWD);
        }

        labelText.setFillColor(slider.getKnobFillColor());

        AIDiffWD.clear();

        AIDiffWD.draw(instruction);
        slider.draw(AIDiffWD);
        labelText.setString("Difficulty: " + labels[slider.getCurrentStep()]);
        AIDiffWD.draw(labelText);
        applyButton.drawText(AIDiffWD);

        AIDiffWD.display();
    }

    chessBoard.setAISkillLevel(slider.getCurrentStep() + 1);

    return true;
}

void GUI::saveGame(ChessBoard* chessBoard) {
    bool isSave = false;

    // Sizes
    const float wdWidth = 800, wdHeight = 400;

    // Window
    RenderWindow window(VideoMode((unsigned)wdWidth, (unsigned)wdHeight), "Save Game", Style::Close | Style::Titlebar);
    Image icon;
    if (!icon.loadFromFile("../assets/Icon/saveGameIcon.png")) {
        throw runtime_error("Cannot load Save Game icon");
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Font
    Font font;
    if (!font.loadFromFile("../assets/fonts/Holen Vintage.otf")) {
        throw runtime_error("Cannot load font save game");
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

void GUI::loadGame(ChessBoard& chessBoard, string path) {
    // Sizes
    const float wdWidth = 800.f, wdHeight = 600.f;

    // Window
    RenderWindow loadWD(VideoMode((unsigned)wdWidth, (unsigned)wdHeight), "Load game", Style::Close | Style::Titlebar);
    loadWD.setFramerateLimit(120);
    Image icon;
    if (!icon.loadFromFile("../assets/Icon/LoadGameIcon.png")) {
        throw runtime_error("Cannot load file load game icon");
    }
    loadWD.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    string folderPath = "../save";

    vector<string> fileName;

    if (filesystem::exists(folderPath) && filesystem::is_directory(folderPath)) {
        for (const auto& entry : filesystem::directory_iterator(folderPath)) {
            if (filesystem::is_regular_file(entry)) {
                fileName.push_back(entry.path().filename().string());
            }
        }
    }

    size_t numberOfFile = fileName.size();

    ScrollableArea scroll(Vector2f(0.f, 0.f), Vector2f(wdWidth, 20.f + 60.f * numberOfFile), loadWD);

    for (size_t i = 0; i < numberOfFile; i++) {
        unique_ptr<Button> item = make_unique<Button>();
        item->setTextButton(string("Button") + to_string(i + 1), fileName[i], "../assets/fonts/TimesNewRoman.ttf", 600.f, 40.f, 40.f, i * 60.f + 20.f);
        scroll.addButtonItem(item);
    }

    Event event;
    // Poll events and handle
    while (loadWD.isOpen()) {
        loadWD.requestFocus();

        while (loadWD.pollEvent(event)) {
            if (event.type == Event::Closed) {
                loadWD.close();
            }

            scroll.handleEvent(event, loadWD);
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                int index = scroll.detectClickedButtonItem(Mouse::getPosition(loadWD), loadWD);
                if (index != -1) cout << fileName[index] << '\n';
            }
        }

        loadWD.clear(Color(80, 80, 80, 255));
        scroll.draw(loadWD);
        loadWD.display();
    }
}