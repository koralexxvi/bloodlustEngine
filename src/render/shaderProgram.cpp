#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_gpu.h>
#include <bloodlustEngine/exception/internalError.hpp>
#include <bloodlustEngine/logging.hpp>
#include <bloodlustEngine/render/shaderProgram.hpp>
#include <bloodlustEngine/render/shaderUtil.hpp>
#include <bloodlustEngine/render/window.hpp>

using Bloodlust::ShaderProgram;

void ShaderProgram::init(
	SDL_GPUDevice* device,
	Window* window,
	const VertShaderDef& vertDef,
	const FragShaderDef& fragDef
) {
	this->device = device;
	this->window = window;
	std::unique_ptr<GPUShader> vertShader = loadVertShader(device, vertDef);
	std::unique_ptr<GPUShader> fragShader = loadFragShader(device, fragDef);

	SDL_GPUColorTargetBlendState blendState = {};
	blendState.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
	blendState.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
	blendState.color_blend_op = SDL_GPU_BLENDOP_ADD;
	blendState.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
	blendState.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_DST_ALPHA;
	blendState.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
	blendState.color_write_mask = 0xF;
	blendState.enable_blend = true;
	SDL_GPUColorTargetDescription colourTargetDescription = {
		.format = SDL_GetGPUSwapchainTextureFormat(device, window->getHandle()),
		.blend_state = blendState
	};

	SDL_GPUVertexAttribute* vertAttribs = new SDL_GPUVertexAttribute[vertDef.vertexAttribCount];
	std::uint32_t offset = 0;
	for (std::uint32_t i = 0; i < vertDef.vertexAttribCount; i++) {
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
	targetInfo.color_target_descriptions = &colourTargetDescription;
	targetInfo.num_color_targets = 1;

	SDL_GPUGraphicsPipelineCreateInfo createInfo = {};
	createInfo.vertex_shader = vertShader->getHandle();
	createInfo.fragment_shader = fragShader->getHandle();
	createInfo.rasterizer_state = {};
	createInfo.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_BACK;
	createInfo.rasterizer_state.front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE;
	createInfo.vertex_input_state = {
		.vertex_buffer_descriptions = &vertBufferDescription,
		.num_vertex_buffers = 1,
		.vertex_attributes = vertAttribs,
		.num_vertex_attributes = vertDef.vertexAttribCount
	};
	createInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
	createInfo.target_info = targetInfo;

	pipeline = SDL_CreateGPUGraphicsPipeline(device, &createInfo);
	delete[] vertAttribs;
	if (pipeline == nullptr) {
		throw InternalError("Could not create graphics pipeline: " + getError());
	}

	resizeVertexBuffer(vertDef.vertBufferSize);
	resizeIndexBuffer(vertDef.indexBufferSize);
	resizeTransferBuffer();
}

void ShaderProgram::resizeVertexBuffer(std::uint32_t size) {
	if (size == vertBufferSize) {
		return;
	}
	SDL_assert(size > 0);
	SDL_ReleaseGPUBuffer(device, vertBuffer);
	SDL_GPUBufferCreateInfo createInfo =
		{.usage = SDL_GPU_BUFFERUSAGE_VERTEX, .size = size, .props = 0};
	vertBufferSize = size;
	vertBuffer = SDL_CreateGPUBuffer(device, &createInfo);
	if (vertBuffer == nullptr) {
		throw InternalError("Could not resize vertex buffer: " + getError());
	}
}

void ShaderProgram::resizeIndexBuffer(std::uint32_t size) {
	if (size == indexBufferSize) {
		return;
	}
	SDL_assert(size > 0);
	SDL_ReleaseGPUBuffer(device, indexBuffer);
	SDL_GPUBufferCreateInfo createInfo =
		{.usage = SDL_GPU_BUFFERUSAGE_INDEX, .size = size, .props = 0};
	indexBufferSize = size;
	indexBuffer = SDL_CreateGPUBuffer(device, &createInfo);
	if (indexBuffer == nullptr) {
		throw InternalError("Could not resize index buffer: " + getError());
	}
}

void ShaderProgram::resizeTransferBuffer() {
	std::uint32_t newSize = vertBufferSize + indexBufferSize;
	if (newSize == transBufSize) {
		return;
	}
	SDL_assert(newSize > 0);
	SDL_ReleaseGPUTransferBuffer(device, vertTransBuf);
	SDL_GPUTransferBufferCreateInfo createInfo =
		{.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, .size = newSize, .props = 0};
	transBufSize = newSize;
	vertTransBuf = SDL_CreateGPUTransferBuffer(device, &createInfo);
	if (vertTransBuf == nullptr) {
		throw InternalError("Could not resize transfer buffer: " + getError());
	}
}

ShaderProgram::~ShaderProgram() noexcept {
	SDL_ReleaseGPUBuffer(device, vertBuffer);
	SDL_ReleaseGPUBuffer(device, indexBuffer);
	SDL_ReleaseGPUTransferBuffer(device, vertTransBuf);
	SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
}
