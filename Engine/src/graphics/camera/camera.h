#pragma once

#include "graphics/renderer/renderer.h"
#include "components/3d/transform_3d.h"
#include "math/matrix/mat4.h"
#include "core/app_settings.h"

class Camera
{
public:
	static const float NEAR;
	static const float FAR;
	static const float ASPECT_RATIO;

	Camera();

	Transform3D& GetTransform();
	void SetFOV(float FOV);

	// LHS coord system, clip space is [0, 1]
	const Mat4& GetProjection() const;
	Mat4 GetView() const;

private:
	Transform3D m_transform;
	float m_FOV;
	Mat4 m_projection;

	void RecalculateProjection();

};