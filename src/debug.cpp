#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "debug.h"


#if HAVE_VSYSLOG
#include <syslog.h>
#endif /* HAVE_VSYSLOG */

static int _debug = 0;
static int _syslog = 0;

void pwm_set_debug(int config) {
    _debug = config;
}
void pwm_set_syslog(int config) {
    _syslog = config;
}

void pwm_debug(const char *msg, ...)
{
    va_list ap;
    if (_debug)
    {
        va_start(ap, msg);
#if HAVE_VSYSLOG
        if (_syslog)
        {
            vsyslog(LOG_DEBUG, msg, ap);
        }
        else
#endif
            vprintf(msg, ap);
        va_end(ap);
    }
}

void pwm_error(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);
#if HAVE_VSYSLOG
    if (_syslog)
    {
        vsyslog(LOG_ERR, msg, ap);
    }
    else
#endif
        vfprintf(stderr, msg, ap);
    va_end(ap);
}

void pwm_info(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);
#if HAVE_VSYSLOG
    if (_syslog)
    {
        vsyslog(LOG_INFO, msg, ap);
    }
    else
#endif
        vfprintf(stderr, msg, ap);
    va_end(ap);
}