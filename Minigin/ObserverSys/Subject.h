#pragma once
#include <vector>
#include <algorithm>
#include "Observer.h"
#include "Event.h"

namespace dae
{
	class Subject
	{
	public:
		void AddObserver(Observer* observer)
		{
			m_observers.push_back(observer);
		}

		void RemoveObserver(Observer* observer)
		{
			std::erase(m_observers, observer);
		}

		void NotifyObservers(const Event* event)
		{
			for (auto observer : m_observers)
				observer->HandleEvent(event);
		}

	private:
		std::vector<Observer*> m_observers;
	};
}