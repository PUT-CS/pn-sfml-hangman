#include <iostream>
#include "logic.hpp"

std::wstring hideWord(std::wstring word){
    std::wstring word_hidden = L"";
    for (int i=0; i<word.size(); i++) {
        word_hidden+=L'_';
    }
    return word_hidden;
}
int playerHasWon(std::wstring word_hidden){
    for (int i=0; i<word_hidden.size(); i++) {
        if (word_hidden[i] == '_') {
            return 0;
        }
    }
    return 1;
}
std::wstring fillWord(std::wstring word, std::wstring hidden_word, std::wstring letter){
    for (int i=0; i<word.size(); i++) {
        wchar_t spacecheck = hidden_word[i];
        if (word[i] == letter[0] && spacecheck != L' ') {
            hidden_word[i] = letter[0];
        }
    }
    return hidden_word;
}
