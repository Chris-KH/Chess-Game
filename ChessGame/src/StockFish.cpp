#include "../lib/StockFish.h"

Stockfish::Stockfish() {
    process = nullptr;
}

Stockfish::Stockfish(const string& path) {
    // Start stockfish
    process = _popen(path.c_str(), "w");
    if (!process) {
        throw runtime_error("Unable to start Stockfish.");
    }
    sendCommand("uci");
    waitForReady();

    readThread = thread(&Stockfish::readLoop, this);
}

Stockfish::~Stockfish() {
    //Close stockfish
    quit();
    if (readThread.joinable()) {
        readThread.join(); // Đảm bảo thread được dừng khi hủy đối tượng
    }
}

void Stockfish::sendCommand(const std::string& command) {
    unique_lock<mutex> lock(commandMutex);
    fprintf(process, "%s\n", command.c_str());
    fflush(process);
}

void Stockfish::readLoop() {
    while (running) {
        string response = readResponse();
        if (response.find("bestmove") != string::npos) {
            lock_guard<mutex> lock(commandMutex);
            bestMove = response.substr(response.find("bestmove ") + 9, 5);  // Lấy nước đi tốt nhất
            bestMoveCondition.notify_all();  // Thông báo rằng có nước đi mới
        }
    }
}

void Stockfish::waitForReady() {
    sendCommand("isready");
    string response;
    while ((response = readResponse()).find("readyok") == string::npos) {
        // Đợi cho đến khi nhận được "readyok"
    }
}

// Đọc phản hồi từ Stockfish
string Stockfish::readResponse() {
    char buffer[128];
    string result;
    while (fgets(buffer, sizeof(buffer), process)) {
        result += buffer;
        if (result.find("bestmove") != string::npos || result.find("uciok") != string::npos || result.find("readyok") != string::npos) {
            break;
        }
    }
    return result;
}

// Đọc phân tích từ Stockfish
string Stockfish::readAnalysis() {
    string result;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), process)) {
        result += buffer;
        if (result.find("bestmove") != string::npos) {
            break;
        }
    }
    return result;
}

// Bắt đầu ván cờ mới
void Stockfish::startNewGame() {
    sendCommand("ucinewgame");
    movesHistory.clear();
    waitForReady();
}

// Thiết lập vị trí bàn cờ từ FEN hoặc từ lịch sử các nước đi
void  Stockfish::setPosition(const string& fen) {
    sendCommand("position fen " + fen);
}

void  Stockfish::setPositionFromMoves(const string& initialPosition, const vector<string>& moves) {
    string command = "position " + initialPosition;
    if (!moves.empty()) {
        command += " moves";
        for (const auto& move : moves) {
            command += " " + move;
        }
    }
    sendCommand(command);
    movesHistory = moves;
}

// Lấy nước đi tốt nhất
string  Stockfish::getBestMove(int timeLimit) {
    unique_lock<mutex> lock(commandMutex);
    sendCommand("go movetime " + to_string(timeLimit));
    bestMoveCondition.wait(lock, [this]() { return !bestMove.empty(); });
    string move = bestMove;
    bestMove.clear();
    return move;
}

// Phân tích nước đi với độ sâu hoặc thời gian giới hạn
string  Stockfish::analyzeWithLimits(int depth, int timeLimit) {
    sendCommand("go depth " + to_string(depth) + " movetime " + to_string(timeLimit));
    return readAnalysis();
}

// Đặt mức độ chơi của động cơ (cấp độ từ 0 đến 20)
void  Stockfish::setSkillLevel(int level) {
    if (level < 0 || level > 20) {
        throw invalid_argument("Skill level must be between 0 and 20.");
    }
    sendCommand("setoption name Skill Level value " + to_string(level));
}

// Thiết lập thời gian cho trận đấu
void  Stockfish::setTimeControl(int whiteTime, int blackTime) {
    sendCommand("go wtime " + to_string(whiteTime) + " btime " + to_string(blackTime));
}

// Lùi lại một nước đi
void  Stockfish::undoLastMove() {
    if (!movesHistory.empty()) {
        movesHistory.pop_back();
        setPositionFromMoves("startpos", movesHistory);
    }
}

// Phân tích sau trận đấu
string  Stockfish::postGameAnalysis() {
    setPositionFromMoves("startpos", movesHistory);
    sendCommand("go infinite");  // Phân tích vô thời hạn
    return readAnalysis();
}

// Thiết lập tùy chọn động cơ (Engine Options)
void  Stockfish::setEngineOption(const string& name, const string& value) {
    sendCommand("setoption name " + name + " value " + value);
}

// Thoát Stockfish
void Stockfish::quit() {
    if (process) {
        sendCommand("quit");
        _pclose(process);
        process = nullptr;
        running = false;
        if (readThread.joinable()) {
            readThread.join();  // Đảm bảo thread kết thúc
        }
    }
}
