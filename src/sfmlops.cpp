#include <SFML/Graphics/Color.hpp>
#include <iostream>
#include "sfmlops.hpp"
#include "config.hpp"

sf::Text center(sf::Text object, float x, float y){
    sf::FloatRect objectRect = object.getLocalBounds();
    object.setOrigin(objectRect.width/2,objectRect.height/2);
    object.setPosition(sf::Vector2f(SCREEN_X/x,SCREEN_Y/y));
    return object;
}
sf::Text applyStyle(sf::Text object, sf::Font &font, int size, sf::Color color, float multiplier){
    object.setFont(font);
    object.setCharacterSize(size*multiplier);
    object.setStyle(sf::Text::Bold);
    object.setFillColor(color);
    return object;
}
