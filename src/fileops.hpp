#ifndef fileops
#define fileops
#include <SFML/Config.hpp>
#include <iostream>

std::string exec(const char* cmd);
int getRandomNumMax(int max);
char* getRandomWord2(std::wstring filenamewstr);
std::wstring loadSaveLine(int line_num, std::string savefile_name);
void saveGame(std::wstring word, std::wstring word_hidden, std::wstring used, std::string savename);

#endif
