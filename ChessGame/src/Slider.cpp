#include "../lib/Slider.h"

Slider::Slider(float x, float y, float width, float height, int steps)
    : position(x, y), size(width, height), steps(steps) {
    // Initialize the track
    track.setSize(size);
    track.setFillColor(sf::Color(150, 150, 150));
    track.setPosition(position);

    // Initialize the knob
    knob.setRadius(size.y);
    knob.setFillColor(sf::Color(0, 0, 255));
    knob.setOrigin(knob.getRadius(), knob.getRadius());
    knob.setPosition(position.x, position.y + size.y / 2);
    knob.setOutlineThickness(-1);
    knob.setOutlineColor(Color::Black);

    stepSize = size.x / (steps - 1);
    currentStep = 0; // Start at the first step
}

void Slider::handleEvent(const sf::Event& event, sf::RenderWindow& window, const View& view) {
    Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }, view);
    // Nhấn chuột trái
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        // Knob chứa con chuột
        if (knob.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            // Cập nhật knob đang bị kéo
            isDragging = true;
        }
    }
    // Nhả chuột trái
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            // Cập nhật knob không bị kéo
            isDragging = false;
    }
    // Con knob đang bị kéo
    if (isDragging) {
        // Tính toán tọa độ x mới của knob
        float newX = clamp((float)Mouse::getPosition(window).x, position.x, position.x + size.x);

        // Snap to nearest step
        float relativeX = newX - position.x;
        int newStep = std::round(relativeX / stepSize);
        knob.setPosition(position.x + newStep * stepSize, knob.getPosition().y);
        // Change color
        if(changeKnobColor) 
            knob.setFillColor(Color(idUp[newStep], 0, idDown[newStep]));

        currentStep = newStep;
    }
    draw(window);
}

void Slider::draw(sf::RenderWindow& window) {
    window.draw(track);
    window.draw(knob);
}

void Slider::setChangeKnobColor(bool change) {
    changeKnobColor = change;
    idUp.resize(steps);
    idDown.resize(steps);
    for (int i = 0; i < steps; i++)
        idUp[i] = 255 * i / (steps - 1),
        idDown[i] = 255 - 255 * i / (steps - 1);
}

void Slider::setCurrentStep(int x) {
    currentStep = x;
    knob.setPosition(position.x + x * stepSize, knob.getPosition().y);
    if (changeKnobColor)
        knob.setFillColor(Color(idUp[x], 0, idDown[x]));
}

int Slider::getCurrentStep() const {
    return currentStep;
}

Color Slider::getKnobFillColor(void) const {
    return knob.getFillColor();
}