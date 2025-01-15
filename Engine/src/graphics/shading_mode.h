#pragma once

// This enum determines how the renderer draws meshes
// but would serve better as a vertex/fragment shader
// type thing in a larger system
enum ShadingMode
{
	WIREFRAME,
	FILLED,
	SHADED
};