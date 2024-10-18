#include <SFML/Graphics.hpp>

int main() {
    const int boardSize = 8; // Kích thước bàn cờ (8x8)
    const int cellSize = 80;  // Kích thước mỗi ô (80x80 pixel)

    // Tạo cửa sổ
    sf::RenderWindow window(sf::VideoMode(boardSize * cellSize, boardSize * cellSize), "ChessBoard");

    // Vòng lặp chính
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(); // Xóa màn hình

        // Vẽ bàn cờ
        for (int row = 0; row < boardSize; ++row) {
            for (int col = 0; col < boardSize; ++col) {
                sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));

                // Đặt màu cho ô
                if ((row + col) % 2 == 0) {
                    cell.setFillColor(sf::Color::White); // Ô trắng
                }
                else {
                    cell.setFillColor(sf::Color::Black); // Ô đen
                }

                // Đặt vị trí cho ô
                cell.setPosition(col * cellSize, row * cellSize);
                window.draw(cell); // Vẽ ô
            }
        }

        window.display(); // Hiển thị cửa sổ
    }

    return 0;
}
