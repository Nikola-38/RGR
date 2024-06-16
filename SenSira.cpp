#include "Read.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <Windows.h>

using namespace std;


string encrypt_SenSira(const string& plaintext, const string& key) {
    string ciphertext;
    int key_length = key.length();

    for (size_t i = 0; i < plaintext.length(); ++i) {
        char current_char = plaintext[i];
        char key_char = key[i % key_length];
        char encrypted_char = current_char + key_char;
        ciphertext += encrypted_char;
    }

    return ciphertext;
}

string decrypt_SenSira(const string& ciphertext, const string& key) {
    string plaintext;
    int key_length = key.length();

    for (size_t i = 0; i < ciphertext.length(); ++i) {
        char current_char = ciphertext[i];
        char key_char = key[i % key_length];
        char decrypted_char = current_char - key_char;
        plaintext += decrypted_char;
    }

    return plaintext;
}

void SenSira(const string& password1, const string& password2) {
    int choice, encryptionChoice;
    string pass1, pass2, message, filename, key;

    while (true) {
        try {
            
            cout << "_______SenSira_______" << endl;
            cout << "Выберите действие:" << endl;
            cout << "<1> Зашифровать" << endl;
            cout << "<2> Дешифровать" << endl;
            cout << "<3> Выход" << endl;
            cout << "Введите свой выбор:" << endl;
            cout << "______________________" << endl;

            cin >> choice;
            if (cin.fail() || cin.peek() != '\n') {
                throw logic_error("Ошибка ввода!");
            }

            cin.ignore(); // Clear the input buffer
            
            // Request the key only once
            cout << "Введите ключ шифрования: ";
            getline(cin, key);

            switch (choice) {
            case 1: {
                cout << "Введите пароль для кодировки: ";
                getline(cin, pass1);
                if (pass1 != password1) {
                    throw logic_error("Неверный пароль для кодировки");
                }
                cout << "Входное действие:" << endl
                    << "<1> Введите текст с консоли" << endl
                    << "<2> Чтение текста из файла" << endl;
                cin >> encryptionChoice;
                if (cin.fail() || cin.peek() != '\n') {
                    throw logic_error("Ошибка ввода!");
                }

                cin.ignore(); // Clear the input buffer

                if (encryptionChoice == 1) {
                    cout << "Введите текст для шифрования: ";
                    string message;
                    getline(cin, message);
                    string filename;
                    cout << "Введите имя файла для сохранения введенного текста: ";
                    getline(cin, filename);
                    writeToFile(filename, message);
                    string ciphertext = encrypt_SenSira(message, key);
                    writeToFile("encrypted_SenSira.txt", ciphertext);
                    cout << "Введенный текст сохранен в файл: " << filename << endl;
                    cout << "Зашифрованный текст, записанный в файл: encrypted_SenSira.txt" << endl;
                }
                
                else if (encryptionChoice == 2) {
                    cout << "Введите имя файла с расширением: ";
                    getline(cin, filename);
                    string plaintext = readFromFile(filename);
                    string ciphertext = encrypt_SenSira(plaintext, key);
                    writeToFile("encrypted_SenSira.txt", ciphertext);
                    cout << "Зашифрованный текст, записанный в файл: encrypted_SenSira.txt" << endl;
                   
                }
           
                else {
                    throw logic_error("Неверный выбор");
                }
                break;
                
            }
            case 2: {
                system("cls");
                cout << "Введите пароль для расшифровки: ";
                getline(cin, pass2);
                if (pass2 != password2) {
                    throw logic_error("Неверный пароль для расшифровки");
                }
                string ciphertext = readFromFile("encrypted_SenSira.txt");
                string plaintext = decrypt_SenSira(ciphertext, key);
                writeToFile("decrypted_SenSira.txt", plaintext);
                cout << "Расшифрованный текст, записанный в файл: decrypted_SenSira.txt" << endl;
                break;
                
            }
            case 3:
                return;
            default:
                throw logic_error("Неверный выбор");
            }
        }
        catch (const exception& e) {
            cerr << e.what() << endl;
            cin.clear(); // Clear the error state

            system("pause");
        }
    }
}
