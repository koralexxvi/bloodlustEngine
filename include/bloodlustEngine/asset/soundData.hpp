#pragma once

#include <SDL3/SDL_audio.h>
#include <cstdint>
#include <string_view>

namespace Bloodlust {

// Contains audio sample data.
class SoundData {
  private:
	// The sample data of this SoundData.
	std::uint8_t* data;

	// The length of this sample data.
	std::uint32_t length;

	// The format of this data.
	SDL_AudioSpec format;

  public:
	// Loads sound data from a path.
	SoundData(std::string_view path);

	// Prevents the SoundData object from being copied.
	SoundData(const SoundData&) = delete;

	// The SoundData's move constructor.
	SoundData(SoundData&& other)
		: data(other.data),
		  length(other.length),
		  format(other.format) {}

	// Prevents the SoundData object from being copied.
	void operator=(const SoundData&) = delete;

	// The SoundData's move assignment operator.
	void operator=(SoundData&& other) {
		data = other.data;
		length = other.length;
		format = other.format;
	}

	// Destroys this SoundData.
	~SoundData();

	// Returns the internal sample data.
	constexpr std::uint8_t* getData() const {
		return data;
	}

	// Returns the length of this SoundData in samples.
	constexpr std::uint32_t getLength() const {
		return length;
	}

	// Returns the format of this SoundData.
	constexpr const SDL_AudioSpec& getFormat() const {
		return format;
	}
};

}  // namespace Bloodlust
