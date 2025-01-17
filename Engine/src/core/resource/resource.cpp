#include "pch.h"
#include "resource.h"

Resource::Resource(const std::string& path) :
    m_rid(INVALID_RID), m_path(path)
{}

RID Resource::GetRID() const
{
    return m_rid;
}

const std::string& Resource::GetPath() const
{
    return m_path;
}
