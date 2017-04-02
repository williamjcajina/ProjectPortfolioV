#include "SceneRenderer.h"



SceneRenderer::SceneRenderer()
{
	for (int i = 0; i < 256; i++)
	{
		buttons[i] = false;

	}
	for (int i = 0; i < 2; i++)
	{
		currMousePos[i] = 0;
		preMousePos[i] = 0;

	}
}


SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::createBuffers()
{
	ModelBuffers floor;
	
	static const VertexPositionColor floorVertices[] =
	{
		{ XMFLOAT3(-5, 0.0f, 5), XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(5, 0.0f,  5), XMFLOAT3(0.0f, 1.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-5,  0.0f, -5), XMFLOAT3(0.0f, 0.0f, 1.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(5,  0.0f, -5), XMFLOAT3(1.0f, 1.0f, 1.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) }


	};

	D3D11_SUBRESOURCE_DATA vertexBufferData1 = { 0 };
	vertexBufferData1.pSysMem = floorVertices;
	vertexBufferData1.SysMemPitch = 0;
	vertexBufferData1.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc1(sizeof(floorVertices), D3D11_BIND_VERTEX_BUFFER);
	HRESULT hr = Resources.device->CreateBuffer(&vertexBufferDesc1, &vertexBufferData1, &floor.m_model_vertexBuffer);

	

	static const unsigned short floorIndices[] =
	{
		0,3,2,0,1,3

	};

	floor.m_model_indexCount = ARRAYSIZE(floorIndices);

	D3D11_SUBRESOURCE_DATA indexBufferData1 = { 0 };
	indexBufferData1.pSysMem = floorIndices;
	indexBufferData1.SysMemPitch = 0;
	indexBufferData1.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc1(sizeof(floorIndices), D3D11_BIND_INDEX_BUFFER);
	hr =Resources.device->CreateBuffer(&indexBufferDesc1, &indexBufferData1, &floor.m_model_indexBuffer);


	DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX scaling;

	scaling = DirectX::XMMatrixScaling(1, 1, 1);
	DirectX::XMMATRIX transform = DirectX::XMMatrixMultiply(scaling, identity);
	XMStoreFloat4x4(&floor.worldMatrix, transform);


	Models.push_back(floor);

	Model test;
	test.loadModelFBX("Teddy_Idle.fbx");


	auto creatteBuffers();
	{
		ModelBuffers modelName;

		modelName.name = "helicopter2.obj";
		
		Models.push_back(modelName);
		


		for (int i = 0; i <Models.size(); i++)
		{


			Model model;
			if (Models[i].name == nullptr)
				continue;
			model.loadModel(Models[i].name);
		

			const int  vertsNumber = model.vertexList.size();
			VertexPositionColor* modelVertices = new VertexPositionColor[vertsNumber];
			srand(0);


			for (int i = 0; i < vertsNumber; i++)
			{
				modelVertices[i].pos = model.vertexList[i].pos;
				modelVertices[i].color = model.vertexList[i].uv;
				modelVertices[i].normal = model.vertexList[i].normal;
				/*modelVertices[i].tangent = model.vertexList[i].tangent;
				modelVertices[i].binormal = model.vertexList[i].binormal;*/


			}



			D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
			vertexBufferData.pSysMem = modelVertices;
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionColor)*vertsNumber, D3D11_BIND_VERTEX_BUFFER);
			Resources.device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &Models[i].m_model_vertexBuffer);



			unsigned short* modelIndices = new unsigned short[model.vertexIndexes.size() - 1];
			for (int i = 0; i < model.vertexIndexes.size(); i++)
			{
				modelIndices[i] = i;

			}


			Models[i].m_model_indexCount = model.vertexIndexes.size() - 1;
			D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
			indexBufferData.pSysMem = modelIndices;
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short)*Models[i].m_model_indexCount, D3D11_BIND_INDEX_BUFFER);
			Resources.device->CreateBuffer(&indexBufferDesc, &indexBufferData, &Models[i].m_model_indexBuffer);

			DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX scaling;

				scaling = DirectX::XMMatrixScaling(0.1, 0.1, 0.1);

			/*DirectX::XMMATRIX translate;
			if (i == 3)
				translate = DirectX::XMMatrixTranslation(5.0, 0.0f, 0.0f);
			else
				translate = DirectX::XMMatrixTranslation(i * 40, 0.0f, 0.0f);*/

			DirectX::XMMATRIX transform = DirectX::XMMatrixMultiply(scaling, identity);
			/*transform = DirectX::XMMatrixMultiply(translate, transform);*/

			/*transform = DirectX::XMMatrixTranspose(transform);*/


			XMStoreFloat4x4(&Models[i].worldMatrix, transform);
		
		}
	}

}

void SceneRenderer::createConstantBuffers()
{
	D3D11_BUFFER_DESC matrixBufferDesc;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(ModelViewProjectionConstantBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;


	HRESULT result = Resources.device->CreateBuffer(&matrixBufferDesc, NULL, &m_constantBuffer);

	//CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	//HRESULT hr = Resources.device->CreateBuffer(&constantBufferDesc, nullptr, m_constantBuffer.);

	//int y = 0;


}

void SceneRenderer::setCamera()
{
	
	float aspectRatio = SCREEN_WIDTH/ SCREEN_HEIGHT;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}


	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 100.0f);
	XMMATRIX orientation = DirectX::XMMatrixIdentity();
	

	

	XMStoreFloat4x4(&m_constantBufferData.projection, XMMatrixTranspose(perspectiveMatrix * orientation));

	
	static const XMVECTORF32 eye = { 0.0f, 0.7f, -1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_camera, XMMatrixInverse(nullptr, XMMatrixLookAtLH(eye, at, up)));
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
}

void SceneRenderer::Render()
{
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_camera))));

	
	auto context = Resources.context;

	


	//context->OMSetRenderTargets(1, &Resources.rtv, Resources.dsv);

	
	
	float color[4];

	color[0] = 0.2;
	color[1] = 0.1;
	color[2] = 0.5;
	color[3] = 1;
	
	
	

	context->ClearRenderTargetView(Resources.rtv, color);
	context->ClearDepthStencilView(Resources.m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	context->IASetInputLayout(Resources.layout);
	context->VSSetShader(Resources.vertexShader, nullptr, 0);
	context->PSSetShader(Resources.pixelShader, nullptr, 0);
	context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	
	drawModel(Models[0]);
	drawModel(Models[1]);

	
}

void SceneRenderer::UpdateCamera(MSG msg, XTime timer)
{
	const float delta_time = timer.SmoothDelta();
	bool mLClick;
	if (msg.message == WM_KEYDOWN)
		buttons[msg.wParam] = true;

	if (msg.message == WM_KEYUP)
		buttons[msg.wParam] = false;

	if (msg.message == WM_RBUTTONDOWN)
		tracking = true;

	if (msg.message == WM_RBUTTONUP)
		tracking = false;

	if (buttons['W'])
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, cameraSpeed * delta_time );
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_camera, result);
	}

	if (buttons['S'])
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, -cameraSpeed * delta_time);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_camera, result);
	}
	if (buttons['A'])
	{
		XMMATRIX translation = XMMatrixTranslation(-cameraSpeed * delta_time, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_camera, result);
	}
	if (buttons['D'])
	{
		XMMATRIX translation = XMMatrixTranslation(cameraSpeed * delta_time, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_camera, result);
	}
	if (buttons['X'])
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, -cameraSpeed * delta_time, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_camera, result);
	}
	if (buttons[VK_SPACE])
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, cameraSpeed * delta_time, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_camera, result);
	}

	
		currMousePos[0] = msg.pt.x;
		currMousePos[1] = msg.pt.y;
		if (tracking )
		{
			float dx = currMousePos[0] - preMousePos[0];
			float dy = currMousePos[1] - preMousePos[1];
			dx *= 0.5f;
			dy *= 0.5f;
			XMFLOAT4 pos = XMFLOAT4(m_camera._41, m_camera._42, m_camera._43, m_camera._44);

			m_camera._41 = 0;
			m_camera._42 = 0;
			m_camera._43 = 0;

			XMMATRIX rotX = XMMatrixRotationX(dy * mouseSpeed * delta_time);
			XMMATRIX rotY = XMMatrixRotationY(dx * mouseSpeed * delta_time);

			XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
			temp_camera = XMMatrixMultiply(rotX, temp_camera);
			temp_camera = XMMatrixMultiply(temp_camera, rotY);

			XMStoreFloat4x4(&m_camera, temp_camera);

			m_camera._41 = pos.x;
			m_camera._42 = pos.y;
			m_camera._43 = pos.z;
			
			
			
			
		}
		else {
			preMousePos[0] = currMousePos[0];
			preMousePos[1] = currMousePos[1];
		}
}

void SceneRenderer::drawModel(ModelBuffers model)
{
	auto context = Resources.context;
	D3D11_MAPPED_SUBRESOURCE mapSubRes;
	ZeroMemory(&mapSubRes, sizeof(mapSubRes));
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	m_constantBufferData.model = model.worldMatrix;
	
	context->Map(m_constantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubRes);
	memcpy(mapSubRes.pData, &m_constantBufferData, sizeof(ModelViewProjectionConstantBuffer));
	context->Unmap(m_constantBuffer, NULL);

	context->IASetIndexBuffer(model.m_model_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	context->IASetVertexBuffers(0, 1, &model.m_model_vertexBuffer, &stride, &offset);
	context->DrawIndexed(model.m_model_indexCount, 0, 0);
	Resources.swapChain->Present(0, 0);
}
