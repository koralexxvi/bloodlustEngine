#pragma once

#include <cstdint>

namespace Bloodlust {

// The structure that dictates the configuration of the running app.
struct EngineConfig {
	// The name of the app.
	const char* appName = "BloodlustApp";

	// The version of the app.
	const char* appVersion = "0.0.0";

	// The app's metadata ID.
	const char* appID = "com.example.bloodlustapp";

	// The text to be shown on the window's titlebar.
	const char* windowTitle = appName;

	// The initial width of the window.
	const uint32_t windowWidth = 960;

	// The initial height of the window.
	const uint32_t windowHeight = 540;

	// The time between calls to App::step(), in nanoseconds. Defaults to 60fps.
	uint64_t appTimestep = 1'000'000'000LL / 60LL;

	// How many steps the app is allowed to process at a time to keep up with realtime.
	// Must be greater than 0.
	uint32_t appMaxCatchupSteps = 4;

	[[maybe_unused]]
	char _padding[4] = {};
};

// The configuration of the running app.
extern EngineConfig config;

}  // namespace Bloodlust
