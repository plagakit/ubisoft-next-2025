#include "golfball_system.h"

#include "game/components/golfball.h"
#include "game/components/fake_3d.h"
#include "game/components/obstacle.h"
#include "game/course/tile.h"

GolfballSystem::GolfballSystem(EntityManager& registry, Renderer& renderer, ResourceManager& resourceMgr, Input& input, TweenManager& tweenMgr) :
	System(registry),
	m_renderer(renderer),
	m_resourceMgr(resourceMgr),
	m_input(input),
	m_tweenMgr(tweenMgr)
{
	auto [colRID, col] = m_resourceMgr.LoadAndGet<CircleCollider>("golfball_collider");
	col.radius = 0.5f;
	m_ballCollider = colRID;

	m_ballPNG = m_resourceMgr.Load<Texture>("res/sprites/golfball.png");
	m_soundHitHard = m_resourceMgr.Load<Audio>("res/audio/hit_ball_hard.wav");
	m_soundHitSoft = m_resourceMgr.Load<Audio>("res/audio/hit_ball_soft.wav");
	m_soundBallCollide = m_resourceMgr.Load<Audio>("res/audio/ball_collide.wav");
	m_soundSink = m_resourceMgr.Load<Audio>("res/audio/hole.wav");
}

GolfballSystem::~GolfballSystem()
{
	m_resourceMgr.Unload(m_ballPNG);
}

void GolfballSystem::RegisterAllRequiredComponents(size_t n)
{
	m_registry.RegisterComponentType<Golfball>(n);

	// For collisions
	m_registry.RegisterComponentType<Tile>();
	m_registry.RegisterComponentType<ObstacleLipOut>();
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

	for (auto [id, g, tf2D, tf3D] : m_registry.AllWith<Golfball, Transform2D, Transform3D>())
	{
		if (g.state == Golfball::State::IN_PLAY)
		{
			// Hit
			if (m_justReleased)
			{
				float str = Math::Lerp(STR_MIN, STR_MAX, m_strength);
				tf2D.velocity += -m_releaseDir * str;
				float newSpeed = tf2D.velocity.Length();
			}

			// Friction
			float speed = tf2D.velocity.Length();
			Vec2 dir = tf2D.velocity / speed;
			if (speed > EPSILON)
			{
				tf2D.velocity -= tf2D.velocity * FRICTION * dt;
			}

			if (speed > MAX_SPEED)
			{
				tf2D.velocity = Math::Lerp(tf2D.velocity, dir * MAX_SPEED, 0.5f);
			}
		}
	}
}

void GolfballSystem::RenderSpeedParticles()
{
	for (auto [id, g, tf] : m_registry.AllWith<Golfball, Transform2D>())
	{
		float speed = tf.velocity.Length();
	}
}

void GolfballSystem::RenderSwooshParticles(Entity focusBall)
{
	if (focusBall == NULL_ENTITY)
		return;

	const Transform2D& tf = m_registry.Get<Transform2D>(focusBall);
	float speed = tf.velocity.Length();
	
	if (speed > SWOOSH_SPEED_THSHLD)
	{
		Vec2 moveDir = tf.velocity / speed;
		CreateSwooshParticle(moveDir);
	}
}

void GolfballSystem::RenderArrow()
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
	if (m_registry.Has<Golfball>(e1) && m_registry.Has<Tile>(e2))
	{
		Tile tile = m_registry.Get<Tile>(e2);
		if (tile.type == Tile::Type::WALL)
			ReflectBallOffWall(e1, e2, result);
	}
}

void GolfballSystem::OnTrigger(Entity trigger, Entity e, CollisionResult2D result)
{
	if (m_registry.Has<Golfball>(e))
	{
		Golfball& ball = m_registry.Get<Golfball>(e);

		if (m_registry.Has<Tile>(trigger))
		{
			Tile& tile = m_registry.Get<Tile>(trigger);
			if (ball.state == Golfball::State::IN_PLAY && tile.type == Tile::Type::HOLE)
				SinkBall(e, trigger);
		}
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
		if (m_strength > 0.5f)	m_resourceMgr.Get<Audio>(m_soundHitHard).Play();
		else					m_resourceMgr.Get<Audio>(m_soundHitSoft).Play();
	}
}

void GolfballSystem::ReflectBallOffWall(Entity ball, Entity wall, CollisionResult2D col)
{
	const Transform2D& wallTF = m_registry.Get<Transform2D>(wall);
	Transform2D& tf = m_registry.Get<Transform2D>(ball);
	Vec2 refAxis = tf.velocity.ProjectOnto(col.contactNormal);
	tf.velocity -= refAxis * 2.0f;
	tf.velocity *= BOUNCE_ENERGY_LOSS;
	tf.velocity += wallTF.velocity;

	m_resourceMgr.Get<Audio>(m_soundBallCollide).Play();
}

void GolfballSystem::SinkBall(Entity ball, Entity hole)
{
	Golfball& gb = m_registry.Get<Golfball>(ball);
	gb.state = Golfball::State::SINKING;

	Transform2D& tf = m_registry.Get<Transform2D>(ball);
	const Transform2D& holeTf = m_registry.Get<Transform2D>(hole);
	tf.velocity = Vec2::ZERO;

	Transform3D& tf3D = m_registry.Get<Transform3D>(ball);

	m_tweenMgr.CreateTween<Vec2>(tf.position, holeTf.position, tf.position, 0.5f, EasingMode::CUBIC, EasingType::OUT);
	m_tweenMgr.CreateTween<Vec3>(tf3D.scale, Vec3::ZERO, tf3D.scale, 0.5f, EasingMode::CUBIC, EasingType::OUT);
	m_resourceMgr.Get<Audio>(m_soundSink).Play();

	s_SunkBall.Emit(ball);
}

void GolfballSystem::CreateSwooshParticle(Vec2 direction)
{
}
