#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"
#include <algorithm>
#include "Transform.h"
#include <imgui.h>
#include <typeinfo>

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
void dae::GameObject::RenderGUI()
{
	ImGui::Text("GameObject Details");
	ImGui::Separator();

	ImGui::PushID("Transform");
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		m_transform.RenderGUI();
	}
	ImGui::PopID();

	ImGui::Checkbox("Is Active", &m_IsActive);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Text("Components:");

	for (const auto& comp : m_components)
	{
		ImGui::PushID(comp.get());

		// this resolves issues with emscripten
		Component* pRawComp = comp.get();

		if (ImGui::CollapsingHeader(typeid(*pRawComp).name(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			comp->RenderGUI();
		}

		ImGui::PopID();
	}
}

void dae::GameObject::RenderHierarchy(GameObject** selectedObject)
{
	if (IsMarkedForDeletion())
	{
		if (*selectedObject == this)
		{
			*selectedObject = nullptr;
		}
		return;
	}

	ImGui::PushID(this);

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (*selectedObject == this)
		nodeFlags |= ImGuiTreeNodeFlags_Selected;

	if (m_children.empty())
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;

	bool nodeOpen = ImGui::TreeNodeEx(m_name.c_str(), nodeFlags);

	if (ImGui::IsItemClicked())
	{
		*selectedObject = this;
	}

	if (nodeOpen)
	{
		for (auto child : m_children)
		{
			child->RenderHierarchy(selectedObject);
		}
		ImGui::TreePop();
	}

	ImGui::PopID();
}
void dae::GameObject::Destroy()
{
	m_markedForDeletion = true;

	for (auto& child : m_children)
		child->Destroy();
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
	if (m_parent) m_parent->AddChild(this);
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

void dae::GameObject::AddChild(GameObject* child)
{
	m_children.emplace_back(child);
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	std::erase(m_children, child);
}

