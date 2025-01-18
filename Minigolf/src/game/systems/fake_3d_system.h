#pragma once

#include <engine.h>

// Lets objects be rendered in 3D while their behaviour is 2D.
// Maps X/Y 2D axis onto X/Z plane in 3D. Ex. objects at (3, 2)
// will be rendered in 3D at (3, fake3d.y, 2).
class Fake3DSystem : public System
{
public:
	void RegisterAllRequiredComponents(size_t n) override;

	void Update(float dt);

};