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
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <locale>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <string.h>
#include <fstream>
#define NUM_OF_WORDS 610875

const char* filename = "slowa.txt";
int mode = 0;
int fails = -1;
// 0 - menu
// 1 - gra
// 2 - end screen

int getRandomNum(){
    int lower = 0;
    int num = (rand() % (NUM_OF_WORDS - lower + 1)) + lower;
    printf("%d\n", num);
    return num;
}

char* getWord(){
    FILE *wordlist;
    char* word;
    char* line = NULL;
    int line_num = getRandomNum();

    struct stat sb;
    stat(filename, &sb);
    char *current_line = (char*)malloc(sb.st_size);

    wordlist = fopen(filename, "r");
    if (wordlist == NULL) {
        printf("Unable to read the %s\n", filename);
        exit(1);
    }

    for(int i=0; i<=line_num; i++){
        fscanf(wordlist, "%[^\n] ", current_line);
    }
    fclose(wordlist);
    return current_line;
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

std::wstring stringToWstring(std::string word){
    static std::wstring output = L"";
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

std::string getWordstr(){
    std::ifstream wordlist;
    wordlist.open(filename);
    std::string line;
    int line_num = getRandomNum();
    for (int i=0; i<line_num; i++) {
        wordlist>>line;
    }
    return line;
}



// int findWstr(std::wstring wstr, std::wstring letter){
//     for (int i=0; i<wstr.size(); i++) {
//         if (wstr[i] == letter[0]) {
//             return 1;
//         }
//     }
//     return 0;
// }

std::wstring hideWord(std::wstring word){
    std::wstring word_hidden = L"";
    for (int i=0; i<word.size(); i++) {
        word_hidden+=L'_';
    }
    return word_hidden;
}

int main(void){
    setlocale(LC_ALL, "pl_PL.UTF-8");
    srand(time(NULL));

    char* dirty_word = getWord();
    std::string wordstr(dirty_word, dirty_word + strlen(dirty_word)-1);
    // std::string dirty_word = getWordstr();
    std::wstring word = stringToWstring(wordstr);
    // std::wstring word = stringToWstring(dirty_word);
    std::wstring word_hidden;
    word_hidden = hideWord(word);

    std::wcout<<word_hidden<<std::endl;


    sf::RenderWindow window(sf::VideoMode(1000,1000), "Hangman");
    sf::Font font;
    if(!font.loadFromFile("fonts/NotoSansMono-Regular.ttf")){
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
    title.setFont(font);
    title.setCharacterSize(80);
    title.setStyle(sf::Text::Bold);
    title.setString(word);
    // title.setString(L"Hangman");
    sf::FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.width/2,titleRect.height/3);
    title.setPosition(sf::Vector2f(1000/2.0f,1000/7.0f));

    sf::Text main_menu;
    main_menu.setFont(font);
    main_menu.setCharacterSize(56);
    main_menu.setStyle(sf::Text::Bold);
    main_menu.setString(L"Main Menu");
    sf::FloatRect main_menuRect = main_menu.getLocalBounds();
    main_menu.setOrigin(main_menuRect.width/2,main_menuRect.height/3);
    main_menu.setPosition(sf::Vector2f(1000/2.0f,1000/2.8f));

    sf::Text authors;
    authors.setFont(font);
    authors.setCharacterSize(20);
    authors.setStyle(sf::Text::Bold);
    authors.setString(L"Michał Miłek & Sebastian Nowak");
    sf::FloatRect authorsRect = authors.getLocalBounds();
    authors.setOrigin(authorsRect.width/2,authorsRect.height/3);
    authors.setPosition(sf::Vector2f(1000/2.0f,1000/1.1f));

    sf::Text year;
    year.setFont(font);
    year.setCharacterSize(30);
    year.setStyle(sf::Text::Bold);
    year.setString("2022");
    sf::FloatRect yearRect = year.getLocalBounds();
    year.setOrigin(yearRect.width/2,yearRect.height/2);
    year.setPosition(sf::Vector2f(1000/2.0f,1000/1.05f));

    sf::Text new_game;
    new_game.setFont(font);
    new_game.setCharacterSize(30);
    new_game.setStyle(sf::Text::Bold);
    new_game.setString("New Game");
    sf::FloatRect new_gameRect = new_game.getLocalBounds();
    new_game.setPosition(sf::Vector2f(985/2.0f,1000/2.03f));
    new_game.setOrigin(new_gameRect.width/2,new_gameRect.height/2);

    sf::Text load_save;
    load_save.setFont(font);
    load_save.setCharacterSize(30);
    load_save.setStyle(sf::Text::Bold);
    load_save.setString("Load Save");
    sf::FloatRect load_saveRect = load_save.getLocalBounds();
    load_save.setPosition(sf::Vector2f(985/2.0f,1000/1.52f));
    load_save.setOrigin(load_saveRect.width/2, load_saveRect.height/2);

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
    hidden_word.setFont(font);
    hidden_word.setCharacterSize(50);
    hidden_word.setStyle(sf::Text::Bold);
    hidden_word.setString(word_hidden);
    // hidden_word.setString(L"Hidden word");
    hidden_word.setLetterSpacing(3.0f);
    sf::FloatRect hidden_wordRect = hidden_word.getLocalBounds();
    hidden_word.setOrigin(hidden_wordRect.width/2,hidden_wordRect.height/2);
    hidden_word.setPosition(sf::Vector2f(1000/2.0f,1000/2.0f));

    /*
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f,
        textRect.top  + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(SCRWIDTH/2.0f,SCRHEIGHT/2.0f))
    */
    std::wstring letter;
    std::wstring used;

    while (window.isOpen()) {
        while (window.pollEvent(event)){

            if (event.type == sf::Event::Closed){
                window.close();
            }
            if (mode == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                mode = 1;
            }
            if(event.type == sf::Event::TextEntered){
                if (mode == 1) {
                    if (event.text.unicode > 128) {
                        letter = getPolishLetter(event.text.unicode);
                        word_hidden = fillWord(word, word_hidden, letter);
                        hidden_word.setString(word_hidden);
                        if (!isLetterInWstring(used,letter)) {
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
            window.draw(title);
            if (mode == 0) {
                window.draw(main_menu);
                window.draw(begin_game_button);
                window.draw(end_game_button);
                window.draw(new_game);
                window.draw(load_save);
            }
            if (mode == 1) {
                window.draw(hidden_word);
            }
            if (mode == 2) {
                title.setFont(font);
                title.setString(L"You have won!");
                sf::FloatRect titleRect = title.getLocalBounds();
                title.setOrigin(titleRect.width/2,titleRect.height/2);
                title.setPosition(sf::Vector2f(1000/2.0f,1000/3.0f));
                window.draw(title);
            }
            window.draw(authors);
            window.draw(year);
            window.display();
        }
    }
    return EXIT_SUCCESS;
}
