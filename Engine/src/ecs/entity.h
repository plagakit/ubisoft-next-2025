#pragma once

#include "core/uid/uid.h"
#include <bitset>
#include <type_traits>

using Entity = UID;
constexpr Entity NULL_ENTITY = INVALID_UID;

using ComponentID = unsigned char;
using Signature = uint64_t;
constexpr ComponentID MAX_COMPONENT_TYPES = sizeof(Signature) * 8;

//template <typename T>
//struct IsComponent
//{
//	static constexpr bool value = std::is_pod<T>::value;
//};
//
//template <typename T>
//constexpr bool IsComponentV = IsComponent<T>::value;