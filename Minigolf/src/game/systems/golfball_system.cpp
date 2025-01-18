#include "golfball_system.h"

#include "game/components/golfball.h"

GolfballSystem::GolfballSystem(EntityManager& registry, Renderer& renderer, ResourceManager& resourceMgr, Input& input) :
	System(registry),
	m_renderer(renderer),
	m_resourceMgr(resourceMgr),
	m_input(input),
	m_isDragging(false)
{
	m_ballPNG = m_resourceMgr.Load<Texture>("res/sprites/golfball.png");
	m_hitHardWAV = m_resourceMgr.Load<Audio>("res/audio/hit_ball_hard.wav");
	m_hitSoftWAV = m_resourceMgr.Load<Audio>("res/audio/hit_ball_soft.wav");
}

GolfballSystem::~GolfballSystem()
{
	m_resourceMgr.Unload(m_ballPNG);
}

void GolfballSystem::RegisterAllRequiredComponents(size_t n)
{
	m_registry.RegisterComponentType<Golfball>(n);
}

Entity GolfballSystem::CreatePlayer()
{
	Entity ball = m_registry.CreateEntity();


	Transform2D tf; 

	Sprite s = { m_ballPNG };

	Golfball g;

	m_registry.Add<Transform2D>(ball, tf);
	m_registry.Add<Transform3D>(ball, {});
	m_registry.Add<Sprite>(ball, s);
	m_registry.Add<Golfball>(ball, g);
	return ball;
}

void GolfballSystem::Update(float dt)
{
	UpdateDrag();

	for (auto [id, g, tf] : m_registry.AllWith<Golfball, Transform3D>())
	{
		if (m_justReleased)
		{
			Vec2 release = m_releaseDir * m_strength;
			tf.velocity = Vec3(release.x, 0.0f, release.y);
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

		m_renderer.DrawLine(m_dragStart, m_dragEnd, col, 5);
		m_renderer.DrawLine(m_dragStart, m_dragStart + point1, col, 5);
		m_renderer.DrawLine(m_dragStart, m_dragStart + point2, col, 5);
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
			Logger::Info("Start: %s", m_dragStart.ToString().c_str());
		}
	}
	// If holding drag
	else if (m_input.IsPressed("mouse-left"))
	{
		m_dragEnd = m_input.GetMousePos();

		float lineLen = (m_dragEnd - m_dragStart).Length();
		m_strength = Math::InvLerp(MIN_STRENGTH_VEC_LEN, MAX_STRENGTH_VEC_LEN, lineLen);
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
