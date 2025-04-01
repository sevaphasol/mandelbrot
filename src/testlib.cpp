#include <math.h>
#include <time.h>
#include "mandelbrot.h"
#include "testlib.h"

//================================================================================

status_t testing_mode(context_t* ctx)
{
    status_t (*compute_func)(context_t*);

    switch (ctx->argv.mode)
    {
        case NORMAL: compute_func = &compute_mandelbrot_normal; break;
        case AVX2:   compute_func = &compute_mandelbrot_avx2;   break;
        default:     return ERROR;
    }

    testlib_ctx_h test_ctx;

    uint32_t max_runs = ctx->argv.runs;

    for (uint i = 1; i < N_POINTS + 1; i++)
    {
        ctx->argv.runs = i;

        test_ctx.x[i - 1] = (double) i;

        uint64_t start = _rdtsc();
        compute_func(ctx);
        uint64_t end = _rdtsc();

        test_ctx.y[i - 1] = (double) (end - start);
    }

    calc_least_squares(&test_ctx);
    graphic_plot(&test_ctx);

    printf("Time:      (%.1f ± %.1f) ms\n", 1e-3 * test_ctx.slope / AVG_FREQUENCY,
                                            1e-3 * test_ctx.slope_error / AVG_FREQUENCY);
    printf("Overheads: (%.1f ± %.1f) ms\n", 1e-3 * test_ctx.intercept / AVG_FREQUENCY,
                                            1e-3 * test_ctx.intercept_error / AVG_FREQUENCY);

    return SUCCESS;
}

//================================================================================

testlib_status_h calc_least_squares(testlib_ctx_h* ctx)
{
    double* x = ctx->x;
    double* y = ctx->y;

    double av_x  = 0.0,
           av_y  = 0.0,
           av_xy = 0.0,
           av_x2 = 0.0,
           av_y2 = 0.0;

    for (int i = 0; i < N_POINTS; i++)
    {
        av_x  += x[i];
        av_y  += y[i];
        av_xy += x[i] * y[i];
        av_x2 += x[i] * x[i];
        av_y2 += y[i] * y[i];
    }

    av_x  /= N_POINTS;
    av_y  /= N_POINTS;
    av_xy /= N_POINTS;
    av_x2 /= N_POINTS;
    av_y2 /= N_POINTS;

    double s  = (av_xy - av_x * av_y) /
                (av_x2 - av_x * av_x);

    double i  = av_y - s * av_x;

    double se = sqrt(((av_y2 - av_y * av_y) /
                      (av_x2 - av_x * av_x) - i * i) /
                     N_POINTS);

    double ie = se * sqrt(av_x2 - av_x * av_x);

    ctx->slope           = s;
    ctx->intercept       = i;
    ctx->slope_error     = se;
    ctx->intercept_error = ie;

    return TESTLIB_SUCCESS;
}

//================================================================================

testlib_status_h graphic_plot(testlib_ctx_h* ctx)
{
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (!gnuplot)
    {
        fprintf(stderr, "Error: Unable to open Gnuplot.\n");
        return TESTLIB_ERROR;
    }

    double* x = ctx->x;
    double* y = ctx->y;

    fprintf(gnuplot,
            "set title 'Linear Regression Plot'\n"
            "set xlabel 'Repeats'\n"
            "set ylabel 'Cycles'\n"
            "set grid\n"
            "set key left top\n"
            "plot '-' with points pointtype 7 pointsize 1.5 title 'Data Points', "
            "'-' with lines linewidth 2 title 'Regression Line'\n");

    for (int i = 0; i < N_POINTS; i++) {
        fprintf(gnuplot, "%f %f\n", x[i], y[i]);
    }

    double slope     = ctx->slope;
    double intercept = ctx->intercept;

    fprintf(gnuplot, "e\n"
                     "%f %f\n"
                     "%f %f\n"
                     "e\n",
                     0.0,             slope * 0.0             + intercept,
                     x[N_POINTS - 1], slope * x[N_POINTS - 1] + intercept);

    pclose(gnuplot);

    return TESTLIB_SUCCESS;
}
