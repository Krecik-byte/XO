#include "TicTacToe.h"

int main() {

    int size, win_condition;
    std::wstring input;

    while (true) {
        std::wcout << L"Podaj rozmiar planszy (3 lub wiecej): ";
        std::getline(std::wcin, input);
        std::wistringstream iss(input);

        if (!(iss >> size) || size < 3) {
            std::wcout << L"Nieprawidlowy rozmiar. ";
            continue;
        }

        std::wstring remaining;
        if (iss >> remaining) {
            std::wcout << L"Podaj tylko jedna liczbe. ";
            continue;
        }
        break;
    }

    do {
        std::wcout << L"Podaj liczbe znakow w rzedzie do wygranej (2-" << size << L"): ";
        std::getline(std::wcin, input);
        std::wistringstream iss_win(input);

        if (!(iss_win >> win_condition) || win_condition < 2 || win_condition > size) {
            std::wcout << L"Nieprawidlowa liczba. ";
            continue;
        }

        if (win_condition == 2) {
            std::wcout << L"UWAGA: Gra z warunkiem wygranej = 2 bedzie bardzo latwa!\n";
        }
        break;
    } while (true);

    wchar_t human_player, computer_player;
    std::wcout << L"Kto zaczyna? (1 - Gracz, 2 - Komputer): ";
    int choice;
    std::getline(std::wcin, input);
    std::wistringstream iss_choice(input);

    while (!(iss_choice >> choice) || (choice != 1 && choice != 2)) {
        std::wcout << L"Nieprawidlowy wybor. Wybierz 1 (Gracz) lub 2 (Komputer): ";
        std::getline(std::wcin, input);
        iss_choice.str(input);
        iss_choice.clear();
    }

    if (choice == 1) {
        human_player = L'X';
        computer_player = L'O';
    }
    else {
        human_player = L'O';
        computer_player = L'X';
    }

    TicTacToe game(size, win_condition, human_player, computer_player);
    game.play();
    return 0;
}