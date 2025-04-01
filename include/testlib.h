#ifndef TESTLIB_H
#define TESTLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>

#define N_POINTS  10

enum testlib_status_h
{
    TESTLIB_ERROR   = 0,
    TESTLIB_SUCCESS = 1,
};

struct testlib_ctx_h
{
    double x[N_POINTS];
    double y[N_POINTS];
    double slope;
    double intercept;
    double slope_error;
    double intercept_error;
};

testlib_status_h calc_least_squares(testlib_ctx_h* ctx);
testlib_status_h graphic_plot(testlib_ctx_h* ctx);
uint64_t         measure_time(int n_reps);

#endif // TESTLIB_H
