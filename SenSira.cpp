#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

string encrypt(string plaintext, string key) {
    string ciphertext = "";
    int key_length = key.length();
    
    for (int i = 0; i < plaintext.length(); i++) {
        char current_char = plaintext[i];
        char key_char = key[i % key_length];
        int shifted_char = (static_cast<int>(current_char) + static_cast<int>(key_char)) % 256;
        ciphertext += static_cast<char>(shifted_char);
    }
    
    return ciphertext;
}

string decrypt(string ciphertext, string key) {
    string plaintext = "";
    int key_length = key.length();
    
    for (int i = 0; i < ciphertext.length(); i++) {
        char current_char = ciphertext[i];
        char key_char = key[i % key_length];
        int shifted_char = (static_cast<int>(current_char) - static_cast<int>(key_char) + 256) % 256;
        plaintext += static_cast<char>(shifted_char);
    }
    
    return plaintext;
}

int main() {
    SetConsoleCP(1251);        //для корректного ввода и вывода русских символов в консоли
    SetConsoleOutputCP(1251);
    string plaintext, key;
    cout << "Введите открытый текст: ";
    getline(cin, plaintext);
    cout << "Введите ключ: ";
    getline(cin, key);
    
    string ciphertext = encrypt(plaintext, key);
    cout << "Зашифрованный текст: " << ciphertext << endl;
    
    string decrypted_text = decrypt(ciphertext, key);
    cout << "Расшифрованный текст: " << decrypted_text << endl;
    
    return 0;
}


