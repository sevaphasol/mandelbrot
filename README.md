# Исследование различных методов оптимизации с использованием SIMD-инструкций

## Введение

Передо мной стояли следующие задачи:
1) Написать программу для тестирования оптимизаций.
2) Реализовать основные вычисления в программе с помощью различных SIMD-инструкций.
3) [Замерить количество тактов, требуемое для вычислений в различных реализациях.](#результаты-измерений)
4) [Проанализировать работу программы при различных флагах оптимизации компилятора.](#сравнение-различных-оптимизаций-компилятора)

В качестве программы для тестирования оптимизаций было предложено вычисление и визуализация множества Мандельброта.

#### Реализации
1) **BASELINE** – базовая реализация без оптимизаций.            (64-битные double).
2) **SSE**      – оптимизация с использованием SSE-инструкций    (128-битные векторы).
3) **AVX2**     – оптимизация с использованием AVX2-инструкций   (256-битные векторы).
4) **AVX512**   – оптимизация с использованием AVX512-инструкций (512-битные векторы).

## Сборка и запуск

Для запуска необходимы
- Поддержка AVX512.
- Графическая библиотека SFML для визуализации.
- gnuplot для посторения графиков в режиме тестирования.

В папке склонированного репозитория введите следующие комманды
```bash
make
bin/mandelbrot [опции]
```
#### Поддерживаемые опции

| Опция                | Описание                                                                 |
|----------------------|--------------------------------------------------------------------------|
| `--help`, `-h`       | Выводит сообщение с описанием всех доступных опций.                      |
| `--stats`, `-s`      | Показывает статистику в графическом режиме.                              |
| `--plot`, `-p`       | Включает построение графика.                                             |
| `--Pulsation`, `-P`  | Включает анимацию (пульсация) изображения.                               |
| `--graphics`, `-g`   | Графический режим (с визуализацией). Режим по умолчанию.                 |
| `--testing`, `-t`    | Режим тестирования программы.                                            |
| `--benchmark`, `-b`  | Выводит только статистику (без визуализации).                            |
| `--mode=<number>`    | Устанавливает режим работы программы:                                    |
|                      | `0` — graphics mode                                                      |
|                      | `1` — testing mode                                                       |
|                      | `2` — benchmark mode                                                     |
| `--alpha<number>`    | Устанавливает значение альфа (прозрачность пикселей).                    |
| `--runs=<number>`    | Устанавливает количество запусков в тестовом режиме.                     |
| `--calc=<number>`    | Устанавливает режим вычислений:                                          |
|                      | `0` — BASELINE                                                           |
|                      | `1` — SSE                                                                |
|                      | `2` — AVX2                                                               |
|                      | `3` — AVX-512                                                            |
| `--n_points=<number>`| Устанавливает количесто экспериментальных точек в **testing mode**.      |

## Измерения в различных реализациях

Программа была запущена в **testing mode** — режим когда программа делает *n_points* измерений.
В каждом новом измерении она повышает количество повторений вычисления множества Мандельброта с
шагом *runs*. Таким образом мы получаем зависимость количества тактов, необходимых для
вычисления, от количества повторов.

Очевидно, что если условия исполнения программы стационарны, зависимость должна быть линейной.
Используя метод наименьших квадратов (МНК), можно найти коэффициент линейной регрессии, который
будет являться средним количеством тактов для вычисления множества Мандельброта, и его погрешность.

### Условия для измерения

Для поддержки стационарности условий исполнения программы необходимо
1) Зафиксировать или ограничить частоту процессора.
2) Избежать смены контекста программы между ядрами

Для ограничения частоты процессора воспользуемся утилитой cpupower

```bash
sudo cpupower frequency-set -d <frequency> # нижний предел
sudo cpupower frequency-set -u <frequency> # верхний предел
```

Для того, чтобы ОС не "перекидывала" программу между ядрами,
тем самым портя чистоту эксперимента, воспользуемся утилитой taskset,
которая "закрепяет" программу за выбранным ядром.

```bash
taskset -c <core number>
```

Условия, в которых проводились измерения
1) Тактовая частота процессора фиксирована и равна 3.2 GHz.
2) Различные оптимизации запускаются на одном и том же ядре C0.
3) Компилятор x86-64 gcc версии 13.3.0 с флагом оптимизации -O2.
4) Процессор AMD Ryzen 7 8845H, 32 Гб ОЗУ.
5) ОС Ubuntu 24.04.2 LTS.

Во время работы программы я слежу за условиями с помощью программы btop.
В ней я вижу текущую тактовую частоту и температуру процессора.

![btop](https://github.com/user-attachments/assets/e654156c-7054-4cde-b444-ca7219f3b27a)

Тактовая частота остаётся постоянной, а температура не превышает 40°C (нет троттлинга).
Это означает, что условия эксперимента стационарны.

### Результаты измерений

<table>
    <tr>
        <td><img src="https://github.com/user-attachments/assets/7084a9e8-f616-416d-9a2c-c64c2de8c04d"/></td>
        <td>
            <table>
            	<tr><th>Repeats</th><th>Cycles</th></tr>
            	<tr><th>10</th><th>3836.9⋅10⁶</th></tr>
            	<tr><th>20</th><th>7711.3⋅10⁶</th></tr>
            	<tr><th>30</th><th>11496.3⋅10⁶</th></tr>
            	<tr><th>40</th><th>15317.2⋅10⁶</th></tr>
            	<tr><th>50</th><th>19171.1⋅10⁶</th></tr>
            	<tr><th>60</th><th>23089.3⋅10⁶</th></tr>
            	<tr><th>70</th><th>26963.5⋅10⁶</th></tr>
            	<tr><th>80</th><th>30740.2⋅10⁶</th></tr>
            	<tr><th>90</th><th>34594.1⋅10⁶</th></tr>
            	<tr><th>100</th><th>38338.5⋅10⁶</th></tr>
            </table>
        </td>
    </tr>
</table>

<table>
    <tr>
        <td><img src="https://github.com/user-attachments/assets/64f52c0f-27d4-40e5-b4bd-1d65cb2efff4"/></td>
        <td>
            <table>
            	<tr><th>Repeats</th><th>Cycles</th></tr>
            	<tr><th>10</th><th>2057.5⋅10⁶</th></tr>
            	<tr><th>20</th><th>4078.6⋅10⁶</th></tr>
            	<tr><th>30</th><th>6135.6⋅10⁶</th></tr>
            	<tr><th>40</th><th>8197.9⋅10⁶</th></tr>
            	<tr><th>50</th><th>10210.3⋅10⁶</th></tr>
            	<tr><th>60</th><th>12256.4⋅10⁶</th></tr>
            	<tr><th>70</th><th>14285.3⋅10⁶</th></tr>
            	<tr><th>80</th><th>16347.7⋅10⁶</th></tr>
            	<tr><th>90</th><th>18378.4⋅10⁶</th></tr>
            	<tr><th>100</th><th>20435.6⋅10⁶</th></tr>
            </table>
        </td>
    </tr>
</table>

<table>
    <tr>
        <td><img src="https://github.com/user-attachments/assets/13b38472-ac3d-44a3-ad41-65308b06e394"/></td>
        <td>
            <table>
            	<tr><th>Repeats</th><th>Cycles</th></tr>
            	<tr><th>10</th><th>1056.7⋅10⁶</th></tr>
            	<tr><th>20</th><th>2099.8⋅10⁶</th></tr>
            	<tr><th>30</th><th>3137.1⋅10⁶</th></tr>
            	<tr><th>40</th><th>4187.1⋅10⁶</th></tr>
            	<tr><th>50</th><th>5243.5⋅10⁶</th></tr>
            	<tr><th>60</th><th>6279.4⋅10⁶</th></tr>
            	<tr><th>70</th><th>7327.1⋅10⁶</th></tr>
            	<tr><th>80</th><th>8385.2⋅10⁶</th></tr>
            	<tr><th>90</th><th>9416.9⋅10⁶</th></tr>
            	<tr><th>100</th><th>10466.3⋅10⁶</th></tr>
            </table>
        </td>
    </tr>
</table>

<table>
    <tr>
        <td><img src="https://github.com/user-attachments/assets/72df650b-db1e-400e-bf49-41d3eb273a2a"/></td>
        <td>
            <table>
                <tr><th>Repeats</th><th>Cycles</th></tr>
                <tr><th>10</th><th>714.6⋅10⁶</th></tr>
                <tr><th>20</th><th>1421.1⋅10⁶</th></tr>
                <tr><th>30</th><th>2157.6⋅10⁶</th></tr>
                <tr><th>40</th><th>2887.1⋅10⁶</th></tr>
                <tr><th>50</th><th>3562.3⋅10⁶</th></tr>
                <tr><th>60</th><th>4281.5⋅10⁶</th></tr>
                <tr><th>70</th><th>4973.0⋅10⁶</th></tr>
                <tr><th>80</th><th>5659.6⋅10⁶</th></tr>
                <tr><th>90</th><th>6367.3⋅10⁶</th></tr>
                <tr><th>100</th><th>7071.8⋅10⁶</th></tr>
            </table>
        </td>
    </tr>
</table>

## Обсуждение результатов измерений

Проведённые исследования демонстрируют значительный выигрыш в производительности при использовании SIMD-инструкций для вычисления множества Мандельброта.

- AVX-512 показал наилучшие результаты с ускорением в ~5.4 раза по сравнению с базовой реализацией
- AVX2 обеспечил ускорение в ~3.7 раза
- SSE дал ускорение в ~1.9 раза

<table>
    <tr>
        <td><img src="https://github.com/user-attachments/assets/c27bc5e5-4fdf-4aff-94ad-5d64aca22ca4"/></td>
        <td>
            <table>
            	<tr><th>Реализация</th><th>Количество тактов</th></tr>
            	<tr><th>BASELINE</th><th>(384.1 ± 0.5)⋅10⁶</th></tr>
            	<tr><th>SSE</th><th>(204.2 ± 0.1)⋅10⁶</th></tr>
            	<tr><th>AVX2</th><th>(104.6 ± 0.1)⋅10⁶</th></tr>
            	<tr><th>AVX512</th><th>(70.5 ± 0.2)⋅10⁶</th></tr>
            </table>
        </td>
    </tr>
</table>

## Сравнение различных оптимизаций компилятора

### Измерения

Для начала замерим количество тактов, требуемых для вычислений в различных оптимизациях.

Измерения проводятся в следующих условиях
1) тактовая частота процессора 3.2 Ghz
2) ядро C0
3) реализация AVX512
4) runs = 10
5) n_points = 10

| Оптимизация | Количество тактов |
|:-----------:|:-----------------:|
|     O0      | (624.2 ± 0.5)⋅10⁶ |
|     O1      | (72.5  ± 0.1)⋅10⁶ |
|     O2      | (69.7  ± 0.2)⋅10⁶ |
|     O3      | (69.8  ± 0.1)⋅10⁶ |

Как видно из таблицы, O2 и O3 одинаково оптимизируют код.

O1 чуть-чуть отстаёт от O2 и O3.

O0 сильно отстаёт от других оптимизаций.

### Генерация ассемблерного кода

Воспользуемся сайтом [godbolt](https://godbolt.org/) для представления C-кода на языке ассемблера.

Дизассемблируем функцию *compute_mandelbrot_avx512*.
Количество тактов были измерены для основного вычислительного цикла в ней.

Введём код из файла [for_godbolt.cpp](for_godbolt/for_godbolt.cpp) в godbolt.
Компилятор — x86-61 gcc версии 13.3.0 (такой же каким был собран исполняемый файл).

Компилируя с различными флагами оптимизации получим следующие ассемблерные коды
- [O0](for_godbolt/O0.asm)
- [O1](for_godbolt/O1.asm)
- [O2](for_godbolt/O2.asm)
- [O3](for_godbolt/O3.asm)

### Анализ ассемблерного кода

Заметим для начала, что коды O2 и O3 абсолютно идентичны, то есть они ничем не отличаются.

**Сравним O0 и O1.**

Критическое отличие очевидно — в O0 локальные переменные хранятся в памяти, а в O1 в регистрах.
Обращение к памяти — очень дорогостоящая операция, что и делает O0 неэффективным.

**Сравним O1 и O2.**

Здесь ситуация менее тривиальна. Коды выглядят по разному, но работают практически одинаково.
O2 хорошо оптимизировал инициализацию переменных перед главным вычислительным циклом.

Например вычисление y0_start:

**O1:**
```asm
3.  vmovsd  xmm2, QWORD PTR 24[rdi]         ; xmm2  = scale
8.  vmulsd  xmm2, xmm2, QWORD PTR .LC2[rip] ; xmm2  = scale * 0.5
9.  vmovsd  xmm13, QWORD PTR 16[rdi]        ; xmm13 = ctx->center_y
10. vsubsd  xmm13, xmm13, xmm2              ; xmm13 = ctx->center_y - scale * 0.5
11. vbroadcastsd zmm13, xmm13               ; zmm13 = y0_start
```

Ничего необычного, однако можно заметить, что O1 не особо старается заполнить конвейер, когда это возможно.

**O2**
```asm
8.  vmovsd xmm5, QWORD PTR .LC2[rip]            ; xmm5  = 0.5
10. vmovsd xmm12, xmm1, xmm1                    ; xmm12 = scale
14. vfnmadd213sd xmm12, xmm5, QWORD PTR 16[rdi] ; xmm12 = -(scale * 0.5) + ctx->center_y
22. vbroadcastsd zmm12, xmm12                   ; zmm12 = y0_start
```

O2 использует инструкцию ```vfnmadd213sd```, которая одновременно умножает и вычитает
 и вычисляется за 4 такта (также как и vmulsd).
Более того, O2 "раскидывает" инструкции, чтобы они могли конверизоваться
(пропадает зависимость по данным).

Вычисление x0_start:

**O1:**
```asm
12. vmovsd  xmm1, QWORD PTR 8[rdi]          ; xmm1 = cxt->center_x
13. vsubsd  xmm2, xmm1, xmm2                ; xmm2 = -(scale * 0.5) + ctx->center_x
14. vaddsd  xmm4, xmm0, xmm0                ; xmm4 = single_dx * 2.0
15. vmulsd  xmm5, xmm0, QWORD PTR .LC3[rip] ; xmm5 = single_dx * 3.0
16. vmulsd  xmm3, xmm0, QWORD PTR .LC4[rip] ; xmm3 = single_dx * 5.0
17. vmulsd  xmm6, xmm0, QWORD PTR .LC5[rip] ; xmm6 = single_dx * 6.0
18. vmulsd  xmm1, xmm0, QWORD PTR .LC6[rip] ; xmm1 = single_dx * 4.0
19. vmulsd  xmm7, xmm0, QWORD PTR .LC7[rip] ; xmm7 = single_dx * 7.0

```
**O2:**
```asm
6.  vmulsd  xmm0, xmm1, QWORD PTR .LC0[rip]   ; xmm0 = scale / WIDTH = single_dx
9.  vmulsd  xmm8, xmm0, QWORD PTR .LC7[rip]   ; xmm8 = single_dx * 7.0
11. vmulsd  xmm3, xmm0, QWORD PTR .LC4[rip]   ; xmm3 = single_dx * 2.0
12. vfnmadd213sd xmm1, xmm5, QWORD PTR 8[rdi] ; xmm1 = -(scale * 0.5) + ctx->center_x
13. vmulsd  xmm7, xmm0, QWORD PTR .LC5[rip]   ; xmm7 = single_dx * 6.0
15. vmulsd  xmm2, xmm0, QWORD PTR .LC6[rip]   ; xmm2 = single_dx * 4.0
16. vmulsd  xmm6, xmm0, QWORD PTR .LC3[rip]   ; xmm6 = single_dx * 3.0
17. vaddsd  xmm4, xmm0, xmm0                  ; xmm4 = single_dx * 2.0
20. vbroadcastsd zmm11, xmm1                  ; xmm1 = -(scale* 0.5) + ctx->center_x
32. vaddpd  zmm11, zmm11, zmm0                ; zmm1 = x0_start
```

Ещё один пример как O2 использует быструю инструкцию ```vfnmadd213sd``` и старается заполнить конвейер,
чего не делает O1.

В остальном инициализация в O1 и O2 одинаковая.

Разберём теперь главный вычислительный цикл, который больше всего влияет на общее количество тактов.

В целом реализация в O1 и O2 практически одинаковая за исключением используемых регистров и одного нюанса. Нюанс заключается в присваивании ```__m512d y = y0;```

**O1:**
```asm
.L8:
        vmovapd zmm2, zmm6       ; x    = x0
        vmovapd zmm0, zmm11      ; y    = y0
        mov     edx, 128         ; edx  = MAX_ITER
        vpxor   xmm3, xmm3, xmm3 ; iter = [0, 0, 0, 0, 0, 0, 0, 0]
.L9:
        lea     rcx, -8192[rsi]  ; xi   = 0
        vmovapd zmm6, zmm12      ; x0   = x0_start;
        vmovapd zmm11, zmm8      ; y    = y0;
        jmp     .L8
```

Можно заметить, что строка ```vmovapd zmm11, zmm8``` избыточна.
Нет никакой надобности сохранять *zmm8* в *zmm11*, можно сразу
сделать ```vmovapd zmm0, zmm8``` в теле **.L8**.

**O2:**
```asm
.L9:
        lea     rcx, -8192[rsi]  ; xi = 0
        vmovapd zmm6, zmm11      ; x0 = x0_start
.L8:
        vmovapd zmm2, zmm6       ; x = x0
        vmovapd zmm0, zmm7       ; y = y0
        mov     edx, 128         ; edx = MAX_ITER
        vpxor   xmm3, xmm3, xmm3 ; iter = [0, 0, 0, 0, 0, 0, 0, 0]
        jmp     .L5
```

-O2 такой ошибки не допустил.

Также интересно, что обе реализации инициализируют счётчик xi инструкцией ```lea rcx, -8192[rsi]```.
Это связано с тем, что на самом деле rcx здесь не просто счетчик, а индекс в массиве pixels, кратный 8192 (поскольку WIDTH = 64*16 = 1024, а uint64_t занимает 8 байт, то 1024 * 8 = 8192 — это размер строки в байтах).

lea rcx, -8192[rsi] вычисляет адрес начала текущей строки (yi * WIDTH * sizeof(uint64_t)), а затем rcx увеличивается на 64 (размер zmm-регистра) в каждой итерации цикла по xi.

### Результаты анализа ассемблерного кода

- O0 крайне неэффективен, так как хранит локальные переменные в памяти, что приводит к значительным задержкам из-за частых обращений к памяти.
- O1 уже оптимизирует хранение переменных в регистрах, что резко сокращает время выполнения (в ~8.6 раз быстрее, чем O0).
- O1 немного отстаёт от O2 (~4% медленнее).

![histogram](https://github.com/user-attachments/assets/ef6ff1c8-5d0b-4918-86a0-46fb269049b3)

Также хотелось бы упомянуть, что [godbolt](https://godbolt.org/) очень удобен для анализа скомпилированного кода. Подсветка соотвествующих частей C-кода и кода на ассемблере значительно упрощает анализ.

## Использованная литература

1. [Intel® Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html)
