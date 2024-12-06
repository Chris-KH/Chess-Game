#include "../lib/ScrollableArea.h"

ScrollableArea::ScrollableArea(Vector2f position, Vector2f size, RenderWindow& window) {
    // Khu vực hiển thị
    container.setPosition(position);
    container.setSize(Vector2f(size.x, max(size.y, (float)window.getSize().y)));
    container.setFillColor(Color(50, 50, 50)); // Màu nền
    container.setOutlineColor(Color::Black);

    // Đường dẫn thanh cuộn
    scrollBarTrack.setSize({ 20, float(window.getSize().y) - 20 });
    scrollBarTrack.setPosition(position.x + size.x - 20, position.y + 10);
    scrollBarTrack.setFillColor(Color(80, 80, 80));

    // Tay cầm thanh cuộn
    float thumbHeight = scrollBarTrack.getSize().y * (float)window.getSize().y / (container.getSize().y);
    scrollBarThumb.setSize({ 20, min(thumbHeight, scrollBarTrack.getSize().y)});
    scrollBarThumb.setPosition(scrollBarTrack.getPosition());
    scrollBarThumb.setFillColor(Color(120, 120, 120));

    // Tạo view giới hạn nội dung trong container
    view.setCenter(Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));
    view.setSize(Vector2f((float)window.getSize().x, (float)window.getSize().y));

    maxScroll = container.getSize().y - window.getSize().y;

    scrollOffset = 0.0f;
    isDragging = false;
    contentHeight = 0.0f; // Ban đầu nội dung trống
    scrollSpeed = (view.getSize().y / 10);
    lastButton = -1;
    //cout << container.getSize().y << '\n';
    //cout << scrollBarTrack.getSize().y << '\n';
    //cout << scrollBarThumb.getSize().y << '\n';
}

// Thêm các phần tử bị kéo
// Bên trong
void ScrollableArea::addButtonItem(unique_ptr<Button>& item) {
    buttonItems.push_back(move(item));
}

void ScrollableArea::addSpriteItem(unique_ptr<Sprite>& item) {
    spriteItems.push_back(move(item));
}

void ScrollableArea::addDropDownButtonItem(unique_ptr<DropDownButton>& item) {
    dropDownButtonItems.push_back(move(item));
}

// Bên ngoài
void ScrollableArea::addExternalButton(Button* item) {
    extButs.push_back(item);
}

void ScrollableArea::addExternalSprite(Sprite* item) {
    extSprites.push_back(item);
}

void ScrollableArea::addExternalDropDownButton(DropDownButton* item) {
    extDropDownButs.push_back(item);
}

void ScrollableArea::addExternalText(Text* item) {
    extTexts.push_back(item);
}

// Thêm các phần tử cố định
// Bên trong
// Bên ngoài
void ScrollableArea::addFixedExternalRectangleShape(RectangleShape* item) {
    fixedExtRectShapes.push_back(item);
}

void ScrollableArea::addFixedExternalButton(Button* item) {
    fixedExtButs.push_back(item);
}

// Xử lý sự kiện
void ScrollableArea::handleEvent(Event& event, RenderWindow& window) {
    if (event.type == Event::MouseMoved) {
        Vector2f mousePos = window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y });

        // Đổi màu nếu chuột đang trên thumb
        if (scrollBarThumb.getGlobalBounds().contains(mousePos)) {
            scrollBarThumb.setFillColor(Color(170, 170, 170)); // Màu xám sáng
        }
        else {
            scrollBarThumb.setFillColor(Color(120, 120, 120)); // Màu mặc định
        }
    }

    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        // Chuyển đổi tọa độ chuột từ pixel sang hệ tọa độ của view
        Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }, view);
        // Kiểm tra xem chuột có nằm trong phạm vi của scrollBarThumb không
        if (scrollBarThumb.getGlobalBounds().contains(mousePos)) {
            isDragging = true; // Bắt đầu kéo thanh cuộn
            lastMousePos = mousePos;
        }
    }
    else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
        isDragging = false; // Dừng kéo
    }
    else if (event.type == Event::MouseMoved && isDragging) {
        // Lấy vị trí chuột trong không gian tọa độ của cửa sổ
        Vector2f curMousePos = window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y }, view);

        float delta = curMousePos.y - lastMousePos.y;
        float totalScroll = delta * (container.getSize().y - window.getSize().y) / (scrollBarTrack.getSize().y - scrollBarThumb.getSize().y);
        if (abs(scrollBarTrack.getSize().y - scrollBarThumb.getSize().y) < 10e-9) totalScroll = 0.f;

        if (totalScroll > 0) { // Lăn xuống
            view.move(0, totalScroll);
        }
        else if (curMousePos.y - lastMousePos.y < 0) { // Lăn lên
            view.move(0, totalScroll);
        }

        // Giới hạn vùng nhìn thấy
        if (view.getCenter().y - view.getSize().y / 2 < 0)
            view.setCenter(view.getCenter().x, view.getSize().y / 2);
        if (view.getCenter().y + view.getSize().y / 2 > container.getSize().y)
            view.setCenter(view.getCenter().x, container.getSize().y - view.getSize().y / 2);

        // Update scroll offset
        scrollOffset += totalScroll;

        //scrollOffset Limited
        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > (container.getSize().y - window.getSize().y)) scrollOffset = container.getSize().y - window.getSize().y;

        // Calculate scroll ratio
        float scrollRatio = scrollOffset / (container.getSize().y - window.getSize().y);
        if (abs(container.getSize().y - window.getSize().y) < 10e-9) scrollRatio = 0.f;
        // Update postion of scroll bar track
        scrollBarTrack.setPosition(scrollBarTrack.getPosition().x, 10 + scrollOffset);

        // Update postion of scroll bar thumb
        float thumbY = scrollBarTrack.getPosition().y + scrollRatio * (scrollBarTrack.getSize().y - scrollBarThumb.getSize().y);
        scrollBarThumb.setPosition(scrollBarThumb.getPosition().x, thumbY);

        lastMousePos = window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y }, view);
    }
    else if (event.type == Event::MouseWheelScrolled) {
        float totalScroll = 0;
        if (event.mouseWheelScroll.delta < 0) { // Lăn xuống
            view.move(0, scrollSpeed);
            totalScroll += scrollSpeed;
        }
        else if (event.mouseWheelScroll.delta > 0) { // Lăn lên
            view.move(0, -scrollSpeed);
            totalScroll -= scrollSpeed;
        }

        // Giới hạn vùng nhìn thấy
        if (view.getCenter().y - view.getSize().y / 2 < 0)
            view.setCenter(view.getCenter().x, view.getSize().y / 2);
        if (view.getCenter().y + view.getSize().y / 2 > container.getSize().y)
            view.setCenter(view.getCenter().x, container.getSize().y - view.getSize().y / 2);

        // Update scroll offset
        scrollOffset += totalScroll;

        //scrollOffset Limited
        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > container.getSize().y - window.getSize().y) scrollOffset = container.getSize().y - window.getSize().y;

        // Calculate scroll ratio
        float scrollRatio = scrollOffset / (container.getSize().y - window.getSize().y);
        if (abs(container.getSize().y - window.getSize().y) < 10e-9) scrollRatio = 0.f;
        // Update postion of scroll bar track
        scrollBarTrack.setPosition(scrollBarTrack.getPosition().x, 10 + scrollOffset);

        // Update postion of scroll bar thumb
        float thumbY = scrollBarTrack.getPosition().y + scrollRatio * (scrollBarTrack.getSize().y - scrollBarThumb.getSize().y);
        scrollBarThumb.setPosition(scrollBarThumb.getPosition().x, thumbY);
    }

    if (event.type == Event::MouseWheelScrolled || event.type == Event::MouseMoved) {
        // Đổi màu nếu chuột đang trên thumb
        Vector2i mousePos = Mouse::getPosition(window);
        
        int but = detectClickedButtonItem(mousePos, window);
        if (but != lastButton && lastButton != -1) buttonItems[lastButton]->getRectangle().setFillColor(Color::White);
        if (but != -1) {
            lastButton = but;
            buttonItems[but]->getRectangle().setFillColor(Color(180, 180, 180));
        }
        else {
            lastButton = -1;
        }
    }
}

// Phát hiện phần tử được nhấn
// Bên trong
int ScrollableArea::detectClickedButtonItem(Vector2i mousePosition, RenderWindow& window) {
    Vector2f worldPos = window.mapPixelToCoords(mousePosition, view);

    for (int i = 0; i < buttonItems.size(); ++i) {

        if (buttonItems[i]->getRectangle().getGlobalBounds().contains(worldPos)) {
            return i; // Trả về chỉ số phần tử được nhấn
        }
    }
    return -1; // Không nhấn vào phần tử nào
}

int ScrollableArea::detectClickedDropDownButtonItem(Vector2i mousePosition, RenderWindow& window) {
    Vector2f worldPos = window.mapPixelToCoords(mousePosition, view);

    for (int i = 0; i < dropDownButtonItems.size(); ++i) {

        if (dropDownButtonItems[i]->contain(worldPos.x, worldPos.y)) {
            return i; // Trả về chỉ số phần tử được nhấn
        }
    }
    return -1; // Không nhấn vào phần tử nào
}

// Bên ngoài
int ScrollableArea::detectClickedExternalButton(Vector2i mousePosition, RenderWindow& window) {
    Vector2f worldPos = window.mapPixelToCoords(mousePosition, view);

    for (int i = 0; i < extButs.size(); ++i) {

        if (buttonItems[i]->getRectangle().getGlobalBounds().contains(worldPos)) {
            return i; // Trả về chỉ số phần tử được nhấn
        }
    }
    return -1; // Không nhấn vào phần tử nào
}

int ScrollableArea::detectClickedExternalDropDownButton(Vector2i mousePosition, RenderWindow& window) {
    Vector2f worldPos = window.mapPixelToCoords(mousePosition, view);

    for (int i = 0; i < extDropDownButs.size(); ++i) {

        if (dropDownButtonItems[i]->contain(worldPos.x, worldPos.y)) {
            return i; // Trả về chỉ số phần tử được nhấn
        }
    }
    return -1; // Không nhấn vào phần tử nào
}

// Vẽ khu vực cuộn
void ScrollableArea::draw(RenderWindow& window) {
    window.clear();

    window.setView(view);
    window.draw(container);

    window.setView(window.getDefaultView());
    // Vẽ các phần tử cố định
    for (RectangleShape* rectShape : fixedExtRectShapes) {
        window.draw(*rectShape);
    }
    for (Button* but : fixedExtButs) {
        but->drawText(window);
    }

    window.setView(view);

    // Vẽ khu vực và thanh cuộn
    
    window.draw(scrollBarTrack);
    window.draw(scrollBarThumb);

    // Vẽ các phần tử
    for (unique_ptr<Button>& but : buttonItems) {
        but->drawText(window);
    }

    for (unique_ptr<Sprite>& sprite : spriteItems) {
        window.draw(*sprite);
    }

    for (unique_ptr<DropDownButton>& but : dropDownButtonItems) {
        but->draw(window);
    }

    for (Button* but : extButs) {
        but->drawText(window);
    }

    for (Sprite* sprite : extSprites) {
        window.draw(*sprite);
    }

    for (DropDownButton* but : extDropDownButs) {
        but->draw(window);
    }

    for (Text* text : extTexts) {
        window.draw(*text);
    }

    for (unique_ptr<DropDownButton>& but : dropDownButtonItems) {
        but->drawOption(window);
    }

    for (DropDownButton* but : extDropDownButs) {
        but->drawOption(window);
    }

    // Khôi phục view gốc
    window.setView(window.getDefaultView());
}