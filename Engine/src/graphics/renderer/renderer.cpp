#include "pch.h"
#include "renderer.h"

#include "core/debug/logger.h"
#include <App/App.h>

Font Renderer::defaultFont = { Font::Type::HELVETICA_18 };

void Renderer::DrawTextLine(float x, float y, const char* text, Color col, Font font)
{
	App::Print(x, y, text, col.r, col.g, col.b, font.GetGLUTFont());
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