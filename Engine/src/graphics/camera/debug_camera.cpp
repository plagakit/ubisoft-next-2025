#include "pch.h"
#include "debug_camera.h"

DebugCamera::DebugCamera(const Input& input) :
	m_input(input)
{}

void DebugCamera::Update(float dt)
{
	Vec3 move = 
		  Vec3::FORWARD * m_input.GetAxis("down", "up")
		+ Vec3::RIGHT	* m_input.GetAxis("left", "right")
		+ Vec3::UP		* m_input.GetAxis("Q", "E");

	move = m_transform.orientation * move;
	m_transform.position += move * dt * 5.0f;

	float yawChange = m_input.GetAxis("J", "L") * 2.0f * dt;
	float pitchChange = m_input.GetAxis("K", "I") * -2.0f * dt;
	m_transform.orientation = Quat::FromEulerAngles(0, yawChange, 0) * m_transform.orientation;
	m_transform.orientation *= Quat::FromEulerAngles(0, 0, pitchChange);
}
