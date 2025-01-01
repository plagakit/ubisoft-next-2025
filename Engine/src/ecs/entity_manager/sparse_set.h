#pragma once

#include "ecs/entity.h"
#include "core/debug/assert.h"

#include <vector>
#include <type_traits>

class ISparseSet
{};

/**
* 
* In the sparse array, the value E at index N represents the index
* in the dense array of the data of Entity N.
* 
* Sparse [#######]
*          ^   |
*          |   v
* Dense  [#######] <-> Data [DDDDDDD]
* 
* The sparse set's type must be a plain old data type (for cache-friendly iteration),
* otherwise a compile error will be thrown. Since ECS components should all be PODs,
* this might be rare.
* 
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
	void Remove(UID id);

	const std::vector<size_t>& GetSparse() const { return m_sparse; }
	const std::vector<size_t>& GetDense() const { return m_dense; }

	size_t Size() const;
	size_t Capacity() const;

private:
	std::vector<size_t> m_sparse;
	std::vector<size_t> m_dense;
	std::vector<T> m_data;

	size_t m_capacity;
	size_t m_size;

};

template<typename T>
inline SparseSet<T>::SparseSet() :
	m_capacity(0), m_size(0)
{}

template<typename T>
inline SparseSet<T>::SparseSet(size_t capacity) :
	m_capacity(capacity), m_size(0)
{
	m_sparse.reserve(capacity);
	m_dense.reserve(capacity);
	m_data.reserve(capacity);
}

template<typename T>
inline bool SparseSet<T>::Has(UID id) const
{
	return id < m_capacity && m_sparse[id] < m_size && m_dense[m_sparse[id]] == id;
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
	ASSERT_ERROR(!Has(id), "SparseSet::Insert-ing %s w/ UID %d into when UID already present", id, typeid(T).name());

	if (!Has(id))
	{
		if (id >= m_capacity)
		{
			m_sparse.resize(id + 1);
			m_dense.resize(id + 1);
			m_data.resize(id + 1);
			m_capacity = id + 1;
		}

		m_sparse[id] = m_size;
		m_dense[m_size] = id;
		m_data[m_size] = obj;
		m_size++;
	}
}

template<typename T>
inline void SparseSet<T>::Remove(UID id)
{
	if (Has(id))
	{
		// Swap w/ the last element in the set
		m_dense[m_sparse[id]] = m_dense[m_size - 1];
		m_data[m_sparse[id]] = m_data[m_size - 1];
		m_sparse[m_dense[m_size - 1]] = m_sparse[id];
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
