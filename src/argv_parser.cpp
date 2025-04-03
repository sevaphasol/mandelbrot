#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "argv_parser.h"

//==============================================================================

inline void print_usage()
{
    puts("Usage: bin/out [options]\n"
         "Options:\n"
         "  --help, -h           Print this message.\n"
         "  --stats, -s          Show stats in graphic mode.\n"
         "  --plot, -p           Turn on plotting a graph.\n"
         "  --Pulsation, -P      Turn on pulsation of image.\n"
         "  --graphics, -g       Run graphics mode (default).\n"
         "  --testing, -t        Run testing mode.\n"
         "  --benchmark, -b      Run benchmark mode.\n"
         "  --mode=<number>      Set programm mode.\n"
         "                       0 — GRAPHICS\n"
         "                       1 — TESTING\n"
         "                       2 — BENCHMARK\n"
         "  --alpha<number>      Set alpha (transparency of pixels).\n"
         "  --runs=<number>      Set number of runs in testing mode.\n"
         "  --calc=<number>      Set mode of calculating.\n"
         "                       0 — BASELINE\n"
         "                       1 — SSE\n"
         "                       2 — AVX2\n"
         "                       3 — AVX-512\n"
         "  --n_points=<number>  Set amount of points on a plot.\n");
}

//==============================================================================

ap_status_t parse_argv(int argc, char *argv[], ap_ctx_t* ctx)
{
    int opt    = 0;
    int optind = 0;
    //--------------------------------------------------------------------------
    option l_opts[] = {
        {"help",      no_argument,       NULL, 'h'},
        {"stats",     no_argument,       NULL, 's'},
        {"plot",      no_argument,       NULL, 'p'},
        {"Pulsation", no_argument,       NULL, 'P'},
        {"graphics",  no_argument,       NULL, 'g'},
        {"testing",   no_argument,       NULL, 't'},
        {"benchmark", no_argument,       NULL, 'b'},
        {"mode",      required_argument, NULL, 'm'},
        {"alpha",     required_argument, NULL, 'a'},
        {"runs",      required_argument, NULL, 'r'},
        {"calc",      required_argument, NULL, 'c'},
        {"n_points",  required_argument, NULL, 'n'}
    };
    //--------------------------------------------------------------------------
    const char* s_opts = "hspPtgbm:a:r:c:n:";
    //--------------------------------------------------------------------------
    while ((opt = getopt_long(argc, argv, s_opts, l_opts, &optind)) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                break;
            case 's':
                ctx->stats = true;
                break;
            case 'p':
                ctx->plot = true;
                break;
            case 'P':
                ctx->pulsation = true;
                break;
            case 'g':
                ctx->mode = 0;
                break;
            case 't':
                ctx->mode = 1;
                break;
            case 'b':
                ctx->mode = 2;
                break;
            case 'm':
                ctx->mode = atoi(optarg);
                break;
            case 'a':
                ctx->alpha = (uint8_t )atoi(optarg);
                break;
            case 'r':
                ctx->runs = atoi(optarg);
                break;
            case 'c':
                ctx->calc = atoi(optarg);
                break;
            case 'n':
                ctx->n_points = atoi(optarg);
                break;
            case '?':
                print_usage();
                return AP_ERROR;
            default:
                print_usage();
                return AP_ERROR;
        }
    }
    //--------------------------------------------------------------------------
    return AP_SUCCESS;
}
