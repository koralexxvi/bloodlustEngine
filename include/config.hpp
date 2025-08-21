#pragma once

#include <cstdint>

namespace Bloodlust {

struct EngineConfig {
	const char* appName = "BloodlustApp";			 // The name of the app.
	const char* appVersion = "0.0.0";				 // The version of the app.
	const char* appID = "com.example.bloodlustapp";	 // The app's metadata ID.

	const char* windowTitle = appName;	// The text to be shown on the window's titlebar.
	const uint32_t windowWidth = 960;	// The width of the window.
	const uint32_t windowHeight = 540;	// The height of the window.

	// The time between calls to App::step(), in nanoseconds. Defaults to 60fps.
	uint64_t appTimestep = 1'000'000'000LL / 60LL;

	// How many steps the app is allowed to process at a time to keep up with realtime.
	// Must be greater than 0.
	uint32_t appMaxCatchupSteps = 4;

	[[maybe_unused]]
	char _padding[4] = {};
};

extern EngineConfig config;

}  // namespace Bloodlust
