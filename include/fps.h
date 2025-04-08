#ifndef FPS_H_
#define FPS_H_

#include "mandelbrot.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

const size_t FPS_STR_LEN = 4;

void print_fps (sf::Clock& clock, sf::Time& current_time, sf::Time& previous_time, sf::RenderWindow& win);

#endif