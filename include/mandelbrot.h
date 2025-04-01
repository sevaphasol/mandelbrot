#ifndef MANDELBROT_H__
#define MANDELBROT_H__

#include <SFML/Graphics.hpp>
#include <inttypes.h>
#include <immintrin.h>
#include "argv_parser.h"

#define AVG_FREQUENCY   3792

#define WIDTH           960u
#define HEIGHT          800u
#define MAX_ITER        300u
#define START_CENTER_X -0.75f
#define START_CENTER_Y  0.0f
#define START_SCALE     2.5f
#define MAX_RADIUS      10.f

#define NORMAL          0
#define AVX2            2

enum status_t
{
    ERROR   = 0,
    SUCCESS = 1,
};

struct context_t
{
    ap_ctx_t argv;

    sf::RenderWindow& window;
    sf::VertexArray&  pixels;
    alignas(32) uint colors[WIDTH*HEIGHT];

    float center_x;
    float center_y;
    float scale;
};

status_t ctx_ctor                  (context_t* ctx);
status_t ctx_update                (context_t* ctx);
status_t draw_mandelbrot           (context_t* ctx);
status_t handle_events             (context_t* ctx);

status_t testing_mode              (context_t* ctx);
status_t graphic_mode              (context_t* ctx);

sf::Color get_color                (uint32_t iter);

status_t compute_mandelbrot_normal (context_t* ctx);
status_t compute_mandelbrot_avx2   (context_t* ctx);

#endif // MANDELBROT_H__
