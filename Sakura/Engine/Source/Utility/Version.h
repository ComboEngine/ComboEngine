#pragma once

#include <pch.h>

struct Version {
	int major, minor, build;
	Version(int major,int minor,int build) : major(major), minor(minor), build(build) {

	}

	std::string GetVersionString() {
		return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(build);
 	}
};