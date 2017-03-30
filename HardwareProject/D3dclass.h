#pragma once

#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")

#include "VertexShader.csh"
#include "PixelShader.csh"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include "ShaderStructures.h"
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
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


		ID3D11InputLayout *layout;


		ID3D11DepthStencilView* dsv;
		ID3D11PixelShader *pixelShader;
		ID3D11VertexShader *vertexShader;

		ID3D11DepthStencilState* m_depthDisabledStencilState;
		ID3D11DepthStencilState* m_depthStencilState;
		
		void setSwapChain(HWND window);
		void setView();
		void createInputLayout();
		void setDepthStencilView();
		void Shutdown();
	
};

