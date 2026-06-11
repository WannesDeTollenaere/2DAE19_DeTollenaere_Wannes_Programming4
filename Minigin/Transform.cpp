#include "Transform.h"
#include "GameObject.h"
#include <imgui.h>

void dae::Transform::SetLocalPosition(const float x, const float y, const float z)
{
	m_localPosition.x = x;
	m_localPosition.y = y;
	m_localPosition.z = z;
	SetPositionDirty();
}

void dae::Transform::SetLocalPosition(const glm::vec3& position)
{
	m_localPosition = position; 
	SetPositionDirty();
}

void dae::Transform::SetPositionDirty()
{
	m_positionIsDirty = true;

	for (int i = 0; i < GetOwner()->GetChildCount(); ++i)
	{
		GetOwner()->GetChildAt(i)->GetTransform().SetPositionDirty();
	}
}

void dae::Transform::RenderGUI()
{
	float pos[3] = { m_localPosition.x, m_localPosition.y, m_localPosition.z };

	if (ImGui::DragFloat3("Local Position", pos, 0.1f))
	{
		SetLocalPosition(pos[0], pos[1], pos[2]);
	}

	ImGui::Text("World position: [%.2f, %.2f, %.2f]", m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);

	if (ImGui::DragFloat("Scale", &m_localScale, 0.01f, 0.01f, 10.0f))
	{
		SetPositionDirty();
	}
}

const glm::vec3& dae::Transform::GetWorldPosition()
{
	if (m_positionIsDirty)
		UpdateWorldPosition();

	return m_worldPosition;
}

float dae::Transform::GetWorldScale()
{
	if (m_positionIsDirty)
		UpdateWorldPosition();

	return m_worldScale;
}

void dae::Transform::UpdateWorldPosition()
{
	if (m_positionIsDirty)
	{
		if (GetOwner()->GetParent() == nullptr)
		{
			m_worldPosition = m_localPosition;
			m_worldScale = m_localScale;
		}
		else
		{
			auto& parentTransform = GetOwner()->GetParent()->GetTransform();
			m_worldPosition = parentTransform.GetWorldPosition() + m_localPosition;
			m_worldScale = parentTransform.GetWorldScale() * m_localScale;
		}
	}
	m_positionIsDirty = false;
}