#include "PwmFanMgr.h"
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <cmath>
#include "debug.h"


using namespace kiah;

PwmFanMgr *PwmFanMgr::Instance(float max_temp)
{
    static PwmFanMgr instance_;
    instance_.SetMaxTemp(max_temp);
    return &instance_;
}

bool PwmFanMgr::Reset(const char *config)
{
    struct stat f_stat;
    if (config == nullptr)
    {
        return false;
    }
    if(ParseConfig(config)) {
        Start();
        return true;
    } else {
        return false;
    }
}

void PwmFanMgr::Run()
{
    pwm_debug("pmw thread running\n");
    while (is_runnning_)
    {
        float temp = thermal_handler_.GetTemperator();
        auto record = pwm_handler_.GetLevelRecord(temp);
        if(record==nullptr) {
            pwm_error("fail to query perf level w/ %f.\n", temp);
            continue;
        }
        if(curr_record.dev_level != record->dev_level && std::abs(curr_temp-temp) > PWM_FAN_DIFF_THES) {
            curr_record = *record;
            curr_temp = temp;
            pwm_info("using %s w/ temp %f\n", curr_record.ToString().c_str(), curr_temp);
            auto set_ret = thermal_handler_.SetFanPwm(curr_record.dev_level);            
        }

        int interval = std::max<int>(std::floor(curr_record.sample_interval * 1000), 250);
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
    }
    pwm_debug("pwm thread exited.\n");
    is_runnning_ = false;
}

bool PwmFanMgr::Start()
{
    std::lock_guard<std::mutex> _l(state_lock_);
    if (is_runnning_)
    {
        pwm_error("pwm has started\n");
        return true;
    }
    is_runnning_ = true;
#if USING_THREAD
    monitor_thread_ = std::thread(&PwmFanMgr::Run, this);
#else
    Run();
#endif
}

bool PwmFanMgr::ParseConfig(const char *config)
{
    int ret;
    FILE *fp;
    std::vector<char> contents;
    if ((fp = fopen(config, "r")) == NULL)
    {
        pwm_error("read %s failed(%s).\n", config, strerror(errno));
        return false;
    }
    fseek(fp, 0L, SEEK_END);
    long length = ftell(fp);
    if (length <= 0)
    {
        pwm_error("invalid file length %d\n", length);
        fclose(fp);
        return false;
    }
    fseek(fp, 0L, SEEK_SET);
    contents.resize(length);
    ret = fread(contents.data(), sizeof(char), length, fp);
    fclose(fp);

    if (length != contents.size())
    {
        pwm_error("error %d and %d\n", length, contents.size());
        fclose(fp);
        return false;
    }
    {
        std::lock_guard<std::mutex> _l(state_lock_);
        if(!pwm_handler_.Parse(contents.data(), contents.size())) {
            pwm_error("pwm handler fail to parse the content\n");
            return false;
        }
     
    }
    return true;
}

void PwmFanMgr::Join() {
    pwm_debug("wait for exit.\n");
    if (monitor_thread_.joinable())
    {
        monitor_thread_.join();
    }
}

PwmFanMgr::~PwmFanMgr()
{
    is_runnning_ = false;
    if (monitor_thread_.joinable())
    {
        monitor_thread_.join();
    }
    pwm_debug("pwm mgr destroyed.\n");
}