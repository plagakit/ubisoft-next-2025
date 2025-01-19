#pragma once

#include <engine.h>

// Controls golfballs, which are controlled by the player
class GolfballSystem : public System
{
public:
	GolfballSystem(EntityManager& registry, Renderer& renderer, ResourceManager& resourcemgr, Input& input);
	~GolfballSystem();
	void RegisterAllRequiredComponents(size_t n) override;

	Entity CreateGolfball();

	void Update(float dt);
	void Render();

	void OnCollision(Entity e1, Entity e2, CollisionResult2D result);

private:
	Renderer& m_renderer;
	ResourceManager& m_resourceMgr;
	Input& m_input;

	RID m_ballCollider;
	RID m_ballPNG;
	RID m_hitHardWAV;
	RID m_hitSoftWAV;
	RID m_ballCollideWAV;

	// Player controls
	static constexpr float FRICTION = 0.3f;
	static constexpr float WALL_BOUNCE = 0.6f;
	static constexpr float MAX_SPEED = 20.0f;
	static constexpr float STR_MIN = 1.0f;
	static constexpr float STR_MAX = 25.0f;
	static constexpr float STR_MIN_LINE_LEN = 20.0f;
	static constexpr float STR_MAX_LINE_LEN = 300.0f;

	bool m_isDragging	= false;
	bool m_justReleased = false;
	Vec2 m_dragStart	= Vec2::ZERO;
	Vec2 m_dragEnd		= Vec2::ZERO;
	Vec2 m_releaseDir	= Vec2::ZERO;
	float m_strength	= 0.0f;
	
	void UpdateDrag();
	void ReflectBallOffWall(Entity ball, CollisionResult2D collision);

};