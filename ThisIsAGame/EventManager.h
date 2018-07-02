#pragma once
#include <queue>
#include <memory>
#include "Event.h"

class EventManager
{
public:
	EventManager();
	virtual ~EventManager();

	static void AddEvent(Event *e);
	static bool Poll();

private:
	static std::queue<std::unique_ptr<Event>> m_queue;
};

