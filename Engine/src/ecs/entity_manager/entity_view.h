#pragma once

// INCLUDED IN entity_manager.h B/C OF CIRCULAR DEPENDENCIES

/*
* In this file we define 3 specializations of EntityView<Ts...>. Each specialization
* is different, requiring different implementations:
* 
* 1. EntityView<> - iterates over signature sparse set
* 2. EntityView<T> - iterates over component sparse set
* 3. EntityView<T, Ts...> - iterates over component sparse set + signature sparse set for component-checking
* 
* References:
* https://cplusplus.com/reference/iterator/ForwardIterator/
* https://www.fluentcpp.com/2018/05/08/std-iterator-deprecated/
* https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
*/

/**
The empty EntityView specialization allows you to iterate over all 
entities, returning every Entity UID in no particular order.
*/
template <>
class EntityView<>
{
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = Entity;
	using pointer = Entity*;
	using reference = Entity&;

	EntityView(EntityManager& em, size_t denseIdx) :
		m_em(em), m_denseIdx(denseIdx)
	{}

	reference operator*() const
	{
		return m_em.m_signatures.m_dense[m_denseIdx];
	}

	EntityView& operator++() { m_denseIdx++; return *this; }
	EntityView operator++(int) { EntityView tmp = *this; ++(*this); return tmp; }

	friend bool operator==(const EntityView& a, const EntityView& b) { return a.m_denseIdx == b.m_denseIdx; }
	friend bool operator!=(const EntityView& a, const EntityView& b) { return a.m_denseIdx != b.m_denseIdx; }

	EntityView begin()
	{
		// idx 1 b/c idx 0 is null entity (invalid UID)
		return EntityView(m_em, 1);
	}

	EntityView end()
	{
		// + 1 for null entity
		size_t lastEntity = m_em.m_signatures.Size() + 1;
		return EntityView(m_em, lastEntity);
	}

private:
	EntityManager& m_em;
	size_t m_denseIdx;
};

/**


*/
template <typename T, typename... Ts>
class EntityView<T, Ts...>
{
public:
	// Iterator tags, needed to be an iterator
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = std::tuple<Entity&, T&, Ts&...>;
	using pointer = value_type*;
	using reference = value_type&;

	EntityView(EntityManager& em, size_t denseIdx) :
		m_em(em), 
		m_denseIdx(denseIdx), 
		m_mainComponentSS(em.GetSparseSet<T>()),
		m_entityArr(em.GetSparseSet<T>().m_dense),
		m_componentArrs(em.GetSparseSet<T>().m_data, em.GetSparseSet<Ts>().m_data...)
	{}

	value_type operator*() const
	{
		return std::tie(
			m_entityArr[m_denseIdx],
			m_mainComponentSS.m_data[m_denseIdx]
		);
	}

	EntityView& operator++() { m_denseIdx++; return *this; }
	EntityView operator++(int) { EntityView tmp = *this; ++(*this); return tmp; }

	friend bool operator==(const EntityView& a, const EntityView& b) { return a.m_denseIdx == b.m_denseIdx; }
	friend bool operator!=(const EntityView& a, const EntityView& b) { return a.m_denseIdx != b.m_denseIdx; }

	EntityView begin()
	{
		// idx 1 b/c idx 0 is null entity (invalid UID)
		return EntityView(m_em, 1);
	}

	EntityView end()
	{
		// + 1 for null entity
		size_t lastEntity = m_mainComponentSS.Size() + 1;
		return EntityView(m_em, lastEntity);
	}

private:
	EntityManager& m_em;
	std::vector<UID>& m_entityArr;
	std::tuple<std::vector<T>&, std::vector<Ts>&...> m_componentArrs;

	SparseSet<T>& m_mainComponentSS;
	size_t m_denseIdx;

};