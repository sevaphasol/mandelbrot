#include <immintrin.h>
#include "mandelbrot.h"

#include <immintrin.h>
#include "mandelbrot.h"

//================================================================================

status_t compute_mandelbrot_normal(context_t* ctx, uint32_t runs)
{
    double scale    = ctx->scale;

    double dy       = scale / ((double) HEIGHT);
    double dx       = scale / ((double) WIDTH);

    double x0_start = ctx->center_x - scale * 0.5f;
    double y0_start = ctx->center_y - scale * 0.5f;

    double max_r2   = MAX_RADIUS * MAX_RADIUS;

    for (uint32_t n = 0; n < runs; n++)
    {
        double y0 = y0_start;

        for (uint32_t yi = 0; yi < HEIGHT; yi++)
        {
            double x0 = x0_start;

            for (uint32_t xi = 0; xi < WIDTH; xi++)
            {
                double y = y0;
                double x = x0;

                uint32_t iter = 0;

                for (uint32_t i = 0; i < MAX_ITER; i++)
                {
                    double x2 = x * x;
                    double y2 = y * y;

                    double r2 = x2 + y2;

                    if (r2 >= max_r2)
                        break;

                    iter++;

                    double xy = x * y;
                    x = x2 - y2 + x0;
                    y = xy + xy + y0;
                }

                ctx->pixels[yi * WIDTH + xi] = iter;
                x0 += dx;
            }

            y0 += dy;
        }
    }

    return SUCCESS;
}

//================================================================================

status_t compute_mandelbrot_sse(context_t* ctx, uint32_t runs)
{
    double scale     = ctx->scale;
    double single_dx = scale / ((double) WIDTH);

    __m128d dy = _mm_set1_pd(scale / ((double) HEIGHT));
    __m128d dx = _mm_set1_pd((double) 2 * single_dx);

    __m128d y0_start = _mm_set1_pd(ctx->center_y - scale * 0.5f);
    __m128d x0_start = _mm_add_pd(_mm_set1_pd(ctx->center_x - scale * 0.5f),
                                    _mm_set_pd (single_dx,
                                                0.0f));

    __m128d max_r2 = _mm_set1_pd(MAX_RADIUS * MAX_RADIUS);
    __m128i mask   = _mm_set1_epi64x(1);

    for (uint32_t n = 0; n < runs; n++)
    {
        __m128d y0 = y0_start;

        for (uint32_t yi = 0; yi < HEIGHT; yi++)
        {
            __m128d x0 = x0_start;

            for (uint32_t xi = 0; xi < WIDTH; xi += 2)
            {
                __m128d y = y0;
                __m128d x = x0;

                __m128i iter = _mm_setzero_si128();;

                for (uint32_t i = 0; i < MAX_ITER; i++)
                {
                    __m128d x2 = _mm_mul_pd(x, x);
                    __m128d y2 = _mm_mul_pd(y, y);

                    __m128d r2 = _mm_add_pd(x2, y2);

                    __m128d cmp = _mm_cmp_pd(r2, max_r2, _CMP_LT_OS);

                    if (_mm_testz_pd(cmp, cmp))
                        break;

                    iter = _mm_add_epi64(_mm_and_si128((__m128i) cmp, mask), iter);

                    __m128d xy = _mm_mul_pd(x, y);
                    x = _mm_add_pd(_mm_sub_pd(x2, y2), x0);
                    y = _mm_add_pd(_mm_add_pd(xy, xy), y0);
                }

                _mm_store_si128((__m128i*) &ctx->pixels[yi * WIDTH + xi], iter);

                x0 = _mm_add_pd(x0, dx);
            }

            y0 = _mm_add_pd(y0, dy);
        }
    }

    return SUCCESS;
}

//================================================================================

status_t compute_mandelbrot_avx2(context_t* ctx, uint32_t runs)
{
    double scale     = ctx->scale;
    double single_dx = scale / ((double) WIDTH);

    __m256d dy = _mm256_set1_pd(scale / ((double) HEIGHT));
    __m256d dx = _mm256_set1_pd((double) 4 * single_dx);

    __m256d y0_start = _mm256_set1_pd(ctx->center_y - scale * 0.5f);
    __m256d x0_start = _mm256_add_pd(_mm256_set1_pd(ctx->center_x - scale * 0.5f),
                                    _mm256_set_pd (single_dx * 4.0f,
                                                   single_dx * 2.0f,
                                                   single_dx,
                                                   0.0f));

    __m256d  max_r2 = _mm256_set1_pd(MAX_RADIUS * MAX_RADIUS);
    __m256i mask   = _mm256_set1_epi64x(1);

    for (uint32_t n = 0; n < runs; n++)
    {
        __m256d y0 = y0_start;

        for (uint32_t yi = 0; yi < HEIGHT; yi++)
        {
            __m256d x0 = x0_start;

            for (uint32_t xi = 0; xi < WIDTH; xi += 4)
            {
                __m256d y = y0;
                __m256d x = x0;

                __m256i iter = _mm256_setzero_si256();

                for (uint32_t i = 0; i < MAX_ITER; i++)
                {
                    __m256d x2 = _mm256_mul_pd(x, x);
                    __m256d y2 = _mm256_mul_pd(y, y);

                    __m256d r2 = _mm256_add_pd(x2, y2);

                    __m256d cmp = _mm256_cmp_pd(r2, max_r2, _CMP_LT_OS);

                    if (_mm256_testz_pd(cmp, cmp))
                        break;

                    iter = _mm256_add_epi64(_mm256_and_si256((__m256i) cmp, mask), iter);

                    __m256d xy = _mm256_mul_pd(x, y);

                    x = _mm256_add_pd(_mm256_sub_pd(x2, y2), x0);
                    y = _mm256_add_pd(_mm256_add_pd(xy, xy), y0);
                }

                _mm256_store_si256((__m256i*) &ctx->pixels[yi * WIDTH + xi], iter);

                x0 = _mm256_add_pd(x0, dx);
            }

            y0 = _mm256_add_pd(y0, dy);
        }
    }

    return SUCCESS;
}

//================================================================================

status_t compute_mandelbrot_avx512(context_t* ctx, uint32_t runs)
{
    double scale     = ctx->scale;
    double single_dx = scale / ((double) WIDTH);

    __m512d dy = _mm512_set1_pd(scale / ((double) HEIGHT));
    __m512d dx = _mm512_set1_pd((double) 8 * single_dx);

    __m512d y0_start = _mm512_set1_pd(ctx->center_y - scale * 0.5f);
    __m512d x0_start = _mm512_add_pd(_mm512_set1_pd(ctx->center_x - scale * 0.5f),
                                    _mm512_set_pd (single_dx * 7.0f,
                                                   single_dx * 6.0f,
                                                   single_dx * 5.0f,
                                                   single_dx * 3.0f,
                                                   single_dx * 4.0f,
                                                   single_dx * 2.0f,
                                                   single_dx,
                                                   0.0f));

    __m512d  max_r2 = _mm512_set1_pd(MAX_RADIUS * MAX_RADIUS);
    __m512i mask    = _mm512_set1_epi64(1);

    for (uint32_t n = 0; n < runs; n++)
    {
        __m512d y0 = y0_start;

        for (uint32_t yi = 0; yi < HEIGHT; yi++)
        {
            __m512d x0 = x0_start;

            for (uint32_t xi = 0; xi < WIDTH; xi += 8)
            {
                __m512d y = y0;
                __m512d x = x0;

                __m512i iter = _mm512_setzero_si512();

                for (uint32_t i = 0; i < MAX_ITER; i++)
                {
                    __m512d x2 = _mm512_mul_pd(x, x);
                    __m512d y2 = _mm512_mul_pd(y, y);

                    __m512d r2 = _mm512_add_pd(x2, y2);

                    __mmask8 cmp = _mm512_cmp_pd_mask(r2, max_r2, _CMP_LT_OS);

                    if (_mm512_kortestz(cmp, cmp))
                        break;

                    iter = _mm512_mask_add_epi64(iter, cmp, iter, mask);

                    __m512d xy = _mm512_mul_pd(x, y);

                    x = _mm512_add_pd(_mm512_sub_pd(x2, y2), x0);
                    y = _mm512_add_pd(_mm512_add_pd(xy, xy), y0);
                }

                _mm512_store_si512((__m512i*) &ctx->pixels[yi * WIDTH + xi], iter);

                x0 = _mm512_add_pd(x0, dx);
            }

            y0 = _mm512_add_pd(y0, dy);
        }
    }

    return SUCCESS;
}

//================================================================================
