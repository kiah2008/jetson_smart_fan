#pragma once

#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif
    extern void pwm_set_debug(int config);
    extern void pwm_set_syslog(int config);
    extern void pwm_debug(const char *msg, ...);
    extern void pwm_error(const char *msg, ...);
    extern void pwm_info(const char *msg, ...);

#ifdef __cplusplus
}
#endif