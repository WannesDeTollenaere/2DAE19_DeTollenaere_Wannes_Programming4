#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct FPSDynamicTextComponentDTO
    {
        std::string postfix;

        static FPSDynamicTextComponentDTO FromJson(const nlohmann::json& data)
        {
            FPSDynamicTextComponentDTO dto;
            dto.postfix = data.value("postfix", " FPS");
            return dto;
        }
    };
}