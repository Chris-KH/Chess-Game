#include "../lib/StockFish.h"

Stockfish::Stockfish() {
    process = nullptr;
}

Stockfish::Stockfish(const string& path) {
    // Start stockfish
    process = _popen(path.c_str(), "w");
    if (!process) {
        throw std::runtime_error("Unable to start Stockfish.");
    }
}

Stockfish::~Stockfish() {
    //Close stockfish
    _pclose(process);
}

string Stockfish::sendCommand(const string& command) {
    if (fprintf(process, "%s\n", command.c_str()) < 0) {
        throw runtime_error("Failed to send command to Stockfish.");
    }
    fflush(process);

    // Get respond from stockfish
    string result;
    char buffer[128];

    while (fgets(buffer, sizeof(buffer), process)) {
        result += buffer;
        // Stop when get 'uciok' or 'bestmove'
        if (result.find("uciok") != string::npos || result.find("bestmove") != string::npos) {
            break;
        }
    }

    return result;
}

void Stockfish::setPosition(const std::string& fen) {
    sendCommand("position " + fen);
}

string Stockfish::calculateBestMove(int timeLimit) {
    sendCommand("go movetime " + std::to_string(timeLimit));
    return sendCommand("stop"); 
}

void Stockfish::setSkillLevel(int level) {
    if (level < 0 || level > 20) {
        throw std::invalid_argument("Skill level must be between 0 and 20.");
    }
    sendCommand("setoption name Skill Level value " + to_string(level));
}