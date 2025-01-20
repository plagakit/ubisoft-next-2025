#pragma once

#include <engine.h>

/** 
* Controls golfballs, which are controlled by the player.
*/ 
class GolfballSystem : public System
{
public:
	GolfballSystem(EntityManager& registry, Renderer& renderer, ResourceManager& resourcemgr, Input& input, TweenManager& m_tweenMgr);
	~GolfballSystem();
	void RegisterAllRequiredComponents(size_t n) override;

	Entity CreateGolfball();

	void Update(float dt);
	void RenderArrow();
	void RenderSpeedParticles();
	void RenderSwooshParticles(Entity focusBall);

	Signal<Entity> s_SunkBall;

	void OnCollision(Entity e1, Entity e2, CollisionResult2D result);
	void OnTrigger(Entity e1, Entity e2, CollisionResult2D result);

private:
	Renderer& m_renderer;
	ResourceManager& m_resourceMgr;
	Input& m_input;
	TweenManager& m_tweenMgr;

	// Resources
	RID m_ballCollider;
	RID m_ballPNG;

	RID m_soundHitHard;
	RID m_soundHitSoft;
	RID m_soundBallCollide;
	RID m_soundSink;

	// Constants
	static constexpr float FRICTION = 0.3f;
	static constexpr float BOUNCE_ENERGY_LOSS = 0.9f;
	static constexpr float MAX_SPEED = 25.0f;
	static constexpr float STR_MIN = 1.0f;
	static constexpr float STR_MAX = 30.0f;
	static constexpr float STR_MIN_LINE_LEN = 20.0f;
	static constexpr float STR_MAX_LINE_LEN = 400.0f;

	static constexpr float SWOOSH_SPEED_THSHLD = 10.0f;

	// Player controls
	bool m_isDragging	= false;
	bool m_justReleased = false;
	Vec2 m_dragStart	= Vec2::ZERO;
	Vec2 m_dragEnd		= Vec2::ZERO;
	Vec2 m_releaseDir	= Vec2::ZERO;
	float m_strength	= 0.0f;
	
	void UpdateDrag();
	void ReflectBallOffWall(Entity ball, CollisionResult2D collision);
	void SinkBall(Entity ball, Entity hole);

	void CreateSwooshParticle(Vec2 direction);
};