#include "obstacle_system.h"

#include "game/components/obstacle.h"
#include "game/components/golfball.h"

ObstacleSystem::ObstacleSystem(EntityManager& registry) :
	System(registry)
{
}

void ObstacleSystem::RegisterAllRequiredComponents(size_t n)
{
	m_registry.RegisterComponentType<ObstacleLipOut>(n);
	m_registry.RegisterComponentType<ObstacleMoving>(n);
}

void ObstacleSystem::Update(float dt)
{
	for (auto [id, om, tf] : m_registry.AllWith<ObstacleMoving, Transform2D>())
	{
		Vec2& start = om.toDest ? om.src : om.dest;
		Vec2& end = om.toDest ? om.dest : om.src;
		float t = (tf.position - start).Length() / (end - start).Length();
		//Logger::Info("MOVING OB: %f", t);

		if (t > 1.0f)
		{
			tf.position = end;
			tf.velocity = (start - end).Normalized() * om.speed;
			om.toDest = !om.toDest;
		}
	}
}

void ObstacleSystem::OnTrigger(Entity trigger, Entity e, CollisionResult2D col)
{
	//Logger::Info("OnTrigger inside OS %d %d", trigger, e);
	if (m_registry.Has<Golfball>(e))
	{
		Golfball& g = m_registry.Get<Golfball>(e);
		if (g.state != Golfball::State::IN_PLAY)
			return;

		if (m_registry.Has<ObstacleLipOut>(trigger))
			ApplyLipoutForce(trigger, e);
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
	const ObstacleLipOut& lipData = m_registry.Get<ObstacleLipOut>(lip);
	Transform2D& ballTF = m_registry.Get<Transform2D>(ball);

	Vec2 dir = (lipTF.position - ballTF.position).Normalized();
	Vec2 centripetalForce = dir; //Vec2(dir.y, -dir.x);
	ballTF.velocity += centripetalForce * lipData.centripetalMultiplier;
}
