#include "TicTacToe.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

TicTacToe::TicTacToe(int size, int win_condition, wchar_t human, wchar_t computer)
    : board(size, std::vector<wchar_t>(size, L' ')),
    human_player(human),
    computer_player(computer),
    size(size),
    win_condition(win_condition) {
    max_depth = calculateDepth();
}

int TicTacToe::calculateDepth() {
    if (size <= 3) return 9;
    else if (size <= 5) return 6;
    else return 4;
}

void TicTacToe::displayColumnHeader(int width, int cellWidth) {
    std::wcout << std::setw(width + 1) << L" ";
    for (int j = 0; j < size; ++j) {
        std::wcout << std::setw(cellWidth) << j + 1 << L" ";
    }
    std::wcout << L"\n";
}

void TicTacToe::displayHorizontalLine(int width) {
    std::wcout << std::setw(width + 1) << L" ";
    for (int j = 0; j < size; ++j) {
        std::wcout << L"+---";
    }
    std::wcout << L"+\n";
}

void TicTacToe::displayBoard() {
    int width = std::to_wstring(size).length();
    int cellWidth = 3;
    displayColumnHeader(width, cellWidth);

    for (int i = 0; i < size; ++i) {
        displayHorizontalLine(width);
        std::wcout << std::setw(width) << i + 1 << L" ";
        for (int j = 0; j < size; ++j) {
            wchar_t cell = board[i][j];
            std::wcout << L"| " << cell << L" ";
        }
        std::wcout << L"|\n";
    }
    displayHorizontalLine(width);
}

bool TicTacToe::isWin(wchar_t player) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j <= size - win_condition; ++j) {
            if (checkLine(player, i, j, 0, 1)) return true;
            if (checkLine(player, j, i, 1, 0)) return true;
        }
    }
    for (int i = 0; i <= size - win_condition; ++i) {
        for (int j = 0; j <= size - win_condition; ++j) {
            if (checkLine(player, i, j, 1, 1)) return true;
            if (checkLine(player, i, j + win_condition - 1, 1, -1)) return true;
        }
    }
    return false;
}

bool TicTacToe::checkLine(wchar_t player, int start_row, int start_col, int row_dir, int col_dir) {
    for (int k = 0; k < win_condition; ++k) {
        if (board[start_row + k * row_dir][start_col + k * col_dir] != player) {
            return false;
        }
    }
    return true;
}

bool TicTacToe::isDraw() {
    for (const auto& row : board) {
        if (std::any_of(row.begin(), row.end(), [](wchar_t cell) { return cell == L' '; })) {
            return false;
        }
    }
    return true;
}

bool TicTacToe::makeMove(int row, int col, wchar_t player) {
    --row; --col;

    if (row < 0 || row >= size || col < 0 || col >= size || board[row][col] != L' ') {
        return false;
    }

    board[row][col] = player;
    return true;
}

int TicTacToe::evaluate() {
    int score = 0;
    int i = 0;

    while (i < size) {
        int j = 0;
        while (j <= size - win_condition) {
            score += evaluateLine(computer_player, i, j, 0, 1);
            score -= evaluateLine(human_player, i, j, 0, 1);
            score += evaluateLine(computer_player, j, i, 1, 0);
            score -= evaluateLine(human_player, j, i, 1, 0);
            ++j;
        }
        ++i;
    }

    i = 0;
    while (i <= size - win_condition) {
        int j = 0;
        while (j <= size - win_condition) {
            score += evaluateLine(computer_player, i, j, 1, 1);
            score -= evaluateLine(human_player, i, j, 1, 1);
            score += evaluateLine(computer_player, i, j + win_condition - 1, 1, -1);
            score -= evaluateLine(human_player, i, j + win_condition - 1, 1, -1);
            ++j;
        }
        ++i;
    }

    return score;
}


int TicTacToe::evaluateLine(wchar_t player, int start_row, int start_col, int row_dir, int col_dir) {
    int player_count = 0;
    int opponent_count = 0;
    wchar_t opponent = (player == computer_player) ? human_player : computer_player;

    for (int k = 0; k < win_condition; ++k) {
        wchar_t cell = board[start_row + k * row_dir][start_col + k * col_dir];
        if (cell == player) player_count++;
        else if (cell == opponent) opponent_count++;
    }

    if (player_count > 0 && opponent_count == 0) return player_count;
    else if (opponent_count > 0 && player_count == 0) return -opponent_count;
    else return 0;
}

int TicTacToe::minimax(int depth, int alpha, int beta, bool maximizing) {
    if (isWin(computer_player)) return 10 - depth;
    if (isWin(human_player)) return depth - 10;
    if (isDraw()) return 0;
    if (depth >= max_depth) return evaluate();

    if (maximizing) return maximize(depth, alpha, beta);
    else return minimize(depth, alpha, beta);
}

int TicTacToe::maximize(int depth, int alpha, int beta) {
    int best = std::numeric_limits<int>::min();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (board[i][j] == L' ') {
                board[i][j] = computer_player;
                best = std::max(best, minimax(depth + 1, alpha, beta, false));
                board[i][j] = L' ';
                alpha = std::max(alpha, best);
                if (beta <= alpha) return best;
            }
        }
    }
    return best;
}

int TicTacToe::minimize(int depth, int alpha, int beta) {
    int best = std::numeric_limits<int>::max();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (board[i][j] == L' ') {
                board[i][j] = human_player;
                best = std::min(best, minimax(depth + 1, alpha, beta, true));
                board[i][j] = L' ';
                beta = std::min(beta, best);
                if (beta <= alpha) return best;
            }
        }
    }
    return best;
}

void TicTacToe::computerMove() {
    int best_score = std::numeric_limits<int>::min();
    int move_row = -1;
    int move_col = -1;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == L' ') {
                board[i][j] = computer_player;
                int score = minimax(0, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
                board[i][j] = L' ';

                if (score > best_score) {
                    best_score = score;
                    move_row = i;
                    move_col = j;
                }
            }
        }
    }

    if (move_row != -1 && move_col != -1) {
        makeMove(move_row + 1, move_col + 1, computer_player);
    }
}

void TicTacToe::play() {
    std::wstring input;
    bool human_turn = (human_player == L'X');

    while (true) {
        displayBoard();
        if (human_turn) {
            int row, col;
            std::wcout << L"Gracz " << human_player << L", podaj ruch (wiersz i kolumna 1-" << size << L"): ";
            std::getline(std::wcin, input);
            std::wistringstream iss(input);
            if (!(iss >> row >> col)) {
                std::wcout << L"Nieprawidlowy format. Podaj DWIE liczby oddzielone spacja." << std::endl;
                continue;
            }
            std::wstring remaining;
            if (iss >> remaining) {
                std::wcout << L"Za duzo liczb. Podaj tylko wiersz i kolumne." << std::endl;
                continue;
            }
            if (!makeMove(row, col, human_player)) {
                std::wcout << L"Nieprawidlowy ruch. Podaj liczby od 1 do " << size << L"." << std::endl;
                continue;
            }
        }
        else {
            std::wcout << L"Komputer wykonuje ruch..." << std::endl;
            computerMove();
        }

        if (isWin(human_player)) {
            displayBoard();
            std::wcout << L"Gracz " << human_player << L" wygrywa!" << std::endl;
            break;
        }
        else if (isWin(computer_player)) {
            displayBoard();
            std::wcout << L"Komputer (" << computer_player << L") wygrywa!" << std::endl;
            break;
        }
        else if (isDraw()) {
            displayBoard();
            std::wcout << L"Remis!" << std::endl;
            break;
        }

        human_turn = !human_turn;
    }
}
