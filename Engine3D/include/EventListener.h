#pragma once
#include <cstring>
#include "Event.h"

// Event listener interface
class EventListener {
public:
	virtual ~EventListener() = default;
	virtual void OnEvent(const Event& event) = 0;
};

class EventDispatcher
{
private:
	const Event& event;

public:
	explicit EventDispatcher(const Event& e) : event(e) {}

	template<typename T, typename F>
	bool Dispatch(const F& handler)
	{
		if (std::strcmp(event.GetType(), T::GetStaticType()) == 0)
		{
			handler(static_cast<const T&>(event));
			return true;
		}
		return false;
	}
};