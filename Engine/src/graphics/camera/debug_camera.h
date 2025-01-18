#pragma once

#include "camera.h"
#include "core/input/input.h"

class DebugCamera : public Camera
{
public:
	DebugCamera(const Input& input);
	void Update(float dt);

private:
	const Input& m_input;

};