#include "pch.h"
#include "entity_manager.h"

EntityManager::EntityManager() :
	m_count(0),
	m_registeredTypeCount(0)
{}

void EntityManager::ReserveEntities(size_t reserveCap)
{
	m_signatures = SparseSet<Signature>(reserveCap);
}

Entity EntityManager::CreateEntity()
{
	Entity id;

	if (!m_recycleQueue.empty())
	{
		id = m_recycleQueue.front();
		m_recycleQueue.pop_front();
	}
	else
	{
		id = static_cast<Entity>(m_entityGenerator.Next());
	}
		
	m_count++;
	m_signatures.Insert(id, static_cast<Signature>(0));

	return id;
}

void EntityManager::QueueDelete(Entity entity)
{
	// If not already queued, queue it
	if (std::find(m_deletionQueue.begin(), m_deletionQueue.end(), entity) == m_deletionQueue.end())
		m_deletionQueue.push_back(entity);
}

void EntityManager::FlushDeleteQueue()
{
	for (Entity e : m_deletionQueue)
		DeleteEntity(e);
	m_deletionQueue.clear();
}

bool EntityManager::Exists(Entity id) const
{
	return m_signatures.Has(id);
}

size_t EntityManager::Count() const
{
	return m_count;
}

EntityView<> EntityManager::All()
{
	return EntityView<>(*this, 0);
}

void EntityManager::DeleteEntity(Entity id)
{
	Signature s = m_signatures.Get(id);

	for (auto& [type, cid] : m_componentIDs)
	{
		// Equivalent to Has() - if has component, remove it
		if (s & (static_cast<Signature>(1) << cid))
			m_componentsMap[type]->Remove(id);
	}

	m_signatures.Remove(id);
	m_recycleQueue.push_back(id);
	m_count--;
}
