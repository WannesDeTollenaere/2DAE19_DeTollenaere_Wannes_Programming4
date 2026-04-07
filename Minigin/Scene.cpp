#include <algorithm>
#include "Scene.h"
#include "Components/TagComponent.h"
#include "sdbm_hash.h"

using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(const GameObject& object)
{
	m_objects.erase(
		std::remove_if(
			m_objects.begin(),
			m_objects.end(),
			[&object](const auto& ptr) { return ptr.get() == &object; }
		),
		m_objects.end()
	);
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void dae::Scene::FixedUpdate()
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
	DestroyGameObjectsMarkedForDeletion();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void dae::Scene::RenderGUI()
{
	for (const auto& object : m_objects)
	{
		if (object->GetParent() == nullptr)
		{
			object->RenderGUI();
		}
	}
}

void dae::Scene::RenderHierarchy(GameObject** selectedObject)
{
	for (const auto& object : m_objects)
	{
		if (object->GetParent() == nullptr)
		{
			object->RenderHierarchy(selectedObject);
		}
	}
}

// returns nullptr if nothing found
GameObject* Scene::GetGameObjectByTag(const std::string& tag) const
{
	for (const auto& object : m_objects)
	{
		auto tagComp = object->GetComponent<TagComponent>();
		if (tagComp && tagComp->HasTag(make_sdbm_hash_rt(tag)))
		{
			return object.get();
		}
	}
	return nullptr;
}

std::vector<GameObject*> Scene::GetGameObjectsByTag(const std::string& tag) const
{
	std::vector<GameObject*> foundObjects;
	for (const auto& object : m_objects)
	{
		auto tagComp = object->GetComponent<TagComponent>();
		if (tagComp && tagComp->HasTag(make_sdbm_hash_rt(tag)))
		{
			foundObjects.push_back(object.get());
		}
	}
	return foundObjects;
}

void dae::Scene::DestroyGameObjectsMarkedForDeletion()
{
	// DELETE gameobjects marked for deletion
	m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
		[](const std::unique_ptr<GameObject>& object) {
			return object->IsMarkedForDeletion();
		}),
		m_objects.end());
}

