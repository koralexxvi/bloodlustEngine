#include "../../include/render/graphics.hpp"

#include "../../include/app.hpp"
#include "../../include/config.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>
#include <stb/stb_image.h>

namespace {

bool clipRectActive = false;

Bloodlust::Colour currentColour = {};
std::size_t currentLayer = 0;

bool canUseDepthTexFormat(SDL_GPUTextureFormat format) {
	return SDL_GPUTextureSupportsFormat(
		Bloodlust::app->renderState.device,
		format,
		SDL_GPU_TEXTURETYPE_2D,
		SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET
	);
}

size_t getClipRectIndex() {
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
	for (std::size_t i = 0; i < BL_LAYER_COUNT; i++) {
		app->renderState.internalShaders[i].rectFill =
			std::make_unique<RectFillShader>(app->renderState.device, app->window, success);
		if (!success) {
			return false;
		}
		app->renderState.internalShaders[i].texture =
			std::make_unique<TextureShader>(app->renderState.device, app->window, success);
		if (!success) {
			return false;
		}
		app->renderState.internalShaders[i].text =
			std::make_unique<TextShader>(app->renderState.device, app->window, success);
		if (!success) {
			return false;
		}
		app->renderState.internalShaders[i].line =
			std::make_unique<LineShader>(app->renderState.device, app->window, success);
		if (!success) {
			return false;
		}
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

	SDL_GPUColorTargetInfo colourTargetInfo = {};
	colourTargetInfo.texture = swapTex;
	colourTargetInfo.clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
	colourTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
	colourTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

	SDL_GPUDepthStencilTargetInfo stencilTargetInfo = {};
	stencilTargetInfo.texture = app->renderState.depthTexture;
	stencilTargetInfo.clear_depth = 1.0f;
	stencilTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
	stencilTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
	stencilTargetInfo.stencil_load_op = SDL_GPU_LOADOP_CLEAR;
	stencilTargetInfo.stencil_store_op = SDL_GPU_STOREOP_STORE;
	stencilTargetInfo.clear_stencil = 0xff;

	for (const auto& v : app->renderState.internalShaders) {
		v.rectFill->prepareShader();
		v.line->prepareShader();
		v.texture->prepareShader();
		v.text->prepareShader();
	}

	copyPass = SDL_BeginGPUCopyPass(cmdbuf);
	if (copyPass == nullptr) {
		setError("Failed to begin copy pass: %s", getError());
		goto fail;
	}

	for (const auto& v : app->renderState.internalShaders) {
		v.rectFill->runCopyPass(copyPass);
		v.line->runCopyPass(copyPass);
		v.texture->runCopyPass(copyPass);
		v.text->runCopyPass(copyPass);
	}
	SDL_EndGPUCopyPass(copyPass);

	renderPass = SDL_BeginGPURenderPass(cmdbuf, &colourTargetInfo, 1, &stencilTargetInfo);
	if (renderPass == nullptr) {
		setError("Failed to begin render pass: %s", getError());
		goto fail;
	}
	for (const auto& v : app->renderState.internalShaders) {
		v.rectFill->runRenderPass(cmdbuf, renderPass);
		v.line->runRenderPass(cmdbuf, renderPass);
		v.texture->runRenderPass(cmdbuf, renderPass);
		v.text->runRenderPass(cmdbuf, renderPass);
	}
	SDL_EndGPURenderPass(renderPass);

	if (SDL_SubmitGPUCommandBuffer(cmdbuf)) {
		return true;
	}

fail:
	SDL_CancelGPUCommandBuffer(cmdbuf);
	clearAll();
	return false;
}

bool Bloodlust::onResize(std::uint32_t width, std::uint32_t height) {
	if (!resizeSwapchainTexture(width, height)) {
		return false;
	}
	app->renderState.clipRects[0] = {0, 0, int(width), int(height)};
	for (const auto& v : app->renderState.internalShaders) {
		v.rectFill->onResize(width, height);
		v.line->onResize(width, height);
		v.texture->onResize(width, height);
		v.text->onResize(width, height);
	}
	return true;
}

bool Bloodlust::resizeSwapchainTexture(std::uint32_t w, std::uint32_t h) {
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

double Bloodlust::getAlphaBetweenSteps() {
	std::uint64_t now = SDL_GetTicksNS();
	return (double(now) - double(Bloodlust::app->lastStep)) / double(Bloodlust::config.appTimestep);
}

void Bloodlust::clearAll() {
	app->renderState.clipCount = 0;
	for (const auto& v : app->renderState.internalShaders) {
		v.rectFill->clearData();
		v.line->clearData();
		v.texture->clearData();
		v.text->clearData();
	}
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

void Bloodlust::drawLine(const Vector2& start, const Vector2& end, float width) {
	app->renderState.internalShaders[currentLayer]
		.line->pushLine(start, end, width, currentColour, getClipRectIndex());
}

void Bloodlust::drawRectOutline(const Vector2& pos, const Vector2& size, float width) {
	drawRectOutline({pos.x, pos.y, size.x, size.y}, width);
}

void Bloodlust::drawRectOutline(const SDL_FRect& rect, float width) {
	fillRect({rect.x, rect.y + rect.h, rect.w + width, width});
	fillRect({rect.x - width, rect.y, width, rect.h + width});
	fillRect({rect.x + rect.w, rect.y - width, width, rect.h + width});
	fillRect({rect.x - width, rect.y - width, rect.w + width, width});
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
		app->renderState.internalShaders[currentLayer].rectFill->pushRect(
			clippedRect,
			currentColour
		);
	} else {
		app->renderState.internalShaders[currentLayer].rectFill->pushRect(rect, currentColour);
	}
}

// TODO: draw the whole outline as one mesh
void Bloodlust::drawCircleOutline(const Vector2& pos, float radius, float width) {
	float interval = (BL_PI_F * 2.0f) / float(BL_CIRCLE_POINTS);
	Vector2 lastPoint = pos + (Vector2::fromAngle(-interval) * radius);
	for (float i = 0.0f; i < BL_PI_F * 2.0f; i += interval) {
		Vector2 newPoint = pos + Vector2::fromAngle(i) * radius;
		drawLine(lastPoint, newPoint, width);
		lastPoint = newPoint;
	}
}

void Bloodlust::fillCircle(const Vector2&, float) {}

void Bloodlust::drawTexture(const Texture* texture, const SDL_FRect& rect) {
	app->renderState.internalShaders[currentLayer].texture->pushTexture(
		texture,
		{rect.x, rect.y},
		{rect.w, rect.h},
		currentColour,
		getClipRectIndex()
	);
}

void Bloodlust::drawTexture(const Texture* texture, const Vector2& pos, const Vector2& size) {
	app->renderState.internalShaders[currentLayer]
		.texture->pushTexture(texture, pos, size, currentColour, getClipRectIndex());
}

void Bloodlust::drawText(TTF_Text* textObject, const Vector2& pos, float size) {
	if (size > 0.0f) {
		app->renderState.internalShaders[currentLayer]
			.text->pushText(textObject, pos, size, currentColour, getClipRectIndex());
	}
}

void Bloodlust::setPixel(const Vector2& pos) {
	app->renderState.internalShaders[currentLayer].rectFill->pushRect(
		{pos.x, pos.y, 1.0f, 1.0f},
		currentColour
	);
}

Bloodlust::Colour Bloodlust::getDrawColor() {
	return currentColour;
}

void Bloodlust::setDrawColour(const Colour& colour) {
	currentColour = colour;
}

void Bloodlust::setDrawColour(std::uint32_t colour) {
	currentColour = Colour::fromInt32(colour);
}

void Bloodlust::setDrawColour(float r, float g, float b, float a) {
	currentColour = {r, g, b, a};
}

std::size_t Bloodlust::getDrawLayer() {
	return currentLayer;
}

void Bloodlust::setDrawLayer(std::size_t newLayer) {
	SDL_assert(newLayer < BL_LAYER_COUNT);
	currentLayer = newLayer;
}

void Bloodlust::closeGraphics() {
	if (app == nullptr) {
		return;
	}
	for (auto&& v : app->renderState.internalShaders) {
		v.rectFill = nullptr;
		v.line = nullptr;
		v.texture = nullptr;
		v.text = nullptr;
	}
	TTF_DestroyGPUTextEngine(app->renderState.textEngine);
	SDL_ReleaseGPUTexture(app->renderState.device, app->renderState.depthTexture);
	SDL_DestroyGPUDevice(app->renderState.device);
}
