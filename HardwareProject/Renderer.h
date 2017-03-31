#pragma once
class Renderer
{
#include "D3dclass.h"
public:
	Renderer();
	~Renderer();

	ID3D11Buffer*	m_model_vertexBuffer;
	ID3D11Buffer*   m_model_indexBuffer;
	unsigned int m_cube_indexCount = 1;
	ID3D11Buffer*   m_constantBuffer;
	D3dclass resource;
	void Render();
	void createVertexBuffers();
	void createConstantBuffers();

};

