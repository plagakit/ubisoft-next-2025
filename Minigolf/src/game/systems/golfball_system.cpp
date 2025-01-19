#include "golfball_system.h"

#include "game/components/golfball.h"
#include "game/components/fake_3d.h"
#include "game/components/obstacle.h"

GolfballSystem::GolfballSystem(EntityManager& registry, Renderer& renderer, ResourceManager& resourceMgr, Input& input) :
	System(registry),
	m_renderer(renderer),
	m_resourceMgr(resourceMgr),
	m_input(input)
{
	auto [colRID, col] = m_resourceMgr.LoadAndGet<CircleCollider>("golfball_collider");
	col.radius = 0.5f;
	m_ballCollider = colRID;

	m_ballPNG = m_resourceMgr.Load<Texture>("res/sprites/golfball.png");
	m_hitHardWAV = m_resourceMgr.Load<Audio>("res/audio/hit_ball_hard.wav");
	m_hitSoftWAV = m_resourceMgr.Load<Audio>("res/audio/hit_ball_soft.wav");
	m_ballCollideWAV = m_resourceMgr.Load<Audio>("res/audio/ball_collide.wav");
}

GolfballSystem::~GolfballSystem()
{
	m_resourceMgr.Unload(m_ballPNG);
}

void GolfballSystem::RegisterAllRequiredComponents(size_t n)
{
	m_registry.RegisterComponentType<Golfball>(n);
}

Entity GolfballSystem::CreateGolfball()
{
	Entity ball = m_registry.CreateEntity();

	m_registry.Add<Transform2D>(ball, {});
	m_registry.Add<Transform3D>(ball, {});
	m_registry.Add<Fake3D>(ball, { 0.5f });

	m_registry.Add<Physics2D>(ball, { m_ballCollider });

	m_registry.Add<Sprite>(ball, { m_ballPNG });
	m_registry.Add<Golfball>(ball, {});
	return ball;
}

void GolfballSystem::Update(float dt)
{
	UpdateDrag();

	for (auto [id, g, tf] : m_registry.AllWith<Golfball, Transform2D>())
	{
		// Hit
		if (m_justReleased)
		{
			float str = Math::Lerp(STR_MIN, STR_MAX, m_strength);
			tf.velocity += -m_releaseDir * str;
			float newSpeed = tf.velocity.Length();
		}

		// Friction
		float speed = tf.velocity.Length();
		Vec2 dir = tf.velocity / speed;
		if (speed > EPSILON)
		{
			tf.velocity -= tf.velocity * FRICTION * dt;
		}
		
		if (speed > MAX_SPEED)
		{
			tf.velocity = Math::Lerp(tf.velocity, dir * MAX_SPEED, 0.5f);
		}
	}
}

void GolfballSystem::Render()
{
	// Draw arrow
	if (m_isDragging)
	{
		Color col;
		if (m_strength <= 0.5f)
			col = Math::Lerp(Color::RED, Color::YELLOW, m_strength * 2.0f);
		else
			col = Math::Lerp(Color::YELLOW, Color::GREEN, (m_strength - 0.5f) * 2.0f);

		Vec2 dir = (m_dragEnd - m_dragStart).Normalized();
		Vec2 point1 = dir.Rotated(-35.0f * DEG2RAD) * 20.0f;
		Vec2 point2 = dir.Rotated(35.0f * DEG2RAD) * 20.0f;

		m_renderer.Draw2DLine(m_dragStart, m_dragEnd, col, 5);
		m_renderer.Draw2DLine(m_dragStart, m_dragStart + point1, col, 5);
		m_renderer.Draw2DLine(m_dragStart, m_dragStart + point2, col, 5);
	}

}

void GolfballSystem::OnCollision(Entity e1, Entity e2, CollisionResult2D result)
{
	if (m_registry.Has<Golfball>(e1)
		&& m_registry.Has<Obstacle>(e2))
	{
		const Obstacle& o = m_registry.Get<Obstacle>(e2);
		if (o.type == Obstacle::Type::WALL)
			ReflectBallOffWall(e1, result);
	}
}

void GolfballSystem::UpdateDrag()
{
	m_justReleased = false;
	if (!m_isDragging)
	{
		// Detect start of drag
		if (m_input.IsJustPressed("mouse-left"))
		{
			m_isDragging = true;
			m_dragStart = m_input.GetMousePos();
		}
	}
	// If holding drag
	else if (m_input.IsPressed("mouse-left"))
	{
		m_dragEnd = m_input.GetMousePos();

		float lineLen = (m_dragEnd - m_dragStart).Length();
		m_strength = Math::InvLerp(STR_MIN_LINE_LEN, STR_MAX_LINE_LEN, lineLen);
		m_strength = Math::Clamp(0.0f, 1.0f, m_strength);
	}
	// Released
	else
	{
		m_isDragging = false;
		m_justReleased = true;
		m_releaseDir = (m_dragEnd - m_dragStart).Normalized();
		if (m_strength > 0.5f)	m_resourceMgr.Get<Audio>(m_hitHardWAV).Play();
		else					m_resourceMgr.Get<Audio>(m_hitSoftWAV).Play();
	}
}

void GolfballSystem::ReflectBallOffWall(Entity ball, CollisionResult2D col)
{
	Transform2D& tf = m_registry.Get<Transform2D>(ball);
	Vec2 refAxis = tf.velocity.ProjectOnto(col.contactNormal);
	tf.velocity -= refAxis * 2.0f;
	tf.velocity *= WALL_BOUNCE;

	m_resourceMgr.Get<Audio>(m_ballCollideWAV).Play();
}
