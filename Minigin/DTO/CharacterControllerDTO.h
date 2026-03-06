#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace dae
{
    struct CharacterControllerDTO
    {
        float speed;
        bool useKeyboard;
        int controllerIndex;

        static CharacterControllerDTO FromJson(const nlohmann::json& data)
        {
            CharacterControllerDTO dto;
            dto.speed = data.value("speed", 50.0f);

            std::string inputType = data.value("inputType", "keyboard");
            dto.useKeyboard = (inputType == "keyboard");

            dto.controllerIndex = data.value("controllerIndex", 0);

            return dto;
        }
    };
}