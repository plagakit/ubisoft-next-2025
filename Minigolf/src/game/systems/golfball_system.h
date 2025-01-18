#pragma once

#include <engine.h>

// Controls golfballs, which are controlled by the player
class GolfballSystem : public System
{
public:
	GolfballSystem(EntityManager& registry, Renderer& renderer, ResourceManager& resourcemgr, Input& input);
	~GolfballSystem();
	void RegisterAllRequiredComponents(size_t n) override;

	Entity CreatePlayer();

	void Update(float dt);
	void Render();

private:
	Renderer& m_renderer;
	ResourceManager& m_resourceMgr;
	Input& m_input;

	RID m_ballPNG;
	RID m_hitHardWAV;
	RID m_hitSoftWAV;

	// Player controls
	bool m_isDragging;
	bool m_justReleased;
	Vec2 m_dragStart;
	Vec2 m_dragEnd;
	Vec2 m_releaseDir;

	static constexpr float MIN_STRENGTH_VEC_LEN = 20.0f;
	static constexpr float MAX_STRENGTH_VEC_LEN = 300.0f;
	float m_strength;

	void UpdateDrag();
};