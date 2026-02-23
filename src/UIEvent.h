#pragma once
#include "Event.h"

class UIEvent : public Event
{
private:
	UiElement* target;
	bool bubbles;
	bool cancelBubble = false;

public:
	UIEvent(UIElement* targetElement, bool bubbling =true) : target(targetElement), bubbles(bubbling){}

	UiElement* GetTarget() const { return target; }
	bool Bubbles() const { return bubbles; }

	void StopPropagation()
	{
		cancelBubble = true;
	}

	bool isPropagationStop() const
	{
		return cancelBubble;
	}

	DEFINE_EVENT_TYPE(UIEvent, static_cast<int>(EventCategory::UI))
};

class UISystem
{
public:
	void DispatchEvent(UIEvent& event)
	{
		UIElement* target = event.GetTarget();

		//Capturing Phase
		std::vector<UIElement*> path;
		UIElement* current = target;

		while (current)
		{
			path.push_back(current);
			current = current->GetParent();
		}

		//Dispatch to each element (bottom-up)
		for (auto it = path.rbegin(); it != path.rend(); ++it)
		{
			(*it)->OnEvent(event);

			if (event.isPropagationStop())
			{
				break;
			}
		}
	}
};
