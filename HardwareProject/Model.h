#pragma once

#include "D3dclass.h"
#include "XTime.h"
#include "ShaderStructures.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../FBX_exporter/FBX_Exporter.h"

using namespace std;
class Model 
{


public:


	struct VertexPositionUVNormal
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 uv;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 jointIndex;
		DirectX::XMFLOAT4 weights;
		VertexPositionUVNormal()
		{
			pos = XMFLOAT3(0, 0, 0);
			uv = XMFLOAT3(0, 0, 0);
			normal = XMFLOAT3(0, 0, 0);
			jointIndex = XMFLOAT4(0, 0, 0,0);
			weights = XMFLOAT4(0, 0, 0, 0);
		}
		/*DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 binormal;*/
	};
	struct TempVertex
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct Vector
	{
		float x, y, z;
	};


	
	bool isFBX;
	void loadModel(const char* file_name);
	bool loadModelFBX(const char* file_name);
	unsigned short *indices;
	AnimationData animation;
	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<DirectX::XMFLOAT3> uvs;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<unsigned int> vertexIndexes;
	
	std::vector<unsigned int> normalIndexes;
	std::vector<unsigned int> uvIndexes;
	std::vector<VertexPositionUVNormal> vertexList;
	void CalculateTangentBinormal(TempVertex vertex1, TempVertex vertex2, TempVertex vertex3, Vector &tangent, Vector &binormal);
	void CalculateNormal(Vector tangent, Vector binormal, Vector &normal);
	void CreateVertexList();
	void CalculateVectors();
	unsigned int vertsNumber =0;
	



   Model();

};

