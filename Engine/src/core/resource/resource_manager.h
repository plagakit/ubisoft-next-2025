#pragma once

#include "core/resource/resource.h"
#include "core/debug/assert.h"

#include <unordered_map>
#include <memory>
#include <tuple>

class ResourceManager
{
public:
	// Creates the resource and loads it into the resource map.
	// Uses perfect forwarding to allow user to enter in the resource's
	// constructor's arguments into Load.
	template<typename T, typename... Args>
	RID Load(Args... args);

	// Creates, loads, but then returns a reference
	template<typename T, typename... Args>
	std::tuple<RID, T&> LoadAndGet(Args... args);

	void Unload(RID handle);

	template<typename T>
	T& Get(RID handle);

private:
	UIDGenerator m_ridGenerator;
	std::unordered_map<RID, std::unique_ptr<Resource>> m_resources;

	// Could also use a sparse set for storing resources because the
	// keys are RIDs, maybe faster than unordered_map, would need to
	// be profiled. Would take up a lot more memory though, and references
	// would be invalidated if the container shrinks/grows I think

};

template<typename T, typename... Args>
inline RID ResourceManager::Load(Args... args)
{
	static_assert(std::is_base_of<Resource, T>::value, "Trying to Load a non-Resource!");

	// Check if already loaded
	//for (auto& [rid, res] : m_resources)
	//{
	//	if (res->m_path == path)
	//	{
	//		Logger::Warn("Reloading resource %s!", path);
	//		return rid;
	//	}
	//}

	// If not, load it. Perfect forward the arguments
	std::unique_ptr<Resource> resource = std::make_unique<T>(std::forward<Args>(args)...);
	RID rid = m_ridGenerator.Next();
	resource->m_rid = rid;
	//resource->m_path = path;
	//resource->Load(path);
	
	m_resources.insert({ rid, std::move(resource)});
	return rid;
}

template<typename T, typename... Args>
inline std::tuple<RID, T&> ResourceManager::LoadAndGet(Args... args)
{
	RID rid = Load<T>(std::forward<Args>(args)...);
	return { rid, Get<T>(rid) };
}

template<typename T>
inline T& ResourceManager::Get(RID handle)
{
	static_assert(std::is_base_of<Resource, T>::value, "Trying to Get a non-Resource!");
	ASSERT_ERROR(m_resources.find(handle) != m_resources.end(), "Resource w/ handle %d doesn't exist (ResourceManager::Get)", handle);

	return *static_cast<T*>(m_resources[handle].get());
}
