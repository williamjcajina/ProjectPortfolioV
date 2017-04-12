#pragma once
#include "D3dclass.h"
#include <DirectXMath.h>
using namespace DirectX;
	
	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 jointIndex;
		DirectX::XMFLOAT4 weights;
		float pad[3];
	/*	DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 binormal;*/
	};
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	/*	DirectX::XMFLOAT2 pad;*/

	};

	struct JointMatrix
	{
	
		DirectX::XMFLOAT4X4 inverseBindPose[40];
		DirectX::XMFLOAT4X4 jointsMatrices[40];
     

	};





