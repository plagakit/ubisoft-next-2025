#include "pch.h"
#include "renderer.h"

#include "core/debug/logger.h"
#include "graphics/texture/texture.h"
#include <App/App.h>
#include <App/SimpleSprite.h>




Renderer::Renderer(ResourceManager& resourceManager) :
	m_resourceManager(resourceManager)
{
	m_defaultFontHandle = m_resourceManager.Load<Font>("HELVETICA_18");
	m_defaultGlutFont = m_resourceManager.Get<Font>(m_defaultFontHandle).GetGLUTFont();
}

void Renderer::DrawTextLine(float x, float y, const char* text, Color col)
{
	App::Print(x, y, text, col.r, col.g, col.b, m_defaultGlutFont);
}

void Renderer::DrawTextLine(float x, float y, const char* text, Color col, RID fontHandle)
{
	void* glutFont = m_resourceManager.Get<Font>(fontHandle).GetGLUTFont();
	App::Print(x, y, text, col.r, col.g, col.b, glutFont);
}

void Renderer::DrawLine(float x0, float y0, float x1, float y1, Color col)
{
	App::DrawLine(x0, y0, x1, y1, col.r, col.g, col.b);
}

void Renderer::DrawRect(float x0, float y0, float x1, float y1, Color col)
{
	App::DrawLine(x0, y0, x1, y0, col.r, col.g, col.b);
	App::DrawLine(x0, y1, x1, y0, col.r, col.g, col.b);
	App::DrawLine(x0, y0, x0, y1, col.r, col.g, col.b);
	App::DrawLine(x1, y0, x1, y1, col.r, col.g, col.b);
}

void Renderer::DrawFilledRect(float x0, float y0, float x1, float y1, Color col)
{
	// TODO: assert y0 < y1
	for (float y = y0; y <= y1; y++)
		App::DrawLine(x0, y, x1, y, col.r, col.g, col.b);
}

void Renderer::DrawTexture(float x, float y, RID textureHandle)
{
	CSimpleSprite& spr = m_resourceManager.Get<Texture>(textureHandle).Get();
	spr.SetPosition(x, y);
	spr.Draw();
}
