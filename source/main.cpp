#include "mandelbrot.h"
#include "program_control.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main (int argc, char *argv[])
{
    sf::RenderWindow window (sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot");
    
    float x_center =     0;
    float y_center =     0;
    float scale    = SCALE;

    sf::Clock clock;
    sf::Time previous_time = clock.getElapsedTime();
    sf::Time current_time;

    while (window.isOpen())
    {
        sf::Event event;
        window_control (&scale, &y_center, &x_center, &event, &window);

        int opt_type = atoi (argv[1]);
        previous_time = clock.getElapsedTime();
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
        
        print_fps (&clock, &current_time, &previous_time, &window);
        window.display();
    }

}