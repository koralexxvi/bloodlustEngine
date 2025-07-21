#include "../../include/render/graphics.hpp"

#include "../../extern/stb_image.h"
#include "../../include/app.hpp"
#include "../../include/config.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>

namespace {

bool clipRectActive = false;

Bloodlust::Color currentColor = {};

bool canUseDepthTexFormat(SDL_GPUTextureFormat format) {
	return SDL_GPUTextureSupportsFormat(
		Bloodlust::app->renderState.device,
		format,
		SDL_GPU_TEXTURETYPE_2D,
		SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET
	);
}

int getClipRectIndex() {
	return clipRectActive ? Bloodlust::app->renderState.clipCount : 0;
}

}  // namespace

SDL_GPUTextureFormat Bloodlust::getDepthTexFormat() {
	if (canUseDepthTexFormat(SDL_GPU_TEXTUREFORMAT_D32_FLOAT)) {
		return SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
	} else if (canUseDepthTexFormat(SDL_GPU_TEXTUREFORMAT_D24_UNORM)) {
		return SDL_GPU_TEXTUREFORMAT_D24_UNORM;
	}
	return SDL_GPU_TEXTUREFORMAT_D16_UNORM;
}

bool Bloodlust::initGraphics() {
	app->renderState = {};
#ifdef BL_PRODUCTION
	app->renderState.device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, nullptr);
#else
	app->renderState.device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
#endif
	if (app->renderState.device == nullptr) {
		return false;
	}
	app->renderState.textEngine = TTF_CreateGPUTextEngine(app->renderState.device);
	if (app->renderState.textEngine == nullptr) {
		return false;
	}
	TTF_SetGPUTextEngineWinding(
		app->renderState.textEngine,
		TTF_GPU_TEXTENGINE_WINDING_COUNTER_CLOCKWISE
	);
	stbi_set_flip_vertically_on_load_thread(1);
	if (!SDL_ClaimWindowForGPUDevice(app->renderState.device, app->window)) {
		return false;
	}
	bool success;
	app->renderState.rectFillShader =
		new RectFillShader(app->renderState.device, app->window, success);
	if (!success) {
		return false;
	}
	app->renderState.textureShader =
		new TextureShader(app->renderState.device, app->window, success);
	if (!success) {
		return false;
	}
	app->renderState.textShader =
		new TextShader(app->renderState.device, app->window, app->renderState.textEngine, success);
	if (!success) {
		return false;
	}
	return onResize(config.windowWidth, config.windowHeight);
}

bool Bloodlust::runRenderPass() {
	SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(app->renderState.device);
	if (cmdbuf == nullptr) {
		return false;
	}
	SDL_GPUTexture* swapTex;
	if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, app->window, &swapTex, nullptr, nullptr)) {
		return false;
	}
	if (swapTex == nullptr) {
		SDL_CancelGPUCommandBuffer(cmdbuf);
		return true;
	}

	SDL_GPUCopyPass* copyPass;
	SDL_GPURenderPass* renderPass;

	SDL_GPUColorTargetInfo colorTargetInfo = {};
	colorTargetInfo.texture = swapTex;
	colorTargetInfo.clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
	colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
	colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

	SDL_GPUDepthStencilTargetInfo stencilTargetInfo = {};
	stencilTargetInfo.texture = app->renderState.depthTexture;
	stencilTargetInfo.clear_depth = 1.0f;
	stencilTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
	stencilTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
	stencilTargetInfo.stencil_load_op = SDL_GPU_LOADOP_CLEAR;
	stencilTargetInfo.stencil_store_op = SDL_GPU_STOREOP_STORE;
	stencilTargetInfo.clear_stencil = 0xff;

	app->renderState.rectFillShader->prepareShader();
	app->renderState.textureShader->prepareShader();
	app->renderState.textShader->prepareShader();

	copyPass = SDL_BeginGPUCopyPass(cmdbuf);
	if (copyPass == nullptr) {
		setError("Failed to begin copy pass: %s", getError());
		goto fail;
	}
	app->renderState.rectFillShader->runCopyPass(cmdbuf, copyPass);
	app->renderState.textureShader->runCopyPass(cmdbuf, copyPass);
	app->renderState.textShader->runCopyPass(cmdbuf, copyPass);
	SDL_EndGPUCopyPass(copyPass);

	renderPass = SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, &stencilTargetInfo);
	if (renderPass == nullptr) {
		setError("Failed to begin render pass: %s", getError());
		goto fail;
	}
	app->renderState.rectFillShader->runRenderPass(cmdbuf, renderPass, swapTex);
	app->renderState.textureShader->runRenderPass(cmdbuf, renderPass, swapTex);
	app->renderState.textShader->runRenderPass(cmdbuf, renderPass, swapTex);
	SDL_EndGPURenderPass(renderPass);

	if (SDL_SubmitGPUCommandBuffer(cmdbuf)) {
		return true;
	}

fail:
	SDL_CancelGPUCommandBuffer(cmdbuf);
	clearAll();
	return false;
}

bool Bloodlust::onResize(uint32_t width, uint32_t height) {
	if (!resizeSwapchainTexture(width, height)) {
		return false;
	}
	app->renderState.clipRects[0] = {0, 0, int(width), int(height)};
	app->renderState.rectFillShader->onResize(width, height);
	app->renderState.textureShader->onResize(width, height);
	app->renderState.textShader->onResize(width, height);
	return true;
}

bool Bloodlust::resizeSwapchainTexture(uint32_t w, uint32_t h) {
	SDL_ReleaseGPUTexture(app->renderState.device, app->renderState.depthTexture);
	SDL_GPUTextureCreateInfo createInfo = {
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = getDepthTexFormat(),
		.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
		.width = w,
		.height = h,
		.layer_count_or_depth = 1,
		.num_levels = 1,
		.sample_count = SDL_GPU_SAMPLECOUNT_1,
		.props = 0
	};
	return (app->renderState.depthTexture =
				SDL_CreateGPUTexture(app->renderState.device, &createInfo))
		!= nullptr;
}

void Bloodlust::clearAll() {
	app->renderState.clipCount = 0;
	app->renderState.rectFillShader->clearData();
	app->renderState.textShader->clearData();
	app->renderState.textureShader->clearData();
}

void Bloodlust::clipToRect(const SDL_Rect& rect) {
	if (app->renderState.clipCount == BL_MAX_CLIP_COUNT) {
		clipRectActive = false;
		return;
	}
	clipRectActive = true;
	app->renderState.clipCount++;
	app->renderState.clipRects[app->renderState.clipCount] = rect;
}

void Bloodlust::resetClip() {
	clipRectActive = false;
}

void Bloodlust::drawLine(const Vector2& start, const Vector2& end) {}

void Bloodlust::drawRectOutline(const Vector2& pos, const Vector2& size, float width) {
	drawRectOutline({pos.x, pos.y, size.x, size.y}, width);
}

void Bloodlust::drawRectOutline(const SDL_FRect& rect, float width) {
	app->renderState.rectFillShader->pushRect(
		{rect.x, rect.y + rect.h, rect.w + width, width},
		currentColor
	);
	app->renderState.rectFillShader->pushRect(
		{rect.x - width, rect.y, width, rect.h + width},
		currentColor
	);
	app->renderState.rectFillShader->pushRect(
		{rect.x + rect.w, rect.y - width, width, rect.h + width},
		currentColor
	);
	app->renderState.rectFillShader->pushRect(
		{rect.x - width, rect.y - width, rect.w + width, width},
		currentColor
	);
}

void Bloodlust::fillRect(const Vector2& pos, const Vector2& size) {
	fillRect({pos.x, pos.y, size.x, size.y});
}

void Bloodlust::fillRect(const SDL_FRect& rect) {
	if (clipRectActive) {
		SDL_FRect fClip;
		SDL_FRect clippedRect;
		SDL_RectToFRect(&app->renderState.clipRects[app->renderState.clipCount], &fClip);
		if (!SDL_GetRectIntersectionFloat(&rect, &fClip, &clippedRect)) {
			return;
		}
		app->renderState.rectFillShader->pushRect(clippedRect, currentColor);
	} else {
		app->renderState.rectFillShader->pushRect(rect, currentColor);
	}
}

void Bloodlust::drawCircleOutline(const Vector2& origin, float radius, float width) {}
void Bloodlust::fillCircle(const Vector2& origin, float radius) {}

void Bloodlust::drawTexture(Texture* texture, const SDL_FRect& rect) {
	app->renderState.textureShader
		->pushTexture(texture, {rect.x, rect.y}, {rect.w, rect.h}, getClipRectIndex());
}

void Bloodlust::drawTexture(Texture* texture, const Vector2& pos, const Vector2& size) {
	app->renderState.textureShader->pushTexture(texture, pos, size, getClipRectIndex());
}

void Bloodlust::drawText(const char* str, Font* font, const Vector2& pos, float size) {
	if (size > 0.0f) {
		app->renderState.textShader->pushText(str, font, pos, size, currentColor);
	}
}

void Bloodlust::setPixel(const Vector2& pos) {}

void Bloodlust::setDrawColor(const Color& color) {
	currentColor = color;
}

void Bloodlust::setDrawColor(uint32_t color) {
	currentColor = Color::fromInt32(color);
}

void Bloodlust::setDrawColor(float r, float g, float b, float a) {
	currentColor = {r, g, b, a};
}
