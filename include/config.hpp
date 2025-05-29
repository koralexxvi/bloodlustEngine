#ifndef BL_CONFIG_H
#define BL_CONFIG_H

namespace Bloodlust {

struct EngineConfig {
	const char* appName = "BloodlustApp";			 // The name of the app.
	const char* appVersion = "0.0.0";				 // The version of the app.
	const char* appID = "com.example.bloodlustapp";	 // The app's metadata ID.

	const char* windowTitle = appName;	// The text to be shown on the window's titlebar.
	const int windowWidth = 960;
	const int windowHeight = 540;
};

extern EngineConfig* config;

}  // namespace Bloodlust

#endif
