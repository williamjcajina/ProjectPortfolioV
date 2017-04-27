#include "SceneRenderer.h"



SceneRenderer::SceneRenderer()
{
	for (int i = 0; i < 256; i++)
	{
		buttons[i] = false;
		prevButtons[i] = false;

	}
	for (int i = 0; i < 2; i++)
	{
		currMousePos[i] = 0;
		preMousePos[i] = 0;

	}
	m_loadingComplete = false;
	debug.joints = true;
	debug.bones = true;
	debug.axis = true;
	debug.wireframe =true;

	
}


SceneRenderer::~SceneRenderer()
{
	
	

	
}

void SceneRenderer::init(D3dclass resource)
{
	Resources = resource;
	createBuffers();
	createConstantBuffers();
	setCamera();
	
	light.ambientColor = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuseColor = XMFLOAT4(1, 1, 1, 1);
	light.lightDirection = XMFLOAT3(-1, 0, 0);
	light.pointLightPosition = XMFLOAT3(0, 0, 0);
	light.pointColor = XMFLOAT4(0, 0, 0, 1);
	light.specularColor = XMFLOAT4(1, 1, 1, 1);
	light.specularPower = 20.0f;
	light.cameraPos = XMFLOAT3(m_camera._41, m_camera._42, m_camera._43);
	light.spotColor = XMFLOAT4(0, 0, 0, 0);
	light.conedir = XMFLOAT3(0, 0, 0);
	light.spotPos = XMFLOAT3(0, 0, 8);
	light.coneRatio = 0;
	light.spotInnerRatio = 0;
	light.spotOuterRatio = 0;
	light.pointRadius = 0.0f;

}

void SceneRenderer::createBuffers()
{   
	auto createFloor(); {
	ModelBuffers floor;

		static const VertexPositionColor floorVertices[] =
		{
			{ XMFLOAT3(-5, 0.0f, 5), XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT3(-1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(5, 0.0f,  5), XMFLOAT3(1.0f, 1.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(-5,  0.0f, -5), XMFLOAT3(0.0f, 0.0f, 0.5f) , XMFLOAT3(-1.0f, 0.0f, -1.0f) },
			{ XMFLOAT3(5,  0.0f, -5), XMFLOAT3(0.0f, 1.0f, 0.5f) , XMFLOAT3(1.0f, 0.0f, 1.0f) }


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
		hr = Resources.device->CreateBuffer(&indexBufferDesc1, &indexBufferData1, &floor.m_model_indexBuffer);


		DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX scaling;

		scaling = DirectX::XMMatrixScaling(100, 100, 100);
		DirectX::XMMATRIX transform = DirectX::XMMatrixMultiply(scaling, identity);
		XMStoreFloat4x4(&floor.worldMatrix, transform);

		floor.isFBX = false;
		HRESULT r = CreateDDSTextureFromFile(Resources.device, L"floor.dds", (ID3D11Resource**)&floor.Texture, &floor.textureView);
		floor.Textured = true;
		Models.push_back(floor);


	}




	auto createPyramid(); {
		ModelBuffers pyramid;

		static const VertexPositionColor pyramidVertices[] =
		{
			{ XMFLOAT3(-1.5f, 0.0f, 1.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(1.5, 0.0f,  1.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(-1.5,  0.0f, -1.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(1.5,  0.0f, -1.5), XMFLOAT3(0.0f, 0.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(0.0,  2.0f, 0.0), XMFLOAT3(0.0f, 0.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) }


		};

		D3D11_SUBRESOURCE_DATA vertexBufferData1 = { 0 };
		vertexBufferData1.pSysMem = pyramidVertices;
		vertexBufferData1.SysMemPitch = 0;
		vertexBufferData1.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc1(sizeof(pyramidVertices), D3D11_BIND_VERTEX_BUFFER);
		HRESULT hr = Resources.device->CreateBuffer(&vertexBufferDesc1, &vertexBufferData1, &pyramid.m_model_vertexBuffer);



		static const unsigned short pyramidIndices[] =
		{
			//base
			0,1,2,
			1,3,2,

			//
		   0,4,1,
		   2,4,0,
		 
		   1,4,3,
		   3,4,2,





		};

		pyramid.m_model_indexCount = ARRAYSIZE(pyramidIndices);

		D3D11_SUBRESOURCE_DATA indexBufferData1 = { 0 };
		indexBufferData1.pSysMem = pyramidIndices;
		indexBufferData1.SysMemPitch = 0;
		indexBufferData1.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc1(sizeof(pyramidIndices), D3D11_BIND_INDEX_BUFFER);
		hr = Resources.device->CreateBuffer(&indexBufferDesc1, &indexBufferData1, &pyramid.m_model_indexBuffer);


		DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX scaling;

		scaling = DirectX::XMMatrixScaling(1, 1, 1);
		DirectX::XMMATRIX transform = DirectX::XMMatrixMultiply(scaling, identity);
		XMStoreFloat4x4(&pyramid.worldMatrix, identity);

		pyramid.isFBX = false;
		Models.push_back(pyramid);
		
	}





	auto creatteBuffers();
	{
		ModelBuffers modelName;

		/*modelName.name = "helicopter2.obj";*/
		modelName.name = "Teddy_Run.fbx"; 
		modelName.textureName = L"Teddy_D.dds";
		modelName.isFBX = true;
		modelName.Textured = true;
		Models.push_back(modelName);
		
		ModelBuffers modelName2;

	
		modelName2.name = "barrel.obj";
		modelName2.textureName = L"barrel.dds";
		modelName2.Textured = true;
		modelName2.isOBJ = true;
		Models.push_back(modelName2);


		for (unsigned int i = 0; i <Models.size(); i++)
		{


			Model* model = new Model;
			if (!Models[i].isFBX && !Models[i].isOBJ)
			{
				delete model;
				continue;
			}
			if(Models[i].isFBX)
				model->loadModelFBX(Models[i].name);
			if(Models[i].isOBJ)
			model->loadModel(Models[i].name);
			
			if(Models[i].Textured)
			HRESULT r = CreateDDSTextureFromFile(Resources.device, Models[i].textureName, (ID3D11Resource**)&Models[i].Texture, &Models[i].textureView);
			Models[i].Textured = true;
			const int  vertsNumber = model->vertexList.size();
			VertexPositionColor* modelVertices = new VertexPositionColor[vertsNumber];
			

			for (int i = 0; i < vertsNumber; i++)
			{
				modelVertices[i].pos = model->vertexList[i].pos;
				modelVertices[i].color = model->vertexList[i].uv;
				modelVertices[i].normal = model->vertexList[i].normal;
				modelVertices[i].jointIndex = model->vertexList[i].jointIndex;
				modelVertices[i].weights = model->vertexList[i].weights;
			
			}



			D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
			vertexBufferData.pSysMem = modelVertices;
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionColor)*vertsNumber, D3D11_BIND_VERTEX_BUFFER);
			Resources.device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &Models[i].m_model_vertexBuffer);

	

			unsigned short* modelIndices = new unsigned short[model->vertexIndexes.size()];
			for (unsigned int i = 0; i < model->vertexIndexes.size(); i++)
			{
				modelIndices[i] = i;

			}

		
			Models[i].m_model_indexCount = model->vertexIndexes.size();
			D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
			indexBufferData.pSysMem = modelIndices;
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short)*Models[i].m_model_indexCount, D3D11_BIND_INDEX_BUFFER);
			Resources.device->CreateBuffer(&indexBufferDesc, &indexBufferData, &Models[i].m_model_indexBuffer);

		
				

			
		/*	modelIndices = nullptr;*/
			
			delete[] modelVertices;
			delete[] modelIndices;

			XMStoreFloat4x4(&Models[i].worldMatrix, DirectX::XMMatrixIdentity());
			Models[i].model = model;
			if (model->isFBX)
			{
				
				Models[i].interpolator = new Interpolator;
				Models[i].interpolator->animation = &Models[i].model->animation;
				Models[i].isFBX = true;
			}
		}
	}
	createLineBuffer();
	auto createAxises();
	{
		createAxisBuffer();
		timer.Signal();
	};
	m_loadingComplete = true;
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

	
	D3D11_BUFFER_DESC jointMatrixBufferDesc;

	jointMatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	jointMatrixBufferDesc.ByteWidth = sizeof(JointMatrix);
	jointMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	jointMatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	jointMatrixBufferDesc.MiscFlags = 0;
	jointMatrixBufferDesc.StructureByteStride = 0;
	result = Resources.device->CreateBuffer(&jointMatrixBufferDesc, NULL, &m_AnimationBuffer);

	CD3D11_BUFFER_DESC constantBufferDesc2(sizeof(Light), D3D11_BIND_CONSTANT_BUFFER);
	result = (Resources.device->CreateBuffer(&constantBufferDesc2, nullptr, &m_light_constantBuffer));

	
}

void SceneRenderer::setCamera()
{
	
	float aspectRatio = SCREEN_WIDTH/ SCREEN_HEIGHT;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}


	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 1000.0f);
	XMMATRIX orientation = DirectX::XMMatrixIdentity();
	

	

	XMStoreFloat4x4(&m_constantBufferData.projection, XMMatrixTranspose(perspectiveMatrix * orientation));

	
	static const XMVECTORF32 eye = { 0.0f, 0.7f, -1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_camera, XMMatrixInverse(nullptr, XMMatrixLookAtLH(eye, at, up)));
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
	
	XMMATRIX translation = XMMatrixTranslation(0.0f, 90.0f, -180.0f);
	XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
	XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
	XMStoreFloat4x4(&m_camera, result);
}

void SceneRenderer::Render()
{

	
	
	timer.Signal();
	auto context = Resources.context;

	
	

	float color[4];


	color[0] = 180.0f / 255.0f;
	color[1] = 200.0f / 255.0f;
	color[2] = 1.0f;
	color[3] = 1.0f;
	context->ClearRenderTargetView(Resources.rtv, color);
	context->ClearDepthStencilView(Resources.m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);



	
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_camera))));

	
	


	context->IASetInputLayout(Resources.layout);



	
	if (debug.on)
		debugRender(Models[2]);
	else
		drawModel(Models[2]);
	
	drawModel(Models[0]);
	int w = 21;
	if (Models[2].interpolator->currPose.size() > 0)
	{
		
		Models[3].worldMatrix = buildMatrix(Models[2].interpolator->currPose[w].translation, Models[2].interpolator->currPose[w].rotation, XMFLOAT4(5,30,5,1));
	}
	if(!debug.on)
	drawModel(Models[3]);
	
	Resources.swapChain->Present(0, 0);
}

void SceneRenderer::UpdateCamera(MSG msg, XTime timer)
{
	const float delta_time = (float)timer.SmoothDelta();
	/*bool mLClick;*/

	if (msg.wParam < 256 )
	{
		prevButtons['P'] = buttons['P'];
		prevButtons['J'] = buttons['J'];
		prevButtons['B'] = buttons['B'];
		prevButtons['U'] = buttons['U'];
		prevButtons['K'] = buttons['K'];
		prevButtons['Q'] = buttons['Q'];
		prevButtons['E'] = buttons['E'];
		prevButtons['R'] = buttons['R'];

	}
	
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

	if (buttons['P'] && !prevButtons['P'])
	{
		debug.wireframe = !debug.wireframe;
	}
	
	if (buttons['J'] && !prevButtons['J'])
	{
		debug.joints = !debug.joints;
	}
	if (buttons['B'] && !prevButtons['B'])
	{
		debug.bones = !debug.bones;
	}
	if (buttons['U'] && !prevButtons['U'])
	{
		debug.on = !debug.on;
	}
	if (buttons['K'] && !prevButtons['K'])
	{
		debug.axis = !debug.axis;
	}
	

	if (buttons['R'] && !prevButtons['R'])
	{
		debug.anim = !debug.anim;
	}

	if (buttons['Q'] && !prevButtons['Q'])
	{
		currentPose--;
		if (currentPose < 0)
			currentPose = Models[2].model->animation.frames.size() - 1;
	}
	if (buttons['E'] && !prevButtons['E'])
	{
		currentPose++;
		if (currentPose > (int)Models[2].model->animation.frames.size() - 1)
			currentPose = 0;
	}

	if (buttons[VK_UP])
	{
		light.ambientColor.x += light.ambientColor.x*delta_time;
		light.ambientColor.y += light.ambientColor.y*delta_time;
		light.ambientColor.z += light.ambientColor.z*delta_time;
	}
	if (buttons[VK_DOWN])
	{
		light.ambientColor.x -= light.ambientColor.x*delta_time;
		light.ambientColor.y -= light.ambientColor.y*delta_time;
		light.ambientColor.z -= light.ambientColor.z*delta_time;
	}
	if (buttons[VK_LEFT])
	{
		light.lightDirection.x += 1.0f*delta_time;
		if (light.lightDirection.x > 1.0f)
			light.lightDirection.x = 1.0f;
	}
	if (buttons[VK_RIGHT])
	{
		light.lightDirection.x -= 1.0f*delta_time;
		if (light.lightDirection.x < 1.0f)
			light.lightDirection.x = -1.0f;
	}


		currMousePos[0] =(float) msg.pt.x;
		currMousePos[1] =(float) msg.pt.y;
		if (tracking )
		{
			float dx = currMousePos[0] - preMousePos[0];
			float dy = currMousePos[1] - preMousePos[1];
			
			if (abs(dx) < 10 && abs(dy) < 10)
			{
				dy = 0;
				dx = 0;
			}
			
			if (dx >  mouseSensitivity)
				dx = mouseSensitivity;;
			if (dx < -mouseSensitivity)
				dx = -mouseSensitivity;;

			if (dy > mouseSensitivity)
				dy = mouseSensitivity;
			if (dy < -mouseSensitivity)
				dy = -mouseSensitivity;

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

void SceneRenderer::drawJoint(JointData &joint)
{
	
	if (!debug.joints)
		return;
	
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	auto context = Resources.context;
	
	


	
	
	
	Models[1].worldMatrix = buildMatrix(joint.translation, joint.rotation, joint.scale);

  

	context->IASetIndexBuffer(Models[1].m_model_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	context->IASetVertexBuffers(0, 1, &Models[1].m_model_vertexBuffer, &stride, &offset);



	drawModel(Models[1]);




}



void SceneRenderer::updateConstanBufferModel(DirectX::XMFLOAT4X4 model, bool isFBX)
{
	auto context = Resources.context;
	D3D11_MAPPED_SUBRESOURCE mapSubRes;
	ZeroMemory(&mapSubRes, sizeof(mapSubRes));

	if(!isFBX)
	context->VSSetShader(Resources.vertexShader, nullptr, 0);
	else
	context->VSSetShader(Resources.skinVertexShader, nullptr, 0);
	
	context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	m_constantBufferData.model = model;
	
	HRESULT l =context->Map(m_constantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubRes);
	memcpy(mapSubRes.pData, &m_constantBufferData, sizeof(ModelViewProjectionConstantBuffer));
	context->Unmap(m_constantBuffer, NULL);

	if (isFBX)
	{
		ID3D11Buffer* buffers[2];
		buffers[0] = m_constantBuffer;
		buffers[1] = m_AnimationBuffer;

		context->VSSetConstantBuffers(0, 2, buffers);
	
		ZeroMemory(&mapSubRes, sizeof(mapSubRes));
		l = context->Map(m_AnimationBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubRes);
		memcpy(mapSubRes.pData, &m_jointCbufferData, sizeof(JointMatrix));
		context->Unmap(m_AnimationBuffer, NULL);
	}

}

void SceneRenderer::drawModel(ModelBuffers model)
{
	
	auto context = Resources.context;
	context->RSSetState(0);

	if (debug.on)
		light.Tex = false;
	else
		light.Tex = true;
	if (model.Textured /*&& !debug.on*/)
	{
		context->PSSetShader(Resources.pixelShader, nullptr, 0);
		context->PSSetShaderResources(0, 1, &model.textureView);
	
		context->PSSetConstantBuffers(0, 1, &m_light_constantBuffer);
		context->UpdateSubresource(m_light_constantBuffer, 0, NULL, &light, 0, 0);
	}
	else
		context->PSSetShader(Resources.d_PixelShader, nullptr, 0);
		
	


	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;

	if (model.isFBX)
	{
		model.interpolator->SetTime(timer);
		std::vector<JointData> current;
		if (debug.anim)
		 current = model.interpolator->currentPose();
		else
		current = model.interpolator->animation->frames[currentPose].joints;
		
		setAnimationCbuffer(model.model->animation, current);
	}
	
	updateConstanBufferModel(model.worldMatrix, model.isFBX);
	context->IASetIndexBuffer(model.m_model_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	context->IASetVertexBuffers(0, 1, &model.m_model_vertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	context->DrawIndexed(model.m_model_indexCount, 0, 0);
	



}



void SceneRenderer::createAxisBuffer()
{

	
	

	
	HRESULT hr;


	const int  vertsNumber = 4;
	VertexPositionColor modelVertices[vertsNumber] =
	{ 
		{ XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3( 6.0f,0.0f,0.0f ), XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.0f,6.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.0f,0.0f,10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },
	};




	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = modelVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionColor)*vertsNumber, D3D11_BIND_VERTEX_BUFFER);
	hr =Resources.device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &axis.m_model_vertexBuffer);



	unsigned short modelIndices[6] = { 0,1,0,2,0,3 };




	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = modelIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short) * 6, D3D11_BIND_INDEX_BUFFER);
	hr = Resources.device->CreateBuffer(&indexBufferDesc, &indexBufferData, &axis.m_model_indexBuffer);


	

}

void SceneRenderer::debugRender(ModelBuffers model)
{

	Resources.context->PSSetShader(Resources.d_PixelShader, nullptr, 0);
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	auto context = Resources.context;
	
	
	model.interpolator->SetTime(timer);
	std::vector<JointData> current;
	if(debug.anim)
	current = model.interpolator->currentPose();
	else
		current = model.interpolator->animation->frames[currentPose].joints;
	
	setAnimationCbuffer(model.model->animation, current);
	updateConstanBufferModel(model.worldMatrix, model.isFBX);
	
	
	context->RSSetState(Resources.m_rasterState);

	context->IASetIndexBuffer(model.m_model_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	context->IASetVertexBuffers(0, 1, &model.m_model_vertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	
	
	if (debug.wireframe)
	context->DrawIndexed(model.m_model_indexCount, 0, 0);
	
	
	if (!model.isFBX)
		return;
	
	
	
	
	for (unsigned int i = 0; i <current.size() ; i++)
	{
		drawJoint(current[i]);
	
		
		
		
		

		XMFLOAT4X4 axis;
		axis = buildMatrix(current[i].translation, current[i].rotation, current[i].scale);
		drawAxis(axis);
		
		
		if (current[i].parentIndex == -1)
			continue;
		
		
		XMFLOAT3 x,y;
		x.x= current[i].translation.x;
		x.y = current[i].translation.y;
		x.z = current[i].translation.z;

		y.x = current[current[i].parentIndex].translation.x;
		y.y = current[current[i].parentIndex].translation.y;
		y.z = current[current[i].parentIndex].translation.z;
		draWLine(x,y);
	
	}

	
	
	
	context->RSSetState(0);
	
}

void SceneRenderer::drawAxis(XMFLOAT4X4 matrix)
{
	if (!debug.axis)
		return;
	auto context = Resources.context;

	context->RSSetState(0);



	
	
	updateConstanBufferModel(matrix,false);

	context->PSSetShader(Resources.d_PixelShader, nullptr, 0);

	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	context->IASetIndexBuffer(axis.m_model_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	context->IASetVertexBuffers(0, 1, &axis.m_model_vertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	context->DrawIndexed(6, 0, 0);

}

void SceneRenderer::draWLine(XMFLOAT3 x, XMFLOAT3 y)
{
	if (!debug.bones)
		return;
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	auto context = Resources.context;

	D3D11_MAPPED_SUBRESOURCE mapSubRes;
	ZeroMemory(&mapSubRes, sizeof(mapSubRes));

	const int  vertsNumber = 2;
	VertexPositionColor modelVertices[vertsNumber] =
	{ { XMFLOAT3(x.x,x.y,x.z), XMFLOAT3(0.0f, 0.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },
	{ XMFLOAT3(y.x,y.y,y.z), XMFLOAT3(0.0f, 0.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },


	};


	HRESULT l = context->Map(line.m_model_vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubRes);


	memcpy(mapSubRes.pData, &modelVertices, sizeof(VertexPositionColor)*vertsNumber);

	context->Unmap(line.m_model_vertexBuffer, NULL);

	XMFLOAT4X4 identity;
	XMStoreFloat4x4(&identity, DirectX::XMMatrixIdentity());
	updateConstanBufferModel(identity,false);

	context->RSSetState(0);
	
	
	context->PSSetShader(Resources.d_PixelShader, nullptr, 0);

	context->IASetIndexBuffer(line.m_model_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	context->IASetVertexBuffers(0, 1, &line.m_model_vertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	context->DrawIndexed(2, 0, 0);
}

XMFLOAT4X4 SceneRenderer::buildMatrix(XMFLOAT4 pos, XMFLOAT4 rot, XMFLOAT4 sca)
{
	DirectX::XMVECTOR scale, translation, rotation, quaternion;

	scale = DirectX::XMVectorSet(sca.x,sca.y,sca.z ,sca.w);
	translation = DirectX::XMVectorSet(pos.x,pos.y,pos.z,pos.w);
	rotation = DirectX::XMVectorSet(0, 0, 0, 0);
	quaternion = DirectX::XMVectorSet(rot.x,rot.y,rot.z,rot.w);



	
	DirectX::XMMATRIX matrix = DirectX::XMMatrixAffineTransformation(scale, rotation, quaternion, translation);


	matrix = DirectX::XMMatrixTranspose(matrix);

	XMFLOAT4X4 result;
	XMStoreFloat4x4(&result, matrix);
		return result;
}

void SceneRenderer::createLineBuffer()
{

	const int  vertsNumber = 2;
	VertexPositionColor modelVertices[vertsNumber] =
	{ { XMFLOAT3(0, 0, 0), XMFLOAT3(0.0f, 0.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },
	{ XMFLOAT3(0, 0, 0), XMFLOAT3(0.0f, 0.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f) },


	};




	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = modelVertices;
	
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionColor)*vertsNumber, D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Resources.device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &line.m_model_vertexBuffer);



	unsigned short modelIndices[2] = { 0,1 };
	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = modelIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short) * 2, D3D11_BIND_INDEX_BUFFER);
	Resources.device->CreateBuffer(&indexBufferDesc, &indexBufferData, &line.m_model_indexBuffer);
}

void SceneRenderer::setAnimationCbuffer(AnimationData anim, std::vector<JointData> currPose)
{
	for (unsigned int i = 0; i < anim.frames[0].joints.size(); i++)
	{
		XMMATRIX currInverse = XMLoadFloat4x4(&buildMatrix(anim.frames[0].joints[i].translation, anim.frames[0].joints[i].rotation, anim.frames[0].joints[i].scale));
		
	
		

		if (currPose.size() == anim.frames[0].joints.size())
		{
			JointData currP = currPose[i];
			currInverse = DirectX::XMMatrixInverse(nullptr, currInverse);

			XMMATRIX currMatrix = XMLoadFloat4x4(&buildMatrix(currP.translation, currP.rotation, currP.scale));
			XMStoreFloat4x4(&m_jointCbufferData.jointsMatrices[i], XMMatrixMultiply(currMatrix,currInverse));
			


		}
		
	}


}

void SceneRenderer::Shutdown()
{
	for (unsigned int i = 0; i < Models.size(); i++)
	{
		Models[i].m_model_vertexBuffer->Release();
		Models[i].m_model_indexBuffer->Release();
		if (Models[i].isFBX || Models[i].isOBJ)
		delete Models[i].model;
		if (Models[i].isFBX)
		delete Models[i].interpolator;
		if (Models[i].Textured)
		{
			Models[i].textureView->Release();
			Models[i].Texture->Release();
		}
	}

	m_light_constantBuffer->Release();
	m_AnimationBuffer->Release();
	axis.m_model_indexBuffer->Release();
	axis.m_model_vertexBuffer->Release();
	line.m_model_indexBuffer->Release();
	line.m_model_vertexBuffer->Release();
	m_constantBuffer->Release();
	Resources.Shutdown();
}



