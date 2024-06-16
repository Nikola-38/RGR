#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <Windows.h>

using namespace std;

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
    if (!file.is_open()) {
        throw logic_error("Невозможно прочитать файл: " + filename);
    }

    string content, line;
    while (getline(file, line)) {
        content += line + "\n";
    }
    file.close();
    return content;
}

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
            cout << "------SenSira------" << endl
                << "Select an action:" << endl
                << "<1> Encrypt" << endl
                << "<2> Decrypt" << endl
                << "<3> Exit to the main menu" << endl;
            cin >> choice;
            if (cin.fail() || cin.peek() != '\n') {
                throw logic_error("Input error!");
            }
            

            switch (choice) {
            case 1: {
                cout << "Enter the encoding password: ";
                getline(cin, pass1);
                if (pass1 != password1) {
                    throw logic_error("Invalid encoding password");
                }
                cout << "Input action:" << endl
                    << "<1> Enter text from the console" << endl
                    << "<2> Read text from a file" << endl;
                cin >> encryptionChoice;
                if (cin.fail() || cin.peek() != '\n') {
                    throw logic_error("Input error!");
                }
                
                if (encryptionChoice == 1) {
                    cout << "Enter the text to encrypt: ";
                    getline(cin, message);
                    cout << "Enter the encryption key: ";
                    getline(cin, key);
                    string ciphertext = encrypt_SenSira(message, key);
                    writeToFile("encrypted_SenSira.txt", ciphertext);
                    cout << "Encrypted text written to file: encrypted_SenSira.txt" << endl;
                }
                else if (encryptionChoice == 2) {
                    cout << "Enter the name of the file with the extension: ";
                    getline(cin, filename);
                    string plaintext = readFromFile(filename);
                    cout << "Enter the encryption key: ";
                    getline(cin, key);
                    string ciphertext = encrypt_SenSira(plaintext, key);
                    writeToFile("encrypted_SenSira.txt", ciphertext);
                    cout << "Encrypted text written to file: encrypted_SenSira.txt" << endl;
                }
                else {
                    throw logic_error("Invalid choice");
                }
                break;
            }
            case 2: {
                cout << "Enter the decoding password: ";
                getline(cin, pass2);
                if (pass2 != password2) {
                    throw logic_error("Invalid decoding password");
                }
                string ciphertext = readFromFile("encrypted_SenSira.txt");
                cout << "Enter the decryption key: ";
                getline(cin, key);
                string plaintext = decrypt_SenSira(ciphertext, key);
                writeToFile("decrypted_SenSira.txt", plaintext);
                cout << "Decrypted text written to file: decrypted_SenSira.txt" << endl;
                break;
            }
            case 3:
                return;
            default:
                throw logic_error("Invalid choice");
            }
        }
        catch (const exception& e) {
            cerr << e.what() << endl;
            cin.clear(); // Clear the error state
            
            system("pause");
        }
    }
}
