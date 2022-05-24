#ifndef sfmlops
#define sfmlops
#include <SFML/Config.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>

#define SCREEN_X 1000
#define SCREEN_Y 1000

sf::Text center(sf::Text object, float x, float y);
sf::Text applyStyle(sf::Text object, sf::Font &font, int size, sf::Color color, float multiplier);

#endif
