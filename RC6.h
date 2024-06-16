#pragma once

#include <iostream>
#include <string>

using namespace std;

void StringToKey(const std::string& key, uint8_t* K, int keySize);
uint32_t Odd(double x);
void KeyExpansion(uint8_t* K, int b, uint32_t* L, int& c);
void InitializeS(uint32_t* S, int r);
void MixKeys(uint32_t* L, int c, uint32_t* S, int r);
void Encrypt_RC6(uint32_t& A, uint32_t& B, uint32_t& C, uint32_t& D, uint32_t* S, int r);
void Decrypt_RC6(uint32_t& A, uint32_t& B, uint32_t& C, uint32_t& D, uint32_t* S, int r);
void RC6(const string& password1, const string& password2);
