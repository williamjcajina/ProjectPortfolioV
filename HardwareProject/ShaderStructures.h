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
	
		DirectX::XMFLOAT4X4 jointsMatrices[40];
     

	};


	struct Light
	{
		DirectX::XMFLOAT3 cameraPos;
		float Tex;
		DirectX::XMFLOAT4 ambientColor;
		DirectX::XMFLOAT4 diffuseColor;
		DirectX::XMFLOAT3 lightDirection;
		float pointRadius;
		DirectX::XMFLOAT4 pointColor;
		DirectX::XMFLOAT3 pointLightPosition;
		float specularPower;
		DirectX::XMFLOAT4 specularColor;
		DirectX::XMFLOAT3 spotPos;
		float spotInnerRatio;
		DirectX::XMFLOAT3 conedir;
		float coneRatio;
		DirectX::XMFLOAT4 spotColor;
		float spotOuterRatio;
		float normalMap;
		DirectX::XMFLOAT2 padd;


	};


