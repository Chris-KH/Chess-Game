#include <SFML/Graphics.hpp>
#include <iostream>
#include "../lib/ScrollableArea.h"

using namespace std;

int main() {
    RenderWindow window(sf::VideoMode(800, 600), "Click Detection with Scrolling");
    window.setFramerateLimit(60);

    ScrollableArea scroll(Vector2f(0, 0), Vector2f(800, 1200), window);

    // Thêm một số hình chữ nhật nhỏ để minh họa
    string tmp = "Button ";
    for (int i = 0; i < 10; i++) {
        Button item;
        item.setTextButton(tmp + to_string(i + 1), tmp + to_string(i + 1), "../assets/fonts/Vogue.ttf", 600, 100, 100, i * 110 + 10);
        scroll.addItem(item);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            scroll.handleEvent(event, window);
        }



        // Vẽ nội dung

        // Reset View để vẽ UI cố định (nếu cần)
        scroll.draw(window);

        window.display();
    }

    return 0;
}

/*
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Click Detection with Scrolling");
    window.setFramerateLimit(60);

    // Tạo nội dung lớn hơn cửa sổ
    sf::RectangleShape content(sf::Vector2f(800, 1200));
    content.setFillColor(sf::Color(220, 220, 220));
    content.setOutlineColor(sf::Color::Black);
    content.setOutlineThickness(-2);

    // Thêm một số hình chữ nhật nhỏ để minh họa
    std::vector<sf::RectangleShape> items;
    for (int i = 0; i < 20; i++) {
        sf::RectangleShape item(sf::Vector2f(700, 50));
        item.setFillColor(sf::Color(100 + i * 5, 150, 200));
        item.setPosition(50, 20 + i * 60); // Cách nhau 60 pixel
        items.push_back(item);
    }

    // Tạo View (vùng nhìn thấy)
    sf::View view(sf::FloatRect(0, 0, 800, 600));

    float scrollSpeed = 20.0f; // Tốc độ cuộn
    float maxScroll = content.getSize().y - window.getSize().y; // Phạm vi cuộn

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Xử lý lăn chuột để cuộn
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta < 0) { // Lăn xuống
                    view.move(0, scrollSpeed);
                } else if (event.mouseWheelScroll.delta > 0) { // Lăn lên
                    view.move(0, -scrollSpeed);
                }

                // Giới hạn vùng nhìn thấy
                if (view.getCenter().y - view.getSize().y / 2 < 0)
                    view.setCenter(view.getCenter().x, view.getSize().y / 2);
                if (view.getCenter().y + view.getSize().y / 2 > content.getSize().y)
                    view.setCenter(view.getCenter().x, content.getSize().y - view.getSize().y / 2);
            }

            // Xử lý nhấp chuột
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // Chuyển đổi tọa độ chuột sang hệ tọa độ thế giới
                sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), view);

                // Kiểm tra xem chuột nhấp vào hình chữ nhật nào
                for (size_t i = 0; i < items.size(); i++) {
                    if (items[i].getGlobalBounds().contains(worldPos)) {
                        std::cout << "Clicked on item " << i << "\n";
                        items[i].setFillColor(sf::Color::Red); // Đổi màu đối tượng được nhấp
                        break;
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        // Áp dụng View
        window.setView(view);

        // Vẽ nội dung
        window.draw(content);
        for (const auto& item : items)
            window.draw(item);

        // Reset View để vẽ UI cố định (nếu cần)
        window.setView(window.getDefaultView());

        window.display();
    }

    return 0;
}
*/