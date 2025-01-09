#include "pch.h"
#include "resource.h"

RID Resource::GetRID() const
{
    return m_rid;
}

const std::string& Resource::GetPath() const
{
    return m_path;
}
