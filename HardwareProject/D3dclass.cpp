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
	dscd.BufferDesc.Width = 1000;
	dscd.BufferDesc.Height = 1000;
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

	viewPort.Width = 500;
	viewPort.Height = 500;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	device->CreateRenderTargetView(pBuffer, NULL, &rtv);

	pBuffer->Release();
}

void D3dclass::createBuffers()
{
	Simple_Vertex circleVertices[361];

	for (int i = 0; i < VertexNumber; i++)
	{
		float angle = (i*DirectX::XM_PI) / 180.0;
		circleVertices[i].vertex.y = 0.20*sinf(angle);
		circleVertices[i].vertex.x = 0.20*cosf(angle);
		

	}

	D3D11_BUFFER_DESC circleDesc;
	ZeroMemory(&circleDesc, sizeof(D3D11_SUBRESOURCE_DATA));
	circleDesc.Usage = D3D11_USAGE_IMMUTABLE;

	circleDesc.ByteWidth = sizeof(Simple_Vertex) * 361;
	circleDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	circleDesc.CPUAccessFlags = 0;
	circleDesc.MiscFlags = 0;




	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = circleVertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	device->CreateBuffer(&circleDesc, &InitData, &vertexBuffer);
}

void D3dclass::setConstantBuffers()
{
	D3D11_BUFFER_DESC constantDesc;

	constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantDesc.ByteWidth = sizeof(SEND_TO_VRAM);
	constantDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantDesc.MiscFlags = NULL;
	constantDesc.StructureByteStride = sizeof(SEND_TO_VRAM);
	constantDesc.Usage = D3D11_USAGE_DYNAMIC;


	device->CreateBuffer(&constantDesc, NULL, &constantBuffer);
	toShader.offset.x = 0;
	toShader.offset.y = 0;
	toShader.rgba.x = 1;
	toShader.rgba.y = 1;
	toShader.rgba.z = 0;
	toShader.rgba.w = 1;

}

void D3dclass::createInputLayout()
{
	device->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &vertexShader);
	device->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	
	device->CreateInputLayout(inputElementDesc, 1, &Trivial_VS, sizeof(Trivial_VS), &layout);
}

void D3dclass::draw()
{
	context->OMSetRenderTargets(1, &rtv, NULL);
	
	context->RSSetViewports(1, &viewPort);
	
	float color[4];

	color[0] = 0;
	color[1] = 0;
	color[2] = 0.5;
	color[3] = 1;

	context->ClearRenderTargetView(rtv, color);

	D3D11_MAPPED_SUBRESOURCE mapSubRes;
	


	context->VSSetConstantBuffers(0, 1, &constantBuffer);

	UINT stride = sizeof(Simple_Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &gridBuffer, &stride, &offset);
	context->VSSetShader(vertexShader, NULL, NULL);
	context->PSSetShader(pixelShader, NULL, NULL);


	context->IASetInputLayout(layout);


	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	context->Draw(numGridVertices, 0);





	ZeroMemory(&mapSubRes, sizeof(mapSubRes));
	context->Map(constantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubRes);
	memcpy(mapSubRes.pData, &toShader, sizeof(SEND_TO_VRAM));
	context->Unmap(constantBuffer, NULL);

	
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	context->Draw(VertexNumber, 0);

	swapChain->Present(0, 0);
}

void D3dclass::Shutdown()
{

	device->Release();
	context->Release();
	rtv->Release();
	swapChain->Release();
	gridBuffer->Release();
	vertexBuffer->Release();
	constantBuffer->Release();

}
