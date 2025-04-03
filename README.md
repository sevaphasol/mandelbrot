# Оптимизация программы с помощью SIMD-инструкций

## Введение

Передо мной стояли следующие задачи:
1) Написать программу для тестирования оптимизаций.
2) Реализовать основные вычисления в программе с помощью различных SIMD-инструкций.
3) [Замерить количество тактов, требуемое для вычислений в различных реализациях.](#результаты-измерений)

В качестве программы для тестирования оптимизаций было предложено вычисление и визуализация множества Мандельброта.

#### Реализации
1) **NORMAL** – базовая реализация без оптимизаций.
2) **SSE**    – оптимизация с использованием SSE-инструкций    (128-битные векторы).
3) **AVX2**   – оптимизация с использованием AVX2-инструкций   (256-битные векторы).
4) **AVX512** – оптимизация с использованием AVX512-инструкций (512-битные векторы).

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
|                      | `0` — NORMAL                                                             |
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
3) Компиляция программы с флагом -O2.

Во время работы программы я слежу за условиями с помощью программы btop.
В ней я вижу текущую тактовую частоту и температуру процессора.

![btop](https://github.com/user-attachments/assets/e654156c-7054-4cde-b444-ca7219f3b27a)

Тактовая частота остаётся постоянной, а температура не превышает 40°C (нет троттлинга).
Это означает, что условия эксперимента стационарны.

### Результаты измерений

<table>
    <tr>
        <td><img src="https://github.com/user-attachments/assets/ecac6db8-5b0f-4494-af4a-1acc667c5903"/></td>
        <td>
            <table>
            	<tr><th>Repeats</th><th>Cycles</th></tr>
            	<tr><th>10</th><th>3862.9⋅10⁶</th></tr>
            	<tr><th>20</th><th>7688.8⋅10⁶</th></tr>
            	<tr><th>30</th><th>11491.9⋅10⁶</th></tr>
            	<tr><th>40</th><th>15373.4⋅10⁶</th></tr>
            	<tr><th>50</th><th>19273.7⋅10⁶</th></tr>
            	<tr><th>60</th><th>23025.0⋅10⁶</th></tr>
            	<tr><th>70</th><th>26952.4⋅10⁶</th></tr>
            	<tr><th>80</th><th>30792.5⋅10⁶</th></tr>
            	<tr><th>90</th><th>34677.4⋅10⁶</th></tr>
            	<tr><th>100</th><th>38366.3⋅10⁶</th></tr>
            </table>
        </td>
    </tr>
</table>

<table>
    <tr>
        <td><img src="https://github.com/user-attachments/assets/aa2c9031-9c02-4c38-939b-c3ed2c15c011"/></td>
        <td>
            <table>
            	<tr><th>Repeats</th><th>Cycles</th></tr>
            	<tr><th>10</th><th>2044.1⋅10⁶</th></tr>
            	<tr><th>20</th><th>4058.9⋅10⁶</th></tr>
            	<tr><th>30</th><th>6099.7⋅10⁶</th></tr>
            	<tr><th>40</th><th>8184.1⋅10⁶</th></tr>
            	<tr><th>50</th><th>10236.7⋅10⁶</th></tr>
            	<tr><th>60</th><th>12252.6⋅10⁶</th></tr>
            	<tr><th>70</th><th>14318.8⋅10⁶</th></tr>
            	<tr><th>80</th><th>16371.7⋅10⁶</th></tr>
            	<tr><th>90</th><th>18414.0⋅10⁶</th></tr>
            	<tr><th>100</th><th>20446.9⋅10⁶</th></tr>
            </table>
        </td>
    </tr>
</table>

<table>
    <tr>
        <td><img src="https://github.com/user-attachments/assets/c9ff0e55-65f4-4ffb-a8ba-1d3cb1c1be66"/></td>
        <td>
            <table>
            	<tr><th>Repeats</th><th>Cycles</th></tr>
            	<tr><th>10</th><th>1066.7⋅10⁶</th></tr>
            	<tr><th>20</th><th>2095.9⋅10⁶</th></tr>
            	<tr><th>30</th><th>3141.4⋅10⁶</th></tr>
            	<tr><th>40</th><th>4190.9⋅10⁶</th></tr>
            	<tr><th>50</th><th>5239.3⋅10⁶</th></tr>
            	<tr><th>60</th><th>6292.0⋅10⁶</th></tr>
            	<tr><th>70</th><th>7316.2⋅10⁶</th></tr>
            	<tr><th>80</th><th>8361.0⋅10⁶</th></tr>
            	<tr><th>90</th><th>9422.8⋅10⁶</th></tr>
            	<tr><th>100</th><th>10481.7⋅10⁶</th></tr>
            </table>
        </td>
    </tr>
</table>

<table>
    <tr>
        <td><img src="https://github.com/user-attachments/assets/858beb54-2e45-4372-bcd0-e469b4ead3a1"/></td>
        <td>
            <table>
                <table>
                	<tr><th>Repeats</th><th>Cycles</th></tr>
                	<tr><th>10</th><th>720.7⋅10⁶</th></tr>
                	<tr><th>20</th><th>1434.2⋅10⁶</th></tr>
                	<tr><th>30</th><th>2133.1⋅10⁶</th></tr>
                	<tr><th>40</th><th>2844.0⋅10⁶</th></tr>
                	<tr><th>50</th><th>3568.8⋅10⁶</th></tr>
                	<tr><th>60</th><th>4268.6⋅10⁶</th></tr>
                	<tr><th>70</th><th>4978.0⋅10⁶</th></tr>
                	<tr><th>80</th><th>5676.4⋅10⁶</th></tr>
                	<tr><th>90</th><th>6354.0⋅10⁶</th></tr>
                	<tr><th>100</th><th>7066.1⋅10⁶</th></tr>
                </table>
            </table>
        </td>
    </tr>
</table>

## Обсуждение результатов
Проведённые исследования демонстрируют значительный выигрыш в производительности при использовании SIMD-инструкций для вычисления множества Мандельброта.

- AVX-512 показал наилучшие результаты с ускорением в ~5.5 раза по сравнению с базовой реализацией
- AVX2 обеспечил ускорение в ~3.7 раза
- SSE дал ускорение в ~1.9 раза

<table>
    <tr>
        <td><img src="https://github.com/user-attachments/assets/5067046a-0ea1-4794-98fb-4a2053413403"/></td>
        <td>
            <table>
            	<tr><th>Реализация</th><th>Количество тактов</th></tr>
            	<tr><th>No optimization</th><th>(384.5 ± 0.5)⋅10⁶</th></tr>
            	<tr><th>SSE</th><th>(204.8 ± 0.2)⋅10⁶</th></tr>
            	<tr><th>AVX2</th><th>(104.6 ± 0.1)⋅10⁶</th></tr>
            	<tr><th>AVX512</th><th>(70.5 ± 0.1)⋅10⁶</th></tr>
            </table>
        </td>
    </tr>
</table>
