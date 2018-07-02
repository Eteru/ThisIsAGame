
#include "EventManager.h"

std::queue<std::unique_ptr<Event>> EventManager::m_queue;

EventManager::EventManager()
{
}


EventManager::~EventManager()
{
}

void EventManager::AddEvent(Event * e)
{
	std::unique_ptr<Event> ptr(e);
	m_queue.push(std::move(ptr));
}

bool EventManager::Poll()
{
	if (true == m_queue.empty()) {
		return false;
	}

	std::unique_ptr<Event> ptr = std::move(m_queue.front());
	m_queue.pop();

	ptr->What();
	ptr->Execute();

	return true;
}
