# Множество Мандельброта

Данный проект демонстрирует несколько оптимизаций машинных вычислений на примере рендеринга множества Мандельброта
  

## Введение
Множество мандельброта -- это «множество таких точек C на комплексной плоскости, для которых рекуррентное соотношение $Z_{n+1} = Z_n^2 + C$ при $Z_0=0$ задаёт ограниченную последовательность» [wiki](https://ru.wikipedia.org/wiki/%D0%9C%D0%BD%D0%BE%D0%B6%D0%B5%D1%81%D1%82%D0%B2%D0%BE_%D0%9C%D0%B0%D0%BD%D0%B4%D0%B5%D0%BB%D1%8C%D0%B1%D1%80%D0%BE%D1%82%D0%B0).

Иными словами, координаты каждой точки комплексной плоскости циклически обрабатываются следующим образом: 
> $P_n = P_{n-1} ^ 2 + P_0$ <
где $P_0$ - начальные координаты точки,  
    $P_n$ координаты точки на n-ой итерации  
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

1 без оптимизаций
2 использование массивов
3 использование SIMD инструкций

## Внешний вид и горячие клавиши
В верхнем левом углу изображения указано текущее время, необходимое для рендера одного кадра, в тиках процессора
Список горячих клавиш используемых программой:
↑ сдвиг вверх
↓ сдвиг вниз
← сдвиг влево
→ сдвиг вправо
> приближение изображения
< отдаление изображения

## Виды рендеринга

### Рендеринг без оптимизаций
Здесь и далее от номера итерации, на котором точка покинула заданный радиус, определяет цвет, в который она будет окрашена.

В первом варианте рендеринга используется наивный и понятный рассчет этого номера. С помощью двойного цикла осуществляется проход по каждому пикселю.


### Рендеринг с использованием массивов

Этот вариант рендеринга похож на предыдущий, однако пиксели в нем обрабатываются в массивах по 8 штук. С флагом -O3 это позволяет компилятору использовать SIMD-инструкции, ускоряющие выполнение кода.

SIMD (англ. single instruction, multiple data — одиночный поток команд, множественный поток данных) - "принцип компьютерных вычислений, позволяющий обеспечить параллелизм на уровне данных." [wiki](https://ru.wikipedia.org/wiki/SIMD). В этом режиме одна и та же операция применяется сразу к нескольким разным значениям данных. На протяжении многих лет эти расширения продолжали развиваться, и после каждого существенного изменения их названия менялись, а совсем недавно они стали называться AVX. 
SIMD-расширения используют для управления данными набор регистров, называемых "MM" регистрами. Для AVX это "YMM" - 32-байтные регистры, способные хранить восемь 32-разрядных значений или четыре 64-разрядных. Исходя из этого выбран размер массивов - 8, так как при обработке пикселей их координаты представлены типом данных float (4 байта).


### Рендеринг с использованием SIMD intrinsic
intrinsic-и являются набором особых функций компилятора. При выполнении кода с ними происходит не вызов функций, а непосредственная вставка инструкций выполняющих ту или иную операцию. Использование intrinsic-ов значительно ускоряет программу



## Выводы
Ниже приведена таблица с количеством тиков процессора, необходимых для рендеринга одного кадра, для каждого метода. Измерение времени именно в тиках процессора выбрано для максимальной точности данных, так как другие методы измерения времени (например, встроенные в графическую библиотеку sfml) имеют значительную погрешность для измеряемых интервалов времени.

Без оптимизаций Массивы intrinsic

Таким образом, можем видеть, что использование массивов (при компиляции с флагом -O3) ускоряет программу в раза, а использование intrinsic-ов в раз. Таким образом, было наглядно продемонстрировано, что использование SIMD-инструкций способно многократно ускорить программу.