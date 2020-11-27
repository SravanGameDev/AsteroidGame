#ifndef RESOURCELOADER_H_INCLUDED
#define RESOURCELOADER_H_INCLUDED

#include <map>

class ResourceLoader
{
public:
	ResourceLoader();
	~ResourceLoader();

	struct Resource
	{
		void *data;
		uint32_t size;
	};

	bool LoadResource(int resourceId, Resource *resource);

private:
	ResourceLoader(const ResourceLoader &);
	void operator=(const ResourceLoader &);

	typedef std::map<int, Resource> ResourceMap;

	bool FetchCachedResource(int resourceId, Resource *resource);
	void CacheResource(int resourceId, Resource *resource);

	bool LoadResourceFromSystem(int resourceId, Resource *resource);

	ResourceMap cachedResources_;

};

#endif // RESOURCELOADER_H_INCLUDED
