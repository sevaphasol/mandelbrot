#include <immintrin.h>
#include "mandelbrot.h"

//================================================================================

status_t compute_mandelbrot_normal(context_t* ctx)
{
    uint32_t runs  = ctx->argv.runs;
    float    scale = ctx->scale;

    float dy       = scale / ((float) HEIGHT);
    float dx       = scale / ((float) WIDTH);

    float x0_start = ctx->center_x - scale * 0.5f;
    float y0_start = ctx->center_y - scale * 0.5f;

    float max_r2   = MAX_RADIUS * MAX_RADIUS;

    for (uint32_t n = 0; n < runs; n++)
    {
        float y0 = y0_start;

        for (uint32_t yi = 0; yi < HEIGHT; yi++)
        {
            float x0 = x0_start;

            for (uint32_t xi = 0; xi < WIDTH; xi++)
            {
                float y = y0;
                float x = x0;

                uint32_t iter = 0;

                for (uint32_t i = 0; i < MAX_ITER; i++)
                {
                    float x2 = x * x;
                    float y2 = y * y;

                    float r2 = x2 + y2;

                    if (r2 >= max_r2)
                        break;

                    iter++;

                    float xy = x * y;
                    x = x2 - y2 + x0;
                    y = xy + xy + y0;
                }

                ctx->colors[yi * WIDTH + xi] = iter;
                x0 += dx;
            }

            y0 += dy;
        }
    }

    return SUCCESS;
}

//================================================================================

#define MM_SET_PS(_e7, _e6, _e5, _e4, _e3, _e2, _e1, _e0) \
    _mm256_set_ps(_e7, _e6, _e5, _e4, _e3, _e2, _e1, _e0)
#define MM_SET1_PS(_a) _mm256_set1_ps(_a)

#define MM_ADD_PS(_a, _b) _mm256_add_ps(_a, _b)
#define MM_MUL_PS(_a, _b) _mm256_mul_ps(_a, _b)
#define MM_SUB_PS(_a, _b) _mm256_sub_ps(_a, _b)

#define MM_FLOAT_TO_INT(_a) _mm256_cvtps_epi32(_a)
#define MM_ADD_INT(_a, _b)  _mm256_add_epi32(_a, _b)

#define MM_SET1_INT(_a)  _mm256_set1_epi32(_a)
#define MM_AND_INT(_a, _b) _mm256_and_si256(_a, _b)
#define MM_CMP_PS(_a, _b) _mm256_cmp_ps(_a, _b, _CMP_LT_OS)
#define MM_IS_ZERO(_a)    _mm256_testz_ps(_a, _a)
#define MM_ZERO _mm256_setzero_si256()

#define MM_SIZE 8
#define X_DELTAS(_dx) MM_SET_PS(_dx * 7.0f, \
                                _dx * 6.0f, \
                                _dx * 5.0f, \
                                _dx * 4.0f, \
                                _dx * 3.0f, \
                                _dx * 2.0f, \
                                _dx * 1.0f, \
                                      0.0f)

#define MM_STORE(_mem_addr, _a) _mm256_store_si256(_mem_addr, _a)

typedef __m256  mm_float_t;
typedef __m256i mm_int_t;

//================================================================================

status_t compute_mandelbrot_avx2(context_t* ctx)
{
    uint32_t runs   = ctx->argv.runs;
    float scale     = ctx->scale;
    float single_dx = scale / ((float) WIDTH);

    mm_float_t dy = MM_SET1_PS(scale / (float) HEIGHT);
    mm_float_t dx = MM_SET1_PS((float) MM_SIZE * single_dx);

    mm_float_t y0_start = MM_SET1_PS(ctx->center_y - scale * 0.5f);
    mm_float_t x0_start = MM_ADD_PS(MM_SET1_PS(ctx->center_x - scale * 0.5f),
                                    X_DELTAS(single_dx));

    mm_float_t max_r2  = MM_SET1_PS(MAX_RADIUS * MAX_RADIUS);

    mm_int_t mask = MM_SET1_INT(1);

    for (uint32_t n = 0; n < runs; n++)
    {
        mm_float_t y0 = y0_start;

        for (uint32_t yi = 0; yi < HEIGHT; yi++)
        {
            mm_float_t x0 = x0_start;

            for (uint32_t xi = 0; xi < WIDTH; xi += MM_SIZE)
            {
                mm_float_t y = y0;
                mm_float_t x = x0;

                mm_int_t iter = MM_ZERO;

                for (uint32_t i = 0; i < MAX_ITER; i++)
                {
                    mm_float_t x2 = MM_MUL_PS(x, x);
                    mm_float_t y2 = MM_MUL_PS(y, y);

                    mm_float_t r2 = MM_ADD_PS(x2, y2);

                    mm_float_t cmp = MM_CMP_PS(r2, max_r2);

                    if (MM_IS_ZERO(cmp))
                        break;

                    iter = MM_ADD_INT(MM_AND_INT((mm_int_t) cmp, mask), iter);

                    mm_float_t xy = MM_MUL_PS(x, y);
                    x = MM_ADD_PS(MM_SUB_PS(x2, y2), x0);
                    y = MM_ADD_PS(MM_ADD_PS(xy, xy), y0);
                }

                MM_STORE((mm_int_t*) &ctx->colors[yi * WIDTH + xi], iter);

                x0 = MM_ADD_PS(x0, dx);
            }

            y0 = MM_ADD_PS(y0, dy);
        }
    }

    return SUCCESS;
}

//================================================================================
