#include "mandelbrot.h"
#include "argv_parser.h"

//================================================================================

int main(int argc, char* argv[])
{
    context_t ctx = {};
    ctx_ctor(&ctx);

    parse_argv(argc, argv, &ctx.argv);

    switch (ctx.argv.mode)
    {
        case GRAPHICS: graphic_mode  (&ctx); break;
        case TESTING:  testing_mode  (&ctx); break;
        case BENCMARK: benchmark_mode(&ctx); break;
        default:                             break;
    }

    ctx_dtor(&ctx);

    return 0;
}

//================================================================================

status_t benchmark_mode(context_t* ctx)
{
    while (true)
    {
        show_stats(START_TIMER);
        ctx_update(ctx);
        show_stats(END_TIMER);
    }
}

//================================================================================

status_t graphic_mode(context_t* ctx)
{
    bool stats = ctx->argv.stats;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), WINDOW_NAME);
    sf::Event event;

    while (window.isOpen())
    {
        if (stats)
            show_stats(START_TIMER);

        handle_events(ctx, window, event);

        ctx_update(ctx);

        window.clear();
        window.draw(ctx->sprite);
        window.display();

        if (ctx->argv.stats)
            show_stats(END_TIMER);
    }

    if (stats)
        putchar('\n');

    return SUCCESS;
}

//================================================================================

status_t show_stats(bool mode)
{
    static timespec time_start  = {};
    static timespec time_end    = {};
    static uint64_t cycle_start = 0;
    static uint64_t cycle_end   = 0;
    static uint64_t time        = 0;
    static uint32_t n_frames    = 0;
    static uint32_t n_spikes    = 0;
    static uint32_t freq        = 0;

    if (mode)
    {
        clock_gettime(CLOCK_MONOTONIC, &time_start); // START_TIMER
        cycle_start = _rdtsc();

        return SUCCESS;
    }

    clock_gettime(CLOCK_MONOTONIC, &time_end); // END_TIMER
    cycle_end = _rdtsc();

    time = (time_end.tv_sec   - time_start.tv_sec) * (uint64_t) 1e9 +
           (time_end.tv_nsec  - time_start.tv_nsec);  // nsec
    freq = (uint) ((double) (cycle_end - cycle_start) / (double) time * 1e3); // MhZ

    n_frames++;

    if (time > (uint64_t) 1e8)
        n_spikes++;

    printf("\r| fps = %05.1f | time = %05.1f ms | freq = %u MHz | frames = %u | spikes(AVX2) = %u |",
            1.0 / ((double) time / 1e9), (double) time / 1e6, freq, n_frames, n_spikes);
    fflush(stdout);

    return SUCCESS;
}

//================================================================================

status_t handle_events(context_t* ctx, sf::Window& window, sf::Event& event)
{
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    int  left   = sf::Keyboard::isKeyPressed(sf::Keyboard::Left );
    int  right  = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    int  up     = sf::Keyboard::isKeyPressed(sf::Keyboard::Up   );
    int  down   = sf::Keyboard::isKeyPressed(sf::Keyboard::Down );
    bool zoom   = sf::Keyboard::isKeyPressed(sf::Keyboard::K    );
    bool reduce = sf::Keyboard::isKeyPressed(sf::Keyboard::J    );
    bool reset  = sf::Keyboard::isKeyPressed(sf::Keyboard::R    );

    ctx->center_x += ctx->scale * DEFAULT_MOVING * (double)(right - left);
    ctx->center_y += ctx->scale * DEFAULT_MOVING * (double)(down  - up  );

    if (zoom)
        ctx->scale *= (1 - DEFAULT_SCALING / 2);

    if (reduce)
        ctx->scale *= (1 + DEFAULT_SCALING / 2);

    if (reset)
    {
        ctx->center_x = START_CENTER_X;
        ctx->center_y = START_CENTER_Y;
        ctx->scale    = START_SCALE;
    }

    return SUCCESS;
}

//================================================================================

status_t ctx_ctor(context_t* ctx)
{
    ctx->argv.mode     = GRAPHICS;
    ctx->argv.stats    = DEFAULT_STATS;
    ctx->argv.calc     = AVX512;
    ctx->argv.runs     = DEFAULT_RUNS;
    ctx->argv.n_points = DEFAULT_N_POINTS;
    ctx->center_x      = START_CENTER_X;
    ctx->center_y      = START_CENTER_Y;
    ctx->scale         = START_SCALE;

    ctx->pixels = (uint64_t*)   _mm_malloc(WIDTH*HEIGHT * sizeof(uint64_t), 64);
    ctx->colors = (sf::Uint32*) calloc(WIDTH*HEIGHT, sizeof(sf::Uint32));

    return SUCCESS;
}

//================================================================================

status_t ctx_dtor (context_t* ctx)
{
    _mm_free(ctx->pixels);
    free    (ctx->colors);

    return SUCCESS;
}

//================================================================================

status_t ctx_update(context_t* ctx)
{
    switch (ctx->argv.calc)
    {
        case NORMAL: compute_mandelbrot_normal(ctx, 1); break;
        case SSE:    compute_mandelbrot_sse   (ctx, 1); break;
        case AVX2:   compute_mandelbrot_avx2  (ctx, 1); break;
        case AVX512: compute_mandelbrot_avx512(ctx, 1); break;
        default:     return ERROR;
    }

    if (ctx->argv.mode != BENCMARK)
    {
        compute_pixel_colors(ctx);
        ctx->texture.update((sf::Uint8*) ctx->colors, WIDTH, HEIGHT, 0, 0);
    }

    return SUCCESS;
}

//================================================================================

status_t compute_pixel_colors(context_t* ctx)
{
    static uint8_t alpha = ctx->argv.alpha;

    if (ctx->argv.pulsation)
    {
        static sf::Clock clock;
        uint16_t param = (uint16_t) (clock.getElapsedTime().asMilliseconds() / 10) % 512;

        if (param < 256)
            alpha = (uint8_t) param;
        else
            alpha = (uint8_t) (511 - param);
    }

    static uint64_t*   pixels = ctx->pixels;
    static sf::Uint32* colors = ctx->colors;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            int ind = y * WIDTH + x;
            colors[ind] = get_color(pixels[ind], alpha);
        }
    }

    return SUCCESS;
}

//================================================================================

sf::Uint32 get_color(uint64_t iter, uint8_t alpha)
{
    if (iter == MAX_ITER)
        return sf::Color(0, 0, 0, 0).toInteger();

    double t = ((double) iter) / MAX_ITER;
    double r = (t + 1/3 + 0.05*((double) (iter%2))) * 3.14;
    double g = (t + 7/3 + 0.01*((double) (iter%2))) * 3.14;
    double b = (t + 5/3 +    0*((double) (iter%2))) * 3.14;

    return sf::Color((uint8_t) (255 * (r - r*r*r/6)),
                     (uint8_t) (255 * (g - g*g*g/6)),
                     (uint8_t) (255 * (b - b*b*b/6)),
                     alpha).toInteger();
}

//================================================================================

status_t testing_mode(context_t* ctx)
{
    status_t (*compute_func)(context_t* ctx, uint32_t runs) = nullptr;

    switch (ctx->argv.calc)
    {
        case NORMAL: compute_func = &compute_mandelbrot_normal; break;
        case SSE:    compute_func = &compute_mandelbrot_sse;    break;
        case AVX2:   compute_func = &compute_mandelbrot_avx2;   break;
        case AVX512: compute_func = &compute_mandelbrot_avx512; break;
        default:     return ERROR;
    }

    uint32_t n_points = ctx->argv.n_points;
    uint32_t runs     = ctx->argv.runs;

    test_ctx_t test_ctx = {.n_points = n_points};

    test_ctx.x = (double*) calloc(2 * n_points, sizeof(double));
    test_ctx.y = test_ctx.x + n_points;

    uint32_t avg_freq = 0;

    timespec time_start = {};
    timespec time_end   = {};

    uint64_t time = 0;
    uint32_t freq = 0;

    for (uint32_t point = 1; point < n_points + 1; point++)
    {
        uint64_t cycle_start = _rdtsc();
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time_start);

        volatile status_t dummy = compute_func(ctx, point * runs);

        uint64_t cycle_end = _rdtsc();
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time_end);

        time = (time_end.tv_sec   - time_start.tv_sec) * (uint64_t) 1e9 +
               (time_end.tv_nsec  - time_start.tv_nsec);  // nsec
        freq = (uint32_t) ((double) (cycle_end - cycle_start) / (double) time * 1e3); // MhZ

        avg_freq += freq;

        test_ctx.x[point - 1] = (double) (point * runs);
        test_ctx.y[point - 1] = (double) (cycle_end - cycle_start);

        show_progress_bar(n_points, point);
    }

    avg_freq /= n_points;

    calc_least_squares(&test_ctx);

    if (ctx->argv.plot)
        graphic_plot(&test_ctx);

    printf("Time:      (%.1f ± %.1f) ms\n", 1e-3 * test_ctx.slope           / avg_freq,
                                            1e-3 * test_ctx.slope_error     / avg_freq);
    printf("Overheads: (%.1f ± %.1f) ms\n", 1e-3 * test_ctx.intercept       / avg_freq,
                                            1e-3 * test_ctx.intercept_error / avg_freq);

    free(test_ctx.x);

    return SUCCESS;
}

//================================================================================

status_t calc_least_squares(test_ctx_t* ctx)
{
    double* x = ctx->x;
    double* y = ctx->y;

    double av_x  = 0.0,
           av_y  = 0.0,
           av_xy = 0.0,
           av_x2 = 0.0,
           av_y2 = 0.0;

    uint32_t n_points = ctx->n_points;

    for (uint32_t i = 0; i < n_points; i++)
    {
        av_x  += x[i];
        av_y  += y[i];
        av_xy += x[i] * y[i];
        av_x2 += x[i] * x[i];
        av_y2 += y[i] * y[i];
    }

    av_x  /= n_points;
    av_y  /= n_points;
    av_xy /= n_points;
    av_x2 /= n_points;
    av_y2 /= n_points;

    double s  = (av_xy - av_x * av_y) /
                (av_x2 - av_x * av_x);

    double i  = av_y - s * av_x;

    double se = sqrt(((av_y2 - av_y * av_y) /
                      (av_x2 - av_x * av_x) - s * s) /
                     n_points);

    double ie = se * sqrt(av_x2 - av_x * av_x);

    ctx->slope           = s;
    ctx->intercept       = i;
    ctx->slope_error     = se;
    ctx->intercept_error = ie;

    return SUCCESS;
}

//================================================================================

status_t graphic_plot(test_ctx_t* ctx)
{
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (!gnuplot)
    {
        fprintf(stderr, "Error: Unable to open Gnuplot.\n");
        return ERROR;
    }

    fprintf(gnuplot,
            "set title 'Linear Regression Plot'\n"
            "set xlabel 'Repeats'\n"
            "set ylabel 'Cycles'\n"
            "set grid\n"
            "set key left top\n"
            "plot '-' with points pointtype 7 pointsize 1.5 title 'Data Points', "
            "'-' with lines linewidth 2 title 'Regression Line'\n");

    double* x = ctx->x;
    double* y = ctx->y;

    uint32_t n_points = ctx->n_points;

    for (uint32_t i = 0; i < n_points; i++)
    {
        fprintf(gnuplot, "%f %f\n", x[i], y[i]);
    }

    fprintf(gnuplot, "e\n"
                     "%f %f\n"
                     "%f %f\n"
                     "e\n",
                     0.0,             ctx->slope * 0.0             + ctx->intercept,
                     x[n_points - 1], ctx->slope * x[n_points - 1] + ctx->intercept);

    pclose(gnuplot);

    return SUCCESS;
}

//================================================================================

status_t show_progress_bar(uint32_t total, uint32_t current)
{
    uint8_t percent = (uint8_t) (((float) current / (float) total) * 100);
    uint8_t filled  = percent / 2;
    uint8_t empty   = 50 - filled;

    printf("\033[37;42m" "\rProgress = %d%%" "\033[0m" " [", percent);
    for (uint8_t i = 0; i < filled; i++)
        putchar('#');

    for (uint8_t i = 0; i < empty; i++)
        putchar('-');

    putchar(']');

    if (current == total) {
        putchar('\n');
    }

    fflush(stdout);

    return SUCCESS;
}

//================================================================================
