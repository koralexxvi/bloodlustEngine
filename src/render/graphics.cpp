#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <bloodlustEngine/app.hpp>
#include <bloodlustEngine/assets.hpp>
#include <bloodlustEngine/datatype/texture.hpp>
#include <bloodlustEngine/exceptions/internalError.hpp>
#include <bloodlustEngine/logging.hpp>
#include <bloodlustEngine/render/graphics.hpp>
#include <cstdint>
#include <stb/stb_image.h>

namespace {

bool clipRectActive = false;

Bloodlust::Colour currentColour = {};
std::size_t currentLayer = 0;

std::size_t getClipRectIndex() noexcept {
	return clipRectActive ? Bloodlust::app->renderState.clipRects.size() - 1 : 0;
}

}  // namespace

void Bloodlust::initGraphics() {
	app->renderState = {};
#ifdef BL_PRODUCTION
	app->renderState.device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, nullptr);
#else
	app->renderState.device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
#endif
	if (app->renderState.device == nullptr) {
		throw InternalError("Could not create GPU device: " + getError());
	}
	app->renderState.textEngine = TTF_CreateGPUTextEngine(app->renderState.device);
	if (app->renderState.textEngine == nullptr) {
		throw InternalError("Could not create text engine: " + getError());
	}
	TTF_SetGPUTextEngineWinding(
		app->renderState.textEngine,
		TTF_GPU_TEXTENGINE_WINDING_COUNTER_CLOCKWISE
	);
	stbi_set_flip_vertically_on_load_thread(1);
	if (!SDL_ClaimWindowForGPUDevice(app->renderState.device, app->window->getHandle())) {
		throw InternalError("Could not claim window: " + getError());
	}
	for (std::size_t i = 0; i < BL_LAYER_COUNT; i++) {
		app->renderState.internalShaders[i].rectFill =
			std::make_unique<RectFillShader>(app->renderState.device, app->window.get());
		app->renderState.internalShaders[i].texture =
			std::make_unique<TextureShader>(app->renderState.device, app->window.get());
		app->renderState.internalShaders[i].text =
			std::make_unique<TextShader>(app->renderState.device, app->window.get());
		app->renderState.internalShaders[i].line =
			std::make_unique<LineShader>(app->renderState.device, app->window.get());
	}
	onResize(app->getConfig().windowWidth, app->getConfig().windowHeight);
}

void Bloodlust::runRenderPass() {
	SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(app->renderState.device);
	if (cmdbuf == nullptr) {
		throw InternalError("Could not acquire command buffer: " + getError());
	}
	SDL_GPUTexture* swapTex;
	if (!SDL_WaitAndAcquireGPUSwapchainTexture(
			cmdbuf,
			app->window->getHandle(),
			&swapTex,
			nullptr,
			nullptr
		))
	{
		throw InternalError("Could not get swapchain texture: " + getError());
	}
	if (swapTex == nullptr) {
		SDL_CancelGPUCommandBuffer(cmdbuf);
		return;
	}

	SDL_GPUCopyPass* copyPass;
	SDL_GPURenderPass* renderPass;

	SDL_GPUColorTargetInfo colourTargetInfo = {};
	colourTargetInfo.texture = swapTex;
	colourTargetInfo.clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
	colourTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
	colourTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

	for (const auto& v : app->renderState.internalShaders) {
		v.rectFill->prepareShader();
		v.line->prepareShader();
		v.texture->prepareShader();
		v.text->prepareShader();
	}

	copyPass = SDL_BeginGPUCopyPass(cmdbuf);
	if (copyPass == nullptr) {
		throw InternalError("Failed to begin copy pass: {}" + getError());
	}

	for (const auto& v : app->renderState.internalShaders) {
		v.rectFill->runCopyPass(copyPass);
		v.line->runCopyPass(copyPass);
		v.texture->runCopyPass(copyPass);
		v.text->runCopyPass(copyPass);
	}
	SDL_EndGPUCopyPass(copyPass);

	renderPass = SDL_BeginGPURenderPass(cmdbuf, &colourTargetInfo, 1, nullptr);
	if (renderPass == nullptr) {
		throw InternalError("Failed to begin render pass: {}" + getError());
	}
	for (const auto& v : app->renderState.internalShaders) {
		v.rectFill->runRenderPass(cmdbuf, renderPass);
		v.line->runRenderPass(cmdbuf, renderPass);
		v.texture->runRenderPass(cmdbuf, renderPass);
		v.text->runRenderPass(cmdbuf, renderPass);
	}
	SDL_EndGPURenderPass(renderPass);

	if (!SDL_SubmitGPUCommandBuffer(cmdbuf)) {
		throw InternalError("Failed to submit command buffer: " + getError());
	}
}

void Bloodlust::onResize(std::uint32_t width, std::uint32_t height) noexcept {
	// TODO: there is probably a way better way to do this
	SDL_Rect rect = {0, 0, static_cast<int>(width), static_cast<int>(height)};
	if (app->renderState.clipRects.empty()) {
		app->renderState.clipRects.push_back(rect);
	} else {
		app->renderState.clipRects[0] = rect;
	}
	for (const auto& v : app->renderState.internalShaders) {
		v.rectFill->onResize(width, height);
		v.line->onResize(width, height);
		v.texture->onResize(width, height);
		v.text->onResize(width, height);
	}
}

double Bloodlust::getAlphaBetweenSteps() noexcept {
	std::uint64_t now = SDL_GetTicksNS();
	return (static_cast<double>(now) - static_cast<double>(Bloodlust::app->lastStep))
		 / static_cast<double>(app->getConfig().appTimestep);
}

void Bloodlust::clearAll() noexcept {
	// TODO: improve memory management here as this is really slow
	app->renderState.clipRects.resize(1);
	for (const auto& v : app->renderState.internalShaders) {
		v.rectFill->clearData();
		v.line->clearData();
		v.texture->clearData();
		v.text->clearData();
	}
}

void Bloodlust::clipToRect(const SDL_Rect& rect) noexcept {
	clipRectActive = true;
	app->renderState.clipRects.push_back(rect);
}

void Bloodlust::resetClip() noexcept {
	clipRectActive = false;
}

void Bloodlust::drawLine(const Vector2& start, const Vector2& end, float width) noexcept {
	app->renderState.internalShaders[currentLayer]
		.line->pushLine(start, end, width, currentColour, getClipRectIndex());
}

void Bloodlust::drawRectOutline(const Vector2& pos, const Vector2& size, float width) noexcept {
	drawRectOutline({pos.x, pos.y, size.x, size.y}, width);
}

void Bloodlust::drawRectOutline(const SDL_FRect& rect, float width) noexcept {
	fillRect({rect.x, rect.y + rect.h, rect.w + width, width});
	fillRect({rect.x - width, rect.y, width, rect.h + width});
	fillRect({rect.x + rect.w, rect.y - width, width, rect.h + width});
	fillRect({rect.x - width, rect.y - width, rect.w + width, width});
}

void Bloodlust::fillRect(const Vector2& pos, const Vector2& size) noexcept {
	fillRect({pos.x, pos.y, size.x, size.y});
}

void Bloodlust::fillRect(const SDL_FRect& rect) noexcept {
	if (clipRectActive) {
		SDL_FRect fClip;
		SDL_FRect clippedRect;
		SDL_RectToFRect(&app->renderState.clipRects.back(), &fClip);
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
void Bloodlust::drawCircleOutline(const Vector2& pos, float radius, float width) noexcept {
	float interval = (BL_PI_F * 2.0f) / static_cast<float>(BL_CIRCLE_POINTS);
	Vector2 lastPoint = pos + (Vector2::fromAngle(-interval) * radius);
	for (float i = 0.0f; i < BL_PI_F * 2.0f; i += interval) {
		Vector2 newPoint = pos + Vector2::fromAngle(i) * radius;
		drawLine(lastPoint, newPoint, width);
		lastPoint = newPoint;
	}
}

// TODO
void Bloodlust::fillCircle(const Vector2&, float) noexcept {}

void Bloodlust::drawTexture(const Texture* texture, const SDL_FRect& rect) noexcept {
	SDL_FRect texClip = {
		0.0f,
		0.0f,
		static_cast<float>(texture->getWidth()),
		static_cast<float>(texture->getHeight())
	};
	drawTexture(texture, rect, texClip);
}

void Bloodlust::drawTexture(
	const Texture* texture,
	const SDL_FRect& rect,
	const SDL_FRect& texClip
) noexcept {
	app->renderState.internalShaders[currentLayer].texture->pushTexture(
		texture,
		{rect.x, rect.y},
		{rect.w, rect.h},
		currentColour,
		texClip,
		getClipRectIndex()
	);
}

void Bloodlust::drawTexture(
	const Texture* texture,
	const Vector2& pos,
	const Vector2& size
) noexcept {
	SDL_FRect texClip = {
		0.0f,
		0.0f,
		static_cast<float>(texture->getWidth()),
		static_cast<float>(texture->getHeight())
	};
	drawTexture(texture, pos, size, texClip);
}

void Bloodlust::drawTexture(
	const Texture* texture,
	const Vector2& pos,
	const Vector2& size,
	const SDL_FRect& texClip
) noexcept {
	app->renderState.internalShaders[currentLayer]
		.texture->pushTexture(texture, pos, size, currentColour, texClip, getClipRectIndex());
}

void Bloodlust::drawText(TTF_Text* textObject, const Vector2& pos, float size) noexcept {
	if (size > 0.0f) {
		app->renderState.internalShaders[currentLayer]
			.text->pushText(textObject, pos, size, currentColour, getClipRectIndex());
	}
}

void Bloodlust::setPixel(const Vector2& pos) noexcept {
	app->renderState.internalShaders[currentLayer].rectFill->pushRect(
		{pos.x, pos.y, 1.0f, 1.0f},
		currentColour
	);
}

Bloodlust::Colour Bloodlust::getDrawColor() noexcept {
	return currentColour;
}

void Bloodlust::setDrawColour(const Colour& colour) noexcept {
	currentColour = colour;
}

void Bloodlust::setDrawColour(std::uint32_t colour) noexcept {
	currentColour = Colour::fromInt32(colour);
}

void Bloodlust::setDrawColour(float r, float g, float b, float a) noexcept {
	currentColour = {r, g, b, a};
}

std::size_t Bloodlust::getDrawLayer() noexcept {
	return currentLayer;
}

void Bloodlust::setDrawLayer(std::size_t newLayer) noexcept {
	SDL_assert(newLayer < BL_LAYER_COUNT);
	currentLayer = newLayer;
}

void Bloodlust::closeGraphics() noexcept {
	unloadAssets();
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
	SDL_DestroyGPUDevice(app->renderState.device);
}
