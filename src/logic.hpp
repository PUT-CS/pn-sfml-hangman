#ifndef hangmanlogic
#define hangmanlogic
#include <iostream>

std::wstring fillWord(std::wstring word, std::wstring hidden_word, std::wstring letter);
int playerHasWon(std::wstring word_hidden);
std::wstring hideWord(std::wstring word);

#endif
