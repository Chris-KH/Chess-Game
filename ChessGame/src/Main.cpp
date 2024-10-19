#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>

class Stockfish {
public:
    Stockfish(const std::string& path) {
        // Khởi động Stockfish
        process = _popen(path.c_str(), "w");
        if (!process) {
            throw std::runtime_error("Unable to start Stockfish.");
        }
    }

    ~Stockfish() {
        // Đóng quy trình Stockfish
        _pclose(process);
    }

    std::string sendCommand(const std::string& command) {
        // Gửi lệnh đến Stockfish
        fprintf(process, "%s\n", command.c_str());
        fflush(process);

        // Nhận phản hồi từ Stockfish
        char buffer[128];
        std::string result;
        while (fgets(buffer, sizeof(buffer), process)) {
            result += buffer;
            // Dừng khi nhận được lệnh 'uciok'
            if (result.find("uciok") != std::string::npos) {
                break;
            }
        }
        return result;
    }

private:
    FILE* process;
};

int main() {
    // Khởi động Stockfish
    std::string stockfishPath = "stockfish.exe"; // Thay đổi đường dẫn tới stockfish.exe

    try {
        Stockfish stockfish(stockfishPath);
        std::string response = stockfish.sendCommand("uci");

        // Hiển thị phản hồi
        std::cout << "Stockfish response:\n" << response << std::endl;

        // Khởi động cửa sổ SFML
        sf::RenderWindow window(sf::VideoMode(800, 600), "Stockfish Test");
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.display();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
