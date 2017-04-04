#pragma once
#include "D3dclass.h"
#include "XTime.h"
#include "Model.h"
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
		std::vector<JointData> joints;
		std::vector<Line> bones;
		bool isFBX;
		const char* name = nullptr;
	};
public:
	
	
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
	float currMousePos[2];
	float preMousePos[2];

	void createBuffers();
	void createConstantBuffers();
	void setCamera();
	void Render();
	void UpdateCamera(MSG msg, XTime timer);

	void drawJoint(JointData joint);
	void drawBone(Line bone);
	void updateConstanBufferModel(DirectX::XMFLOAT4X4 model);
	void drawModel( ModelBuffers model);
	void createBonesBuffer(ModelBuffers &model);
	void createAxisBuffer();
	void debugRender(ModelBuffers model);
	void drawAxis(XMFLOAT4X4 matrix);
	void Shutdown();

};

