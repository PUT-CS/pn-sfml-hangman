#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdlib>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#define NUM_OF_WORDS 675956

const char* filename = "slowolista.txt";

int getRandomNum(){
    int lower = 0;
    int upper = NUM_OF_WORDS;
    int num = (rand() % (upper - lower + 1)) + lower;
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
        puts("Unable to read the wordlist\n");
        exit(1);
    }

    for(int i=0; i<=line_num; i++){
        fscanf(wordlist, "%[^\n] ", current_line);
    }
    fclose(wordlist);
    return current_line;
}

int main(void){

    srand(time(NULL));
    char* word = getWord();
    int length = strlen(word);
    char word_hidden[length];
    printf("strlen: %zu\n", strlen(word));
    for (int i=0; i<length; i++) {
        word_hidden[i] = '_';
    }
    printf("word: %s\nhidden word: %s\n", word, word_hidden);

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

    while (window.isOpen()) {
        while (window.pollEvent(event)){

            if (event.type == sf::Event::Closed){
                window.close();
            }
            if (event.type == sf::Event::TextEntered) {
                //window.close();
            }
        }
        window.clear();
        window.draw(title);
        window.draw(main_menu);
        window.draw(authors);
        window.draw(begin_game_button);
        window.draw(end_game_button);
        window.draw(new_game);
        window.draw(load_save);
        window.draw(year);
        window.display();
    }
    return 0;
}

