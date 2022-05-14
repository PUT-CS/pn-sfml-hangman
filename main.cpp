#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <bits/types/FILE.h>
#include <clocale>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <locale>
#include <ostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
// #define NUM_OF_WORDS 9
#define NUM_OF_WORDS 610875
#define MAX_LETTERS 16
#define MIN_LETTERS 5
#define SCREEN_X 1000
#define SCREEN_Y 1000

const char* filename = "assets/wordlists/slowa.txt";
const char* savefile = "saves/save2.hangman";

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

std::wstring getWord2(int line_num){
// char* getWord2(int line_num){
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
    // fgets(line, sizeof line, stdin)
    for(int i=0; i<line_num; i++){
        fscanf(wordlist, "%[^\n] ", current_line);
        // fscanf(wordlist, "%[^\n]", current_line);
        // fgets(current_line, sizeof current_line, wordlist);
    }
    fclose(wordlist);

    std::string str(current_line, current_line + strlen(current_line)-1);
    std::wstring wstr = stringToWstring(str);

    return wstr;
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

int playerHasWon(std::wstring word_hidden){
    for (int i=0; i<word_hidden.size(); i++) {
        if (word_hidden[i] == '_') {
            return 0;
        }
    }
    return 1;
}

int isLetterInWstring(std::wstring word, std::wstring letter){
    for (int i=0; i<word.size(); i++) {
        if (word[i] == letter[0]) {
            return 1;
        }
    }
    return 0;
}

// std::string getWordstr(){
//     std::ifstream wordlist;
//     wordlist.open(filename);
//     std::string line;
//     int line_num = getRandomNum();
//     for (int i=0; i<line_num; i++) {
//         wordlist>>line;
//     }
//     return line;
// }

std::wstring hideWord(std::wstring word){
    std::wstring word_hidden = L"";
    for (int i=0; i<word.size(); i++) {
        word_hidden+=L'_';
    }
    return word_hidden;
}

sf::Text center(sf::Text object, float x, float y){
    sf::FloatRect objectRect = object.getLocalBounds();
    object.setOrigin(objectRect.width/2,objectRect.height/2);
    object.setPosition(sf::Vector2f(SCREEN_X/x,SCREEN_Y/y));
    return object;
}

sf::Text applyStyle(sf::Text object, sf::Font &font, int size){
    object.setFont(font);
    object.setCharacterSize(size);
    object.setStyle(sf::Text::Bold);
    return object;
}

std::string getLine(int line_num, const std::string filename){

	std::string line;
	std::ifstream file(filename);
    for(int i=0; i<line_num; i++){
		// file >> line;
        std::getline(file, line);
    }
	// file.close();
    return line;
}

std::wstring getwLine(int line_num, const std::string filename){

	std::wstring line;
	std::wifstream file(filename, std::ios::binary);
    std::wstring content((std::istreambuf_iterator<wchar_t>(file)),
    {});
    for(int i=0; i<line_num; i++){
		// file >> line;
        // std::getline(file, line);
    }
	// file.close();
    std::wcout<<line<<std::endl;
    return line;
}

std::wstring loadSaveUtil(int line_num, std::string filename){
    std::wifstream file(filename);
    std::wstring line;
    for (int i=0; i<line_num; i++) {
        file>>line;
    }
    return line;
}

int main(void){
    // std::ios_base::sync_with_stdio(false);
    // std::wcout.imbue( std::locale( "pl_PL.UTF-8" ) );
    // setlocale(LC_ALL, "pl_PL.UTF-8");
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
