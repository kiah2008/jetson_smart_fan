
#pragma once

#include <cJSON/cJSON.h>
#include <sys/types.h>
#include <vector>
#include <string>

#define PWM_MAX_TEMPERATOR 60

namespace kiah
{
    typedef struct PwmLevelRecord
    {
        float temp_thsd;
        float sample_interval;
        int32_t dev_level;
        std::string ToString() const {
            return "rec {"+std::to_string(temp_thsd)+"c,"
            +std::to_string(sample_interval)+"s," + std::to_string(dev_level)+"}";
        }
    } PwmLevelRecord;

    class PwmHandlerBase
    {
    public:
        //parse and sort the records
        virtual bool Parse(const char *content, size_t length) = 0;
        virtual const PwmLevelRecord* GetLevelRecord(float temperator) = 0;
        virtual size_t GetRecordsSize() = 0;
    };

    class PwmHandler : public PwmHandlerBase
    {
    public:
        virtual bool Parse(const char *content, size_t length);
        virtual const PwmLevelRecord* GetLevelRecord(float temperator);
        virtual size_t GetRecordsSize() {
            return records_.size();
        }
        virtual ~PwmHandler() = default;

    private:
        std::vector<PwmLevelRecord> records_;
        PwmLevelRecord BuildRecord(const cJSON*);
    };
}