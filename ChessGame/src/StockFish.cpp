#include "../lib/StockFish.h"

Stockfish::Stockfish(const string& path) {
    // Khởi động Stockfish
    process = _popen(path.c_str(), "w");
    if (!process) {
        throw std::runtime_error("Unable to start Stockfish.");
    }
}

Stockfish::~Stockfish() {
    // Đóng quy trình Stockfish
    _pclose(process);
}

string Stockfish::sendCommand(const string& command) {
    // Gửi lệnh đến Stockfish
    fprintf(process, "%s\n", command.c_str());
    fflush(process);

    // Nhận phản hồi từ Stockfish
    char buffer[128];
    string result;
    while (fgets(buffer, sizeof(buffer), process)) {
        result += buffer;
        // Dừng khi nhận được lệnh 'uciok'
        if (result.find("uciok") != string::npos) {
            break;
        }
    }
    return result;
}