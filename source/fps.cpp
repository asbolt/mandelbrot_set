#include "fps.h"

void print_fps (sf::Clock* clock, sf::Time* current_time, sf::Time* previous_time, sf::RenderWindow* win)
{
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