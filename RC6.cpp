#include <iostream>
#include <iomanip>
#include <cstdint>
#include <windows.h>
#include <cmath>
#include <string>

using namespace std;

// Магические константы
#define P32 0xb7e15163
#define Q32 0x9e3779b9

// Размер слова (в битах)
#define w 32

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
void Encrypt(uint32_t& A, uint32_t& B, uint32_t& C, uint32_t& D, uint32_t* S, int r) {
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
void Decrypt(uint32_t& A, uint32_t& B, uint32_t& C, uint32_t& D, uint32_t* S, int r) {
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

int main() {
    SetConsoleCP(1251);        //для корректного ввода и вывода русских символов в консоли
    SetConsoleOutputCP(1251);
    const int r = 20; // Количество раундов
    wstring plaintext;
    wcout << "Enter the encryption text: ";
    getline(wcin, plaintext);

    // Инициализация ключа
    uint8_t K[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    const int b = sizeof(K);
    uint32_t L[b / (w / 8)];
    int c;
    KeyExpansion(K, b, L, c);

    uint32_t S[2 * r + 4];
    InitializeS(S, r);
    MixKeys(L, c, S, r);

    string decryptedText;
    // Шифрование/дешифрование
    for (wchar_t c : plaintext) {
        uint32_t A = c, B = 0, C = 0, D = 0;
        Encrypt(A, B, C, D, S, r);
        cout << "Encrypted: " << hex << setw(8) << setfill('0') << A << ", 0x" << setw(8) << setfill('0') << B << ", 0x" << setw(8) << setfill('0') << C << ", 0x" << setw(8) << setfill('0') << D << endl;
        Decrypt(A, B, C, D, S, r);
        decryptedText += static_cast<char>(A);
    }

    cout << "Decrypted: " << decryptedText << endl;

    return 0;
}
