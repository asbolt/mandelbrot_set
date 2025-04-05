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

    for (int col = 0; col < WIDTH; col++)
    {
        for (int row = 0; row < HEIGHT; row += 8)
        {
            float array_x0 [8] = {0};
            for (int i = 0; i < 8; i++)
            {
                array_x0 [i] = (float)(row + i)/250 - 2.2 + x_center;
            }

            float array_y0 [8] = {0};
            for (int i = 0; i < 8; i++)
            {
                array_y0 [i] = (float)(col)/250 - 1.2 + x_center;
            }
        
            float array_x [8] = {0};
            float array_y [8] = {0};
            int array_step [8] = {0};

            int cmp [8] = {0};
            for (int i = 0; i < 8; i++)
            {
                cmp[i] = 1;
            }

            for (size_t step_cnt = 0; step_cnt < MAX_STEP; step_cnt++)
            {
                float x2[8] = {0};
                float y2[8] = {0};
                float xy[8] = {0};
                float dist2[8] = {0};

                for (int i = 0; i < 8; i++)
                {
                    x2[i] = array_x[i] * array_x[i];
                    y2[i] = array_y[i] * array_y[i];
                    xy[i] = array_x[i] * array_y[i];
                    dist2[i] = x2[i] + y2[i];
                }

                for (size_t el = 0; el < 8; el++)
                    if (dist2[el] > MAX_DIST)                   
                        cmp[el] = 0;
                    
                int mask = 0;
                for (size_t el = 0; el < 8; el++)         
                    mask += cmp[el];

                if (!mask)                                      
                    break;
            
                for (size_t el = 0; el < 8; el++)
                {
                    array_x[el] = x2[el] - y2[el] + array_x0[el];
                    array_y[el] = xy[el] + xy[el] + array_y0[el];
                    array_step[el] = array_step[el] + cmp[el];
                }
            }

            for (int a = 0; a < 8; a++)
                image.setPixel (row + a, col, sf::Color(array_step[a], 100, 100, 100));
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