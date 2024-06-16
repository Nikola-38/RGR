#include "RC6r.h"
#include "SenSira.h"
#include "Backon.h"
#include <iostream>
#include <cstdint>
#include <Windows.h>
#include <string>



using namespace std;
const char* SYSTEM_CLEAR = "cls";


int main() {
    SetConsoleCP(1251);        //для корректного ввода и вывода русских символов в консоли
    SetConsoleOutputCP(1251);

    char pick;
    bool exit = true;
    string passenter = "Hello", pass1 = "enc", pass2 = "dec", passcheck;

    cout << "Здравствуй, дорогой пользователь!" << endl;
    cout << "Авторизация: ";
    cin >> passcheck;
    while (passenter != passcheck) {
        cout << "Неверный пароль для авторизации" << endl;
        cout << "Авторизация ввода пароля:";
        cin >> passcheck;
    }
    system("cls");

    while (exit) {
        try {
            cout <<"  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _" << endl
                << " | Выбрав шифр, введите число:                   |" << endl
                << " | <1> Cipher RC6                                |" << endl
                << " | <2> Bacon's Cipher                            |" << endl
                << " | <3> The Cipher Of The Sensor Line             |" << endl
                << " | <4> Exit                                      |" << endl
                << " |_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|" << endl;
            cin >> pick;
            if (cin.peek() != '\n') {
                throw runtime_error("Вы ввели строку!");
            }
            if (pick != '1' && pick != '2' && pick != '3' && pick != '4') {
                throw runtime_error("Вы ввели неправильную команду!");
            }
            switch (pick) {
            case '1': {
                system(SYSTEM_CLEAR);
                RC6(pass1, pass2); //RC6
                break;
            } case '2': {
                system(SYSTEM_CLEAR);
                Bacon(pass1, pass2); //Бэкон
                break;
            } case '3': {
                system(SYSTEM_CLEAR);
                SenSira(pass1, pass2); //Линейки Сен Сира
                break;
            } case '4': {
                cout << "До свидания!!!";
                exit = false;
            }
            }
        }
        catch (const runtime_error& ex) {
            cerr << ex.what() << "Введите только одну цифру (1, 2 или 3)" << endl;
            cin.clear();
            cin.ignore(256, '\n');
        }
    }
}




