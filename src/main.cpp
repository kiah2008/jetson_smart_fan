#include <iostream>
#include <getopt.h>
#include <signal.h>
#include "PwmFanMgr.h"
#include "debug.h"

const char *print_help = R"(
    pwm_fan -h|-f parse_file
    -h      show this help
    -f      the json config file to be parsed
    -s      save log to syslog
    -d      enable debug log
    -m n    set max temperator threshold   
)";

void handler(int signo)
{
	pwm_info("SIGCONT caught\n");
    auto pwm=kiah::PwmFanMgr::Instance();
    pwm->SetFanPwm(0);
}

int main(int argc, char **argv)
{
    int opt;
    const char *json_file = nullptr;
    float max_temperator = PWM_MAX_TEMPERATOR_THRESHOLD;
    struct sigaction act;

    act.sa_handler = handler;
    act.sa_flags = SA_NODEFER;
    sigemptyset(&act.sa_mask);
    if (sigaction(SIGCONT, &act, 0) == -1)
    {
        pwm_error("Unexpected error while attempting to "
               "setup test pre-conditions");
        exit(1);
    }

    while ((opt = getopt(argc, argv, "hsdf:m:")) != -1)
    {
        switch (opt)
        {
        case '?':
            break;
        case 'h':
            printf("%s\n", print_help);
            break;
        case 'm':
            max_temperator = atof(optarg);
            break;
        case 's':
            pwm_set_syslog(1);
            break;
        case 'd':
            pwm_set_debug(1);
            break;
        case 'f':
            json_file = optarg;
            break;
        }
    }

    pwm_info("cfan started with %s and max temp %f.\n", json_file, max_temperator);
    if (json_file != nullptr)
    {
        auto pwm = kiah::PwmFanMgr::Instance();
        pwm->SetMaxTemp(max_temperator);
        if (pwm != nullptr)
        {
            pwm_info("open config %s\n", json_file);
            pwm->Reset(json_file);
#if USING_THREAD
            pwm->Join();
#endif
        }
    }
    else
    {
        pwm_error("no json config available.\n");
        exit(1);
    }
    pwm_info("pwm process exited.\n");
    return 0;
}
