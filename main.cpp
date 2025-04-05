#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

const size_t HEIGHT   = 800; 
const size_t WIDTH    = 600;
const size_t MAX_STEP = 200;
const float  MAX_DIST = 100;

void temporary_stop_main_loop(sf::RenderWindow& win);
void draw_mandelbrot(float x_center, float y_center, sf::RenderWindow& win);
void color_pixel(size_t step, sf::RectangleShape& pixel);
int add_FPS(sf::Clock& clock, sf::Time& current_time, sf::Time& previous_time, sf::RenderWindow& win);

int main()
{
    sf::RenderWindow win (sf::VideoMode(HEIGHT, WIDTH), "Mandelbrot");
    
    float x_center = 0;
    float y_center = 0;

    sf::Clock clock;
    sf::Time previous_time = clock.getElapsedTime();
    sf::Time current_time;

    while (win.isOpen())
    {
        sf::Event event;
        while (win.pollEvent(event))
            if (event.type == sf::Event::Closed)                    
                win.close();

        draw_mandelbrot(x_center, y_center, win);
        int FPS = add_FPS(clock, current_time, previous_time, win);
        win.display();
    }
}

void draw_mandelbrot(float x_center, float y_center, sf::RenderWindow& window)
{
    float x0 = 0;
    float y0 = 0;

    sf::Image image;
    image.create(800,600, sf::Color::Blue);

    //win.clear(sf::Color::Black);

    for (int col = 0; col < WIDTH; col++)
    {
            for (int row = 0; row < HEIGHT; row++)
        {
            x0 = (float)(row)/250 - 2.2 + x_center;
            y0 = (float)(col)/250 - 1.2 + y_center;
        
            float x = 0;
            float y = 0;
            size_t step = 0;

            for (; step < MAX_STEP; step++)
            {
                float x2 = x * x;
                float y2 = y * y;
                float xy = x * y;
                float dist2 = x2 + y2;
                if (dist2 > MAX_DIST)   break;

                x = x2 - y2 + x0;
                y = xy + xy + y0;
            }
            
            image.setPixel (row, col, sf::Color(step, 100, 100, 100));
        }
    }
    sf::Texture herotexture;
	herotexture.loadFromImage(image);
 
	sf::Sprite herosprite;
	herosprite.setTexture(herotexture);
	herosprite.setPosition(0, 0);

    window.clear();
	window.draw(herosprite);
	window.display();
}

int add_FPS(sf::Clock& clock, sf::Time& current_time, sf::Time& previous_time, sf::RenderWindow& win)
{
    current_time = clock.getElapsedTime();
    float FPS = 1.0 / (current_time.asSeconds() - previous_time.asSeconds());
    int FPS_int  = (int)FPS;
    previous_time = current_time;
        
    char FPS_str[3] = {};
    int snprintf_res = snprintf(FPS_str, 3, "%.0f", FPS);

    sf::Font font;
    font.loadFromFile ("font.ttf");

    sf::Text text("", font, 25);
    text.setFillColor(sf::Color::Black);
    text.setString(FPS_str);
    win.draw(text);

    return FPS_int;
}