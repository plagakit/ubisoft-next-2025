#pragma once

#include "core/debug/assert.h"

/*
A class for using both free functions and member functions
(of possibly different types) as generic, type-safe callbacks
which can be stored and invoked.

I'm avoiding std::function because if the object that is passed
into it is deleted (causing a dangling pointer) it might make a
copy of the object on the heap. I would rather pay the risk of
dangling pointers, since I think the object should always be
alive anyway.

Resources:
https://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible
*/
template <typename Ret, typename... Args>
class Callback
{
	using GenericPtr = void*;
	using GenericFunction = Ret(*)(GenericPtr, Args...);

	// Converts a free function into a generic function by wrapping it
	template <Ret(*F)(Args...)>
	static Ret GeneralizeFree(GenericPtr, Args... args)
	{
		return (F)(args...);
	}

	// Converts a member function into a generic function by wrapping it
	template <class C, Ret(C::*F)(Args...)>
	static Ret GeneralizeMember(GenericPtr instance, Args... args)
	{
		// Cast it back into the original type and invoke
		return (static_cast<C*>(instance)->*F)(args...);
	}


public:
	Callback() :
		m_instance(nullptr), m_function(nullptr), m_isMember(false)
	{}

	// Binds a free function to the callback
	template <Ret (*F)(Args...)>
	void Bind()
	{
		m_isMember = false;
		m_instance = nullptr;
		m_function = &GeneralizeFree<F>;
	}

	// Binds a member function to the callback
	template <typename T, Ret (T::* F)(Args...)>
	void Bind(T* instance)
	{
		m_isMember = true;
		m_instance = static_cast<GenericPtr>(instance);
		m_function = &GeneralizeMember<T, F>;
	}

	// Invokes the callback, throws an error if not bound.
	Ret operator()(Args... args) const
	{
		ASSERT_ERROR(!(m_isMember && m_instance == nullptr), "Callback instance has been deleted!");
		ASSERT_ERROR(m_function != nullptr, "Callback not bound!");
		return m_function(m_instance, args...);
	}

	bool IsInstanceNull() const
	{
		return m_instance == nullptr;
	}

	template <typename T>
	bool IsInstance(T* instance) const
	{
		return static_cast<GenericPtr>(instance) == m_instance;
	}


private:
	GenericPtr m_instance;
	GenericFunction m_function;
	bool m_isMember;

};
