#include "ThermalHandler.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "debug.h"

using namespace kiah;

float ThermalHandler::GetTemperator() {
    char temp_str[24]={0};
    float read_temp = 60.0f;
    FILE* thermal = fopen(GetThermalTempPath(), "r");
    if(thermal == NULL) {
        pwm_error("error to open thermal temp, %s.\n", strerror(errno));
        return read_temp;
    }
    if(fgets(temp_str, 23, thermal)!= NULL) {
        read_temp=atof(temp_str)/1000.0f;
    } else {
        pwm_error("fail to read file %s.\n", strerror(errno));
    }
    fclose(thermal);
    return read_temp;
}

bool  ThermalHandler::SetFanPwm(int level) {
    FILE* fan_target = fopen(GetFanTarget(), "w");
    if(fan_target == NULL) {
        pwm_error("error to open fan target, %s.\n", strerror(errno));
        return false;
    }
    fprintf(fan_target, "%d", level);
    pwm_info("using dev level %d\n", level);
    fclose(fan_target);
    return true;
}