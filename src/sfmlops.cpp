#include <SFML/Graphics/Color.hpp>
#include <iostream>
#include "sfmlops.hpp"
#define SCREEN_X 1000
#define SCREEN_Y 1000

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
    object.setFillColor(sf::Color::White);
    return object;
}
