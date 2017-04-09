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
struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 normal;
	
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

//struct Point
//{
//	float pos[3];
//};

FBX_READER_API bool loadFBX(const char * filename, std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes, AnimationData &animation);
//FBX_READER_API void processData(FbxNode* node, std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes);
FBX_READER_API bool init();
FBX_READER_API bool loadScene(const char * filename);



