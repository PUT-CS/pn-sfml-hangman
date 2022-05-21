#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
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
// const char* savefile = "saves/save10.hangman";

int mode = 0;
// 0 - menu
// 1 - gra
// 2 - win screen
// 3 - human input
// 4 - load save
// 5 - saving
// 6 - lose screen

int getRandomNum(){
    int lower = 0;
    int num = (rand() % (NUM_OF_WORDS - lower + 1)) + lower;
    printf("%d\n", num);
    return num;
}

char* getRandomWord(){
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
    stat(savefile_name.c_str(), &sb);
    // stat(savefile., &sb);
    char *current_line = (char*)malloc(sb.st_size);

    wordlist = fopen(savefile_name.c_str(), "r");
    // wordlist = fopen(savefile, "r");
    if (wordlist == NULL) {
        printf("Unable to read the %s\n", savefile_name.c_str());
        exit(1);
    }
    for(int i=0; i<line_num; i++){
        fscanf(wordlist, "%[^\n] ", current_line);
    }
    fclose(wordlist);

    std::string str(current_line, current_line + strlen(current_line)-1);
    std::wstring wstr = stringToWstring(str);

    if (wstr == L".") { // a symbol for empty used string
        return L"";
    }

    return wstr;
}

void saveGame(std::wstring word, std::wstring word_hidden, std::wstring used, std::string savename){

    std::string path = "saves/";
    // savename = "save10.hangman";
    path += savename;

    std::string rmcommand = "rm saves/";
    rmcommand += savename;
    system(rmcommand.c_str());
    std::string touchcommand = "touch ";
    touchcommand += path;
    system(touchcommand.c_str());

    if (used.size() == 0) { // the player has no fails - fill it with a period.
        used = L".";
    }

    std::wofstream newsave(path, std::ios::binary);
    newsave << word << '\n';
    newsave << word_hidden << '\n';
    newsave << used << '\n';
    newsave.close();

    std::string convertcommand = "./util/unix2dos ";
    convertcommand += path;
    system(convertcommand.c_str());
}

int isAlNum(sf::Uint32 code){
    return (code >=32 && code <= 126) || isLetterAllowed(code)? 1 : 0 ;
}

int main(void){
    srand(time(NULL));

    char* dirty_word = getRandomWord();
    std::string wordstr(dirty_word, dirty_word + strlen(dirty_word)-1);
    std::wstring word = stringToWstring(wordstr);
    std::wstring word_hidden;
    word_hidden = hideWord(word);

    sf::RenderWindow window(sf::VideoMode(1000,1000), "Hangman");
    sf::Font font;
    if(!font.loadFromFile("assets/fonts/NotoSansMono-Regular.ttf")){
        puts("COULDN'T LOAD FONT");
        exit(1);
    }

    sf::Event event;
    sf::Color DarkGreen(16,130,0);
    sf::Color DarkRed(150,0,0);
    sf::Color Gray(90,90,90);

    sf::Text title;
    title = applyStyle(title, font, 80);
    title.setString(word);
    title.setString(L"Hangman");
    title = center(title, 2.0f, 7.0f);

    sf::Text main_menu;
    main_menu = applyStyle(main_menu, font, 56);
    main_menu.setString(L"Main Menu");
    main_menu = center(main_menu, 2.0f, 2.8f);

    sf::Text SFused;
    SFused = applyStyle(SFused, font, 20);
    SFused.setString(L"Michał Miłek & Sebastian Nowak");
    SFused = center(SFused, 2.0f, 1.1f);

    sf::Text SFfails;
    SFfails = applyStyle(SFfails, font, 30);
    SFfails.setString("2022");
    SFfails = center(SFfails, 2.0f, 1.05f);

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

    std::wstring used = L"";
    std::wstring letter;
    std::wstring humanword=L"";
    std::string savefile_name="";
    std::string newsave_name="";
    float fails = 0;
    float correct_guesses = 0;

    sf::Text used_letters;
    used_letters = applyStyle(used_letters, font, 50);
    used_letters.setString(used);
    used_letters.setLetterSpacing(3.0f);
    used_letters = center(used_letters, 2.0f, 1.10f);

    sf::Text SFendfails;
    SFendfails = applyStyle(SFendfails, font, 40);
    SFendfails.setString(std::to_string(fails));
    SFendfails = center(SFendfails, 2.0f, 1.3f);

    sf::Text SFcorrect_incorrect_ratio;
    SFcorrect_incorrect_ratio = applyStyle(SFcorrect_incorrect_ratio, font, 40);
    SFcorrect_incorrect_ratio = center(SFcorrect_incorrect_ratio, 2.0f, 1.1f);

    sf::Text SFendcorrect_guesses;
    SFendcorrect_guesses = applyStyle(SFendcorrect_guesses, font, 40);
    SFendcorrect_guesses.setString(std::to_string(correct_guesses));
    SFendcorrect_guesses = center(SFendcorrect_guesses, 2.0f, 1.2f);

    sf::Text SFword_reveal;
    SFword_reveal = applyStyle(SFword_reveal, font, 40);
    SFword_reveal = center(SFword_reveal, 2.0f, 1.7f);

    sf::Text SFinput_prompt;
    SFinput_prompt = applyStyle(SFinput_prompt, font, 30);
    SFinput_prompt.setString("Input the word for the other player to guess");
    SFinput_prompt = center(SFinput_prompt, 2.0f, 2.0f);

    sf::Text SFhuman_input;
    SFhuman_input = applyStyle(SFhuman_input, font, 25);
    SFhuman_input.setString(L"");
    SFhuman_input = center(SFhuman_input, 2.0f, 1.8f);

    sf::Text SFkeybinds;
    SFkeybinds = applyStyle(SFkeybinds, font, 20);
    SFkeybinds.setString("New game (CPU): CTRL+N\nNew game (HUM): CTRL+H\nLoad Game:      CTRL+L\nQuit:           CTRL+Q ");
    SFkeybinds = center(SFkeybinds, 5.0f, 1.1f);

    while (window.isOpen()) {
        while (window.waitEvent(event)){

            if (event.type == sf::Event::Closed){
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                window.close();
            }
            // if (mode == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Comma)) {
            //     mode = 2; //auto win
            // }
            if (mode == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
                mode = 3; // get human input for the word
            }

            if (mode == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
                mode = 1; // start the game
            }

            if (mode == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
            // if (mode == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::SemiColon)) {
                mode = 4; // load save
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                mode = 5;
            }

            if(event.type == sf::Event::TextEntered){

                if (mode == 3){ //user word input
                    if (event.text.unicode != '\r') {

                        if (event.text.unicode == '\b' && humanword.size() > 0) { //backspace
                            humanword.erase(humanword.size()-1, 1); // delete the last character
                            SFhuman_input.setString(humanword);
                        }
                        else if (humanword.size() <= MAX_LETTERS  && event.text.unicode != ' ' && event.text.unicode != '\b' && isLetterAllowed(event.text.unicode)) { //normal letters
                            humanword.push_back(static_cast<wchar_t>(event.text.unicode));
                            SFhuman_input.setString(humanword);
                        }
                        SFhuman_input = center(SFhuman_input, 2.0f, 1.8f);
                    }
                    else if (humanword.size() >= MIN_LETTERS){
                        word = humanword;
                        title.setString(word);
                        word_hidden = hideWord(word);
                        mode = 1;
                    }
                }

                if (mode == 5) {
                    // title.setString(L"Saving!");
                    if (event.text.unicode == '\b' && newsave_name.size() > 0) { //backspace
                        newsave_name.erase(newsave_name.size()-1, 1); // delete the last character
                        SFused.setString(newsave_name);
                    }
                    else if (isAlNum(event.text.unicode)) {
                        newsave_name.push_back(static_cast<char>(event.text.unicode));
                        SFused.setString(newsave_name);
                    }
                    else if (event.text.unicode == '\r'){
                        saveGame(word, word_hidden, used, newsave_name);
                        mode = 1;
                    }
                }

                if (mode == 4) { // load save

                    if (event.text.unicode != '\r') {

                        if (event.text.unicode == '\b' && savefile_name.size() > 0) { //backspace
                            savefile_name.erase(savefile_name.size()-1, 1); // delete the last character
                            SFused.setString(savefile_name);
                        }
                        else if (isAlNum(event.text.unicode)) {
                            savefile_name.push_back(static_cast<char>(event.text.unicode));
                            SFused.setString(savefile_name);
                        }
                    }

                    else {
                    // else if (savefile_name.size() >= MIN_LETTERS){

                        word = loadSaveLine(1, savefile_name);
                        word_hidden = loadSaveLine(2, savefile_name);
                        used = loadSaveLine(3, savefile_name);

                        title.setString(word);
                        hidden_word.setString(word_hidden);
                        SFused.setString(used);
                        fails = used.size();

                        title = center(title, 2.0f, 7.0f);
                        hidden_word = center(hidden_word, 2.0f, 2.0f);
                        mode = 1;
                    }


                }

                if (mode == 1) { //game loop

                    if (event.text.unicode > 128)
                        letter = getPolishLetter(event.text.unicode);
                    else letter = event.text.unicode;

                    if (isLetterInWstring(word, letter) && !isLetterInWstring(word_hidden, letter)) {
                        correct_guesses++;
                    }

                    word_hidden = fillWord(word, word_hidden, letter);
                    hidden_word.setString(word_hidden);

                    //fail
                    if (!isLetterInWstring(word,letter) && !isLetterInWstring(used,letter) && isLetterAllowed(event.text.unicode)) {
                            used+=letter;
                            fails++;
                    }

                    SFused.setString(used);
                    SFused = center(SFused, 2.0f, 1.1f);
                    SFfails.setString(std::to_wstring((int)fails));
                    SFfails = center(SFfails, 2.0f, 1.05f);

                    // check for the win
                    if (playerHasWon(word_hidden)) {
                        mode = 2;
                    }
                }
            }
            window.clear();
            if (mode == 0) { //menu
                window.draw(title);
                window.draw(main_menu);
                window.draw(begin_game_button);
                window.draw(end_game_button);
                window.draw(new_game);
                window.draw(load_save);
            }
            if (mode == 1) { //game
                window.draw(hidden_word);
                window.draw(used_letters);
                window.draw(SFused);
                window.draw(SFfails);
            }
            if (mode == 2) { //win
                title.setString(L"Congratulations!");
                title = center(title, 2.0f, 7.0f);

                SFword_reveal.setString(L"The word was: "+word);
                SFword_reveal = center(SFword_reveal, 2.0f, 3.6f);
                window.draw(SFword_reveal);

                SFendcorrect_guesses.setString("Correct guesses: " + std::to_string((int)correct_guesses));
                SFendcorrect_guesses = center(SFendcorrect_guesses, 2.0f, 2.6f);
                window.draw(SFendcorrect_guesses);

                SFendfails.setString("Incorrect guesses: " + std::to_string((int)fails));
                SFendfails = center(SFendfails, 2.0f, 2.3f);
                window.draw(SFendfails);

                SFcorrect_incorrect_ratio.setString("Ratio: " + std::to_string((int)(correct_guesses/(correct_guesses+fails)*100))+"%");
                SFcorrect_incorrect_ratio = center(SFcorrect_incorrect_ratio, 2.0f, 1.9f);
                window.draw(SFcorrect_incorrect_ratio);


            }
            if (mode == 3) { // human input
                window.draw(SFinput_prompt);
                window.draw(SFhuman_input);
                // window.draw(SFused);
            }
            if (mode == 4) { // loading a save
                window.draw(SFused);
            }
            if (mode == 5) { // saving the game
                window.draw(SFused);
            }
            window.draw(title);
            window.draw(SFkeybinds);
            window.display();
        }
    }
    return EXIT_SUCCESS;
}
