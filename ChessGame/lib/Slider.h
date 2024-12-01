#pragma once
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class Slider {
public:
    Slider(float x, float y, float width, float height, int steps); // Khởi tạo thanh slider với vị trí (x, y), kích thước (width, height), và số lượng bước đi steps
    void handleEvent(const Event& event, RenderWindow& window); // Xử lí các sự kiện
    void draw(RenderWindow& window); // Vẽ track và knob
    
    void setChangeKnobColor(bool change);
    int getCurrentStep() const;
    Color getKnobFillColor(void) const;

private:
    Vector2f position; // Vị trí thanh slider
    Vector2f size; // Kích thước thanh slider
    int steps; // Số "bước đi"
    int currentStep; // "Bước đi hiện tại"
    float stepSize; // Khoảng cách mỗi lần nhảy
    RectangleShape track; // Thanh track của slider
    CircleShape knob; // Cục knob cho biết bước đi hiện tại
    bool isDragging = false; // Cho biết người dùng có đang drag cục knob không
    bool changeKnobColor = false;
    vector<int> idUp, idDown;
};