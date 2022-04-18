#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdlib>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
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
        perror("Unable to read the wordlist\n");
        exit(0);
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
    printf("%s", word);
    sf::RenderWindow window(sf::VideoMode(1000,1000), "Hangman");

    sf::Font font;
    if(!font.loadFromFile("fonts/NotoSansMono-Regular.ttf")){
        throw("COULDN'T LOAD FONT");
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setStyle(sf::Text::Bold);
    text.setString("Arch Is The Best");
    text.setPosition(0,0);

    sf::Event event;

    sf::RectangleShape begin_game;
    begin_game.setSize(sf::Vector2f(100, 100));
    begin_game.setFillColor(sf::Color::Green);
    begin_game.setOutlineThickness(2);
    begin_game.setPosition(400, 400);

    sf::RectangleShape end_game;
    end_game.setSize(sf::Vector2f(100, 100));
    end_game.setFillColor(sf::Color::Red);
    end_game.setOutlineThickness(2);
    end_game.setPosition(600, 400);
    bool display = true;

    while (window.isOpen()) {
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            if (event.type == sf::Event::TextEntered) {

            }
        }
        window.clear();
        window.draw(text);
        window.draw(begin_game);
        window.draw(end_game);
        window.display();
    }
    return 0;
}

