#include "pch.h"
#include "tween_manager.h"

void TweenManager::Update(float dt)
{
	for (auto& tween : m_tweens)
		tween->Update(dt);

	auto it = std::remove_if(m_tweens.begin(), m_tweens.end(), [](const auto& t) { return t->IsDone(); });
	m_tweens.erase(it, m_tweens.end());
}
