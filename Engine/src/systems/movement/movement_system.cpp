#include "pch.h"
#include "movement_system.h"

#include "components/2d/transform_2d.h"
#include "components/3d/transform_3d.h"

void MovementSystem::Update()
{
	for (auto [id, tf] : m_registry.AllWith<Transform2D>())
	{
		tf.velocity += tf.acceleration;
		tf.position += tf.velocity;
		tf.rotation += tf.angVelocity;
	}

	for (auto [id, tf] : m_registry.AllWith<Transform2D>())
	{

	}
}
