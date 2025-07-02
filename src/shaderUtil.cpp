#include "../include/shaderUtil.hpp"

#include "../extern/stb_image.h"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_stdinc.h>

namespace {

// TODO: support for shaders in formats other than spir-v
template<typename T>
inline void loadShaderCommon(SDL_GPUShaderCreateInfo& createInfo, const T& shaderDef) {
	createInfo.code_size = shaderDef.srcLen;
	createInfo.code = shaderDef.src;
	createInfo.entrypoint = "main";
	createInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
	createInfo.num_samplers = shaderDef.samplerCount;
	createInfo.num_storage_textures = shaderDef.storageTextureCount;
	createInfo.num_storage_buffers = shaderDef.storageBufferCount;
	createInfo.num_uniform_buffers = shaderDef.uniformBufferCount;
	createInfo.props = 0;
}

}  // namespace

SDL_GPUShader* Bloodlust::loadVertShader(
	SDL_GPUDevice* device,
	const Bloodlust::VertShaderDef& shaderDef
) {
	SDL_GPUShaderCreateInfo createInfo;
	createInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
	loadShaderCommon(createInfo, shaderDef);
	return SDL_CreateGPUShader(device, &createInfo);
}

SDL_GPUShader* Bloodlust::loadFragShader(
	SDL_GPUDevice* device,
	const Bloodlust::FragShaderDef& shaderDef
) {
	SDL_GPUShaderCreateInfo createInfo;
	createInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	loadShaderCommon(createInfo, shaderDef);
	return SDL_CreateGPUShader(device, &createInfo);
}
