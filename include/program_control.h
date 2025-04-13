#ifndef PROGRAM_CONTROL_H_
#define PROGRAM_CONTROL_H_

#include "mandelbrot.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

const size_t PROC_TIKS_STR_LEN = 20;

void print_proc_tiks (__uint64_t* current_time, __uint64_t* previous_time, sf::RenderWindow* win);
void window_control (float *scale, float *y_center, float *x_center, sf::Event* event, sf::RenderWindow* window);

#endif