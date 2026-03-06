#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <map>

namespace dae
{
    struct AnimatorComponentDTO
    {
        int frameWidth;
        int frameHeight;
        std::string playOnStart;
        std::map<std::string, dae::AnimationClip> animations;

        static AnimatorComponentDTO FromJson(const nlohmann::json& data)
        {
            AnimatorComponentDTO dto;
            dto.frameWidth = data.value("frameWidth", 16);
            dto.frameHeight = data.value("frameHeight", 16);
            dto.playOnStart = data.value("playOnStart", "");

            if (data.contains("animations") && data["animations"].is_object())
            {
                for (auto& [key, val] : data["animations"].items())
                {
                    dae::AnimationClip clip{};
                    clip.startRow = val.value("startRow", 0);
                    clip.startCol = val.value("startCol", 0);
                    clip.frameCount = val.value("frameCount", 1);
                    clip.frameTime = val.value("frameTime", 0.1f);
                    clip.isLooping = val.value("isLooping", true);

                    dto.animations[key] = clip;
                }
            }
            return dto;
        }
    };
}