#ifndef MANDELBROT_H__
#define MANDELBROT_H__

#include <SFML/Graphics.hpp>
#include <inttypes.h>
#include <immintrin.h>
#include <xmmintrin.h>
#include <math.h>
#include "argv_parser.h"

#define WINDOW_NAME     "MANDELBROT"

#define AVG_FREQUENCY    3792

#define WIDTH            (64*16)
#define HEIGHT           (64*14)
#define MAX_ITER         128
#define START_CENTER_X  -0.75
#define START_CENTER_Y   0.0
#define START_SCALE      2.5
#define MAX_RADIUS       10.0
#define DEFAULT_RUNS     1
#define DEFAULT_N_POINTS 5
#define DEFAULT_STATS    false
#define DEFAULT_SCALING  0.1
#define DEFAULT_MOVING   0.01

#define GRAPHICS         0
#define TESTING          1
#define BENCMARK         2

#define NORMAL           0
#define SSE              1
#define AVX2             2
#define AVX512           3

#define START_TIMER      1
#define END_TIMER        0

enum status_t
{
    ERROR   = 0,
    SUCCESS = 1,
};

struct context_t
{
    ap_ctx_t argv;

    uint64_t* pixels;
    sf::Uint32* colors;

    double center_x;
    double center_y;
    double scale;

    sf::Texture texture;
    sf::Sprite  sprite;

    context_t()
    {
        texture.create(WIDTH, HEIGHT);
        sprite.setTexture(texture);
    }
};

struct test_ctx_t
{
    uint32_t n_points;
    double*  x;
    double*  y;
    double   slope;
    double   intercept;
    double   slope_error;
    double   intercept_error;
};

status_t   ctx_ctor                  (context_t* ctx);
status_t   ctx_update                (context_t* ctx);
status_t   ctx_dtor                  (context_t* ctx);

status_t   graphic_mode              (context_t* ctx);
status_t   testing_mode              (context_t* ctx);
status_t   benchmark_mode            (context_t* ctx);

status_t   compute_pixel_colors      (context_t* ctx);
sf::Uint32 get_color                 (uint64_t iter, uint8_t alpha);

status_t   compute_mandelbrot_normal (context_t* ctx, uint32_t runs);
status_t   compute_mandelbrot_sse    (context_t* ctx, uint32_t runs);
status_t   compute_mandelbrot_avx2   (context_t* ctx, uint32_t runs);
status_t   compute_mandelbrot_avx512 (context_t* ctx, uint32_t runs);

status_t   handle_events             (context_t* ctx,
                                      sf::Window& window,
                                      sf::Event& event);

status_t   show_stats                (bool mode);

status_t   calc_least_squares        (test_ctx_t* ctx);
status_t   graphic_plot              (test_ctx_t* ctx);

status_t   show_progress_bar         (uint32_t total, uint32_t current);

#endif // MANDELBROT_H__
