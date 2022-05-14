#ifndef sfmlops
#define sfmlops
#include <SFML/Config.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>

sf::Text center(sf::Text object, float x, float y);
sf::Text applyStyle(sf::Text object, sf::Font &font, int size);

#endif
