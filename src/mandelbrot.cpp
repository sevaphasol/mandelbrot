#include "mandelbrot.h"
#include <sched.h>
#include "argv_parser.h"

//================================================================================

int main(int argc, char* argv[])
{
    struct sched_param param;
    param.sched_priority = 1;
    sched_setscheduler(0, SCHED_FIFO, &param);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "MANDELBROT");
    sf::VertexArray  pixels(sf::Points, WIDTH * HEIGHT);

    context_t ctx = {.window = window,
                     .pixels = pixels};

    ctx_ctor(&ctx);

    parse_argv(argc, argv, &ctx.argv);

    if (ctx.argv.testing)
        testing_mode(&ctx);
    else
        graphic_mode(&ctx);

    return 0;
}

//================================================================================

status_t graphic_mode(context_t* ctx)
{
    struct timespec time_start, time_end;
    uint64_t        cycle_start = 0, cycle_end = 0, time = 0;
    uint            n_frames = 0, n_spikes = 0, freq = 0;

    while (ctx->window.isOpen())
    {
        if (ctx->argv.stats)
        {
            clock_gettime(CLOCK_MONOTONIC, &time_start);
            cycle_start = _rdtsc();
        }

        handle_events(ctx);

        ctx_update(ctx);

        ctx->window.clear();
        ctx->window.draw(ctx->pixels);
        ctx->window.display();

        if (ctx->argv.stats)
        {
            clock_gettime(CLOCK_MONOTONIC, &time_end);
            cycle_end = _rdtsc();

            time = (time_end.tv_sec   - time_start.tv_sec) * (uint64_t) 1e9 +
                            (time_end.tv_nsec  - time_start.tv_nsec);  // nsec
            freq = (uint) ((double) (cycle_end - cycle_start) / (double) time * 1e3); // MhZ

            n_frames++;

            if (time > (uint64_t) 1e8)
                n_spikes++;

            printf("\r| fps = %05.1f | time = %05.1f ms | freq = %u MHz | frames = %u | spikes = %u |",
                   1.0 / ((double) time / 1e9), (double) time / 1e6, freq, n_frames, n_spikes);
            fflush(stdout);
        }
    }

    putchar('\n');

    return SUCCESS;
}

//================================================================================

status_t ctx_ctor(context_t* ctx)
{
    ctx->argv.testing = false;
    ctx->argv.stats   = false;
    ctx->argv.mode    = AVX2;
    ctx->argv.runs    = 1;

    ctx->center_x = START_CENTER_X;
    ctx->center_y = START_CENTER_Y;
    ctx->scale    = START_SCALE;

    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            ctx->pixels[y * WIDTH + x].position = sf::Vector2f((float) x, (float) y);

    return SUCCESS;
}

//================================================================================

status_t ctx_update(context_t* ctx)
{
    switch (ctx->argv.mode)
    {
        case NORMAL: compute_mandelbrot_normal(ctx); break;
        case AVX2:   compute_mandelbrot_avx2(ctx);   break;
        default:     return ERROR;
    }

    draw_mandelbrot(ctx);

    return SUCCESS;
}

//================================================================================

status_t draw_mandelbrot(context_t* ctx)
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            int ind = y * WIDTH + x;
            ctx->pixels[ind].color = get_color(ctx->colors[ind]);
        }
    }

    return SUCCESS;
}

//================================================================================

sf::Color get_color(uint32_t iter)
{
    if (iter == MAX_ITER)
        return sf::Color::Black;

    double t = ((double) iter) / MAX_ITER;
    double r = (t + 1/3 + 0.05*(iter%2)) * 3.14f;
    double g = (t + 7/3 + 0.01*(iter%2)) * 3.14f;
    double b = (t + 5/3 +    0*(iter%2)) * 3.14f;

    return sf::Color((uint8_t) (255 * (r - r*r*r/6)),
                     (uint8_t) (255 * (g - g*g*g/6)),
                     (uint8_t) (255 * (b - b*b*b/6)));
}

//================================================================================

status_t handle_events(context_t* ctx)
{
    sf::Event event;

    while (ctx->window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            ctx->window.close();

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::Left:  ctx->center_x -= 0.1f * ctx->scale;  break;
                case sf::Keyboard::Right: ctx->center_x += 0.1f * ctx->scale;  break;
                case sf::Keyboard::Up:    ctx->center_y -= 0.1f * ctx->scale;  break;
                case sf::Keyboard::Down:  ctx->center_y += 0.1f * ctx->scale;  break;
                case sf::Keyboard::J:     ctx->scale    *= 1.2f;               break;
                case sf::Keyboard::K:     ctx->scale    *= 0.8f;               break;
                default:                                                       break;
            }
        }
    }

    return SUCCESS;
}

//================================================================================
