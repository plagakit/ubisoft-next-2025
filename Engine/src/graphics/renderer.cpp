#include "renderer.h"

#include "core/logger/logger.h"
#include <App/App.h>

void Renderer::Text(float x, float y, const char* text)
{
	App::Print(x, y, text);
}

void Renderer::DrawLine(float x0, float y0, float x1, float y1)
{
	App::DrawLine(x0, y0, x1, y1);
}

void Renderer::DrawLine(float x0, float y0, float x1, float y1, float r, float g, float b)
{
	App::DrawLine(x0, y0, x1, y1, r, g, b);
}