#include "pch.h"
#include "camera.h"

#include "math/math_utils.h"

const float Camera::NEAR = 1.0f;
const float Camera::FAR = 1000.0f;
const float Camera::ASPECT_RATIO = (float)APP_VIRTUAL_WIDTH / (float)APP_VIRTUAL_HEIGHT;

Camera::Camera()
{
	m_FOV = 60.0f;
	RecalculateProjection();
}

Transform3D& Camera::GetTransform()
{
	return m_transform;
}

void Camera::SetFOV(float FOV)
{
	m_FOV = FOV;
	RecalculateProjection();
}

const Mat4& Camera::GetProjection() const
{
	return m_projection;
}

Mat4 Camera::GetView() const
{
	Mat4 m;
	m.Translate(-m_transform.position);
	//m.Rotate()
	m.Scale(-m_transform.scale);
	return m;
}

void Camera::RecalculateProjection()
{
	// LHS [0, 1]
	const float tanHalfFovy = tanf(DEG2RAD * m_FOV * 0.5f);
	m_projection = Mat4(0.0f);
	m_projection[0] = 1.0f / (ASPECT_RATIO * tanHalfFovy);
	m_projection[5] = 1.0f / (tanHalfFovy);
	m_projection[10] = FAR / (FAR - NEAR);
	m_projection[11] = 1.0f;
	m_projection[14] = -(FAR * NEAR) / (FAR - NEAR);
}
