#ifndef BL_CONFIG_H
#define BL_CONFIG_H

namespace Bloodlust {

struct EngineConfig {
	const char* appName;	 // The name of the app.
	const char* appVersion;	 // The version of the app.
	const char* appID;		 // The app's metadata ID.

	const char* windowTitle = appName;  // The text to be shown on the window's titlebar.
	const unsigned int windowWidth = 960;
	const unsigned int windowHeight = 540;

};

extern EngineConfig* config;

}  // namespace Bloodlust

#endif
