#include <iostream>
#include "websearch.hpp"
#include <string.h>

void research(const char* word, Config CONFIG){
    std::string wordstr(word, word+strlen(word));
    std::string browserstr(CONFIG.BROWSER.begin(), CONFIG.BROWSER.end());
    std::string enginestr(CONFIG.SEARCH_ENGINE.begin(), CONFIG.SEARCH_ENGINE.end());
    std::string command;
    if (enginestr == "google") {
        command = browserstr + " https://www.google.com/search?q=" + wordstr + " &";
    }
    else if (enginestr == "duckduckgo") {
        command = browserstr + " https://duckduckgo.com/?q=" + wordstr + " &";
    }
    else if (enginestr == "bravesearch") {
        command = browserstr + " https://search.brave.com/search?q=" + wordstr + " &";
    }
    else if (enginestr == "swisscows") {
        command = browserstr + " https://swisscows.com/web?&query=" + wordstr + " &";
    }
    else if (enginestr == "yandex") {
        command = browserstr + " https://yandex.com/search/?text=" + wordstr + " &";
    }
    else {
        puts("Uncrecognized search engine!");
        exit(1);
    }
    std::system(command.c_str());
    return;
}
