#include "PrefabFactory.h"
#include "Components/TextureComponent.h"
#include "Components/AnimatorComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/TagComponent.h"
#include "Components/PlayerCharacter/SaltComponent.h"
#include "Components/LifeTimeComponent.h"
#include "Helpers/LevelGrid.h"

namespace dae
{
    std::unique_ptr<GameObject> PrefabFactory::CreateSaltProjectile(const glm::vec3& position)
    {
        auto saltObj = std::make_unique<GameObject>("SaltProjectile");
        saltObj->GetTransform().SetLocalPosition(position);

        float tileSize = LevelGrid::GetInstance().GetTileSize();

        auto tex = saltObj->AddComponent<TextureComponent>();
        tex->SetTexture("burger_time_spritesheet_x3.png");

        auto anim = saltObj->AddComponent<AnimatorComponent>(48, 48);
        anim->AddAnimation("Splash", { 1, 12, 4, 0.1f, false });
        anim->PlayAnimation("Splash");

        saltObj->AddComponent<BoxColliderComponent>(tileSize, tileSize);

        std::unordered_set<Tag> tags = { make_sdbm_hash_rt("Salt") };
        saltObj->AddComponent<TagComponent>(tags);

        saltObj->AddComponent<SaltComponent>();
        saltObj->AddComponent<LifetimeComponent>(0.4f);

        return saltObj;
    }
}