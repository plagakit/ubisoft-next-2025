#pragma once

#include "core/resource/resource.h"
#include "graphics/color/color.h"

struct MeshInstance
{
	enum Mode
	{
		WIREFRAME,
		FILLED
	};

	RID meshHandle	= INVALID_RID;
	// RID m_textureHandle; - if i implemented textures on meshes
	Color color		= Color::BLUE;
	Mode mode		= Mode::WIREFRAME;
};