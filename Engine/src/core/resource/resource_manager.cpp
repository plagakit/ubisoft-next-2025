#include "pch.h"
#include "resource_manager.h"

void ResourceManager::Unload(RID handle)
{
	ASSERT_ERROR(m_resources.find(handle) != m_resources.end(), "Resource w/ handle %d doesn't exist (ResourceManager::Unload)", handle);
	m_resources.erase(handle);
}
