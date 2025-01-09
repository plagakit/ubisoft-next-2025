#pragma once

#include "core/uid/uid.h"
#include "core/debug/assert.h"

#include <vector>
#include <type_traits>

class ISparseSet
{
public:
	virtual void Remove(UID id) = 0;
};

/**
* 
* In the sparse array, the value at index i represents the index
* in the dense array of the data of UID i. Null entities (N) 
* are kept @ index 0.
* 
*          s[3] = 1
* Sparse [N######]
*            ^
*           /              i's data
*          V                  v
* Dense  [N######] <-> Data [NDDDDDD]
*          ^                  ^
*       d[1] = 3            idx 1
* 
* References:
* https://skypjack.github.io/2020-08-02-ecs-baf-part-9/
* https://habr.com/en/articles/651921/
*/
template <typename T>
class SparseSet : public ISparseSet
{

public:
	SparseSet();
	SparseSet(size_t capacity);

	bool Has(UID id) const;
	T& Get(UID id);
	const T& Get(UID id) const;
	void Insert(UID id, T obj);
	void Remove(UID id) override;

	size_t Size() const;
	size_t Capacity() const;

private:
	// Allow EntityView templates to directly iterate over dense & data arrays
	template<typename... Ts>
	friend class EntityView;

	std::vector<UID> m_sparse;
	std::vector<UID> m_dense;
	std::vector<T> m_data;

	size_t m_size;		// not including invalid UID
	size_t m_capacity;	// including invalid UID

};

template<typename T>
inline SparseSet<T>::SparseSet() :
	SparseSet(0)
{}

template<typename T>
inline SparseSet<T>::SparseSet(size_t capacity) :
	m_capacity(capacity), m_size(0)
{
	m_sparse.resize(capacity + 1, INVALID_UID); // account for invalid UID @ idx 0
	m_dense.resize(capacity + 1, INVALID_UID);
	m_data.resize(capacity + 1); // invalid component will be default constructed
}

template<typename T>
inline bool SparseSet<T>::Has(UID id) const
{
	return id != INVALID_UID
		&& id < m_capacity
		&& m_sparse[id] != INVALID_UID
		&& m_dense[m_sparse[id]] == id;
}

template<typename T>
inline T& SparseSet<T>::Get(UID id)
{
	ASSERT_ERROR(Has(id), "UID %d has no %s (in SparseSet::Get)", id, typeid(T).name());
	return m_data[m_sparse[id]];
}

template<typename T>
inline const T& SparseSet<T>::Get(UID id) const
{
	ASSERT_ERROR(Has(id), "UID %d has no %s (in const SparseSet::Get)", id, typeid(T).name());
	return m_data[m_sparse[id]];
}

template<typename T>
inline void SparseSet<T>::Insert(UID id, T obj)
{
	ASSERT_ERROR(id != INVALID_UID, "Trying to insert invalid UID into sparse set!");

	if (!Has(id))
	{
		if (id >= m_capacity)
		{
			Logger::Warn("SparseSet<%s> hit UID limit, resizing to %d, consider reserving more space", typeid(T).name(), id + 1);

			size_t newCap = static_cast<size_t>(id + 1);
			m_sparse.resize(newCap);
			m_dense.resize(newCap);
			m_data.resize(newCap);
			m_capacity = newCap;
		}

		// insert at last spot, + 1 to jump over invalid UID
		m_sparse[id] = static_cast<UID>(m_size + 1);
		m_dense[m_size + 1] = id;
		m_data[m_size + 1] = obj;
		m_size++;
	}
	else
	{
		ASSERT_ERROR(true, "SparseSet::Insert-ing %s w/ UID %d into when UID already present", typeid(T).name(), id);
	}
}

template<typename T>
inline void SparseSet<T>::Remove(UID id)
{
	if (Has(id))
	{
		// Replace id's spot with last element's spot to preserve contiguity
		// m_sparse[id] --> removed's dense idx
		// m_size --> last elem's dense idx (not size - 1 b/c elems are shifted over by 1 by invalid UID, so last elem is (size + 1) - 1)
		// m_dense[m_size] --> last elem's sparse idx

		m_dense[m_sparse[id]] = m_dense[m_size];
		m_data[m_sparse[id]] = m_data[m_size];
		m_sparse[m_dense[m_size]] = m_sparse[id];
		m_sparse[id] = INVALID_UID;

		m_size--;
	}
}

template<typename T>
inline size_t SparseSet<T>::Size() const
{
	return m_size;
}

template<typename T>
inline size_t SparseSet<T>::Capacity() const
{
	return m_capacity;
}
