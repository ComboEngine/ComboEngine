#include "pch.h"
#include "Event.h"

void Event::Invoke()
{
	for (std::function<void()> Callback : Callbacks) {
		Callback();
	}
}

void Event::Hook(std::function<void()> Callback)
{
	HooksCount++;
	this->Callbacks.push_back(Callback);
}
