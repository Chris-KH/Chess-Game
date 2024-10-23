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
    //sendCommand("isready");
    waitForReady();
}


Stockfish::~Stockfish() {
    //Close stockfish
    quit();
}

void Stockfish::sendCommand(const string& command) {
    if (process) {
        fprintf(process, "%s\n", command.c_str());
        fflush(process);
    }
}


void Stockfish::waitForReady() {
    sendCommand("isready");
    string response = readResponse();
    int attempts = 0;
    const int maxAttempts = 100;

    while ((response = readResponse()).find("readyok") != std::string::npos) {
        //std::cout << "Waiting for readyok, received: " << response << std::endl;
        attempts++;
        if (attempts >= maxAttempts) {
            throw std::runtime_error("Stockfish did not respond with 'readyok' after multiple attempts.");
        }
    }
    cout << "Received readyok!" << std::endl;
}

string Stockfish::readResponse() {
    string response;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), process)) {
        std::cout << "Stockfish sent: " << buffer;
    }

    return response;
}

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

void Stockfish::startNewGame() {
    sendCommand("ucinewgame");
    movesHistory.clear();
    waitForReady();
}

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


string Stockfish::analyzeWithLimits(int depth, int timeLimit) {
    sendCommand("go depth " + to_string(depth) + " movetime " + to_string(timeLimit));
    return readAnalysis();
}

void  Stockfish::setSkillLevel(int level) {
    if (level < 0 || level > 20) {
        throw invalid_argument("Skill level must be between 0 and 20.");
    }
    sendCommand("setoption name Skill Level value " + to_string(level));
}

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
    }
}
