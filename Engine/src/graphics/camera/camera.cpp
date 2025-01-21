#include "pch.h"
#include "camera.h"

#include "math/math_utils.h"
#include "core/app_settings.h"

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

const Mat4& Camera::GetInverseProjection() const
{
	return m_invProjection;
}

Mat4 Camera::GetView() const
{
	Mat4 m = Mat4::IDENTITY;
	m.Translate(-m_transform.position);
	m.Rotate(m_transform.orientation.Inverse());
	m.Scale({ 1.0f / m_transform.scale.x, 1.0f / m_transform.scale.y, 1.0f / m_transform.scale.z });
	return m;
}

Mat4 Camera::GetInverseView() const
{
	return m_transform.ToMatrix();
}

float Camera::GetFOV() const
{
	return m_FOV;
}

Ray3D Camera::ProjectRay(const Vec2& screenPos) const
{
	float ndcX = screenPos.x * 2.0f / APP_VIRTUAL_WIDTH - 1.0f;
	float ndcY = screenPos.y * 2.0f / APP_VIRTUAL_HEIGHT - 1.0f;
	Vec4 clipSpaceNear = Vec4(ndcX, ndcY, 0.0f, 1.0f);
	Vec4 clipSpaceFar = Vec4(ndcX, ndcY, 1.0f, 1.0f);

	Vec3 worldSpaceNear = Vec3::FromHomogenous(m_invProjection * clipSpaceNear);
	Vec3 worldSpaceFar = Vec3::FromHomogenous(m_invProjection * clipSpaceFar);

	// TODO: change this to a matrix multiplication
	worldSpaceNear = m_transform.position + m_transform.orientation * worldSpaceNear;
	worldSpaceFar = m_transform.position + m_transform.orientation * worldSpaceFar;

	Ray3D ray;
	ray.origin = worldSpaceNear;
	ray.direction = (worldSpaceFar - worldSpaceNear).Normalized();
	return ray;
}

Frustum Camera::CreateFrustum() const
{
	// TODO

	// https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
	
	// https://gamedev.stackexchange.com/questions/29999/how-do-i-create-a-bounding-frustum-from-a-view-projection-matrix

	//Vec4 ndcCorners[8] = {
	//	{ -1.0f, -1.0f, -1.0f, 1.0f }, // near bot left		0
	//	{  1.0f, -1.0f, -1.0f, 1.0f }, // near bot right	1
	//	{ -1.0f,  1.0f, -1.0f, 1.0f }, // near top left		2
	//	{  1.0f,  1.0f, -1.0f, 1.0f }, // near top right	3
	//	{ -1.0f, -1.0f,  1.0f, 1.0f }, // far bot left		4
	//	{  1.0f, -1.0f,  1.0f, 1.0f }, // far bot right		5
	//	{ -1.0f,  1.0f,  1.0f, 1.0f }, // far top left		6
	//	{  1.0f,  1.0f,  1.0f, 1.0f }  // far top right		7
	//};

	//// Transform NDC to world space
	//Vec3 world[8];
	//for (int i = 0; i < 8; ++i)
	//{
	//	world[i] = Vec3::FromHomogenous(m_invProjection * ndcCorners[i]);
	//	world[i] = m_transform.position + m_transform.orientation * world[i];
	//}
	//	

	//Vec3 up = (world[2] - world[0]).Normalized();
	//Vec3 right = (world[1] - world[0]).Normalized();
	////Vec3 nearToFarLeft = (world[4] - world[0]).Normalized();
	////Vec3 nearToFarRight = (world[5] - world[1]).Normalized();
	////Vec3 nearToFarTop = (world[6] - world[2]).Normalized();
	////Vec3 nearToFarBot = ()

	//Frustum frustum;
	////frustum.near = { world[0], up.Cross(right) };
	////frustum.far = { world[4], -frustum.near.normal };

	////frustum.left = { world[0], nearToFarLeft.Cross(up) };
	////frustum.right = { world[1], nearToFarRight.Cross(up) };

	//frustum.near = { world[0], (world[0] - world[4]).Normalized() };
	//frustum.far = { world[4], -frustum.near.normal };
	//frustum.left = { world[0], frustum.far.normal.Cross(world[2] - world[0]).Normalized() };
	//frustum.right = { world[1], (world[3] - world[1]).Cross(world[5] - world[1]).Normalized() };
	//frustum.top = { world[2], (world[6] - world[2]).Cross(world[3] - world[2]).Normalized() };
	//frustum.bottom = { world[0], (world[1] - world[0]).Cross(world[5] - world[0]).Normalized() };

	return {};
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
	m_projection[14] = -NEAR * FAR / (FAR - NEAR);

	// https://www.wolframalpha.com/input?i2d=true&i=invert+%7B%7BDivide%5B1%2CAt%5D%2C0%2C0%2C0%7D%2C%7B0%2CDivide%5B1%2Ct%5D%2C0%2C0%7D%2C%7B0%2C0%2CDivide%5BF%2C%5C%2840%29F-N%5C%2841%29%5D%2CDivide%5B-NF%2CF-N%5D%7D%2C%7B0%2C0%2C1%2C0%7D%7D
	m_invProjection = Mat4(0.0f);
	m_invProjection[0] = ASPECT_RATIO * tanHalfFovy;
	m_invProjection[5] = tanHalfFovy;
	m_invProjection[11] = (1.0f / FAR) - (1.0f / NEAR);
	m_invProjection[14] = 1.0f;
	m_invProjection[15] = 1.0f / NEAR;
}
