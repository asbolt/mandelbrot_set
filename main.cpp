#include "mandelbrot.h"
#include "fps.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main (int argc, char *argv[])
{
    sf::RenderWindow win (sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot");
    
    float x_center =     0;
    float y_center =     0;
    float scale    = SCALE;

    sf::Clock clock;
    sf::Time previous_time = clock.getElapsedTime();
    sf::Time current_time;

    while (win.isOpen())
    {
        sf::Event event;
        while (win.pollEvent (event))
        {
            if (event.type == sf::Event::Closed)                    
                win.close();

            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Right:
                        x_center += DELTA_CRD;
                        break;

                    case sf::Keyboard::Left:
                        x_center -= DELTA_CRD;
                        break;

                    case sf::Keyboard::Down:
                        y_center += DELTA_CRD;
                        break;

                    case sf::Keyboard::Up:
                        y_center -= DELTA_CRD;
                        break;

                    case sf::Keyboard::Comma:
                        scale += DELTA_SCALE;
                        break;

                    case sf::Keyboard::Period:
                        scale -= DELTA_SCALE;
                        break;

                    case sf::Keyboard::Enter:
                        win.close();
                
                    default:
                        break;
                }
            }
        }


        int opt_type = atoi (argv[1]);
        switch (opt_type)
        {
        case NO_OPT:
            draw_mandelbrot (scale, x_center, y_center, win);
            break;

        case ARRAYS:
            draw_mandelbrot_arrays (scale, x_center, y_center, win);
            break;

        case SIMD:
            draw_mandelbrot_SIMD (scale, x_center, y_center, win);
            break;
        
        default:
            printf ("Incorrect optimization type\n");
            break;
        }

        print_fps (clock, current_time, previous_time, win);
        win.display();
    }

}