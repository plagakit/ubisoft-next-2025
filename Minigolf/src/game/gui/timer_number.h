#pragma once

#include <engine.h>

class TimerNumber : public GUIObject
{
public:
	TimerNumber(ResourceManager& resourceMgr, RID texNumbers, unsigned int digitPrecision);

	void SetTime(float time);

protected:
	void UpdateInternal(float dt) override;
	void RenderInternal(Renderer& renderer, Vec2 origin, Vec2 scale) override;

	ResourceManager& m_resourceMgr;
	RID m_texNumbers;
	unsigned int m_digitPrecision;

	float m_texWidth;
	float m_texHeight;

	static constexpr float SCALE = 0.3f;
	float m_time;

};