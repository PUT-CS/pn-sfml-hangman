#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <string.h>
#define NUM_OF_WORDS 610875
// #define MAX_LETTERS 14

const char* filename = "slowolista.txt";
int mode = 0;
int fails = 0;
// 0 - menu
// 1 - gra

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
            return L"Error";
            break;
    }
}

int main(void){
    srand(time(NULL));
    char* word = getWord();
    int length = strlen(word);
    // char word_hidden[length];
    std::wstring word_hidden;
    printf("strlen: %zu\n", strlen(word));

    for (int i=0; i<length; i++) {
        word_hidden += '_';
        if (i != length-1) {
            word_hidden += ' ';
        }
    }

    sf::RenderWindow window(sf::VideoMode(1000,1000), "Hangman");
    sf::Font font;
    if(!font.loadFromFile("fonts/NotoSansMono-Regular.ttf")){
        puts("COULDN'T LOAD FONT");
        exit(1);
    }

    sf::Event event;
    sf::Color DarkGreen(16,130,0);
    sf::Color DarkRed(150,0,0);
    sf::Color Gray(90,90,90);

    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(80);
    title.setStyle(sf::Text::Bold);
    title.setString(L"Hangman");
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
    sf::FloatRect hidden_wordRect = hidden_word.getLocalBounds();
    hidden_word.setOrigin(hidden_wordRect.width/2,hidden_wordRect.height/2);
    hidden_word.setPosition(sf::Vector2f(1000/2.0f,1000/2.0f));

    /*
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f,
        textRect.top  + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(SCRWIDTH/2.0f,SCRHEIGHT/2.0f))
    */

    while (window.isOpen()) {
        while (window.pollEvent(event)){

            if (event.type == sf::Event::Closed){
                window.close();
            }
            if(event.type == sf::Event::TextEntered){
                if (event.text.unicode > 128) {
                    std::wstring string = getPolishLetter(event.text.unicode);
                    load_save.setString(string);
                    mode=1;
                }
                else {
                    load_save.setString(event.text.unicode);
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
            window.draw(authors);
            window.draw(year);
            window.display();
        }
    }
    return EXIT_SUCCESS;
}
