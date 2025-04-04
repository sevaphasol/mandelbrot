# Исследование различных методов оптимизации с использованием SIMD-инструкций

## Введение

Передо мной стояли следующие задачи:
1) Написать программу для тестирования оптимизаций.
2) Реализовать основные вычисления в программе с помощью различных SIMD-инструкций.
3) [Замерить количество тактов, требуемое для вычислений в различных реализациях.](#результаты-измерений)

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
3) Компилятор gcc с флагом оптимизации -O2.
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

## Обсуждение результатов
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

## Использованная литература

1. [Intel® Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html)
