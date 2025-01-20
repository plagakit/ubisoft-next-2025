#include "obstacle_system.h"

#include "game/components/obstacle.h"
#include "game/components/golfball.h"

ObstacleSystem::ObstacleSystem(EntityManager& registry) :
	System(registry)
{
}

void ObstacleSystem::RegisterAllRequiredComponents(size_t n)
{
	m_registry.RegisterComponentType<Obstacle>(n);
}

void ObstacleSystem::OnTrigger(Entity trigger, Entity e, CollisionResult2D col)
{
	//Logger::Info("OnTrigger inside OS %d %d", trigger, e);
	if (m_registry.Has<Obstacle>(trigger) && m_registry.Has<Golfball>(e))
	{
		Obstacle& o = m_registry.Get<Obstacle>(trigger);
		Golfball& g = m_registry.Get<Golfball>(e);

		if (g.state == Golfball::State::IN_PLAY)
		{

			if (o.obstacle == Obstacle::Type::LIP_OUT)
				ApplyLipoutForce(trigger, e);
		}
	}
}

void ObstacleSystem::ApplyLipoutForce(Entity lip, Entity ball)
{
	// A heuristic for applying the force that makes the ball 
	// curve around the edge of the hole but not go in. I'm 
	// doing this by applying a small centripetal force to make
	// it curve, and then a centrifugal force to push it out.
	// Reference: https://www.youtube.com/watch?v=Ni5MQmQ-0Rk

	const Transform2D& lipTF = m_registry.Get<Transform2D>(lip);
	Transform2D& ballTF = m_registry.Get<Transform2D>(ball);

	Vec2 dir = (lipTF.position - ballTF.position).Normalized();
	Vec2 centripetalForce = dir; //Vec2(dir.y, -dir.x);
	ballTF.velocity += centripetalForce * CENTRIPETAL_MULTIPLIER;
}
