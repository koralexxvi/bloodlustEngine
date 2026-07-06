#pragma once

#include <exception>
#include <string>
#include <string_view>

namespace Bloodlust {

class InternalError : public std::exception {
	std::string msg;

  public:
	InternalError(std::string_view reason) noexcept : msg(reason) {}

	virtual const char* what() const noexcept override {
		return msg.c_str();
	}
};

}  // namespace Bloodlust
