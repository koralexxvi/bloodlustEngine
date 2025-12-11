#include "../../include/render/graphics.hpp"

#include "../../include/app.hpp"
#include "../../include/assets.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>
#include <stb/stb_image.h>

namespace {

bool clipRectActive = false;

Bloodlust::Point<std::uint32_t> drawRes = {480, 270};
Bloodlust::Vector2 drawScale;

Bloodlust::Colour currentColour = {};
std::size_t currentLayer = 0;

std::size_t getClipRectIndex() {
	return clipRectActive ? Bloodlust::app->renderState.clipRects.size() - 1 : 0;
}

}  // namespace

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
	if (!SDL_ClaimWindowForGPUDevice(app->renderState.device, app->window->getHandle())) {
		return false;
	}
	bool success;
	for (std::size_t i = 0; i < BL_LAYER_COUNT; i++) {
		app->renderState.internalShaders[i].rectFill =
			std::make_unique<RectFillShader>(app->renderState.device, app->window.get(), success);
		if (!success) {
			return false;
		}
		app->renderState.internalShaders[i].texture =
			std::make_unique<TextureShader>(app->renderState.device, app->window.get(), success);
		if (!success) {
			return false;
		}
		app->renderState.internalShaders[i].text =
			std::make_unique<TextShader>(app->renderState.device, app->window.get(), success);
		if (!success) {
			return false;
		}
		app->renderState.internalShaders[i].line =
			std::make_unique<LineShader>(app->renderState.device, app->window.get(), success);
		if (!success) {
			return false;
		}
	}
	return onResize(app->getConfig().windowWidth, app->getConfig().windowHeight);
}

bool Bloodlust::runRenderPass() {
	SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(app->renderState.device);
	if (cmdbuf == nullptr) {
		return false;
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

	for (const auto& v : app->renderState.internalShaders) {
		if (!(v.rectFill->prepareShader() && v.line->prepareShader() && v.texture->prepareShader()
			  && v.text->prepareShader()))
		{
			return false;
		}
	}

	copyPass = SDL_BeginGPUCopyPass(cmdbuf);
	if (copyPass == nullptr) {
		setError("Failed to begin copy pass: %s", getError().c_str());
		goto fail;
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
		setError("Failed to begin render pass: %s", getError().c_str());
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
	// TODO: there is probably a way better way to do this
	SDL_Rect rect = {0, 0, static_cast<int>(width), static_cast<int>(height)};
	if (app->renderState.clipRects.empty()) {
		app->renderState.clipRects.push_back(rect);
	} else {
		app->renderState.clipRects[0] = rect;
	}
	// update drawScale
	setDrawResolution(drawRes);
	for (const auto& v : app->renderState.internalShaders) {
		v.rectFill->onResize(width, height);
		v.line->onResize(width, height);
		v.texture->onResize(width, height);
		v.text->onResize(width, height);
	}
	return true;
}

double Bloodlust::getAlphaBetweenSteps() {
	std::uint64_t now = SDL_GetTicksNS();
	return (static_cast<double>(now) - static_cast<double>(Bloodlust::app->lastStep))
		 / static_cast<double>(app->getConfig().appTimestep);
}

void Bloodlust::clearAll() {
	// TODO: improve memory management here as this is really slow
	app->renderState.clipRects.resize(1);
	for (const auto& v : app->renderState.internalShaders) {
		v.rectFill->clearData();
		v.line->clearData();
		v.texture->clearData();
		v.text->clearData();
	}
}

const Bloodlust::Point<std::uint32_t>& Bloodlust::getDrawResolution() {
	return drawRes;
}

void Bloodlust::setDrawResolution(const Bloodlust::Point<std::uint32_t>& newRes) {
	Vector2 windowSize = app->window->getSize().toVector();
	Vector2 fRes = newRes.toVector();
	drawRes = newRes;
	drawScale.x = newRes.x == 0 ? 1.0f : (windowSize.x / fRes.x);
	drawScale.y = newRes.y == 0 ? 1.0f : (windowSize.y / fRes.y);
}

void Bloodlust::clipToRect(const SDL_Rect& rect) {
	clipRectActive = true;
	app->renderState.clipRects.push_back(rect);
}

void Bloodlust::resetClip() {
	clipRectActive = false;
}

void Bloodlust::drawLine(const Vector2& start, const Vector2& end, float width) {
	app->renderState.internalShaders[currentLayer].line->pushLine(
		start * drawScale,
		end * drawScale,
		width * drawScale.y,
		currentColour,
		getClipRectIndex()
	);
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
	SDL_FRect drawRect;
	if (clipRectActive) {
		SDL_FRect fClip;
		SDL_RectToFRect(&app->renderState.clipRects.back(), &fClip);
		if (!SDL_GetRectIntersectionFloat(&rect, &fClip, &drawRect)) {
			return;
		}
	} else {
		drawRect = rect;
	}
	app->renderState.internalShaders[currentLayer].rectFill->pushRect(
		{drawRect.x * drawScale.x,
		 drawRect.y * drawScale.y,
		 drawRect.w * drawScale.x,
		 drawRect.h * drawScale.y},
		currentColour
	);
}

// TODO: draw the whole outline as one mesh
void Bloodlust::drawCircleOutline(const Vector2& pos, float radius, float width) {
	float interval = (BL_PI_F * 2.0f) / static_cast<float>(BL_CIRCLE_POINTS);
	Vector2 lastPoint = pos + (Vector2::fromAngle(-interval) * radius);
	for (float i = 0.0f; i < BL_PI_F * 2.0f; i += interval) {
		Vector2 newPoint = pos + Vector2::fromAngle(i) * radius;
		drawLine(lastPoint, newPoint, width);
		lastPoint = newPoint;
	}
}

// TODO
void Bloodlust::fillCircle(const Vector2&, float) {}

void Bloodlust::drawTexture(const Texture* texture, const SDL_FRect& rect) {
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
) {
	app->renderState.internalShaders[currentLayer].texture->pushTexture(
		texture,
		{rect.x * drawScale.x, rect.y * drawScale.y},
		{rect.w * drawScale.x, rect.h * drawScale.y},
		currentColour,
		texClip,
		getClipRectIndex()
	);
}

void Bloodlust::drawTexture(const Texture* texture, const Vector2& pos, const Vector2& size) {
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
) {
	app->renderState.internalShaders[currentLayer].texture->pushTexture(
		texture,
		pos * drawScale,
		size * drawScale,
		currentColour,
		texClip,
		getClipRectIndex()
	);
}

void Bloodlust::drawText(TTF_Text* textObject, const Vector2& pos, float size) {
	if (size > 0.0f) {
		app->renderState.internalShaders[currentLayer].text->pushText(
			textObject,
			pos * drawScale,
			size * drawScale.y,
			currentColour,
			getClipRectIndex()
		);
	}
}

void Bloodlust::setPixel(const Vector2& pos) {
	app->renderState.internalShaders[currentLayer].rectFill->pushRect(
		{pos.x * drawScale.x, pos.y * drawScale.y, drawScale.x, drawScale.y},
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
