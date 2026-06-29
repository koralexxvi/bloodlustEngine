#pragma once

#include "../logging.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_log.h>
#include <cstdint>
#include <exception>
#include <memory>

namespace Bloodlust {

// Returns the preferred shader format for this configuration. Currently this is always SPIR-V but
// is likely to change in the future.
SDL_GPUShaderFormat getPreferredShaderFormat();

// Describes the properties of a vertex attribute.
struct VertexAttributeDefinition {
	// The format of this vertex attribute.
	SDL_GPUVertexElementFormat format;

	// The slot that this vertex attribute takes up.
	std::uint32_t bufferSlot = 0;
};

// Describes the properties of a vertex shader.
struct VertShaderDef {
	// A null-terminated string containing the shader's bytecode.
	const std::uint8_t* src;

	// The shader's vertex attributes.
	const VertexAttributeDefinition* vertexAttribDefs;

	// The length of the shader's bytecode.
	const std::size_t srcLen;

	// The size of the shader's vertex buffer in bytes.
	const std::uint32_t vertBufferSize;

	// The size of the shader's index buffer in bytes.
	const std::uint32_t indexBufferSize;

	// The amount of vertex attributes that this shader contains.
	const std::uint32_t vertexAttribCount;

	// The amount of samplers that this shader contains.
	const std::uint32_t samplerCount = 0;

	// The amount of storage texture objects this shader contains.
	const std::uint32_t storageTextureCount = 0;

	// The amount of storage buffer objects this shader contains.
	const std::uint32_t storageBufferCount = 0;

	// The amount of uniform buffer objects this shader has.
	const std::uint32_t uniformBufferCount = 0;

	// The format of this shader.
	const SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
};

// Describes the properties of a fragment shader.
struct FragShaderDef {
	// A null-terminated string containing the shader's bytecode.
	const std::uint8_t* src;

	// The length of this shader's bytecode.
	const std::size_t srcLen;

	// The amount of samplers that this shader contains.
	const std::uint32_t samplerCount = 0;

	// The amount of storage texture objects this shader contains.
	const std::uint32_t storageTextureCount = 0;

	// The amount of storage buffer objects this shader contains.
	const std::uint32_t storageBufferCount = 0;

	// The amount of uniform buffer objects this shader has.
	const std::uint32_t uniformBufferCount = 0;

	// The format of this shader.
	const SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;

	[[maybe_unused]]
	const char _padding[4] = {};
};

class GPUShader {
  private:
	SDL_GPUDevice* device;
	SDL_GPUShader* shader;

  public:
	GPUShader(SDL_GPUDevice* device, const SDL_GPUShaderCreateInfo& params);

	constexpr SDL_GPUShader* getHandle() const {
		return shader;
	}

	~GPUShader();
};

// Loads a vertex shader from the given definition and returns it.
std::unique_ptr<GPUShader> loadVertShader(SDL_GPUDevice* device, const VertShaderDef& shaderDef);

// Loads a fragment shader from the given definition and returns it.
std::unique_ptr<GPUShader> loadFragShader(SDL_GPUDevice* device, const FragShaderDef& shaderDef);

// Returns the size of a vertex element format in bytes.
constexpr std::size_t getFormatSize(SDL_GPUVertexElementFormat format) {
	switch (format) {
		case SDL_GPU_VERTEXELEMENTFORMAT_INT:
		case SDL_GPU_VERTEXELEMENTFORMAT_UINT: return sizeof(int);
		case SDL_GPU_VERTEXELEMENTFORMAT_INT2:
		case SDL_GPU_VERTEXELEMENTFORMAT_UINT2: return sizeof(int) * 2;
		case SDL_GPU_VERTEXELEMENTFORMAT_INT3:
		case SDL_GPU_VERTEXELEMENTFORMAT_UINT3: return sizeof(int) * 3;
		case SDL_GPU_VERTEXELEMENTFORMAT_INT4:
		case SDL_GPU_VERTEXELEMENTFORMAT_UINT4: return sizeof(int) * 4;

		case SDL_GPU_VERTEXELEMENTFORMAT_FLOAT: return sizeof(float);
		case SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2: return sizeof(float) * 2;
		case SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3: return sizeof(float) * 3;
		case SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4: return sizeof(float) * 4;

		case SDL_GPU_VERTEXELEMENTFORMAT_BYTE2:
		case SDL_GPU_VERTEXELEMENTFORMAT_BYTE2_NORM:
		case SDL_GPU_VERTEXELEMENTFORMAT_UBYTE2:
		case SDL_GPU_VERTEXELEMENTFORMAT_UBYTE2_NORM: return 2;

		case SDL_GPU_VERTEXELEMENTFORMAT_BYTE4:
		case SDL_GPU_VERTEXELEMENTFORMAT_BYTE4_NORM:
		case SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4:
		case SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM: return 4;

		case SDL_GPU_VERTEXELEMENTFORMAT_SHORT2:
		case SDL_GPU_VERTEXELEMENTFORMAT_USHORT2:
		case SDL_GPU_VERTEXELEMENTFORMAT_SHORT2_NORM:
		case SDL_GPU_VERTEXELEMENTFORMAT_USHORT2_NORM: return sizeof(short) * 2;

		case SDL_GPU_VERTEXELEMENTFORMAT_SHORT4:
		case SDL_GPU_VERTEXELEMENTFORMAT_SHORT4_NORM:
		case SDL_GPU_VERTEXELEMENTFORMAT_USHORT4:
		case SDL_GPU_VERTEXELEMENTFORMAT_USHORT4_NORM: return sizeof(short) * 4;

		default: {
			log(SDL_LOG_PRIORITY_CRITICAL, "Invalid vertex element format %i", format);
			std::terminate();
		}
	}
}

}  // namespace Bloodlust
