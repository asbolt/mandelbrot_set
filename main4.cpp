#include <math.h>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <immintrin.h>
#include <assert.h>
#include <cstring>

enum OptType
{
    NO_OPT,
    ARRAY,
    INTRIN
};

const size_t       HEIGHT       =  600; 
const size_t       WIDTH        =  800;
const size_t       MAX_STEP     =  200;
const float        MAX_DIST     =  100;
const size_t       VEC_LEN      =    8;
const float        DELTA        = 0.004;
const size_t       FPS_STR_LEN  =   4 ;
const int          ERRVAL       =  303;
const float        CENTER_DELTA =  0.3;
const float        X_OFFSET     = -2.2;
const float        Y_OFFSET     = -1.2;
const float        DELTA_ADD    = 0.0003;
const unsigned int COLOR        = 0xAAEE0011;

void draw_mandelbrot_SIMD(float x_center, float y_center, float delta, int * pixel_arr, bool graph);

int add_FPS(sf::Clock& clock, sf::Time& current_time, sf::Time& previous_time, sf::RenderWindow& win);
static void set_pixel(int* pixel_arr, size_t row, size_t col, size_t step);


int add_FPS(sf::Clock& clock, sf::Time& current_time, sf::Time& previous_time, sf::RenderWindow& win)
{
    current_time = clock.getElapsedTime();
    float FPS = 1.0 / (current_time.asSeconds() - previous_time.asSeconds());
    int FPS_int  = (int)FPS;
    previous_time = current_time;
        
    char FPS_str[FPS_STR_LEN] = {};
    int snprintf_res = snprintf(FPS_str, FPS_STR_LEN, "%.0f", FPS);
    if (snprintf_res >= FPS_STR_LEN)
    {
        printf("snprintf error\n");
        return ERRVAL;
    }

    sf::Font font;
    if (!font.loadFromFile("source/font.ttf"))
    {
        printf("font error\n");
        return ERRVAL;
    }

    sf::Text text("", font, 25);
    text.setFillColor(sf::Color::Black);
    text.setString(FPS_str);
    win.draw(text);

    return FPS_int;
}

void draw_mandelbrot_SIMD(float x_center, float y_center, float delta, int * pixel_arr, bool graph)
{
    assert(pixel_arr);

    __m256 max_dist  = _mm256_set1_ps(MAX_DIST);
    __m256 delta_vec = _mm256_set1_ps(delta);
    __m256 x_offset  = _mm256_set1_ps(X_OFFSET + x_center);
    __m256 y_offset  = _mm256_set1_ps(Y_OFFSET + y_center);

    for (size_t row = 0; row < HEIGHT; row++)
    {
        for (size_t col = 0; col < WIDTH; col += VEC_LEN)
        {
            __m256 y0 = _mm256_set1_ps(row);
            y0 = _mm256_mul_ps(y0, delta_vec);
            y0 = _mm256_add_ps(y0, y_offset);

            __m256 x0 = _mm256_set_ps(col + 7, col + 6, col + 5, col + 4, col + 3, col + 2, col + 1, col);
            x0 = _mm256_mul_ps(x0, delta_vec);
            x0 = _mm256_add_ps(x0, x_offset);

            __m256 x = _mm256_setzero_ps();
            __m256 y = _mm256_setzero_ps();
            __m256i step = _mm256_setzero_si256();

            for (size_t step_cnt = 0; step_cnt < MAX_STEP; step_cnt++)
            {
                __m256 x2    = _mm256_mul_ps(x, x);
                __m256 y2    = _mm256_mul_ps(y, y);
                __m256 xy    = _mm256_mul_ps(x, y);
                __m256 dist2 = _mm256_add_ps(x2, y2);

                __m256 cmp = _mm256_cmp_ps(dist2, max_dist, _CMP_LE_OS);

                int mask = _mm256_movemask_ps(cmp);

                if (!mask)  break;

                __m256i tmp = _mm256_castps_si256(cmp);
                tmp  = _mm256_srli_epi32(tmp, 31);
                step = _mm256_add_epi32(step, tmp);

                x = _mm256_sub_ps(x2, y2);
                x = _mm256_add_ps(x, x0);

                y = _mm256_add_ps(xy, xy);
                y = _mm256_add_ps(y, y0);
            }

            uint32_t* size_t_step = (uint32_t*)(&step);

            if (graph)
                for (int i = 0; i < VEC_LEN; i++)   set_pixel(pixel_arr, row, col + i, size_t_step[i]);
        }
    }
}

void set_pixel(int* pixel_arr, size_t row, size_t col, size_t step)
{
    if (step == MAX_STEP)  pixel_arr[row * 800 + col] = 0x00000000;
    else                   pixel_arr[row * 800 + col] = 0xCCCCCCCC - step * 16;
}

void temporary_stop_main_loop(sf::RenderWindow& win);

int main(int argc, char** argv)
{
    if (argc == 1 || argc > 3)
    {
        printf("wrong arguments amount\n");
        return ERRVAL;
    }

    bool graph = false;
    if (argv[2] && strcmp(argv[2], "-graph") == 0)
        graph = true;

    int opt_type = atoi(argv[1]);

    int* pixel_arr = (int*)calloc(HEIGHT * WIDTH, sizeof(int));
        
    float x_center = 0;
    float y_center = 0;
    float delta = DELTA;

    sf::Clock main_clock;

    if (graph)
    {
        sf::RenderWindow win (sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot");

        sf::Image image;
        image.create(WIDTH, HEIGHT, sf::Color::Black);
        sf::Texture texture = {};
        texture.loadFromImage(image);

        sf::Sprite sprite = {};
        sprite.setTexture(texture);

        sf::Clock clock;
        sf::Time previous_time = clock.getElapsedTime();
        sf::Time current_time;

        while (win.isOpen())
        {
            sf::Event event;
            while (win.pollEvent(event))
                if (event.type == sf::Event::Closed)                    win.close();
        
            if (event.type == sf::Event::LostFocus)                     temporary_stop_main_loop(win);
        
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))    x_center -= CENTER_DELTA;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))   x_center += CENTER_DELTA;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))      y_center -= CENTER_DELTA;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))    y_center += CENTER_DELTA; 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal))   delta    -= DELTA_ADD;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Dash))    delta    += DELTA_ADD; 
        

                draw_mandelbrot_SIMD(x_center, y_center, delta, pixel_arr, graph);
        }
    
        texture.update((sf::Uint8*)pixel_arr, WIDTH, HEIGHT, 0, 0);
        win.clear();
        win.draw(sprite);
    
        int FPS = add_FPS(clock, current_time, previous_time, win);
        if (FPS == ERRVAL)
            return 0;
    
        win.display();
    }

sf::Time prog_time = main_clock.getElapsedTime();
printf("prog time: %lg sec\n", prog_time.asSeconds());

return 0;

}

void temporary_stop_main_loop(sf::RenderWindow& win) 
{
sf::Event event;

while (true) 
    if (win.waitEvent(event) && event.type == sf::Event::GainedFocus)   return;
}