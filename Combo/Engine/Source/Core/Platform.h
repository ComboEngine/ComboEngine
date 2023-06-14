#pragma once
#include "pch.h"
#include "Scope.h"


class Platform {
public:
	static void Create(Scope<Platform>& platform, std::any PlatformData);

	//API Functions
	virtual void Init(std::any PlatformData) = 0;
	virtual std::string GetApiName() = 0;
};