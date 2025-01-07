#pragma once

#include "sparse_set.h"
#include "core/uid/uid.h"
#include "ecs/entity.h"

#include "core/debug/assert.h"

#include <vector>
#include <deque>
#include <unordered_map>
#include <memory>
#include <iterator>
#include <typeindex>
#include <typeinfo>

template<typename... Ts>
class EntityView;

class EntityManager
{
public:
	EntityManager();

	// Entities

	void ReserveEntities(size_t reserveCap);

	Entity CreateEntity();
	void QueueDelete(Entity entity);
	void FlushDeleteQueue();

	bool Exists(Entity id) const;

	size_t Count() const;

	// Components

	template<typename T>
	void RegisterComponentType(size_t reserveCap = 0);

	template<typename T>
	bool IsComponentRegistered() const;

	template<typename T>
	ComponentID GetComponentID() const;

	template<typename T>
	size_t Size() const;

	template<typename T>
	bool Has(Entity id) const;

	template<typename T>
	void Add(Entity id, T value);

	template<typename T>
	void Remove(Entity id);

	// Directly gets an entity's component - slower compared to iterating over an EntityView, but useful in some cases.
	template<typename T>
	T& Get(Entity id);

	template<typename T>
	const T& Get(Entity id) const;

	// Views
	
	EntityView<> All();

	template<typename... Ts>
	EntityView<Ts...> AllWith();

private:
	// Allow all EntityView templates to access EntityManager
	template<typename... Ts>
	friend class EntityView;

	// Entities are implicitly defined by their signatures

	UIDGenerator m_entityGenerator;
	size_t m_count;
	SparseSet<Signature> m_signatures;

	ComponentID m_registeredTypeCount;
	std::unordered_map<std::type_index, ComponentID> m_componentIDs;
	std::unordered_map<std::type_index, std::unique_ptr<ISparseSet>> m_componentsMap;

	std::deque<Entity> m_deletionQueue;
	std::deque<Entity> m_recycleQueue;

	void DeleteEntity(Entity id);

	template<typename T> SparseSet<T>& GetSparseSet();
	template<typename T> const SparseSet<T>& GetSparseSet() const;
	template<typename... Ts> Signature GetSignature() const;
};

// Components Impl

template<typename T>
inline void EntityManager::RegisterComponentType(size_t reserveCap)
{
	const std::type_index& type = typeid(T);
	ASSERT_WARN(!IsComponentRegistered<T>(), "Re-registering component type %s!", type.name());
	
	m_componentIDs.emplace(type, m_registeredTypeCount++);
	m_componentsMap.emplace(type, std::make_unique<SparseSet<T>>(reserveCap));
}

template<typename T>
inline bool EntityManager::IsComponentRegistered() const
{
	return m_componentsMap.find(typeid(T)) != m_componentsMap.end();
	//&& m_componentIDs.find(typeid(T)) != m_componentIDs.end();
}

template<typename T>
inline ComponentID EntityManager::GetComponentID() const
{
	const std::type_index& type = typeid(T);
	ASSERT_ERROR(IsComponentRegistered<T>(), "Component %s not registered during GetComponentID!", type.name());
	return m_componentIDs.at(type);
}

template<typename T>
inline size_t EntityManager::Size() const
{
	return GetSparseSet<T>().Size();
}

template<typename T>
inline bool EntityManager::Has(Entity id) const
{
	if (!m_signatures.Has(id))
		return false;

	const Signature& signature = m_signatures.Get(id);
	ComponentID cid = GetComponentID<T>();
	return signature & (static_cast<Signature>(1) << cid);
}

template<typename T>
inline void EntityManager::Add(Entity id, T value)
{
	const std::type_index& type = typeid(T);
	ASSERT_ERROR(IsComponentRegistered<T>(), "Component %s not registered during Add!", type.name());

	// set IDth bit to 1
	Signature mask = static_cast<Signature>(1) << GetComponentID<T>();
	m_signatures.Get(id) |= mask;

	GetSparseSet<T>().Insert(id, value);
}

template<typename T>
inline void EntityManager::Remove(Entity id)
{
	const std::type_index& type = typeid(T);
	ASSERT_ERROR(IsComponentRegistered<T>(), "Component %s not registered during Remove!", type.name());

	// set IDth bit to 0
	Signature mask = static_cast<Signature>(1) << GetComponentID<T>();
	m_signatures.Get(id) &= ~mask;

	GetSparseSet<T>().Remove(id);
}

template<typename T>
inline T& EntityManager::Get(Entity id)
{
	const std::type_index& type = typeid(T);
	ASSERT_ERROR(IsComponentRegistered<T>(), "Component %s not registered during Get!", type.name());
	return GetSparseSet<T>().Get(id);
}

template<typename T>
inline const T& EntityManager::Get(Entity id) const
{
	const std::type_index& type = typeid(T);
	ASSERT_ERROR(IsComponentRegistered<T>(), "Component %s not registered during const Get!", type.name());
	return GetSparseSet<T>().Get(id);
}

template<typename ...Ts>
inline EntityView<Ts...> EntityManager::AllWith()
{
	return EntityView<Ts...>(*this, 0);
}

template<typename T>
inline SparseSet<T>& EntityManager::GetSparseSet()
{
	ISparseSet* iptr = m_componentsMap[typeid(T)].get();
	return *static_cast<SparseSet<T>*>(iptr);
}

template<typename T>
inline const SparseSet<T>& EntityManager::GetSparseSet() const
{
	ISparseSet* iptr = m_componentsMap.at(typeid(T)).get();
	return *static_cast<SparseSet<T>* const>(iptr);
}

template<typename ...Ts>
inline Signature EntityManager::GetSignature() const
{
	// Fold expression that sets all bits corresponding to all Ts
	return (... | (static_cast<Signature>(1) << GetComponentID<Ts>()));
}

// Views

#include "entity_view.h"