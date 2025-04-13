#include "program_control.h"

void print_proc_tiks (__uint64_t* current_time, __uint64_t* previous_time, sf::RenderWindow* win)
{

    *current_time = __rdtsc();
    float proc_tiks = *current_time - *previous_time;
    int proc_tiks_int  = (int)proc_tiks;
        
    char proc_tiks_str[PROC_TIKS_STR_LEN] = {};
    int snprintf_res = snprintf(proc_tiks_str, PROC_TIKS_STR_LEN, "%.0f", proc_tiks);

    sf::Font font;
    font.loadFromFile ("././include/font/font.ttf");

    sf::Text text("", font, 25);
    text.setFillColor(sf::Color::Black);
    text.setString(proc_tiks_str);
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