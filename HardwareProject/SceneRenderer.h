#pragma once
#include "D3dclass.h"
#include "XTime.h"
#include "Model.h"
class SceneRenderer
{
	struct ModelBuffers
	{
	    ID3D11Buffer*		m_model_vertexBuffer;
		ID3D11Buffer*		m_model_indexBuffer;
		unsigned int m_model_indexCount;
		DirectX::XMFLOAT4X4 worldMatrix;
		unsigned int m_model_vertexCount;
		const char* name = nullptr;
	};
public:


	SceneRenderer();
	~SceneRenderer();
	D3dclass Resources;
	std::vector<ModelBuffers> Models;
	ID3D11Buffer *  m_constantBuffer;
	ModelViewProjectionConstantBuffer	m_constantBufferData;
	DirectX::XMFLOAT4X4 m_camera;
	bool	m_loadingComplete;
	float cameraSpeed = 1.0f;
	bool tracking;
	float mouseSpeed = 0.025f;
	bool	buttons[256];
	float currMousePos[2];
	float preMousePos[2];

	void createBuffers();
	void createConstantBuffers();
	void setCamera();
	void Render();
	void UpdateCamera(MSG msg, XTime timer);

	void drawModel( ModelBuffers model);

};

