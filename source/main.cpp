#include "mandelbrot.h"
#include "program_control.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <immintrin.h>

int main (int argc, char *argv[])
{
    sf::RenderWindow window (sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot");
    
    float x_center =     0;
    float y_center =     0;
    float scale    = SCALE;

    __uint64_t previous_time = 0;
    __uint64_t current_time  = 0;

    while (window.isOpen())
    {
        sf::Event event;
        window_control (&scale, &y_center, &x_center, &event, &window);

        int opt_type = atoi (argv[1]);
        previous_time = __rdtsc();
        sf::Image image;
        image.create (WIDTH, HEIGHT);
        draw_mandelbrot (opt_type, scale, x_center, y_center, &window, &image);

        sf::Texture texture;
	    texture.loadFromImage (image);
 
	    sf::Sprite sprite;
	    sprite.setTexture (texture);
	    sprite.setPosition (0, 0);

        window.clear();
	    window.draw (sprite);
	    window.display();
        
        print_proc_tiks (&current_time, &previous_time, &window);
        window.display();
    }

}