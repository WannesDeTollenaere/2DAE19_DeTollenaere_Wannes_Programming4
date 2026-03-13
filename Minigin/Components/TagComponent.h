#pragma once
#include <Component.h>
#include <string>
#include <unordered_map>

namespace dae
{
    using Tag = unsigned int;
    class TagComponent final : public Component
    {
    public:
        TagComponent(GameObject* owner, const Tag& tag)
            : Component(owner), m_tag(tag)
        {
            s_TaggedObjects[m_tag] = GetOwner();
        }

        ~TagComponent() override
        {
            s_TaggedObjects.erase(m_tag);
        }

        const Tag& GetTag() const { return m_tag; }

        static GameObject* FindGameObject(Tag tag)
        {
            return s_TaggedObjects[tag];
        }

    private:
        Tag m_tag;

        static inline std::unordered_map<Tag, GameObject*> s_TaggedObjects;
    };
}