#include "../../include/render/shaderProgram.hpp"

#include "../../include/graphics.hpp"
#include "../../include/shaderUtil.hpp"

#include <SDL3/SDL_gpu.h>

using Bloodlust::ShaderProgram;

bool ShaderProgram::init(
	SDL_GPUDevice* device,
	SDL_Window* window,
	const VertShaderDef& vertDef,
	const FragShaderDef& fragDef
) {
	this->device = device;
	this->window = window;
	SDL_GPUShader* vertShader = loadVertShader(device, vertDef);
	if (vertShader == nullptr) {
		return false;
	}
	SDL_GPUShader* fragShader = loadFragShader(device, fragDef);
	if (fragShader == nullptr) {
		return false;
	}

	SDL_GPUColorTargetBlendState blendState = {};
	blendState.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
	blendState.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
	blendState.color_blend_op = SDL_GPU_BLENDOP_ADD;
	blendState.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
	blendState.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_DST_ALPHA;
	blendState.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
	blendState.color_write_mask = 0xF;
	blendState.enable_blend = true;
	SDL_GPUColorTargetDescription colorTargetDescription = {
		.format = SDL_GetGPUSwapchainTextureFormat(device, window),
		.blend_state = blendState
	};

	SDL_GPUVertexAttribute* vertAttribs = new SDL_GPUVertexAttribute[vertDef.vertexAttribCount];
	uint32_t offset = 0;
	for (uint32_t i = 0; i < vertDef.vertexAttribCount; i++) {
		SDL_GPUVertexElementFormat format = vertDef.vertexAttribDefs[i].format;
		vertAttribs[i] = {
			.location = i,
			.buffer_slot = vertDef.vertexAttribDefs[i].bufferSlot,
			.format = format,
			.offset = offset
		};
		offset += getFormatSize(format);
	}
	SDL_GPUVertexBufferDescription vertBufferDescription = {
		.slot = 0,
		.pitch = offset,
		.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
		.instance_step_rate = 0
	};

	SDL_GPUGraphicsPipelineTargetInfo targetInfo = {};
	targetInfo.color_target_descriptions = &colorTargetDescription;
	targetInfo.num_color_targets = 1;
	targetInfo.depth_stencil_format = getDepthTexFormat();
	targetInfo.has_depth_stencil_target = true;

	SDL_GPUGraphicsPipelineCreateInfo createInfo = {};
	createInfo.vertex_shader = vertShader;
	createInfo.fragment_shader = fragShader;
	createInfo.vertex_input_state = {
		.vertex_buffer_descriptions = &vertBufferDescription,
		.num_vertex_buffers = 1,
		.vertex_attributes = vertAttribs,
		.num_vertex_attributes = vertDef.vertexAttribCount
	};
	createInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
	createInfo.depth_stencil_state.compare_op = SDL_GPU_COMPAREOP_LESS;
	createInfo.depth_stencil_state.enable_depth_test = true;
	createInfo.depth_stencil_state.compare_mask = 0xFF;
	createInfo.target_info = targetInfo;

	pipeline = SDL_CreateGPUGraphicsPipeline(device, &createInfo);
	SDL_ReleaseGPUShader(device, vertShader);
	SDL_ReleaseGPUShader(device, fragShader);
	delete[] vertAttribs;
	if (pipeline == nullptr) {
		return false;
	}

	SDL_GPUBufferCreateInfo vertBufferInfo =
		{.usage = SDL_GPU_BUFFERUSAGE_VERTEX, .size = uint32_t(vertDef.vertBufferSize), .props = 0};
	SDL_GPUBufferCreateInfo indexBufferInfo =
		{.usage = SDL_GPU_BUFFERUSAGE_INDEX, .size = uint32_t(vertDef.indexBufferSize), .props = 0};
	SDL_GPUTransferBufferCreateInfo vertTransBufInfo = {
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = vertBufferInfo.size + indexBufferInfo.size,
		.props = 0
	};

	vertBuffer = SDL_CreateGPUBuffer(device, &vertBufferInfo);
	indexBuffer = SDL_CreateGPUBuffer(device, &indexBufferInfo);
	vertTransBuf = SDL_CreateGPUTransferBuffer(device, &vertTransBufInfo);
	if (vertBuffer == nullptr || indexBuffer == nullptr) {
		return false;
	}
	return true;
}

ShaderProgram::~ShaderProgram() {
	SDL_ReleaseGPUBuffer(device, vertBuffer);
	SDL_ReleaseGPUBuffer(device, indexBuffer);
	SDL_ReleaseGPUTransferBuffer(device, vertTransBuf);
	SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
}
