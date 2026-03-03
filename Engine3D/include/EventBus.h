#pragma once
#include <algorithm>
#include <mutex>
#include <queue>
#include <vector>

#include "EventListener.h"

class EventBus
{
private:
	struct ListenerInfo
	{
		EventListener* listener;
		EventCategory categoryFilter;
		int priority;
	};

	std::vector<ListenerInfo> listeners;
	std::queue<std::unique_ptr<Event>> eventQueue;
	std::mutex queueMutex;
	bool immediateMode = true;

public:

	void SetImmediateMode(bool immediate)
	{
		immediateMode = immediate;
	}

	void AddListener(EventListener* listener , EventCategory categoryFilter = EventCategory::All, int priority = 0)
	{
		listeners.push_back({listener, categoryFilter, priority});

		std::sort(listeners.begin(), listeners.end(),
			[](const ListenerInfo& a, const ListenerInfo& b)
			{
				return a.priority > b.priority;
			}
		);
	}

	void RemoveListener(EventListener* listener)
	{
		auto it = std::find(listeners.begin(), listeners.end(), [listener](const ListenerInfo& info) {
			return info.listener == listener;
			});
		if (it != listeners.end())
		{
			listeners.erase(it);
		}
	}

	void PublishEvent(const Event& event)
	{
		if (immediateMode)
		{
			//Dispatch event instantly
			for (const auto& infos : listeners)
			{
				if (infos.categoryFilter == EventCategory::All || (event.GetCategoryFlag() & infos.categoryFilter) != EventCategory::None)
				{
					infos.listener->OnEvent(event);
				}
			}
		} else
		{
			//Queue event to process later
			std::scoped_lock lock(queueMutex);
			eventQueue.push(std::unique_ptr<Event>(event.Clone()));
		}
	}

	void ProcessEvent()
	{
		if (immediateMode) return;

		std::queue<std::unique_ptr<Event>> currentEvents;

		{
			std::scoped_lock lock(queueMutex);
			std::swap(currentEvents, eventQueue);
		}

		while (!currentEvents.empty())
		{
			auto& event = *currentEvents.front();

			for (auto info : listeners)
			{
				info.listener->OnEvent(event);
			}

			currentEvents.pop();
		}

	}
};
