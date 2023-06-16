#pragma once
#include "pch.h"

class Event {
public:
	std::vector<std::function<void()>> Callbacks;
	int HooksCount = 0;
	void Invoke();
	void Hook(std::function<void()> Callback);
};