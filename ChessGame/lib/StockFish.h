#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <filesystem>
#include <iostream>

using namespace std;
using namespace sf;

class Stockfish {
private:
    HANDLE hStdinRead, hStdinWrite, hStdoutRead, hStdoutWrite;
    PROCESS_INFORMATION piProcInfo;
    SECURITY_ATTRIBUTES saAttr;

    vector<string> moveHistory;
    vector<string> redoHistory;
public:
    Stockfish();
    ~Stockfish();

    void sendCommand(const string& command);
    string getResponse();
    string calculateBestMove(int timeLimit = 1000);
    string calculateBestMoveWithDepth(int depth, int timeLimit = 1000);
    void setSkillLevel(int level);
    void newGame();
    void setBoardState(const string& fen);
    void setTime(int whiteTime, int blackTime);
    void makeMove(const string& move);
    string getMoveHistory() const;
    void undoMove();
    void redoMove();
    void setHashSize(int size);

};