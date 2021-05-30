#pragma once

#define THERMAL_TEMP "/sys/devices/virtual/thermal/thermal_zone0/temp"
#define PWM_FAN_TARGET "/sys/devices/pwm-fan/target_pwm"

namespace kiah {
    class ThermalHandler {
        public:
            float GetTemperator();
            bool SetFanPwm(int level);

            const char* GetThermalTempPath() {
                return THERMAL_TEMP;
            }

            const char* GetFanTarget() {
                return PWM_FAN_TARGET;
            }
    };
}