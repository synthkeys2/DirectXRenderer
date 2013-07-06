#include "TriangleModel.h"

/** @file TriangleModel.cpp
 ** TriangleModel implementation.*/

namespace Ab2
{
	TriangleModel::TriangleModel()
		:	Model()
	{
		
	}

	TriangleModel::~TriangleModel()
	{

	}

	bool TriangleModel::InitializeBuffers(ID3D11Device* a_pDevice)
	{
		VertexType* pVertices;
		unsigned long* pIndices;
		D3D11_BUFFER_DESC descVertexBuffer, descIndexBuffer;
		D3D11_SUBRESOURCE_DATA dataVertex, dataIndex;
		HRESULT result;

		m_nVertexCount = 3;
		m_nIndexCount = 3;

		pVertices = new VertexType[m_nVertexCount];
		if(!pVertices)
		{
			return false;
		}

		pIndices = new unsigned long[m_nIndexCount];
		if(!pIndices)
		{
			return false;
		}

		pVertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
		pVertices[0].color = XMFLOAT4(0.1f, 0.1f, 0.4f, 1.0f);
		pVertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
		pVertices[1].color = XMFLOAT4(0.1f, 0.1f, 0.4f, 1.0f);
		pVertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
		pVertices[2].color = XMFLOAT4(0.1f, 0.1f, 0.4f, 1.0f);

		pIndices[0] = 0;
		pIndices[1] = 1;
		pIndices[2] = 2;

		descVertexBuffer.Usage = D3D11_USAGE_DEFAULT;
		descVertexBuffer.ByteWidth = sizeof(VertexType) * m_nVertexCount;
		descVertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		descVertexBuffer.CPUAccessFlags = 0;
		descVertexBuffer.MiscFlags = 0;
		descVertexBuffer.StructureByteStride = 0;

		dataVertex.pSysMem = pVertices;
		dataVertex.SysMemPitch = 0;
		dataVertex.SysMemSlicePitch = 0;

		result = a_pDevice->CreateBuffer(&descVertexBuffer, &dataVertex, &m_pVertexBuffer);
		if(FAILED(result))
		{
			return false;
		}

		descIndexBuffer.Usage = D3D11_USAGE_DEFAULT;
		descIndexBuffer.ByteWidth = sizeof(unsigned long) * m_nIndexCount;
		descIndexBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
		descIndexBuffer.CPUAccessFlags = 0;
		descIndexBuffer.MiscFlags = 0;
		descIndexBuffer.StructureByteStride = 0;

		dataIndex.pSysMem = pIndices;
		dataIndex.SysMemPitch = 0;
		dataIndex.SysMemSlicePitch = 0;

		result = a_pDevice->CreateBuffer(&descIndexBuffer, &dataIndex, &m_pIndexBuffer);
		if(FAILED(result))
		{
			return false;
		}

		delete[] pVertices;
		pVertices = NULL;

		delete[] pIndices;
		pIndices = NULL;

		return true;
	}
}