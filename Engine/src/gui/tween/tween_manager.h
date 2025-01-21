#pragma once

#include "gui/tween/tween.h"
#include <vector>
#include <memory>

class TweenManager
{
public:
	void Update(float dt);

	template<typename T>
	Tween<T>& CreateTween(const T& start, const T& end, T& value, float duration, EasingMode mode, EasingType type);

	template<typename T>
	Tween<T>& CreateTween(const T& start, const T& end, T& value, float duration, EasingMode mode, EasingType type, Callback<void> callback);

private:
	std::vector<std::unique_ptr<ITween>> m_tweens;

};

template<typename T>
inline Tween<T>& TweenManager::CreateTween(const T& start, const T& end, T& value, float duration, EasingMode mode, EasingType type)
{
	// https://stackoverflow.com/questions/29922666/is-value-returned-by-stdunique-ptrget-valid-after-moving-unique-ptr

	auto ptr = std::make_unique<Tween<T>>(start, end, value, duration, mode, type);
	Tween<T>& ref = *ptr.get();
	m_tweens.emplace_back(std::move(ptr));
	return ref;
}

template<typename T>
inline Tween<T>& TweenManager::CreateTween(const T& start, const T& end, T& value, float duration, EasingMode mode, EasingType type, Callback<void> callback)
{
	// https://stackoverflow.com/questions/29922666/is-value-returned-by-stdunique-ptrget-valid-after-moving-unique-ptr

	auto ptr = std::make_unique<Tween<T>>(start, end, value, duration, mode, type, callback);
	Tween<T>& ref = *ptr.get();
	m_tweens.emplace_back(std::move(ptr));
	return ref;
}
