#pragma once

// INCLUDED AT END OF entity_manager.h B/C OF CIRCULAR DEPENDENCIES w/ TEMPLATE CLASSES

/*
* In my submission from last year, I wrote about how I wanted to make an iterator that would
* cleanly iterate over sparse sets so that the end user can write something like:
* 
* for (auto& [id, comp1, comp2] : AllWith<Comp1, Comp2>()) { ... }
* 
* The above uses C++17's structured bindings, and the 2nd specialization uses 
* constexpr if statements, so C++17 was needed for this.
* 
* In this file we define 2 specializations of EntityView<Ts...>.
* Both require different implementations:
* 1. EntityView<> - iterates over signature sparse set's dense array
* 2. EntityView<T, Ts...> - iterates over component dense array (and possibly check signature)
* 
* References:
* https://cplusplus.com/reference/iterator/ForwardIterator/
* https://www.fluentcpp.com/2018/05/08/std-iterator-deprecated/
* https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
* https://devblogs.microsoft.com/oldnewthing/20200623-00/?p=103901
*/

/**
* The empty EntityView specialization allows you to iterate over all 
* entities, returning every Entity UID in no particular order.
*/
template <>
class EntityView<>
{
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = Entity;

	EntityView(EntityManager& em, size_t denseIdx) :
		m_em(em), m_denseIdx(denseIdx)
	{}

	value_type operator*() const
	{
		return m_em.m_signatures.m_dense[m_denseIdx];
	}

	EntityView& operator++() 
	{ 
		m_denseIdx++; 
		return *this; 
	}

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

	friend bool operator==(const EntityView& a, const EntityView& b) { return a.m_denseIdx == b.m_denseIdx; }
	friend bool operator!=(const EntityView& a, const EntityView& b) { return a.m_denseIdx != b.m_denseIdx; }

private:
	EntityManager& m_em;
	size_t m_denseIdx;
};

/**
* Iterates over all entities with the specified types, with T
* being the main type - the main sparse set that is iterated
* over. To minimize unnecessary checking, make sure that the
* main type is usually smaller than the other types.
*/
template <typename T, typename... Ts>
class EntityView<T, Ts...>
{
public:
	// Iterator tags, needed to be an iterator
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = std::tuple<Entity, T&, Ts&...>;

	EntityView(EntityManager& em, size_t denseIdx) :
		m_em(em), 
		m_denseIdx(denseIdx), 
		m_mainSS(em.GetSparseSet<T>()),
		m_extraSS(em.GetSparseSet<Ts>()...),
		m_componentSet(em.GetSignature<T, Ts...>())
	{}

	value_type operator*() const
	{
		// Makes a tuple get chain (get<0>, get<1>, ...) corresponding to indices of m_extraSS
		// Needs to be a helper because we need to make the get chain at compile time
		return AccessHelper(std::make_index_sequence<sizeof...(Ts)>{});
	}

	EntityView& operator++() 
	{ 
		m_denseIdx = FindNextIndex(m_denseIdx);
		return *this; 
	}

	EntityView begin()
	{
		size_t start = FindNextIndex(0);
		return EntityView(m_em, start);
	}

	EntityView end()
	{
		// + 1 for null entity
		size_t lastEntity = m_mainSS.Size() + 1;
		return EntityView(m_em, lastEntity);
	}

	friend bool operator==(const EntityView& a, const EntityView& b) { return a.m_denseIdx == b.m_denseIdx; }
	friend bool operator!=(const EntityView& a, const EntityView& b) { return a.m_denseIdx != b.m_denseIdx; }

private:
	EntityManager& m_em;

	// cache sparse sets so we dont have to do GetSparseSet<T> every time
	SparseSet<T>& m_mainSS;
	std::tuple<SparseSet<Ts>&...> m_extraSS;
	Signature m_componentSet;

	size_t m_denseIdx;

	// Helper function that uses fold expressions to create the reference tuple
	template<size_t... Is>
	value_type AccessHelper(std::index_sequence<Is...>) const
	{
		// https://devblogs.microsoft.com/oldnewthing/20200623-00/?p=103901

		Entity id = m_mainSS.m_dense[m_denseIdx];
		return {
			id,
			m_mainSS.m_data[m_denseIdx],
			std::get<Is>(m_extraSS).Get(id)...
		};
	}

	size_t FindNextIndex(size_t idx) const
	{
		// If there's no extra types, we just increment next index of component data array
		if constexpr (sizeof...(Ts) == 0)
			return idx + 1;
		// But if there are, we need to make sure next entity has all specified components
		// This leads to a notable optimization: the first type specified in the EntityView
		// template should be the one with the least entities, to minimize these checks
		else
		{
			while (idx < m_mainSS.Size())
			{
				idx++;

				Entity id = m_mainSS.m_dense[idx];
				const Signature& signature = m_em.m_signatures.Get(id);
				if ((m_componentSet & signature) == m_componentSet)
					return idx;
			}
			return m_mainSS.Size() + 1;
		}
	}
};