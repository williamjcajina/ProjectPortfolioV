#pragma once


#ifdef FBX_EXPORTER_EXPORTS
#define FBX_READER_API __declspec(dllexport)
#else
#define FBX_READER_API __declspec(dllimport)
#endif
#include <DirectXMath.h>
#include <vector>
#include <unordered_map>
#include <string>

struct BlendInfo
{
	int index;
	float weight;
	
};
struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 normal;
	std::vector<BlendInfo> blends;

	bool operator==(const Vertex& vert) const
	{
		bool sameBlendingInfo = true;

		bool x, y, z;
		x = false;
		y = false;
		z = false;
		float ep = 0.0001f;
		if (position.x <= vert.position.x + ep && position.x >= vert.position.x-ep)
			x = true;
		
		if (position.y <= vert.position.y + ep && position.y >= vert.position.y - ep)
			y = true;
		if (position.z <= vert.position.z + ep && position.z >= vert.position.z - ep)
			z = true;

		if (normal.x <= vert.normal.x + ep && normal.x >= vert.normal.x - ep)
			x = true;
		else
			x = false;
		if (normal.y <= vert.normal.y + ep && normal.y >= vert.normal.y - ep)
			y = true;
		else
			y = false;
		if (normal.z <= vert.normal.z + ep && normal.z >= vert.normal.z - ep)
			z = true;
		else
			z = false;

		if (UV.x <= vert.UV.x + ep && UV.x >= vert.UV.x - ep)
			x = true;
		else
			x = false;
		if (UV.y <= vert.UV.y + ep && UV.y >= vert.UV.y - ep)
			y = true;
		else
			y = false;
		
		return (x && y &&z);
	}
	
};

struct JointData
{
	DirectX::XMFLOAT4 translation;
	DirectX::XMFLOAT4 rotation;
	DirectX::XMFLOAT4 scale;
	
	int parentIndex;
	std::string name;
	

};



struct KeyFrameData
{
	std::vector<JointData> joints;
	std::string name;
	double time;
};

struct AnimationData
{
	
	std::vector<KeyFrameData> frames;
	double duration;
};

struct Point
{
	DirectX::XMFLOAT3 point;

	std::vector<BlendInfo> blends;

	/*Point()
	{
		blends.reserve(4);
	}*/
};

FBX_READER_API bool loadFBX(const char * filename, std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes, AnimationData &animation);
//FBX_READER_API void processData(FbxNode* node, std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes);
FBX_READER_API bool init();
FBX_READER_API bool loadScene(const char * filename);
FBX_READER_API unsigned int getJointIndex(std::string name);
FBX_READER_API void removeDuplicates(std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes);


