#pragma once

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_log.h>
#include <cstdlib>

namespace Bloodlust {

SDL_GPUShaderFormat getPreferredShaderFormat();

struct VertexAttributeDefinition {
	SDL_GPUVertexElementFormat format;
	uint32_t bufferSlot = 0;
};

struct VertShaderDef {
	const unsigned char* src;
	const VertexAttributeDefinition* vertexAttribDefs;

	const size_t srcLen;
	const size_t vertBufferSize;
	const size_t indexBufferSize;

	const uint32_t vertexAttribCount;
	const uint32_t samplerCount = 0;
	const uint32_t storageTextureCount = 0;
	const uint32_t storageBufferCount = 0;
	const uint32_t uniformBufferCount = 0;

	const SDL_GPUShaderFormat format = getPreferredShaderFormat();
};

struct FragShaderDef {
	const unsigned char* src;
	const size_t srcLen;

	const uint32_t samplerCount = 0;
	const uint32_t storageTextureCount = 0;
	const uint32_t storageBufferCount = 0;
	const uint32_t uniformBufferCount = 0;

	const SDL_GPUShaderFormat format = getPreferredShaderFormat();

	[[maybe_unused]]
	const char _padding[4] = {};
};

SDL_GPUShader* loadVertShader(SDL_GPUDevice* device, const VertShaderDef& shaderDef);
SDL_GPUShader* loadFragShader(SDL_GPUDevice* device, const FragShaderDef& shaderDef);

constexpr size_t getFormatSize(SDL_GPUVertexElementFormat format) {
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
			SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "Invalid vertex element format %i", format);
			abort();
		}
	}
}

}  // namespace Bloodlust
