#include "mandelbrot.h"

void draw_mandelbrot (float scale, float x_center, float y_center, sf::RenderWindow& window)
{
    float x0 = 0;
    float y0 = 0;

    sf::Image image;
    image.create (WIDTH, HEIGHT, sf::Color::Blue);

    for (int row = 0; row < HEIGHT; row++)
    {
            for (int col = 0; col < WIDTH; col++)
        {
            x0 = col*scale + X_OFFSET*scale + x_center;
            y0 = row*scale + Y_OFFSET*scale + y_center;
        
            float x = 0;
            float y = 0;
            int step = 0;

            for (; step < MAX_STEP; step++)
            {
                float x2 = x * x;
                float y2 = y * y;
                float xy = x * y;
                float dist2 = x2 + y2;
                if (dist2 > MAX_DIST) break;

                x = x2 - y2 + x0;
                y = xy + xy + y0;
            }
            
            if (step == MAX_STEP)
                image.setPixel (col, row, sf::Color(80, 0, 0, 255));
            else
                image.setPixel (col, row, sf::Color(200, 0, (step%5 + 1) * 50, 255));
        }
    }
    sf::Texture texture;
	texture.loadFromImage (image);
 
	sf::Sprite sprite;
	sprite.setTexture (texture);
	sprite.setPosition (0, 0);

    window.clear();
	window.draw (sprite);
	window.display();
}

void draw_mandelbrot_arrays (float scale, float x_center, float y_center, sf::RenderWindow& window)
{
    float x0 = 0;
    float y0 = 0;

    sf::Image image;
    image.create(WIDTH, HEIGHT, sf::Color::Blue);

    for (int row = 0; row < HEIGHT; row++)
    {
        for (int col = 0; col < WIDTH; col += VEC_LEN)
        {
            float x0 [VEC_LEN] = {0};
            for (int element = 0; element < VEC_LEN; element++)
            {
                x0 [element] = (col + element)*scale + X_OFFSET*scale + x_center;
            }

            float y0 [VEC_LEN] = {0};
            for (int element = 0; element < VEC_LEN; element++)
            {
                y0 [element] = row*scale + Y_OFFSET*scale + y_center;
            }
        
            float x [VEC_LEN] = {0};
            float y [VEC_LEN] = {0};
            int step [VEC_LEN] = {0};

            int cmp [VEC_LEN] = {0};
            for (int element = 0; element < 8; element++)
            {
                cmp[element] = 1;
            }

            for (size_t step_cnt = 0; step_cnt < MAX_STEP; step_cnt++)
            {
                float x2[VEC_LEN] = {0};
                float y2[VEC_LEN] = {0};
                float xy[VEC_LEN] = {0};
                float dist2[VEC_LEN] = {0};

                for (int element = 0; element < VEC_LEN; element++)
                {
                    x2[element] = x[element] * x[element];
                    y2[element] = y[element] * y[element];
                    xy[element] = x[element] * y[element];
                    dist2[element] = x2[element] + y2[element];
                }

                for (size_t element = 0; element < VEC_LEN; element++)
                    if (dist2[element] > MAX_DIST)                   
                        cmp[element] = 0;
                    
                int mask = 0;
                for (size_t element = 0; element < VEC_LEN; element++)         
                    mask += cmp[element];

                if (!mask) break;
            
                for (size_t element = 0; element < VEC_LEN; element++)
                {
                    x[element] = x2[element] - y2[element] + x0[element];
                    y[element] = xy[element] + xy[element] + y0[element];
                    step[element] = step[element] + cmp[element];
                }
            }

            for (int element = 0; element < VEC_LEN; element++)
            {
                if (step[element] == MAX_STEP)
                    image.setPixel (col + element, row, sf::Color(80, 0, 0, 255));
                else
                    image.setPixel (col + element, row, sf::Color(200, 0, (step[element]%5 + 1) * 50, 255));
            }
        }
    }
    sf::Texture texture;
	texture.loadFromImage (image);
 
	sf::Sprite sprite;
	sprite.setTexture (texture);
	sprite.setPosition (0, 0);

    window.clear();
	window.draw (sprite);
	window.display();
}

void draw_mandelbrot_SIMD (float scale, float x_center, float y_center, sf::RenderWindow& window)
{
    sf::Image image;
    image.create (WIDTH, HEIGHT, sf::Color::Blue);

    __m256 max_dist  = _mm256_set1_ps (MAX_DIST);
    __m256 delta_vec = _mm256_set1_ps (scale);
    __m256 x_offset  = _mm256_set1_ps (X_OFFSET*scale + x_center);
    __m256 y_offset  = _mm256_set1_ps (Y_OFFSET*scale + y_center);

    for (size_t row = 0; row < HEIGHT; row++)
    {
        for (size_t col = 0; col < WIDTH; col += VEC_LEN)
        {
            __m256 y0 = _mm256_set1_ps (row);
            y0 = _mm256_mul_ps (y0, delta_vec);
            y0 = _mm256_add_ps (y0, y_offset);

            __m256 x0 = _mm256_set_ps (col + 7, col + 6, col + 5, col + 4, col + 3, col + 2, col + 1, col);
            x0 = _mm256_mul_ps (x0, delta_vec);
            x0 = _mm256_add_ps (x0, x_offset);

            __m256 x = _mm256_setzero_ps();
            __m256 y = _mm256_setzero_ps();
            __m256i step = _mm256_setzero_si256();

            for (size_t step_cnt = 0; step_cnt < MAX_STEP; step_cnt++)
            {
                __m256 x2    = _mm256_mul_ps (x, x);
                __m256 y2    = _mm256_mul_ps (y, y);
                __m256 xy    = _mm256_mul_ps (x, y);
                __m256 dist2 = _mm256_add_ps (x2, y2);

                __m256 cmp = _mm256_cmp_ps (dist2, max_dist, _CMP_LE_OS);

                int mask = _mm256_movemask_ps (cmp);

                if (!mask) break;

                __m256i cmp_int = _mm256_castps_si256 (cmp);
                cmp_int  = _mm256_srli_epi32 (cmp_int, 31);
                step = _mm256_add_epi32 (step, cmp_int);

                x = _mm256_sub_ps (x2, y2);
                x = _mm256_add_ps (x, x0);

                y = _mm256_add_ps (xy, xy);
                y = _mm256_add_ps (y, y0);
            }

            uint32_t* size_t_step = (uint32_t*)(&step);

            for (int element = 0; element < VEC_LEN; element++)
            {
                if (size_t_step[element] == MAX_STEP)
                    image.setPixel (col + element, row, sf::Color(80, 0, 0, 255));
                else
                    image.setPixel (col + element, row, sf::Color(200, 0, (size_t_step[element]%5 + 1) * 50, 255));
            }
        }
    }

    sf::Texture texture;
	texture.loadFromImage (image);
 
	sf::Sprite sprite;
	sprite.setTexture (texture);
	sprite.setPosition (0, 0);

    window.clear();
	window.draw (sprite);
	window.display();

}

