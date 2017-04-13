#pragma once
#include "D3dclass.h"
#include "XTime.h"
#include "Model.h"
#include "Interpolator.h"
#include "DDSTextureLoader.h"
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
		bool isFBX = false;
		bool isOBJ =false;
		const char* name = nullptr;
		
		bool Textured = false;
		ID3D11ShaderResourceView* textureView =nullptr;
		ID3D11Texture2D* Texture = nullptr;
		const wchar_t* textureName;
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
	ID3D11Buffer *  m_AnimationBuffer;
	ModelViewProjectionConstantBuffer	m_constantBufferData;
	JointMatrix m_jointCbufferData;
	DirectX::XMFLOAT4X4 m_camera;
	ID3D11Buffer* m_light_constantBuffer;
	Light light;
	Debug debug;

	void init(D3dclass resource);
	
	void Render();
	void UpdateCamera(MSG msg, XTime timer);
	void Shutdown();
private:
	
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
	void drawJoint(JointData &joint);
	void updateConstanBufferModel(DirectX::XMFLOAT4X4 model, bool isFBX);
	void drawModel( ModelBuffers model);
	void createAxisBuffer();
	void debugRender(ModelBuffers model);
	void drawAxis(XMFLOAT4X4 matrix);
	void draWLine(XMFLOAT3 x, XMFLOAT3 y);
	XMFLOAT4X4 buildMatrix(XMFLOAT4 pos, XMFLOAT4 rot, XMFLOAT4 sca);
	void createLineBuffer();
	
	void setAnimationCbuffer(AnimationData anim, std::vector<JointData> currPose);

};

