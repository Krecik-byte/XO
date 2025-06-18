#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <limits>
#include <wchar.h>

class TicTacToe {
private:
    std::vector<std::vector<wchar_t>> board;
    wchar_t human_player;
    wchar_t computer_player;
    int size;
    int win_condition;
    int max_depth;

    void displayBoard();

    void displayColumnHeader(int cellWidth, int labelWidth);
    void displayHorizontalLine(int labelWidth);
    void displayRow(int row, int labelWidth);

    bool isWin(wchar_t player);
    bool checkLine(wchar_t player, int start_row, int start_col, int row_dir, int col_dir);

    bool isDraw();
    bool makeMove(int row, int col, wchar_t player);

    int minimax(int depth, int alpha, int beta, bool maximizing);
    int maximize(int depth, int alpha, int beta);
    int minimize(int depth, int alpha, int beta);

    void computerMove();
    int evaluate();

    int evaluateLine(wchar_t player, int start_row, int start_col, int row_dir, int col_dir);
    int calculateDepth();

public:
    TicTacToe(int size, int win_condition, wchar_t human, wchar_t computer);
    void play();
};

#endif 
