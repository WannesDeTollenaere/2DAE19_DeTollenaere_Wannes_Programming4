#pragma once
#include <cstdint>
#include "GameObject.h"

namespace dae
{

	struct EventArg {};
	using EventId = unsigned int;

	struct Event
	{
		const EventId id;
		explicit Event(EventId _id) : id{ _id } {}
		virtual ~Event() = default; 
	};

	struct GameObjectEvent : public Event
	{
		GameObject* obj;

		virtual ~GameObjectEvent() = default;
		GameObjectEvent(EventId _id, GameObject* object) : Event(_id), obj{ object } {}
	};
}