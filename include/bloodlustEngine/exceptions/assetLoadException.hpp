#pragma once

#include <exception>
#include <string>
#include <string_view>

namespace Bloodlust {

class AssetLoadException : public std::exception {
  private:
	std::string msg;

  public:
	AssetLoadException(std::string_view reason, std::string_view path) noexcept {
		msg = "Could not load asset at path \"";
		msg += path;
		msg += "\": ";
		msg += reason;
	}

	virtual const char* what() const noexcept override {
		return msg.c_str();
	}
};

}  // namespace Bloodlust
