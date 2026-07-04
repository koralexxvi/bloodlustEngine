#include "../../include/render/shaderUtil.hpp"

#include <SDL3/SDL_gpu.h>

namespace {

SDL_GPUShaderFormat preferredShaderFormat = SDL_GPU_SHADERFORMAT_INVALID;

}  // namespace

Bloodlust::GPUShader::GPUShader(
	SDL_GPUDevice* device,
	const SDL_GPUShaderCreateInfo& params
) noexcept
	: device(device),
	  shader(SDL_CreateGPUShader(device, &params)) {}

Bloodlust::GPUShader::~GPUShader() noexcept {
	SDL_ReleaseGPUShader(device, shader);
}

// TODO: automatically determine the preferred shader format
SDL_GPUShaderFormat Bloodlust::getPreferredShaderFormat() noexcept {
	// if (preferredShaderFormat != SDL_GPU_SHADERFORMAT_INVALID) {
	// 	return preferredShaderFormat;
	// }

	// #define BL_CHECK_SHADER_FORMAT(fmt)
	// 	if (SDL_GPUSupportsShaderFormats(fmt, nullptr)) {
	// 		preferredShaderFormat = fmt;
	// 		return fmt;
	// 	}
	// BL_CHECK_SHADER_FORMAT(SDL_GPU_SHADERFORMAT_SPIRV);
	// BL_CHECK_SHADER_FORMAT(SDL_GPU_SHADERFORMAT_MSL);
	// BL_CHECK_SHADER_FORMAT(SDL_GPU_SHADERFORMAT_DXIL);
	// BL_CHECK_SHADER_FORMAT(SDL_GPU_SHADERFORMAT_DXBC);
	// #undef BL_CHECK_SHADER_FORMAT
	preferredShaderFormat = SDL_GPU_SHADERFORMAT_SPIRV;
	return preferredShaderFormat;
}

#define BL_LOADSHADER_COMMON(createInfo, shaderDef)                  \
	createInfo.code_size = shaderDef.srcLen - 1;                     \
	createInfo.code = shaderDef.src;                                 \
	if (shaderDef.format == SDL_GPU_SHADERFORMAT_INVALID) {          \
		createInfo.format = getPreferredShaderFormat();              \
	} else {                                                         \
		createInfo.format = shaderDef.format;                        \
	}                                                                \
	if (createInfo.format == SDL_GPU_SHADERFORMAT_MSL) {             \
		createInfo.entrypoint = "main0";                             \
	} else {                                                         \
		createInfo.entrypoint = "main";                              \
	}                                                                \
	createInfo.num_samplers = shaderDef.samplerCount;                \
	createInfo.num_storage_textures = shaderDef.storageTextureCount; \
	createInfo.num_storage_buffers = shaderDef.storageBufferCount;   \
	createInfo.num_uniform_buffers = shaderDef.uniformBufferCount;

std::unique_ptr<Bloodlust::GPUShader> Bloodlust::loadVertShader(
	SDL_GPUDevice* device,
	const VertShaderDef& shaderDef
) noexcept {
	SDL_GPUShaderCreateInfo createInfo = {};
	createInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
	BL_LOADSHADER_COMMON(createInfo, shaderDef)
	return std::make_unique<GPUShader>(device, createInfo);
}

std::unique_ptr<Bloodlust::GPUShader> Bloodlust::loadFragShader(
	SDL_GPUDevice* device,
	const FragShaderDef& shaderDef
) noexcept {
	SDL_GPUShaderCreateInfo createInfo = {};
	createInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	BL_LOADSHADER_COMMON(createInfo, shaderDef)
	return std::make_unique<GPUShader>(device, createInfo);
}

#undef BL_LOADSHADER_COMMON
