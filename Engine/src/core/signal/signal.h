#pragma once

#include "callback.h"
#include <vector>

/*
An implementation of the Observer design pattern. Maintains a list
of callbacks and calls each of them when emitted. Each callback can
take in variable arguments, but return nothing (the type signature
should be "void fn(...)").
*/
template<typename... Args>
class Signal
{
public:
	template<typename T, void (T::*F)(Args...)>
	void Connect(T* instance)
	{
		auto callback = Callback<Args...>();
		callback.Bind<T, F>(instance);
		m_connections.push_back(callback);
	}

	template<typename T>
	void Disconnect(T* instance)
	{
		auto match = [](Callback<Args...>& cb) { return cb.IsInstance<T>(instance); };
		auto removeItr = std::remove_if(m_connections.begin(), m_connections.end(), match);
		m_connections.erase(removeItr, m_connections.end());
	}

	void Emit(Args... args) const
	{
		for (auto& callback : m_connections)
			callback(args...);
	}

	void Clear()
	{
		m_connections.clear();
	}

	size_t ConnectionCount() const
	{
		return m_connections.size();
	}


private:
	std::vector<Callback<void, Args...>> m_connections;

};