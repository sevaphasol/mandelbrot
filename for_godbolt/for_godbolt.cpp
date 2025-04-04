#include <immintrin.h>
#include <inttypes.h>

#define WIDTH      (64*16)
#define HEIGHT     (64*16)
#define MAX_ITER   128
#define MAX_RADIUS 10.0

enum status_t
{
    SUCCESS = 0,
    ERROR   = 1,
};

typedef struct context_t
{
    uint64_t* pixels;
    double center_x;
    double center_y;
    double scale;
} context_t;

typedef enum mandelbrot_status_t {
    MANDELBROT_SUCCESS = 0,
    MANDELBROT_ERROR   = 1,
} mandelbrot_status_t;

mandelbrot_status_t compute_mandelbrot_avx512(context_t* ctx, uint32_t runs)
{
    double scale     = ctx->scale;
    double single_dx = scale / ((double) WIDTH);
    //--------------------------------------------------------------------------
    __m512d dy = _mm512_set1_pd(scale / ((double) HEIGHT));
    __m512d dx = _mm512_set1_pd((double) 8 * single_dx);
    //--------------------------------------------------------------------------
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
    //--------------------------------------------------------------------------
    __m512d  max_r2 = _mm512_set1_pd(MAX_RADIUS * MAX_RADIUS);
    __m512i mask    = _mm512_set1_epi64(1);
    //--------------------------------------------------------------------------
    for (uint32_t n = 0; n < runs; n++) {
        __m512d y0 = y0_start;
        //----------------------------------------------------------------------
        for (uint32_t yi = 0; yi < HEIGHT; yi++) {
            __m512d x0 = x0_start;
            //------------------------------------------------------------------
            for (uint32_t xi = 0; xi < WIDTH; xi += 8) {
                __m512d y = y0;
                __m512d x = x0;
                //--------------------------------------------------------------
                __m512i iter = _mm512_setzero_si512();
                //--------------------------------------------------------------
                for (uint32_t i = 0; i < MAX_ITER; i++) {
                    __m512d x2 = _mm512_mul_pd(x, x);
                    __m512d y2 = _mm512_mul_pd(y, y);
                    //----------------------------------------------------------
                    __m512d r2 = _mm512_add_pd(x2, y2);
                    //----------------------------------------------------------
                    __mmask8 cmp = _mm512_cmp_pd_mask(r2, max_r2, _CMP_LT_OS);
                    //----------------------------------------------------------
                    if (_mm512_kortestz(cmp, cmp)) {
                        break;
                    }
                    //----------------------------------------------------------
                    iter = _mm512_mask_add_epi64(iter, cmp, iter, mask);
                    //----------------------------------------------------------
                    __m512d xy = _mm512_mul_pd(x, y);
                    //----------------------------------------------------------
                    x = _mm512_add_pd(_mm512_sub_pd(x2, y2), x0);
                    y = _mm512_add_pd(_mm512_add_pd(xy, xy), y0);
                }
                //--------------------------------------------------------------
                _mm512_store_si512((__m512i*) &ctx->pixels[yi * WIDTH + xi], iter);
                //--------------------------------------------------------------
                x0 = _mm512_add_pd(x0, dx);
            }
            //------------------------------------------------------------------
            y0 = _mm512_add_pd(y0, dy);
        }
    }
    //--------------------------------------------------------------------------
    return MANDELBROT_SUCCESS;
}
