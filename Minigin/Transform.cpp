#include "Transform.h"
#include "GameObject.h"

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

const glm::vec3& dae::Transform::GetWorldPosition()
{
	if (m_positionIsDirty)
		UpdateWorldPosition();

	return m_worldPosition;
}

void dae::Transform::UpdateWorldPosition()
{
	if (m_positionIsDirty)
	{
		if (GetOwner()->GetParent() == nullptr)
		{
			m_worldPosition = m_localPosition;
		}
		else
		{
			m_worldPosition = GetOwner()->GetParent()->GetTransform().GetWorldPosition() + m_localPosition;
		}
	}
	m_positionIsDirty = false;
}