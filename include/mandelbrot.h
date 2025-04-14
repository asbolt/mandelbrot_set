#ifndef MANDELBROT_H_
#define MANDELBROT_H_

#include <math.h>
#include <stdlib.h>
#include <immintrin.h>
#include <assert.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

enum OPTIMIZATION_TYPE 
{
    NO_OPT = 1,
    ARRAYS = 2,
    SIMD   = 3
};

const size_t       HEIGHT       =     600; 
const size_t       WIDTH        =     800;
const size_t       MAX_STEP     =     200;
const float        MAX_DIST     =     100;
const size_t       VEC_LEN      =       8;
const float        X_OFFSET     =    -550;
const float        Y_OFFSET     =    -300;
const float        DELTA_CRD    =       5;
const float        DELTA_SCALE  =   0.025;
const float        SCALE        =   0.004;

void draw_mandelbrot (int opt_type, float scale, float x_center, float y_center, sf::RenderWindow* window, sf::Image* image);
void draw_mandelbrot_simple(float scale, float x_center, float y_center, sf::RenderWindow* window, sf::Image* image);
void draw_mandelbrot_arrays(float scale, float x_center, float y_center, sf::RenderWindow* window, sf::Image *image);
void draw_mandelbrot_intrinsics(float scale, float x_center, float y_center, sf::RenderWindow* window, sf::Image *image);

#endif