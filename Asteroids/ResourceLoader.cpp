#include "ResourceLoader.h"
#include <Windows.h>

ResourceLoader::ResourceLoader()
{
}

ResourceLoader::~ResourceLoader()
{
	// No need to clean up our data:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms648047(v=vs.85).aspx
	// The pointer returned by LockResource is valid until the module containing
	// the resource is unloaded. It is not necessary to unlock resources because
	// the system automatically deletes them when the process that created them terminates.
}

bool ResourceLoader::LoadResource(int resourceId, Resource *resource)
{
	if (FetchCachedResource(resourceId, resource))
	{
		return true;
	}

	bool loaded = LoadResourceFromSystem(resourceId, resource);
	if (loaded)
	{
		CacheResource(resourceId, resource);
	}

	return loaded;
}


bool ResourceLoader::FetchCachedResource(int resourceId, Resource *resource)
{
	ResourceMap::const_iterator resourceIt = cachedResources_.find(resourceId);
	if (resourceIt == cachedResources_.end())
	{
		resource->data = 0;
		resource->size = 0;
		return false;
	}

	*resource = resourceIt->second;
	return true;
}

void ResourceLoader::CacheResource(int resourceId, Resource *resource)
{
	cachedResources_.insert(std::make_pair(resourceId, *resource));
}

bool ResourceLoader::LoadResourceFromSystem(int resourceId, Resource *resource)
{
	HRSRC resourceInformationBlock = FindResource(NULL,
		MAKEINTRESOURCE(resourceId),
		RT_RCDATA);
	if (resourceInformationBlock == NULL)
		return false;

	HGLOBAL resourceHandle = ::LoadResource(NULL, resourceInformationBlock);
	if (resourceHandle == NULL)
		return false;

	resource->data = LockResource(resourceHandle);
	resource->size = SizeofResource(NULL, resourceInformationBlock);
	return true;
}
