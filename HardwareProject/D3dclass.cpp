#include "D3dclass.h"



D3dclass::D3dclass()
{
}


D3dclass::~D3dclass()
{
}

void D3dclass::setSwapChain(HWND window)
{
	DXGI_SWAP_CHAIN_DESC dscd;
	dscd.BufferCount = 1;
	dscd.BufferDesc.Width = SCREEN_WIDTH;
	dscd.BufferDesc.Height = SCREEN_HEIGHT;
	dscd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dscd.BufferDesc.RefreshRate.Numerator = 0;
	dscd.BufferDesc.RefreshRate.Denominator = 1;
	dscd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dscd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	dscd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
	dscd.OutputWindow = window;
	dscd.SampleDesc.Count = 1;
	dscd.SampleDesc.Quality = 0;
	dscd.Windowed = TRUE;
	dscd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	dscd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, D3D11_SDK_VERSION, &dscd, &swapChain, &device, NULL, &context);
}

void D3dclass::setView()
{
	ID3D11Texture2D* pBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(void**)&pBuffer);

	viewPort.Width = SCREEN_WIDTH;
	viewPort.Height = SCREEN_HEIGHT;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	device->CreateRenderTargetView(pBuffer, NULL, &rtv);
	context->RSSetViewports(1, &viewPort);
	context->OMSetRenderTargets(1, &rtv, 0);
	pBuffer->Release();
}


void D3dclass::createInputLayout()
{
	HRESULT hr;
	hr =device->CreateVertexShader(VertexShader, sizeof(VertexShader), NULL, &vertexShader);
	hr =device->CreatePixelShader(PixelShader, sizeof(PixelShader), NULL, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	
	 hr =device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), &VertexShader, sizeof(VertexShader), &layout);
	
}

void D3dclass::setDepthStuff()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	
	depthBufferDesc.Width = SCREEN_WIDTH;
	depthBufferDesc.Height = SCREEN_HEIGHT;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	HRESULT result = device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);


	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	result = device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	context->OMSetDepthStencilState(m_depthStencilState, 1);
	
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));


	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	
	device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	context->OMSetRenderTargets(1, &rtv, m_depthStencilView);
	
	
	
	
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	
	device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	context->RSSetState(m_rasterState);
}


void D3dclass::Shutdown()
{

	device->Release();
	context->Release();
	rtv->Release();
	swapChain->Release();
	
}
