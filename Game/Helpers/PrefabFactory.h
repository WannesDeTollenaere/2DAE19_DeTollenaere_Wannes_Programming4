#pragma once
#include "GameObject.h"
#include <glm/vec3.hpp>
#include <memory>

namespace dae
{
    class PrefabFactory
    {
    public:
        static std::unique_ptr<GameObject> CreateSaltProjectile(const glm::vec3& position);
    };
}