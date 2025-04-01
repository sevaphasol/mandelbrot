#ifndef ARGV_PARSER_H__
#define ARGV_PARSER_H__

#include <inttypes.h>

enum ap_status_t
{
    AP_ERROR   = 0,
    AP_SUCCESS = 1,
};

struct ap_ctx_t
{
    bool      stats;
    bool      testing;
    bool      plot;
    uint32_t  runs;
    uint32_t  mode;
};

ap_status_t parse_argv(int argc, char *argv[], ap_ctx_t* ctx);

#endif // ARGV_PARSER_H__
