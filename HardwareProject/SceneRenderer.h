#pragma once
#include "D3dclass.h"
#include "XTime.h"
#include "Model.h"
#include "Interpolator.h"
class SceneRenderer
{
	struct Debug
	{
		bool on = true;
		bool wireframe = true;
		bool joints = true;
		bool axis = true;
		bool bones = true;

	};

	struct Line
	{
		int parentIndex;
		int childIndex;
		ID3D11Buffer*		m_model_vertexBuffer;
		ID3D11Buffer*		m_model_indexBuffer;

	};

	struct ModelBuffers
	{
	    ID3D11Buffer*		m_model_vertexBuffer;
		ID3D11Buffer*		m_model_indexBuffer;
		
		unsigned int m_model_indexCount;
		DirectX::XMFLOAT4X4 worldMatrix;
		unsigned int m_model_vertexCount;
		Model* model;
		Interpolator* interpolator;
		std::vector<Line> bones;
		bool isFBX;
		const char* name = nullptr;
	};
public:
	
	Line line;
	XTime timer;
	SceneRenderer();
	~SceneRenderer();
	D3dclass Resources;
	std::vector<ModelBuffers> Models;
	Line axis;
	ID3D11Buffer *  m_constantBuffer;
	ModelViewProjectionConstantBuffer	m_constantBufferData;
	DirectX::XMFLOAT4X4 m_camera;

	Debug debug;
	bool	m_loadingComplete;
	float cameraSpeed = 100.0f;
	bool tracking;
	float mouseSpeed = 0.020f;
	float mouseSensitivity = 100.0f;
	bool	buttons[256];
	bool	prevButtons[256];
	float currMousePos[2];
	float preMousePos[2];

	void createBuffers();
	void createConstantBuffers();
	void setCamera();
	void Render();
	void UpdateCamera(MSG msg, XTime timer);

	void drawJoint(JointData &joint);
	void updateConstanBufferModel(DirectX::XMFLOAT4X4 model);
	void drawModel( ModelBuffers model);
	void createAxisBuffer();
	void debugRender(ModelBuffers model);
	void drawAxis(XMFLOAT4X4 matrix);
	void draWLine(XMFLOAT3 x, XMFLOAT3 y);
	XMFLOAT4X4 buildMatrix(XMFLOAT4 pos, XMFLOAT4 rot, XMFLOAT4 sca);
	void createLineBuffer();
	void Shutdown();


};

