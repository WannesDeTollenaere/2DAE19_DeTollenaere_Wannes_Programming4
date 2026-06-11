#include "PickupSpawnerComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SceneLoader.h"
#include "Helpers/LevelGrid.h"
#include <nlohmann/json.hpp>
#include <random>
#include "Sound/ServiceLocator.h"
#include "Helpers/SoundIDs.h"
#include "ResourceManager.h"

namespace dae
{
    class PickupSpawnerComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            std::vector<std::string> prefabs = {
                "Prefabs/Pickups/IceCream.json",
                "Prefabs/Pickups/Coffee.json",
                "Prefabs/Pickups/Fries.json"
            };

            if (data.contains("prefabs") && data["prefabs"].is_array())
            {
                prefabs.clear();
                for (const auto& p : data["prefabs"])
                    prefabs.push_back(p.get<std::string>());
            }

            float minInterval = data.value("minInterval", 8.0f);
            float maxInterval = data.value("maxInterval", 15.0f);

            go->AddComponent<PickupSpawnerComponent>(prefabs, minInterval, maxInterval);
        }
    };
    REGISTER_COMPONENT_PARSER(PickupSpawnerComponent, PickupSpawnerComponentParser);

    PickupSpawnerComponent::PickupSpawnerComponent(GameObject* owner, std::vector<std::string> prefabPaths,
        float minInterval, float maxInterval)
        : Component(owner)
        , m_PrefabPaths(std::move(prefabPaths))
        , m_MinInterval(minInterval)
        , m_MaxInterval(maxInterval)
    {
        EXPOSE(m_MinInterval);
        EXPOSE(m_MaxInterval);

        std::string path = dae::ResourceManager::GetInstance().GetFullPathForFile("Audio/Bonus Appear.wav");
        ServiceLocator::GetSoundSystem().load(SoundID::BonusAppear, path);

        ScheduleNext();
    }

    PickupSpawnerComponent::~PickupSpawnerComponent()
    {
        GameTime::GetInstance().RemoveTimer(m_TimerHandle);
    }

    void PickupSpawnerComponent::ScheduleNext()
    {
        static std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> dist(m_MinInterval, m_MaxInterval);

        m_TimerHandle = GameTime::GetInstance().AddTimer(dist(rng), [this]()
            {
                SpawnPickup();
                ScheduleNext();
            });
    }

    void PickupSpawnerComponent::SpawnPickup()
    {
        if (m_PrefabPaths.empty())
            return;

        auto& grid = LevelGrid::GetInstance();
        int cols = grid.GetCols();
        int rows = grid.GetRows();
        if (cols <= 0 || rows <= 0)
            return;

        static std::mt19937 rng{ std::random_device{}() };
        std::uniform_int_distribution<int> colDist(0, cols - 1);
        std::uniform_int_distribution<int> rowDist(0, rows - 1);
        std::uniform_int_distribution<size_t> prefabDist(0, m_PrefabPaths.size() - 1);

        // try to find a walkable platform tile
        int x = 0, y = 0;
        bool found = false;
        for (int attempt = 0; attempt < 50; ++attempt)
        {
            x = colDist(rng);
            y = rowDist(rng);
            TileType tile = grid.GetTile(x, y);
            if (tile == TileType::Platform || tile == TileType::Intersection || tile == TileType::IntersectionDownOnly)
            {
                found = true;
                break;
            }
        }

        if (!found)
            return;

        float tileSize = grid.GetTileSize();
        const std::string& prefabPath = m_PrefabPaths[prefabDist(rng)];

        Scene* scene = SceneManager::GetInstance().GetActiveScene();
        if (!scene)
            return;

        SceneLoader::Instantiate(*scene, prefabPath, x * tileSize, y * tileSize);

        ServiceLocator::GetSoundSystem().play(SoundID::BonusAppear, 1.0f, 0);
    }
}
