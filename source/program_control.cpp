#include "program_control.h"

void print_fps (sf::Clock* clock, sf::Time* current_time, sf::Time* previous_time, sf::RenderWindow* win)
{

    // TODO rdtsc, CLOCKS_PER_SEC

    *current_time = clock->getElapsedTime();
    float fps = 1.0 / (current_time->asSeconds() - previous_time->asSeconds());
    int fps_int  = (int)fps;
        
    char fps_str[FPS_STR_LEN] = {};
    int snprintf_res = snprintf(fps_str, FPS_STR_LEN, "%.0f", fps);

    sf::Font font;
    font.loadFromFile ("././include/font/font.ttf");

    sf::Text text("", font, 25);
    text.setFillColor(sf::Color::Black);
    text.setString(fps_str);
    win->draw(text);

    return;
}

void window_control (float *scale, float *y_center, float *x_center, sf::Event* event, sf::RenderWindow* window)
{
    while (window->pollEvent (*event))
    {
        if (event->type == sf::Event::Closed)         
            window->close();

        if (event->type == sf::Event::KeyPressed)
        {
            switch (event->key.code)
            {
                case sf::Keyboard::Right:
                    *x_center += DELTA_CRD * *scale;
                    break;

                case sf::Keyboard::Left:
                    *x_center -= DELTA_CRD * *scale;
                    break;

                case sf::Keyboard::Down:
                    *y_center += DELTA_CRD * *scale;
                    break;

                case sf::Keyboard::Up:
                    *y_center -= DELTA_CRD * *scale;
                    break;

                case sf::Keyboard::Comma:
                    *scale += DELTA_SCALE * *scale;
                    break;

                case sf::Keyboard::Period:
                    *scale -= DELTA_SCALE * *scale;
                    break;

                case sf::Keyboard::Enter:
                    window->close();
            
                default:
                    break;
            }
        }
    }
}