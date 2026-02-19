#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::FixedUpdate()
{
	if (!m_IsActive) return;

	for (auto& component : m_components)
	{
		if(component->IsActive()) component->FixedUpdate();
	}
}

void dae::GameObject::Update()
{
	if (!m_IsActive) return;

	for (auto& component : m_components)
	{
		if (component->IsActive()) component->Update();
	}
	DestroyComponentsMarkedForDeletion();
}

void dae::GameObject::Render() const
{
	if (!m_IsActive) return;

	for (const auto& component : m_components)
	{
		if (component->IsActive()) component->Render();
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