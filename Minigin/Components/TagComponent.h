#pragma once
#include <Component.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "SceneLoader.h"
#include "sdbm_hash.h"
#include <nlohmann/json.hpp>

namespace dae
{
    using Tag = unsigned int;
    class TagComponent final : public Component
    {
    public:
        TagComponent(GameObject* owner, const std::unordered_set<Tag>& tags = {}, const Tag& unique_Tag = 0)
            : Component(owner), m_TagList(tags), m_UniqueTag(unique_Tag)
        {
            s_TaggedObjects[m_UniqueTag] = GetOwner();
        }

        ~TagComponent() override
        {
            s_TaggedObjects.erase(m_UniqueTag);
        }

        const Tag& GetUniqueTag() const { return m_UniqueTag; }
        GameObject* GetGameObject() const
        {
            return GetOwner();
        }

        bool HasTag(Tag t) const
        { 
            auto find = m_TagList.find(t);
            return find != m_TagList.end();
        }

        static GameObject* FindGameObject(Tag tag)
        {
            return s_TaggedObjects[tag];
        }

    private:
        std::unordered_set<Tag> m_TagList;
        Tag m_UniqueTag;

        static inline std::unordered_map<Tag, GameObject*> s_TaggedObjects;
    };

    class TagComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            std::string uniqueTag = data.value("uniqueTag", "");

            std::unordered_set<Tag> hashedTags;
            if (data.contains("tags") && data["tags"].is_array())
            {
                for (const auto& tagStr : data["tags"])
                {
                    hashedTags.insert(make_sdbm_hash_rt(tagStr.get<std::string>()));
                }
            }

            go->AddComponent<TagComponent>(hashedTags, make_sdbm_hash_rt(uniqueTag));
        }
    };

    REGISTER_COMPONENT_PARSER(TagComponent, TagComponentParser);
}