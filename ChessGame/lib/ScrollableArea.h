#include "Button.h"

class ScrollableArea {
private:
    RectangleShape container;       // Khu vực hiển thị
    RectangleShape scrollBarTrack;  // Đường dẫn thanh cuộn
    RectangleShape scrollBarThumb;  // Tay cầm thanh cuộn

    vector<unique_ptr<Button>> items; // Các phần tử bên trong
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

    void addItem(unique_ptr<Button>& item);
    void handleEvent(Event& event, RenderWindow& window);

    // Phát hiện phần tử được nhấn
    int detectClickedItem(Vector2i mousePosition, RenderWindow& window);

    // Vẽ khu vực cuộn
    void draw(RenderWindow& window);
};