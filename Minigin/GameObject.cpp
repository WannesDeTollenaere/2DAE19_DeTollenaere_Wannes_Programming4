#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"
#include <algorithm>
#include "Transform.h"

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

dae::GameObject* dae::GameObject::GetChildAt(int index) const
{
	if (index < 0 || index >= static_cast<int>(m_children.size()))
		return nullptr;

	return m_children[index];
}

bool dae::GameObject::IsChild(const GameObject* child) const
{
	for (const auto& c : m_children)
	{
		if (c == child || c->IsChild(child))
			return true;
	}
	return false;
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldTransform)
{
	if (IsChild(parent) || parent == this || m_parent == parent)
		return;

	if (parent == nullptr)
	{
		m_transform.SetLocalPosition(m_transform.GetWorldPosition());
	}
	else
	{
		if (keepWorldTransform)
		{
			m_transform.SetLocalPosition(m_transform.GetWorldPosition() - parent->GetTransform().GetWorldPosition());
		}
		m_transform.SetPositionDirty();
	}

	if (m_parent) m_parent->RemoveChild(this);
	m_parent = parent;
	if (m_parent) m_parent->AddChild(this, keepWorldTransform);
}

bool dae::GameObject::IsParent(const GameObject* potentialParent) const
{
	const GameObject* currentParent = m_parent;
	while (currentParent != nullptr)
	{
		if (currentParent == potentialParent)
			return true;
		currentParent = currentParent->GetParent();
	}
	return false;
}

void dae::GameObject::AddChild(GameObject* child, bool keepWorldPosition)
{
	if (child == nullptr || child == this || IsParent(child))
		return;

	if (keepWorldPosition)
	{
		child->GetTransform().SetLocalPosition(
			child->GetTransform().GetWorldPosition() - this->GetTransform().GetWorldPosition()
		);
	}
	else
	{
		child->GetTransform().SetPositionDirty();
	}

	if (child->GetParent() != nullptr)
	{
		child->GetParent()->RemoveChild(child);
	}

	child->m_parent = this;

	m_children.emplace_back(child);
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	if (child == nullptr || child->m_parent != this)
		return;


	child->GetTransform().SetLocalPosition(child->GetTransform().GetWorldPosition());

	std::erase(m_children, child);

	child->m_parent = nullptr;
}

