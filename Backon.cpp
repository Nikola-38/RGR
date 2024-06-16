#include "Read.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;


string encryptBaconian(const string& plaintext) {
    string ciphertext;
    for (char c : plaintext) {
        for (int i = 7; i >= 0; --i) {
            ciphertext += ((c >> i) & 1) ? 'b' : 'a';
        }
    }
    return ciphertext;
}

string decryptBaconian(const string& ciphertext) {
    string plaintext;
    try {
        for (size_t i = 0; i < ciphertext.length(); i += 8) {
            if (i + 7 >= ciphertext.length()) {
                throw out_of_range("Зашифрованный текст не должен быть кратен 8 символам");
            }
            char ch = 0;
            for (int j = 0; j < 8; ++j) {
                if (ciphertext[i + j] != 'a' && ciphertext[i + j] != 'b') {
                    throw invalid_argument("Зашифрованный текст содержит недопустимые символы");
                }
                ch = (ch << 1) | (ciphertext[i + j] == 'b');
            }
            plaintext += ch;
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return "";
    }
    return plaintext;
}

void Bacon(string& password1, string& password2) {
    int pick, pickencode;
    string pass1, pass2, message, filename, encryptedFilename, decryptedFilename;

    while (true) {
       
        cout << "______Bacon Cipher_____" << endl;
        cout << "Выберите действие:" << endl;
        cout << "<1> Зашифровать" << endl;
        cout << "<2> Дешифровать" << endl;
        cout << "<3> Выход" << endl;
        cout << "Введите свой выбор:" << endl;
        cout << "______________________" << endl;
        cin >> pick;

        try {
            if (pick != 1 && pick != 2 && pick != 3) {
                throw logic_error("Вы ввели неправильную команду!");
            }

            if (pick == 1) {
                system("cls");
                cout << "Введите пароль для кодировки: ";
                cin >> pass1;
                system("cls");
                if (pass1 == password1) {
                    cout << "Входное действие:" << endl
                        << "<1> Введите текст с консоли" << endl
                        << "<2> Чтение текста из файла" << endl;
                    cin >> pickencode;
                    if (cin.fail() || cin.peek() != '\n') {
                        throw logic_error("Ошибка ввода!");
                    }
                    if (pickencode != 1 && pickencode != 2) {
                        throw logic_error("Вы ввели неправильную команду!");
                    }
                    if (pickencode == 1) {
                        cout << "Ввод текста: ";
                        cin.ignore(); // Очистка буфера ввода
                        getline(cin, message);

                        string originalFilename = "Backon_original_input.txt";
                        writeToFile(originalFilename, message); // Запись введенного текста в файл
                        cout << "Введенный текст сохранен в файл: " << originalFilename << endl;

                        string ciphertext = encryptBaconian(message);
                        string encryptedFilename = "encrypted_baconian.txt";
                        writeToFile(encryptedFilename, ciphertext); // Запись зашифрованного текста в файл
                        cout << "Зашифрованный текст, сохраненный в: " << encryptedFilename << endl;
                        
                        
                    }

                    else if (pickencode == 2) {
                        system("cls");
                        
                        cout << "Введите имя файла с расширением:";
                        cin >> filename;
                       
                        if (!exist(filename)) {
                            throw logic_error("Файл не существует или не может быть открыт!");
                        }

                        string plaintext = readFromFile(filename);
                        string ciphertext = encryptBaconian(plaintext);
                       
                        encryptedFilename = "encrypted_baconian.txt";
                        writeToFile(encryptedFilename, ciphertext); // Запись зашифрованного текста в файл
                        cout << "Зашифрованный текст, сохраненный в: " << encryptedFilename << endl;
                        
                        
                    }
                }
                else {
                    throw logic_error("Неверный пароль для кодировки");
                }
            }
            
            else if (pick == 2) {
                system("cls");
                cout << "Введите пароль для расшифровки: ";
                cin >> pass2;
                
                if (pass2 == password2) {
                    cout << "Введите название файла с зашифрованным сообщением: ";
                    cin >> filename;

                    if (!exist(filename)) {
                        throw logic_error("Файл не существует или не может быть открыт!");
                    }

                    string ciphertext = readFromFile(filename);
                    string plaintext = decryptBaconian(ciphertext);
                    
                    decryptedFilename = "decrypted_baconian.txt";
                    writeToFile(decryptedFilename, plaintext); // Запись дешифрованного текста в файл
                    cout << "Расшифрованный текст, сохраненный в: " << decryptedFilename << endl;
                }
                else {
                    throw logic_error("Invalid decoding password");
                }
            }
            else if (pick == 3) {
                system("cls");
                return;
            }
        }
        catch (const exception& e) {
            cerr << e.what() << endl;
            cin.clear(); // Clear the error state
            system("pause");
        }
    }
}
