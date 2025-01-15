#pragma once

#include "core/resource/resource.h"
#include "graphics/color/color.h"
#include "graphics/shading_mode.h"

struct MeshInstance
{
	RID meshHandle		= INVALID_RID;
	Color color			= Color::WHITE;
	ShadingMode mode	= ShadingMode::WIREFRAME;
};