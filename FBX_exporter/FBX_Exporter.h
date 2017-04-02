#pragma once


#ifdef FBX_EXPORTER_EXPORTS
#define FBX_READER_API __declspec(dllexport)
#else
#define FBX_READER_API __declspec(dllimport)
#endif
#include <vector>


struct vect
{
	float x, y, z;
};
FBX_READER_API bool loadFBX(const char * filename, std::vector<vect> vertices);
