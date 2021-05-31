#pragma once

#include <string>
#include <atomic>
#include <vector>
#include <mutex>
#include <thread>
#include "PwmHandler.h"
#include "ThermalHandler.h"

#define PWM_MAX_TEMPERATOR_THRESHOLD 62.0f
#define PWM_FAN_DIFF_THES 1.5f

namespace kiah
{
    class PwmFanMgr
    {
    public:
        static PwmFanMgr *Instance();
        void Run();
        bool Reset(const char *config);
        bool Start();
        void Join();
        bool SetPwm(const PwmLevelRecord& rec);
        
        void SetMaxTemp(float max_temp);
        ~PwmFanMgr();

    private:

        bool ParseConfig(const char *config);

        std::string config_file_;
        float max_temp_;
        std::mutex state_lock_;
        std::atomic_bool is_runnning_{false};
        std::thread monitor_thread_;
        PwmHandler pwm_handler_;
        PwmLevelRecord curr_record;
        float curr_temp = 0.0f;
        ThermalHandler thermal_handler_;
    };

    inline void PwmFanMgr::SetMaxTemp(float max_temp) {
            std::lock_guard<std::mutex> _l(state_lock_);
            max_temp_ = max_temp;
        }
}