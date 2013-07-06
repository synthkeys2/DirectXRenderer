#include "Model.h"

/** @file Model.cpp
 ** Model implementation.*/

namespace Ab2
{
	Model::Model()
		:	m_pVertexBuffer(NULL), m_pIndexBuffer(NULL)
	{
		
	}

	Model::~Model()
	{

	}

	bool Model::Initialize(ID3D11Device* a_pDevice)
	{
		bool result;

		result = InitializeBuffers(a_pDevice);
		if(!result)
		{
			return false;
		}

		return true;
	}

	void Model::Shutdown()
	{
		ShutdownBuffers();
	}

	void Model::Render(ID3D11DeviceContext* a_pDeviceContext)
	{
		RenderBuffers(a_pDeviceContext);
	}

	int Model::GetIndexCount()
	{
		return m_nIndexCount;
	}

	void Model::ShutdownBuffers()
	{
		if(m_pIndexBuffer)
		{
			m_pIndexBuffer->Release();
			m_pIndexBuffer = NULL;
		}

		if(m_pVertexBuffer)
		{
			m_pVertexBuffer->Release();
			m_pVertexBuffer = NULL;
		}
	}

	void Model::RenderBuffers(ID3D11DeviceContext* a_pDeviceContext)
	{
		unsigned int nStride;
		unsigned int nOffset;

		nStride = sizeof(VertexType);
		nOffset = 0;

		a_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &nStride, &nOffset);
		a_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		a_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}