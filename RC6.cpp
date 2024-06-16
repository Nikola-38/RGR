#include <iostream>
#include <cstdint>
#include <Windows.h>
#include <cmath>
#include <iomanip>
#include <sstream>  
#include <random>
#include <fstream>
#include <string>
using namespace std;

// Магические константы
#define P32 0xb7e15163
#define Q32 0x9e3779b9

// Размер слова (в битах)
#define w 32

void writeToFile(const string& filename, const string& text) {
    ofstream file(filename);
    if (file.is_open()) {
        file << text;
        file.close();
    }
    else {
        throw logic_error("Невозможно записать данные в файл: " + filename);
    }
}

string readFromFile(const string& filename) {
    ifstream file(filename);
    string content;
    if (file.is_open()) {
        string line;
        while (getline(file, line))
        {
            content += line + "\n";
        }
        file.close();
        return content;
    }
    else {
        throw logic_error("Невозможно прочитать файл: " + filename);
    }
}

// Функция для преобразования строки в массив uint8_t
void StringToKey(const std::string& key, uint8_t* K, int keySize) {
    for (int i = 0; i < keySize; i++) {
        K[i] = i < key.size() ? key[i] : 0;
    }
}

// Функция для округления до ближайшего нечетного целого
uint32_t Odd(double x) {
    return (uint32_t)ceil(x) | 1;
}

// Этап 1: Конвертация секретного ключа
void KeyExpansion(uint8_t* K, int b, uint32_t* L, int& c) {
    c = (b + (w / 8 - 1)) / (w / 8);
    for (int i = b - 1; i >= 0; i--) {
        L[i / (w / 8)] = (L[i / (w / 8)] << 8) + K[i];
    }
    if (b % (w / 8) != 0) {
        for (int i = b; i < c * (w / 8); i++) {
            L[i / (w / 8)] <<= 8;
        }
    }
}

// Этап 2: Инициализация массива ключей
void InitializeS(uint32_t* S, int r) {
    S[0] = P32;
    for (int i = 1; i <= 2 * r + 3; i++) {
        S[i] = S[i - 1] + Q32;
    }
}

// Этап 3: Перемешивание
void MixKeys(uint32_t* L, int c, uint32_t* S, int r) {
    uint32_t A = 0, B = 0, i = 0, j = 0;
    int v = 3 * max(c, 2 * r + 4);
    for (int s = 1; s <= v; s++) {
        A = S[i] = (S[i] + A + B) << 3;
        B = L[j] = (L[j] + A + B) << (A + B);
        i = (i + 1) % (2 * r + 4);
        j = (j + 1) % c;
    }
}

// Шифрование
void Encrypt_RC6(uint32_t& A, uint32_t& B, uint32_t& C, uint32_t& D, uint32_t* S, int r) {
    B = B + S[0];
    D = D + S[1];
    for (int i = 1; i <= r; i++) {
        uint32_t t = (B * (2 * B + 1)) << (int)log2(w);
        uint32_t u = (D * (2 * D + 1)) << (int)log2(w);
        A = ((A ^ t) << u) + S[2 * i];
        C = ((C ^ u) << t) + S[2 * i + 1];
        (A, B, C, D) = (B, C, D, A);
    }
    A = A + S[2 * r + 2];
    C = C + S[2 * r + 3];
}

// Дешифрование
void Decrypt_RC6(uint32_t& A, uint32_t& B, uint32_t& C, uint32_t& D, uint32_t* S, int r) {
    C = C - S[2 * r + 3];
    A = A - S[2 * r + 2];
    for (int i = r; i >= 1; i--) {
        (A, B, C, D) = (D, A, B, C);
        uint32_t u = (D * (2 * D + 1)) << (int)log2(w);
        uint32_t t = (B * (2 * B + 1)) << (int)log2(w);
        C = ((C - S[2 * i + 1]) >> t) ^ u;
        A = ((A - S[2 * i]) >> u) ^ t;
    }
    D = D - S[1];
    B = B - S[0];
}


void RC6(const string& password1, const string& password2) {
    const int r = 20; // Количество раундов
    int pick, pickencode;
    string pass1, pass2, message, filename;
    uint8_t Key[16];
    vector<vector<uint32_t>> encoded;

    while (true) {
        try {
            cout << "------RC6------" << endl
                << "Select an action:" << endl
                << "<1> Encrypt" << endl
                << "<2> Decrypt" << endl
                << "<3> Exit to the main menu" << endl;
            cin >> pick;
            if (cin.fail() || cin.peek() != '\n') {
                throw logic_error("Input error!");
            }
            if (pick != 1 && pick != 2 && pick != 3) {
                throw logic_error("You entered the wrong command!");
            }

            if (pick == 3) {
                break; // Выход в главное меню
            }

            cout << "Enter the encoding password: ";
            cin >> pass1;
            if (pass1 != password1) {
                throw logic_error("Incorrect password!");
            }

            // Инициализация ключа, МИНИМАЛЬНАЯ ДЛИНА ПРИ ВВОДЕ 5 СИМВОЛОВ
            string key;
            cout << "Введите ключ: ";
            getline(cin, key);
            uint8_t K[16]; // Размер ключа 128 бит
            StringToKey(key, K, sizeof(K));

            cout << "Input action:" << endl
                << "<1> Enter text from the console" << endl
                << "<2> Read text from a file" << endl;
            cin >> pickencode;
            if (cin.fail() || cin.peek() != '\n') {
                throw logic_error("Input error!");
            }

            if (pickencode == 1) {
                cout << "Enter the text: ";
                cin.ignore(); // Очистка буфера ввода
                getline(cin, message);
            }
            else if (pickencode == 2) {
                cout << "Enter the file name: ";
                cin >> filename;
                ifstream file(filename);
                if (!file.is_open()) {
                    throw runtime_error("Could not open file!");
                }
                getline(file, message, static_cast<char>(EOF));
                file.close();
            }
            else {
                throw logic_error("You entered the wrong command!");
            }

            const int b = sizeof(Key);
            uint32_t L[b / (w / 8)];
            int c;
            KeyExpansion(Key, b, L, c);
            uint32_t S[2 * r + 4];
            InitializeS(S, r);
            MixKeys(L, c, S, r);

            // Выполнение операции
            if (pick == 1) { // Шифрование
                for (char c : message) {
                    uint32_t A = c, B = 0, C = 0, D = 0;
                    Encrypt_RC6(A, B, C, D, S, r);
                    encoded.push_back({ A, B, C, D });
                }
                cout << "Encrypted:" << endl;
                // Запись зашифрованного текста в файл
                ofstream encryptedFile("encrypted_rc6.txt");
                if (encryptedFile.is_open()) {
                    for (const auto& block : encoded) {
                        encryptedFile << hex << setw(8) << setfill('0') << block[0] << " ";
                    }
                    encryptedFile.close();
                    cout << "The encrypted text has been written to encrypted_rc6.txt" << endl;
                }
                else {
                    throw runtime_error("Failed to open file: encrypted_rc6.txt");
                }
            }
            else if (pick == 2) { // Дешифрование
      // Чтение зашифрованных данных из файла
                ifstream encryptedFile("encrypted_rc6.txt");
                if (!encryptedFile.is_open()) {
                    throw runtime_error("Could not open file: encrypted_rc6.txt");
                }

                string line;
                while (getline(encryptedFile, line)) {
                    stringstream ss(line);
                    string hexBlock;
                    while (ss >> hexBlock) {
                        uint32_t A;
                        stringstream hexConverter;
                        hexConverter << hex << hexBlock;
                        hexConverter >> A;
                        encoded.push_back({ A, 0, 0, 0 }); // Предполагаем, что B, C, D не используются
                    }
                }
                encryptedFile.close();

                // Дешифрование и вывод в файл
                ofstream decryptedFile("decrypted_rc6.txt");
                if (decryptedFile.is_open()) {
                    for (const auto& block : encoded) {
                        uint32_t A = block[0], B = block[1], C = block[2], D = block[3];
                        Decrypt_RC6(A, B, C, D, S, r);
                        decryptedFile << static_cast<char>(A);
                    }
                    decryptedFile.close();
                    cout << "The decrypted text has been written to decrypted_rc6.txt" << endl;
                }
                else {
                    throw runtime_error("Failed to open file: decrypted_rc6.txt");
                }
            }
        }
        catch (const exception& e) {
            cerr << e.what() << endl;
            system("pause");
        }
    }
}
