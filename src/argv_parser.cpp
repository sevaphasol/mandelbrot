#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "argv_parser.h"

inline void print_usage()
{
    printf(
        "Usage: bin/out [options]\n"
           "Options:\n"
           "  --help, -h       Print this message.\n"
           "  --stats, -s      Show stats in graphic mode.\n"
           "  --testing, -t    Disable graphics. Run testing mode.\n"
           "  --plot, -p       Turn on plotting a graph.\n"
           "  --runs=<number>  Set number of runs in testing mode.\n"
           "  --mode=<number>  Set mode of calculating.\n"
           "                   0 — NORMAL\n"
           "                   1 — SSE\n"
           "                   2 — AVX2\n"
           "                   3 — AVX-512\n");
}

ap_status_t parse_argv(int argc, char *argv[], ap_ctx_t* ctx)
{
    int opt = 0, option_index = 0;

    option long_options[] = {
        {"help",    no_argument,       NULL, 'h'},
        {"stats",   no_argument,       NULL, 's'},
        {"testing", no_argument,       NULL, 't'},
        {"plot",    no_argument,       NULL, 'p'},
        {"runs",    required_argument, NULL, 'r'},
        {"mode",    required_argument, NULL, 'm'}
    };

    while ((opt = getopt_long(argc, argv, "hstpr:m:", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
            case 'h': print_usage();               break;
            case 's': ctx->stats   = true;         break;
            case 't': ctx->testing = true;         break;
            case 'p': ctx->plot    = true;         break;
            case 'r': ctx->runs    = atoi(optarg); break;
            case 'm': ctx->mode    = atoi(optarg); break;
            case '?': print_usage();               return AP_ERROR;
            default:  print_usage();               return AP_ERROR;
        }
    }

    return AP_SUCCESS;
}
