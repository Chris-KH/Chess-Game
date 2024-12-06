﻿#include "Button.h"
#include "DropDownButton.h"

class ScrollableArea {
private:
    RectangleShape container;       // Khu vực hiển thị
    RectangleShape scrollBarTrack;  // Đường dẫn thanh cuộn
    RectangleShape scrollBarThumb;  // Tay cầm thanh cuộn

    // Các phần tử bị kéo
    // Các phần tử bên trong
    vector<unique_ptr<Button>> buttonItems;
    vector<unique_ptr<Sprite>> spriteItems;
    vector<unique_ptr<DropDownButton>> dropDownButtonItems;

    // Các phần tử bên ngoài
    vector<Button*> extButs;
    vector<Sprite*> extSprites;
    vector<DropDownButton*> extDropDownButs;
    vector<Text*> extTexts;

    // Các phần tử cố định
    vector<RectangleShape*> fixedExtRectShapes;
    vector<Button*> fixedExtButs;

    float scrollOffset;                   // Độ lệch cuộn
    bool isDragging;                      // Kiểm tra có đang kéo thanh cuộn không

    float contentHeight; // Chiều cao toàn bộ nội dung'

    View view; 

    Vector2f lastMousePos;
    float scrollSpeed = 20.0f; // Tốc độ cuộn
    float maxScroll;
    int lastButton;
public:
    ScrollableArea(Vector2f position, Vector2f size, RenderWindow& window);

    // Thêm các phần tử bị kéo
    // Bên trong
    void addButtonItem(unique_ptr<Button>& item);
    void addSpriteItem(unique_ptr<Sprite>& item);
    void addDropDownButtonItem(unique_ptr<DropDownButton>& item);

    // Bên ngoài
    void addExternalButton(Button* item);
    void addExternalSprite(Sprite* sprite);
    void addExternalDropDownButton(DropDownButton* item);
    void addExternalText(Text* item);

    // Thêm các phần tử cố định
    // Bên trong
    // Bên ngoài
    void addFixedExternalRectangleShape(RectangleShape* item);
    void addFixedExternalButton(Button* item);

    // Xử lí các sự kiện
    void handleEvent(Event& event, RenderWindow& window);

    // Phát hiện phần tử được nhấn
    // Bên trong
    int detectClickedButtonItem(Vector2i mousePosition, RenderWindow& window);
    int detectClickedDropDownButtonItem(Vector2i mousePosition, RenderWindow& window);
    // Bên ngoài
    int detectClickedExternalButton(Vector2i mousePosition, RenderWindow& window);
    int detectClickedExternalDropDownButton(Vector2i mousePosition, RenderWindow& window);

    // Vẽ khu vực cuộn
    void draw(RenderWindow& window);
};