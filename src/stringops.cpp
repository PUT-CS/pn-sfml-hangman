#include "stringops.hpp"
#include <iostream>

int isLetterInWstring(std::wstring word, std::wstring letter){
    for (int i=0; i<word.size(); i++) {
        if (word[i] == letter[0]) {
            return 1;
        }
    }
    return 0;
}

std::wstring getPolishLetter(sf::Uint32 code){
    switch (code) {
        case 0x105:
            return L"ą";
            break;
        case 0x107:
            return L"ć";
            break;
        case 0x119:
            return L"ę";
            break;
        case 0x142:
            return L"ł";
            break;
        case 0x144:
            return L"ń";
            break;
        case 0x0F3:
            return L"ó";
            break;
        case 0x015B:
            return L"ś";
            break;
        case 0x017A:
            return L"ź";
            break;
        case 0x017C:
            return L"ż";
            break;
        default:
            return L"?";
            break;
    }
}

std::wstring stringToWstring(std::string word){
    std::wstring output = L"";
    for (int i=0; i<word.size(); i++) {
        if (word[i]>0) {
            // printf("%d\n", word[i]);
            output.push_back(static_cast<wchar_t>(word[i]));
        }
        else {
            if (word[i] == -60 && word[i+1] == -123) {
                output.push_back(L'ą');
                i++;
            }
            if (word[i] == -60 && word[i+1] == -121) {
                output.push_back(L'ć');
                i++;
            }
            if (word[i] == -60 && word[i+1] == -103) {
                output.push_back(L'ę');
                i++;
            }
            if (word[i] == -59 && word[i+1] == -126) {
                output.push_back(L'ł');
                i++;
            }
            if (word[i] == -59 && word[i+1] == -124) {
                output.push_back(L'ń');
                i++;
            }
            if (word[i] == -59 && word[i+1] == -101) {
                output.push_back(L'ś');
                i++;
            }
            if (word[i] == -61 && word[i+1] == -77) {
                output.push_back(L'ó');
                i++;
            }
            if (word[i] == -59 && word[i+1] == -70) {
                output.push_back(L'ź');
                i++;
            }
            if (word[i] == -59 && word[i+1] == -68) {
                output.push_back(L'ż');
                i++;
            }
        }
    }
    return output;
}
