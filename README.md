# Множество Мандельброта

Данный проект демонстрирует несколько оптимизаций машинных вычислений на примере рендеринга множества Мандельброта
  

## Введение
Множество мандельброта -- это «множество таких точек C на комплексной плоскости, для которых рекуррентное соотношение $Z_{n+1} = Z_n^2 + C$ при $Z_0=0$ задаёт ограниченную последовательность»

Иными словами, координаты каждой точки комплексной плоскости циклически обрабатываются следующим образом: 
> $P_n = P_{n-1} ^ 2 + P_0$ 

где $P_0$ - начальные координаты точки, $P_n$ координаты точки на n-ой итерации  

B конце каждой итерации рассчитывается расстояние от точки, полученной на n-ой итерации до начала сиситемы координат. Если за определенное число шагов координаты точки не покинули заданного радиуса, то начальная точка принадлежит множеству (закрашена черным цветом). Выглядит это примерно так:  

![Множество Мандельброта](https://github.com/asbolt/mandelbrot_set/raw/main/images/mandelbrot.png)

Множетсво Мандельброта является примером фрактала в математике.  
  
    
## Скачивание и установка

Эта программа предназначена для систем с архитектурой Intel x86_64 и Linux в качестве ОС. Стабильная работа при запуске на системах с другой архитектурой/ОС не гарантируется.

Для скачивания и сброрки программы в примере ниже используются утилиты git, make и sfml. В качестве аргумента optimization_type могут выступать значения приведенные в таблице.

 ```
git clone https://github.com/asbolt/mandelbrot_set.git
mkdir build
make
./mandelbrot optimization_type
```
|Значение аргумета|Вид оптимизации|
|-|:-:|
|1| без оптимизаций|
|2| использование массивов|
|3| использование intrinsic-ов|

## Внешний вид и горячие клавиши

В верхнем левом углу изображения указано текущее время, необходимое для рендера одного кадра, в тиках процессора

![Внешний вид](https://github.com/asbolt/mandelbrot_set/raw/main/images/3.png)
![Приближенное изображение](https://github.com/asbolt/mandelbrot_set/raw/main/images/enlarged_image.png)

Список горячих клавиш используемых программой:

| Клавиша | Действие |
|-|-|
|↑| сдвиг вверх|
|↓| сдвиг вниз|
|←| сдвиг влево|
|→| сдвиг вправо|
|>| приближение изображения|
|<| отдаление изображения|


## Виды рендеринга

### Рендеринг без оптимизаций
Здесь и далее от номера итерации, на котором точка покинула заданный радиус, определяет цвет, в который она будет окрашена.

В первом варианте рендеринга используется наивный и понятный рассчет этого номера. С помощью двойного цикла осуществляется проход по каждому пикселю.

```
float x0 = 0;
float y0 = 0;

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
            image->setPixel (col, row, sf::Color(80, 0, 0, 255));
        else
            image->setPixel (col, row, sf::Color(200, 0, (step%5 + 1) * 50, 255));
    }
}
```

![Рендеринг без оптимизаций](https://github.com/asbolt/mandelbrot_set/raw/main/images/1.png)

### Рендеринг с использованием массивов

Этот вариант рендеринга похож на предыдущий, однако пиксели в нем обрабатываются в массивах по 8 штук. С флагом -O3 это позволяет компилятору использовать SIMD-инструкции, ускоряющие выполнение кода.

SIMD (англ. single instruction, multiple data — одиночный поток команд, множественный поток данных) - "принцип компьютерных вычислений, позволяющий обеспечить параллелизм на уровне данных." В этом режиме одна и та же операция применяется сразу к нескольким разным значениям данных. На протяжении многих лет эти расширения продолжали развиваться, и после каждого существенного изменения их названия менялись, а совсем недавно они стали называться AVX. 

SIMD-расширения используют для управления данными набор регистров, называемых "MM" регистрами. Для AVX это "YMM" - 32-байтные регистры, способные хранить восемь 32-разрядных значений или четыре 64-разрядных. Исходя из этого выбран размер массивов - 8, так как при обработке пикселей их координаты представлены типом данных float (4 байта).

```
float x0 = 0;
float y0 = 0;

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
                image->setPixel (col + element, row, sf::Color(80, 0, 0, 255));
            else
                image->setPixel (col + element, row, sf::Color(200, 0, (step[element]%5 + 1) * 50, 255));
        }
    }
}
```

![Рендеринг с использованием массивов](https://github.com/asbolt/mandelbrot_set/raw/main/images/2.png)


### Рендеринг с использованием SIMD intrinsic
intrinsic-и являются набором особых функций компилятора. При выполнении кода с ними происходит не вызов функций, а непосредственная вставка инструкций выполняющих ту или иную операцию. Использование intrinsic-ов значительно ускоряет программу

```
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
    }
}
```

![Рендеринг с использованием intrinsic-ов](https://github.com/asbolt/mandelbrot_set/raw/main/images/3.png)


## Выводы
Ниже приведена таблица с количеством тиков процессора, необходимых для рендеринга одного кадра, для каждого метода. Измерение времени именно в тиках процессора выбрано для максимальной точности данных, так как другие методы измерения времени (например, встроенные в графическую библиотеку sfml) имеют значительную погрешность для измеряемых интервалов времени.

|Без оптимизаций| Массивы |intrinsic|
|:-:|:-:|:-:|
|123000000|65000000|23500000|

Таким образом, можем видеть, что использование массивов (при компиляции с флагом -O3) ускоряет программу в 2 раза, а использование intrinsic-ов в 5 раз. Таким образом, было наглядно продемонстрировано, что использование SIMD-инструкций способно многократно ускорить программу.