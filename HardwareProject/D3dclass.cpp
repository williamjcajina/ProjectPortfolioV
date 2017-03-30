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

void D3dclass::setDepthStencilView()
{
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		lround(SCREEN_WIDTH),
		lround(SCREEN_HEIGHT),
		1, 
		1,
		D3D11_BIND_DEPTH_STENCIL
	);

	ID3D11Texture2D* depthStencil;
	
	device->CreateTexture2D(&depthStencilDesc,nullptr,&depthStencil);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	device->CreateDepthStencilView(depthStencil,&depthStencilViewDesc,&dsv);


	

}


void D3dclass::Shutdown()
{

	device->Release();
	context->Release();
	rtv->Release();
	swapChain->Release();
	
}
