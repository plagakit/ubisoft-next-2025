#pragma once

#include "graphics/renderer/renderer.h"
#include "components/3d/transform_3d.h"
#include "math/matrix/mat4.h"
#include "math/vector/vector2.h"
#include "math/shape/ray_3d.h"
#include "math/shape/frustum.h"
#include "core/app_settings.h"

class Camera
{
public:
	static constexpr float NEAR = 1.0f;
	static constexpr float FAR = 1000.0f;
	static constexpr float ASPECT_RATIO = (float)APP_VIRTUAL_WIDTH / (float)APP_VIRTUAL_HEIGHT;
	
	Camera();

	Transform3D& GetTransform();
	void SetFOV(float FOV);

	// LHS coord system, clip space is [0, 1]
	const Mat4& GetProjection() const;
	const Mat4& GetInverseProjection() const;
	Mat4 GetView() const;
	Mat4 GetInverseView() const;
	float GetFOV() const;

	Ray3D ProjectRay(const Vec2& screenPos) const;
	Frustum CreateFrustum() const;

protected:
	Transform3D m_transform;
	float m_FOV;
	Mat4 m_projection;
	Mat4 m_invProjection;

	void RecalculateProjection();

};