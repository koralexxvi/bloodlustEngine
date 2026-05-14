#include "../../include/render/textShader.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3_ttf/SDL_ttf.h>

using Bloodlust::TextShader;

TextShader::TextShader(SDL_GPUDevice*, Window*, bool&) {}

void TextShader::onResize(std::uint32_t, std::uint32_t) {}

void TextShader::reserve(std::size_t) {}

void TextShader::pushText(TTF_Text*, const Vector2&, float, const Colour&, std::size_t) {}

void TextShader::clearData() {}

bool TextShader::prepareShader() {
	return true;
}

void TextShader::runCopyPass(SDL_GPUCopyPass*) {}

void TextShader::runRenderPass(SDL_GPUCommandBuffer*, SDL_GPURenderPass*) {}

TextShader::~TextShader() {}
