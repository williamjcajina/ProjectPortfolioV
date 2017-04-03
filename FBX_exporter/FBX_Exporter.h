#pragma once


#ifdef FBX_EXPORTER_EXPORTS
#define FBX_READER_API __declspec(dllexport)
#else
#define FBX_READER_API __declspec(dllimport)
#endif

#include <vector>
#include <unordered_map>

struct Vertex
{

	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};
struct vect
{
	float x, y, z, w;
};
struct JointData
{
	int parentIndex;
	vect rotation;
	vect translation;
	vect scale;

	double matrix[16];
};

struct Point
{
	float pos[3];
};

FBX_READER_API bool loadFBX(const char * filename, std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes, std::vector<JointData> &jef);
//FBX_READER_API void processData(FbxNode* node, std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes);
FBX_READER_API bool init();
FBX_READER_API bool loadScene(const char * filename);


