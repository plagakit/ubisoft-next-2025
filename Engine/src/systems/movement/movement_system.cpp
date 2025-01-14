#include "pch.h"
#include "movement_system.h"

#include "components/2d/transform_2d.h"
#include "components/3d/transform_3d.h"

void MovementSystem::Update(float dt)
{
	for (auto [id, tf] : m_registry.AllWith<Transform2D>())
	{
		tf.velocity += tf.acceleration * dt;
		tf.position += tf.velocity * dt;
		tf.rotation += tf.angVelocity * dt;

		if (tf.position.y < 100)
		{
			tf.position.y = 100;
			tf.velocity.y = -tf.velocity.y * 0.6f;
			tf.velocity.x *= 0.5f;
			tf.angVelocity *= 0.5f;
		}
	}

	for (auto [id, tf] : m_registry.AllWith<Transform3D>())
	{
		tf.velocity += tf.acceleration * dt;
		tf.position += tf.velocity * dt;

		tf.orientation *= tf.angVelocity * dt;
	}
}
