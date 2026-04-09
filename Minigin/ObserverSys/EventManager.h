#pragma once
#include "Singleton.h"
#include "Observer.h"
#include "Event.h"
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace dae
{
	class EventManager final : public Singleton<EventManager>
	{
	public:
		void AttachEvent(EventId id, Observer* handler)
		{
			m_observers[id].push_back(handler);
		}

		void DetachEvent(EventId id, Observer* handler)
		{
			auto it = m_observers.find(id);
			if (it != m_observers.end())
			{
				std::erase(it->second, handler);
			}
		}

		void SendEvent(const Event* pEvent)
		{
			auto observerList = m_observers[pEvent->id];
			for (auto observer : observerList)
			{
				observer->HandleEvent(pEvent);
 			}
		}

		void SendEvent(EventId id)
		{
			Event e(id);
			SendEvent(&e);
		}

	private:
		friend class Singleton<EventManager>;
		EventManager() = default;

		std::unordered_map<EventId, std::vector<Observer*>> m_observers;
	};
}