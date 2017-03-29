#pragma once

#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")

#include "Trivial_VS.csh"
#include "Trivial_PS.csh"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include "ShaderStructures.h"
using namespace DirectX;

	class D3dclass
	{
	public:
		D3dclass();
		~D3dclass();


		ID3D11Device *device;
		ID3D11DeviceContext *context;
		ID3D11RenderTargetView *rtv;
		IDXGISwapChain *swapChain;
		D3D11_VIEWPORT viewPort;


		ID3D11Buffer *vertexBuffer;
		ID3D11Buffer *constantBuffer;
		ID3D11InputLayout *layout;


		ID3D11Buffer *gridBuffer;
		unsigned int numGridVertices = 2400;

		Simple_Vertex circleVertices;

		ID3D11PixelShader *pixelShader;
		ID3D11VertexShader *vertexShader;

		SEND_TO_VRAM toShader;
		SEND_TO_VRAM toShaderGrid;
		UINT VertexNumber = 361;

		void setSwapChain(HWND window);

		void setView();
		void createBuffers();
		void setConstantBuffers();
		void createInputLayout();
		void draw();
		void Shutdown();
	
};

