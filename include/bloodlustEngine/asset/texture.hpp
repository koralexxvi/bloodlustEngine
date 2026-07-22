#pragma once

#include "../datatype/vector2.hpp"

#include <SDL3/SDL_gpu.h>
#include <cstdint>

namespace Bloodlust {

// Defines the filtering mode of a texture.
enum class TextureFilterMode {
	LINEAR,	 // Use linear filtering.
	NEAREST	 // Use nearest neighbour filtering.
};

// A class representing a 2D texture.
class Texture {
  public:
	// The creation parameters for initialising textures.
	struct CreateParams {
		// The width of this texture.
		std::uint32_t width;

		// The height of this texture.
		std::uint32_t height;

		// The amount of layers this texture has.
		std::uint32_t layers = 1;

		// The filtering mode of this texture.
		TextureFilterMode filterMode = TextureFilterMode::NEAREST;
	};

  private:
	// A pointer to the SDL_GPUTexture that this texture holds.
	SDL_GPUTexture* textureData = nullptr;

	// The width of this texture.
	std::uint32_t w;

	// The height of this texture.
	std::uint32_t h;

	// The filtering mode of this texture.
	TextureFilterMode filterMode = TextureFilterMode::NEAREST;

  public:
	// Constructs a Texture from the given parameters. Will throw InternalError on failure.
	Texture(const CreateParams& params);

	// Prevents the texture object from being copied.
	Texture(const Texture&) = delete;

	// The texture's move constructor.
	Texture(Texture&& other)
		: textureData(other.textureData),
		  w(other.w),
		  h(other.h),
		  filterMode(other.filterMode) {}

	// Prevents the texture object from being copied.
	void operator=(const Texture&) = delete;

	// The texture's move assignment operator.
	void operator=(Texture&& other) {
		textureData = other.textureData;
		w = other.w;
		h = other.h;
		filterMode = other.filterMode;
	}

	// Destroys this texture. The pointer to the SDL_GPUTexture will no longer be valid.
	~Texture() noexcept;

	// Returns a pointer to the SDL_GPUTexture this texture holds. This is valid until the texture
	// is destroyed.
	constexpr SDL_GPUTexture* getData() const noexcept {
		return textureData;
	}

	// Returns the width of the texture.
	constexpr std::uint32_t getWidth() const noexcept {
		return w;
	}

	// Returns the height of the texture.
	constexpr std::uint32_t getHeight() const noexcept {
		return h;
	}

	// Returns the size of the texture.
	constexpr Vector2 getSize() const noexcept {
		return {static_cast<float>(w), static_cast<float>(h)};
	}

	// Returns the current filtering mode of this texture.
	constexpr TextureFilterMode getFilterMode() const noexcept {
		return filterMode;
	}

	// Sets the filtering mode of this texture.
	inline void setFilterMode(TextureFilterMode newMode) noexcept {
		filterMode = newMode;
	}
};

}  // namespace Bloodlust
