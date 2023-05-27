#pragma once

#include <Engine/EngineHeaders.h>

struct Version {
	int major, minor, build;
	Version(int major,int minor,int build) : major(major), minor(minor), build(build) {

	}

	sakura_string GetVersionString() {
		return to_string(major) + "." + to_string(minor) + "." + to_string(build);
 	}
};