#pragma once
#include "D3dclass.h"
#include "XTime.h"
class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer();
	D3dclass Resources;
	ID3D11Buffer*	m_model_vertexBuffer;
	ID3D11Buffer*   m_model_indexBuffer;
	ID3D11Buffer*	m_floor_vertexBuffer;
	ID3D11Buffer*   m_floor_indexBuffer;
	unsigned int m_floor_indexCount = 1;
	unsigned int m_cube_indexCount = 1;
	ID3D11Buffer *  m_constantBuffer;
	int test = 5;
	DirectX::XMFLOAT4X4 m_camera;
	float cameraSpeed = 1.0f;
	bool tracking;
	float mouseSpeed = 0.025f;
	bool	buttons[256];
	ModelViewProjectionConstantBuffer	m_constantBufferData;
	
	float currMousePos[2];
	float preMousePos[2];

	void createBuffers();
	void createConstantBuffers();
	void setCamera();
	void Render();
	void UpdateCamera(MSG msg, XTime timer);
};

