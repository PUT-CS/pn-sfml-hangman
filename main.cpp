#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <fstream>
#include "src/logic.hpp"
#include "src/stringops.hpp"
#include "src/sfmlops.hpp"

// #define NUM_OF_WORDS 9
#define NUM_OF_WORDS 610875
#define MAX_LETTERS 16
#define MIN_LETTERS 5

const char* filename = "assets/wordlists/slowa.txt";
const char* savefile = "saves/save10.hangman";

int mode = 0;
int fails = -1;
// 0 - menu
// 1 - gra
// 2 - end screen
// 3 - human input
// 4 - load save

int getRandomNum(){
    int lower = 0;
    int num = (rand() % (NUM_OF_WORDS - lower + 1)) + lower;
    printf("%d\n", num);
    return num;
}

char* getWord(){
    FILE *wordlist;
    char* word;
    int line_num = getRandomNum();

    struct stat sb;
    stat(filename, &sb);
    char *current_line = (char*)malloc(sb.st_size);

    wordlist = fopen(filename, "r");
    if (wordlist == NULL) {
        printf("Unable to read the %s\n", filename);
        exit(1);
    }
    // fgets(line, sizeof line, stdin)
    for(int i=0; i<line_num; i++){
        fscanf(wordlist, "%[^\n] ", current_line);
        // fscanf(wordlist, "%[^\n]", current_line);
        // fgets(current_line, sizeof current_line, wordlist);
    }
    fclose(wordlist);
    return current_line;
}

std::wstring getWord2(int line_num){

    FILE *wordlist;
    char* word;
    struct stat sb;
    stat(savefile, &sb);
    char *current_line = (char*)malloc(sb.st_size);

    wordlist = fopen(savefile, "r");
    if (wordlist == NULL) {
        printf("Unable to read the %s\n", savefile);
        exit(1);
    }
    for(int i=0; i<line_num; i++){
        fscanf(wordlist, "%[^\n] ", current_line);
    }
    fclose(wordlist);

    std::string str(current_line, current_line + strlen(current_line)-1);
    std::wstring wstr = stringToWstring(str);

    return wstr;
}

void saveGame(std::wstring word, std::wstring word_hidden, std::wstring used, std::string savename){

    std::string path = "saves/";
    savename = "save10.hangman";
    path += savename;

    std::string rmcommand = "rm saves/";
    rmcommand += savename;
    system(rmcommand.c_str());
    std::string touchcommand = "touch ";
    touchcommand += path;
    system(touchcommand.c_str());

    std::wofstream newsave(path, std::ios::binary);
    newsave << word << '\n';
    newsave << word_hidden << '\n';
    newsave << used << '\n';
    newsave.close();

    std::string convertcommand = "./util/unix2dos ";
    convertcommand += path;
    system(convertcommand.c_str());
}

int main(void){
    srand(time(NULL));

 //tu bylo wczytywanie
     char* dirty_word = getWord();
    std::string wordstr(dirty_word, dirty_word + strlen(dirty_word)-1);
    std::wstring word = stringToWstring(wordstr);
    std::wstring word_hidden;
    word_hidden = hideWord(word);

    // std::wcout<<word_hidden<<std::endl;

    sf::RenderWindow window(sf::VideoMode(1000,1000), "Hangman");
    sf::Font font;
    if(!font.loadFromFile("assets/fonts/NotoSansMono-Regular.ttf")){
        puts("COULDN'T LOAD FONT");
        exit(1);
    }

    //image display
    //
    // sf::Image hangman_image;
    // hangman_image.loadFromFile("assets/states/state10.png");
    // sf::Texture texture;
    // texture.loadFromImage(hangman_image);
    // texture.setSmooth(true);
    // sf::Sprite sprite;
    // sprite.setTexture(texture);

    sf::Event event;
    sf::Color DarkGreen(16,130,0);
    sf::Color DarkRed(150,0,0);
    sf::Color Gray(90,90,90);

    sf::Text title;
    title = applyStyle(title, font, 80);
    title.setString(word);
    // title.setString(L"Hangman");
    title = center(title, 2.0f, 7.0f);

    sf::Text main_menu;
    main_menu = applyStyle(main_menu, font, 56);
    main_menu.setString(L"Main Menu");
    main_menu = center(main_menu, 2.0f, 2.8f);

    sf::Text authors;
    authors = applyStyle(authors, font, 20);
    authors.setString(L"Michał Miłek & Sebastian Nowak");
    authors = center(authors, 2.0f, 1.1f);

    sf::Text year;
    year = applyStyle(year, font, 30);
    year.setString("2022");
    year = center(year, 2.0f, 1.05f);

    sf::Text new_game;
    new_game = applyStyle(new_game, font, 30);
    new_game.setString("New Game");
    new_game = center(new_game, 2.0f, 2.03f);

    sf::Text load_save;
    load_save = applyStyle(load_save, font, 30);
    load_save.setString("Load Save");
    load_save = center(load_save, 2.0f, 1.52f);

    sf::RectangleShape begin_game_button;
    begin_game_button.setSize(sf::Vector2f(555, 100));
    begin_game_button.setFillColor(Gray);
    begin_game_button.setOutlineThickness(2);
    sf::FloatRect begin_game_buttonRect = begin_game_button.getLocalBounds();
    begin_game_button.setPosition(1000/2.0f, 1000/2.0f);
    begin_game_button.setOrigin(begin_game_buttonRect.width/2,begin_game_buttonRect.height/2);

    sf::RectangleShape end_game_button;
    end_game_button.setSize(sf::Vector2f(555, 100));
    end_game_button.setFillColor(Gray);
    end_game_button.setOutlineThickness(2);
    sf::FloatRect end_game_buttonRect = end_game_button.getLocalBounds();
    end_game_button.setPosition(1000/2.0f, 1000/1.5f);
    end_game_button.setOrigin(end_game_buttonRect.width/2,end_game_buttonRect.height/2);

    sf::Text hidden_word;
    hidden_word = applyStyle(hidden_word, font, 50);
    hidden_word.setString(word_hidden);
    hidden_word.setLetterSpacing(3.0f);
    hidden_word = center(hidden_word, 2.0f, 2.0f);

    std::wstring used;
    std::wstring letter;
    std::wstring humanword;

    sf::Text used_letters;
    used_letters = applyStyle(used_letters, font, 50);
    used_letters.setString(used);
    used_letters.setLetterSpacing(3.0f);
    used_letters = center(used_letters, 2.0f, 1.10f);

    while (window.isOpen()) {
        while (window.waitEvent(event)){

            if (event.type == sf::Event::Closed){
                window.close();
            }

            if (mode == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::SemiColon)) {

                mode = 1; // start the game
            }

            if (mode == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Comma)) {
                mode = 2; //auto win
            }

            if (mode == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Slash)) {
                mode = 3; // get human input for the word
            }

            if (mode == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                mode = 4; // load save
            }

            if(event.type == sf::Event::TextEntered){

                if (mode == 3){ //user word input
                    if (event.text.unicode != '\r') {
                        if (event.text.unicode == '\b' && humanword.size() > 0) { //backspace
                            humanword.erase(humanword.size()-1, 1);
                            authors.setString(humanword);
                        }
                        else if (humanword.size() <= MAX_LETTERS  && event.text.unicode != ' ') { //normal letters
                            humanword.push_back(static_cast<wchar_t>(event.text.unicode));
                            authors.setString(humanword);
                        }
                    }
                    else if (humanword.size() >= MIN_LETTERS){
                        word = humanword;
                        title.setString(word);
                        word_hidden = hideWord(word);
                        mode = 1;
                    }
                }

                if (mode == 4) { // load save

                    //// if (used.size() > 0) {
                        // saveGame(word, word_hidden, used, "save10.hangman");
                    //// }

                    word = getWord2(1);
                    word_hidden = getWord2(2);
                    used = getWord2(3);

                    // old way of loading text from file
                    // char* savedword = getWord2(1);
                    // char* savedword = getWord2(1);
                    // std::string str(savedword, savedword+strlen(savedword)); // -1?
                    // std::wstring wstr = stringToWstring(str);

                    title.setString(word);

                    word_hidden = hideWord(word);
                    hidden_word.setString(word_hidden);

                    authors.setString(used);

                    title = center(title, 2.0f, 7.0f);
                    hidden_word = center(hidden_word, 2.0f, 2.0f);

                    mode = 1;
                }

                if (mode == 1) { //game loop

                    if (event.text.unicode > 128) {
                        letter = getPolishLetter(event.text.unicode);
                        word_hidden = fillWord(word, word_hidden, letter);
                        hidden_word.setString(word_hidden);
                        if (!isLetterInWstring(word,letter)) {
                            if (!isLetterInWstring(used,letter)) {
                                used+=letter;
                                fails++;
                            }
                        }
                        authors.setString(used);
                    }
                    else {
                        letter = event.text.unicode;
                        word_hidden = fillWord(word, word_hidden, letter);
                        hidden_word.setString(word_hidden);
                        if (!isLetterInWstring(word,letter)) {
                            if (!isLetterInWstring(used,letter)) {
                                used+=letter;
                                fails++;
                            }
                        }
                        authors.setString(used);
                    }
                    if (playerHasWon(word_hidden)) {
                        mode = 2;
                    }
                    year.setString(std::to_wstring(fails));
                }
            }
            window.clear();
            if (mode == 0) { //menu
                window.draw(main_menu);
                window.draw(begin_game_button);
                window.draw(end_game_button);
                window.draw(new_game);
                window.draw(load_save);
            }
            if (mode == 1) { //game
                window.draw(hidden_word);
                window.draw(used_letters);
            }
            if (mode == 2) { //win
                title.setFont(font);
                title.setString(L"You have won!");
                title = center(title, 2.0f, 3.0f);
                // window.draw(title);
            }
            window.draw(title);
            window.draw(authors);
            window.draw(year);
            window.display();
        }
    }
    return EXIT_SUCCESS;
}
