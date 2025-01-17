#pragma once

#include "core/uid/uid.h"
#include <string>

// Resource ID
using RID = UID;
constexpr RID INVALID_RID = INVALID_UID;

class Resource
{
public:
	Resource(const std::string& path);
	virtual ~Resource() = 0;

	RID GetRID() const;
	const std::string& GetPath() const;

protected:
	friend class ResourceManager;

	RID m_rid;
	std::string m_path;

};
