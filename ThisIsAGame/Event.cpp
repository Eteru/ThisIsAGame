
#include <iostream>

#include "Event.h"


Event::Event(std::function<void(void)> f, std::string desc)
{
	m_function = f;
	m_description = desc;
}

Event::Event(const Event & e)
{
	m_function = e.m_function;
	m_description = e.m_description;
}

Event::~Event()
{
}

void Event::Execute()
{
	m_function();
}

void Event::What() const
{
	std::cout << m_description << std::endl;
}
