#ifndef PROGRAM_CONTROL_H_
#define PROGRAM_CONTROL_H_

#include "mandelbrot.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

const size_t FPS_STR_LEN = 4;

void print_fps (sf::Clock* clock, sf::Time* current_time, sf::Time* previous_time, sf::RenderWindow* win);
void window_control (float *scale, float *y_center, float *x_center, sf::Event* event, sf::RenderWindow* window);

#endif