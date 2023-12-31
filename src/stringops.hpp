#ifndef stringops
#define stringops
#include <SFML/Config.hpp>
#include <iostream>

std::wstring stringToWstring(std::string word);
std::wstring getPolishLetter(sf::Uint32 code);
int isLetterInWstring(std::wstring word, std::wstring letter);
int isLetterAllowed(sf::Uint32 code);
int isAlNum(sf::Uint32 code);

#endif
