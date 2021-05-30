#include <algorithm>
#include "PwmHandler.h"
#include "debug.h"

using namespace kiah;

const PwmLevelRecord* PwmHandler::GetLevelRecord(float temperator) {
    auto record = std::find_if(records_.begin(), records_.end(), [temperator](const PwmLevelRecord& value) -> bool{
        if (value.temp_thsd >= temperator) {
            return true;
        } else {
            return false;
        }
    });

    if (record != records_.end()) {
        return &(*record);
    } else {
        return &(*(records_.begin()+records_.size()-1));
    }
}

PwmLevelRecord PwmHandler::BuildRecord(const cJSON* json) {
    PwmLevelRecord record = {0};
    if (json == nullptr) {
        return record;
    }
    record.temp_thsd = cJSON_GetObjectItem(json, "temp_thsd")->valuedouble;
    record.sample_interval = cJSON_GetObjectItem(json, "sample_interval")->valuedouble;
    record.dev_level = cJSON_GetObjectItem(json, "dev_level")->valueint;
    return record;
}

bool PwmHandler::Parse(const char *content, size_t length)
{
    cJSON *root_json = cJSON_ParseWithLength(content, length);
    if (root_json == NULL)
    {
        pwm_error("json loaded fail.\n");
        return false;
    }
    int array_size = 0;
    std::vector<const cJSON *> json_records;
    if (cJSON_IsArray(root_json))
    {
        array_size = cJSON_GetArraySize(root_json);
        for (int i = 0; i < array_size; i++)
        {
            json_records.push_back(cJSON_GetArrayItem(root_json, i));
        }
    }
    else
    {
        json_records.push_back(root_json);
    }
    std::sort(json_records.begin(), json_records.end(), [](const cJSON * l, const cJSON * r)-> bool
              { 
                  int l_temp = cJSON_GetObjectItem(l, "temp_thsd")->valuedouble;
                  int r_temp = cJSON_GetObjectItem(r, "temp_thsd")->valuedouble;
                  if (l_temp <= r_temp) {
                      return true;
                  } else {
                      return false;
                  }
               });

    records_.clear();
    for(auto& job:json_records) {
        auto pwm_r = BuildRecord(job);
        pwm_debug("%s\n", pwm_r.ToString().c_str());
        records_.push_back(pwm_r);
    }
    pwm_debug("%d\n", records_.size());

    cJSON_Print(root_json);
    return true;
}