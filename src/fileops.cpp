#include "fileops.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <string.h>
#include <fstream>
#include <array>
#include "stringops.hpp"
#include <memory>
#include <stdexcept>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
int getRandomNumMax(int max){
    int lower = 0;
    int num = (rand() % (max - lower + 1)) + lower;
    return num;
}
char* getRandomWord2(std::wstring filenamewstr){
    std::string filepath(filenamewstr.begin(), filenamewstr.end());
    filepath = "assets/wordlists/" + filepath;
    const char* filename = filepath.c_str();
    std::string command = "cat "+filepath+" | wc -l";
    const char* countlinescommand = command.c_str();

    int num_of_lines = stoi(exec(countlinescommand));

    FILE *wordlist;
    char* word;
    int line_num = getRandomNumMax(num_of_lines);

    struct stat sb;
    stat(filename, &sb);
    char *current_line = (char*)malloc(sb.st_size);

    wordlist = fopen(filename, "r");
    if (wordlist == NULL) {
        printf("Unable to read the %s\n", filename);
        exit(1);
    }
    for(int i=0; i<line_num; i++){
        fscanf(wordlist, "%[^\n] ", current_line);
    }
    fclose(wordlist);
    return current_line;
}

std::wstring loadSaveLine(int line_num, std::string savefile_name){

    FILE *wordlist;
    char* word;
    struct stat sb;
    std::string path = "saves/"+savefile_name;
    stat(path.c_str(), &sb);
    // stat(savefile., &sb);
    char *current_line = (char*)malloc(sb.st_size);

    wordlist = fopen(path.c_str(), "r");
    if (wordlist == NULL) {
        printf("Unable to read the %s\n", path.c_str());
        exit(1);
    }
    for(int i=0; i<line_num; i++){
        fscanf(wordlist, "%[^\n] ", current_line);
    }
    fclose(wordlist);

    std::string str(current_line, current_line + strlen(current_line)-1);
    std::wstring wstr = stringToWstring(str);

    if (wstr == L".") // a symbol for empty used string
        return L"";
    return wstr;
}

void saveGame(std::wstring word, std::wstring word_hidden, std::wstring used, std::string savename){

    std::string path = "saves/";
    path += savename;

    std::string rmcommand = "rm saves/";
    rmcommand += savename;
    system(rmcommand.c_str());
    std::string touchcommand = "touch ";
    touchcommand += path;
    system(touchcommand.c_str());
    if (used.size() == 0) { // the player has no fails - fill it with a period
        used = L".";
    }
    std::wofstream newsave(path);
    newsave << word << '\n';
    newsave << word_hidden << '\n';
    newsave << used << '\n';
    newsave.close();

    std::string convertcommand = "./util/unix2dos ";
    convertcommand += path;
    system(convertcommand.c_str());
    newsave.close();
}
