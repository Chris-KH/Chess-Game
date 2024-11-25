#include "../lib/StockFish.h"

Stockfish::Stockfish() {
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Tạo pipe cho stdin và stdout
    if (!CreatePipe(&hStdinRead, &hStdinWrite, &saAttr, 0)) {
        cerr << "CreatePipe for stdin failed\n";
        throw runtime_error("Failed to create stdin pipe");
    }

    if (!CreatePipe(&hStdoutRead, &hStdoutWrite, &saAttr, 0)) {
        cerr << "CreatePipe for stdout failed\n";
        throw runtime_error("Failed to create stdout pipe");
    }

    // Đảm bảo không kế thừa đầu đọc của stdout và đầu ghi của stdin
    SetHandleInformation(hStdoutRead, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(hStdinWrite, HANDLE_FLAG_INHERIT, 0);

    // Cấu hình tiến trình con (Stockfish)
    STARTUPINFO siStartInfo;
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = hStdoutWrite;
    siStartInfo.hStdOutput = hStdoutWrite;
    siStartInfo.hStdInput = hStdinRead;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Tạo một chuỗi char[] để lưu tên chương trình
    wchar_t program[] = L"stockfish.exe";

    // Chạy Stockfish
    if (!CreateProcess(NULL, program, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo)) {
        cerr << "CreateProcess failed\n";
        throw runtime_error("Failed to create Stockfish process");
    }

    // Đóng các handle không cần thiết
    CloseHandle(hStdinRead);
    CloseHandle(hStdoutWrite);

    // Gửi lệnh `uci` để khởi tạo giao tiếp UCI
    sendCommand("uci\n");
    string response = getResponse();
    if (response.find("uciok") == string::npos) {
        throw runtime_error("Failed to initialize UCI communication with Stockfish");
    }
}


Stockfish::~Stockfish() {
    CloseHandle(hStdinWrite);
    CloseHandle(hStdoutRead);
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
}

void Stockfish::sendCommand(const string& command) {
    DWORD dwWritten;
    WriteFile(hStdinWrite, command.c_str(), command.length(), &dwWritten, NULL);
}

string Stockfish::getResponse() {
    char buffer[256];
    DWORD dwRead;
    string response;

    while (true) {
        if (ReadFile(hStdoutRead, buffer, sizeof(buffer) - 1, &dwRead, NULL) && dwRead > 0) {
            buffer[dwRead] = '\0';  // Kết thúc chuỗi
            response += buffer;     // Nối phản hồi vào chuỗi response
            // Dừng khi nhận được các tín hiệu kết thúc
            if (response.find("uciok") != string::npos ||
                response.find("readyok") != string::npos ||
                response.find("bestmove") != string::npos) {
                break;
            }
        }
    }
    //cout << response << '\n';
    return response;
}

string Stockfish::calculateBestMove(int timeLimit) {
    sendCommand("go movetime " + to_string(timeLimit) + "\n");
    string bestMove = getResponse();
    bestMove = bestMove.substr(bestMove.find("bestmove") + 9);
    return bestMove;
}

string Stockfish::calculateBestMoveWithDepth(int depth, int timeLimit) {
    sendCommand("go depth " + to_string(depth) + " movetime " + to_string(timeLimit) + "\n");
    string bestMove = getResponse();
    bestMove = bestMove.substr(bestMove.find("bestmove") + 9);
    return bestMove;
}

void Stockfish::setSkillLevel(int level) {
    if (level > 20) level = 20;
    if (level < 0) level = 0;
    sendCommand("setoption name Skill Level value " + to_string(level) + "\n");
}

void Stockfish::newGame() {
    sendCommand("ucinewgame\n");
    moveHistory.clear(); 
}

void Stockfish::setBoardState(const string& fen) {
    sendCommand(fen + "\n");
}

void Stockfish::setTime(int whiteTime, int blackTime) {
    sendCommand("go wtime " + to_string(whiteTime) + " btime " + to_string(blackTime) + "\n");
}

void Stockfish::makeMove(const string& move) {
    moveHistory.push_back(move);  // Thêm nước đi vào lịch sử
    sendCommand("position startpos moves " + getMoveHistory() + "\n");
}

string Stockfish::getMoveHistory() const {
    string history;
    for (const string& move : moveHistory) {
        history += move + " ";
    }
    return history;
}

void Stockfish::undoMove() {
    if (!moveHistory.empty()) {
        redoHistory.push_back(moveHistory.back());
        moveHistory.pop_back();  
    }
    sendCommand("position startpos moves " + getMoveHistory() + "\n");
}

void Stockfish::redoMove() {
    if (!redoHistory.empty()) {
        moveHistory.push_back(redoHistory.back());
        redoHistory.pop_back();
    }
    sendCommand("position startpos moves " + getMoveHistory() + "\n");
}

void Stockfish::setHashSize(int size) {
    sendCommand("setoption name Hash value " + to_string(size) + "\n");
}