#pragma once
#include <iostream>
#include <string>

using namespace std;

string encrypt_SenSira(const string& plaintext, const string& key);
string decrypt_SenSira(const string& ciphertext, const string& key);
void SenSira(const string& password1, const string& password2);
