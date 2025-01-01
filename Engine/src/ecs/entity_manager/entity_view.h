#pragma once

#include "core/uid/uid.h"
#include "ecs/entity.h"
#include <vector>
#include <iterator>

struct Component { int x;  };

/**

References:
https://cplusplus.com/reference/iterator/ForwardIterator/
https://www.fluentcpp.com/2018/05/08/std-iterator-deprecated/
https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
*/
template <typename T, typename... Ts>
class EntityManager::EntityView
{
	static_assert(IsComponentV<T>, "Component iterator must use a component as the main type.");
	static_assert(IsComponentV<Ts...>, "Component iterator must use components for the additional types.");
	
public:
	// Iterator tags, needed to be an iterator
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = std::tuple<Entity, T, Ts...>;
	using pointer = value_type*;
	using reference = value_type&;

	EntityView()
	{

	}

	reference operator*() const;// { return *m_ptr; }
	pointer operator->();// { return m_ptr; }

	EntityView& operator++();// { m_ptr++; return *this; }
	EntityView operator++(int);// { EntityView tmp = *this; ++(*this); return tmp; }

	friend bool operator==(const EntityView& a, const EntityView& b);// { return a.m_ptr == b.m_ptr; }
	friend bool operator!=(const EntityView& a, const EntityView& b);// { return a.m_ptr != b.m_ptr; }

private:
	pointer m_ptr;

};