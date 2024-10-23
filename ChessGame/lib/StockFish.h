#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

using namespace std;
using namespace sf;

class Stockfish {
public:
    Stockfish();
    Stockfish(const string& path);

    ~Stockfish();
    
    void startNewGame();

    // Thiết lập vị trí bàn cờ từ FEN hoặc từ lịch sử các nước đi
    void setPosition(const string& fen);

    void setPositionFromMoves(const string& initialPosition, const vector<string>& moves);

    // Lấy nước đi tốt nhất
    string getBestMove(int timeLimit = 1000);

    // Lấy danh sách các nước đi hợp lệ (Stockfish không trực tiếp hỗ trợ, nên dùng cách khác)
    vector<string> getLegalMoves(const std::string& fen);

    // Phân tích nước đi với độ sâu hoặc thời gian giới hạn
    string analyzeWithLimits(int depth = 20, int timeLimit = 1000);

    // Đặt mức độ chơi của động cơ (cấp độ từ 0 đến 20)
    void setSkillLevel(int level);

    // Thiết lập thời gian cho trận đấu
    void setTimeControl(int whiteTime, int blackTime);

    // Lùi lại một nước đi
    void undoLastMove();

    // Phân tích sau trận đấu
    string postGameAnalysis();

    // Thiết lập tùy chọn động cơ (Engine Options)
    void setEngineOption(const string& name, const string& value);

    // Thoát Stockfish
    void quit();

private:
    FILE* process = nullptr;
    vector<std::string> movesHistory; // Lưu lịch sử các nước đi
    thread readThread;  // Thread để đọc phản hồi của Stockfish
    mutex commandMutex; // Để bảo vệ các lệnh được gửi tới Stockfish
    condition_variable bestMoveCondition; // Để chờ nước đi tốt nhất
    string bestMove;
    bool running = true;

    void sendCommand(const std::string& command);
    void readLoop();

    void waitForReady();

    // Đọc phản hồi từ Stockfish
    string readResponse();

    // Đọc phân tích từ Stockfish
    string readAnalysis();
};