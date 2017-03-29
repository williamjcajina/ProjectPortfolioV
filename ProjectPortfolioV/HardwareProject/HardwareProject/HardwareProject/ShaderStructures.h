#pragma once
#include "D3dclass.h"
#include <DirectXMath.h>
using namespace DirectX;
	struct Simple_Vertex
	{
		DirectX::XMFLOAT2 vertex;	// need to scope into "DirectX" namespace or add using declaration to top of file


	};

	struct SEND_TO_VRAM
	{
		XMFLOAT4 rgba;
		XMFLOAT2 offset;
		XMFLOAT2 padding;



	};

	/*SEND_TO_VRAM toShader;*/
	/*SEND_TO_VRAM toShaderGrid;*/

