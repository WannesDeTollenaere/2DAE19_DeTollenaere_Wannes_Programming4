#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct CacheProfilerComponentDTO
    {
        static CacheProfilerComponentDTO FromJson(const nlohmann::json&)
        {
            return CacheProfilerComponentDTO{};
        }
    };
}