#include <iostream>
#include <sstream>  
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
