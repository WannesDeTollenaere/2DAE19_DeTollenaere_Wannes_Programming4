#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::FixedUpdate()
{
	for (auto& component : m_components)
	{
		component->FixedUpdate();
	}
}

void dae::GameObject::Update()
{
	for (auto& component : m_components)
	{
		component->Update();
	}
	DestroyComponentsMarkedForDeletion();
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_components)
	{
		component->Render();
	}
}


void dae::GameObject::DestroyComponentsMarkedForDeletion()
{
	m_components.erase(std::remove_if(m_components.begin(), m_components.end(),
		[](const std::unique_ptr<Component>& component) {
			return component->IsMarkedForDeletion();
		}),
		m_components.end());
}