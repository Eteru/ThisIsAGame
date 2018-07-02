#pragma once
#include <string>
#include <functional>

class Event
{
public:
	Event(std::function<void()> f, std::string desc);
	Event(const Event & e);
	virtual ~Event();

	void Execute();
	void What() const;

private:
	std::string m_description;
	std::function<void()> m_function;
};

