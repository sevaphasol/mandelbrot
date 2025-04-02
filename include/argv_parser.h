#ifndef ARGV_PARSER_H__
#define ARGV_PARSER_H__
//——————————————————————————————————————————————————————————————————————————————
#include <inttypes.h>
//——————————————————————————————————————————————————————————————————————————————
enum ap_status_t
{
    AP_ERROR   = 0,
    AP_SUCCESS = 1,
};
//——————————————————————————————————————————————————————————————————————————————
struct ap_ctx_t
{
    bool      stats;
    bool      plot;
    bool      pulsation;
    uint32_t  mode;
    uint8_t   alpha;
    uint32_t  runs;
    uint32_t  calc;
    uint32_t  n_points;
};
//——————————————————————————————————————————————————————————————————————————————
ap_status_t parse_argv(int argc, char *argv[], ap_ctx_t* ctx);
//——————————————————————————————————————————————————————————————————————————————
#endif // ARGV_PARSER_H__
