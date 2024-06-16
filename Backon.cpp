#include <iostream>
#include <string>
#include <fstream>

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

bool exist(const string& filename) {
    ifstream file(filename);
    return file.good();
}

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
                throw out_of_range("Ciphertext is not a multiple of 8 characters");
            }
            char ch = 0;
            for (int j = 0; j < 8; ++j) {
                if (ciphertext[i + j] != 'a' && ciphertext[i + j] != 'b') {
                    throw invalid_argument("Ciphertext contains invalid characters");
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
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        cout << "------Bacon Cipher-----" << endl;
        cout << "Select an action:" << endl;
        cout << "<1> Encrypt" << endl;
        cout << "<2> Decrypt" << endl;
        cout << "<3> Exit" << endl;
        cout << "Enter your choice: ";
        cin >> pick;

        try {
            if (pick != 1 && pick != 2 && pick != 3) {
                throw logic_error("You entered the wrong command!");
            }

            if (pick == 1) {
                cout << "Enter the encoding password: ";
                cin >> pass1;
                if (pass1 == password1) {
                    cout << "Input action:" << endl
                        << "<1> Enter text from the console" << endl
                        << "<2> Read text from a file" << endl;
                    cin >> pickencode;
                    if (cin.fail() || cin.peek() != '\n') {
                        throw logic_error("Input error!");
                    }
                    if (pickencode != 1 && pickencode != 2) {
                        throw logic_error("You entered the wrong command!");
                    }
                    if (pickencode == 1) {
                        cout << "Input text: ";
                        cin.ignore();
                        getline(cin, message);

                        string ciphertext = encryptBaconian(message);
                        cout << "Encrypted message: " << ciphertext << endl;
                        encryptedFilename = "encrypted_baconian.txt";
                        writeToFile(encryptedFilename, ciphertext); // Запись зашифрованного текста в файл
                        cout << "Encrypted text saved to: " << encryptedFilename << endl;
                    }
                    else if (pickencode == 2) {
                        cout << "Enter the name of the file with the extension: ";
                        cin >> filename;

                        if (!exist(filename)) {
                            throw logic_error("File does not exist or cannot be opened!");
                        }

                        string plaintext = readFromFile(filename);
                        string ciphertext = encryptBaconian(plaintext);
                        cout << "Encrypted message: " << ciphertext << endl;
                        encryptedFilename = "encrypted_baconian.txt";
                        writeToFile(encryptedFilename, ciphertext); // Запись зашифрованного текста в файл
                        cout << "Encrypted text saved to: " << encryptedFilename << endl;
                    }
                }
                else {
                    throw logic_error("Invalid encoding password");
                }
            }
            else if (pick == 2) {
                cout << "Enter the decoding password: ";
                cin >> pass2;

                if (pass2 == password2) {
                    cout << "Enter the name of the file with the encrypted message: ";
                    cin >> filename;

                    if (!exist(filename)) {
                        throw logic_error("File does not exist or cannot be opened!");
                    }

                    string ciphertext = readFromFile(filename);
                    string plaintext = decryptBaconian(ciphertext);
                    cout << "Decrypted message: " << plaintext << endl;
                    decryptedFilename = "decrypted_baconian.txt";
                    writeToFile(decryptedFilename, plaintext); // Запись дешифрованного текста в файл
                    cout << "Decrypted text saved to: " << decryptedFilename << endl;
                }
                else {
                    throw logic_error("Invalid decoding password");
                }
            }
            else if (pick == 3) {
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
