#include "CubeModel.h"

/** @file CubeModel.cpp
 ** CubeModel implementation.*/

namespace Ab2
{
	CubeModel::CubeModel()
		:	Model()
	{
		
	}

	CubeModel::~CubeModel()
	{

	}

	bool CubeModel::InitializeBuffers(ID3D11Device* a_pDevice)
	{
		VertexType* pVertices;
		unsigned long* pIndices;
		D3D11_BUFFER_DESC descVertexBuffer, descIndexBuffer;
		D3D11_SUBRESOURCE_DATA dataVertex, dataIndex;
		HRESULT result;

		m_nVertexCount = 8;
		m_nIndexCount = 36;

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

		pVertices[0].position = XMFLOAT3(-20.0f, -20.0f, -20.0f);
		pVertices[0].color = XMFLOAT4(0.1f, 0.1f, 0.4f, 1.0f);
		pVertices[1].position = XMFLOAT3(20.0f, -20.0f, -20.0f);
		pVertices[1].color = XMFLOAT4(0.1f, 0.1f, 0.4f, 1.0f);
		pVertices[2].position = XMFLOAT3(20.0f, -20.0f, 20.0f);
		pVertices[2].color = XMFLOAT4(0.1f, 0.1f, 0.4f, 1.0f);
		pVertices[3].position = XMFLOAT3(-20.0f, -20.0f, 20.0f);
		pVertices[3].color = XMFLOAT4(0.1f, 0.1f, 0.4f, 1.0f);
		pVertices[4].position = XMFLOAT3(-20.0f, 20.0f, -20.0f);
		pVertices[4].color = XMFLOAT4(0.1f, 0.1f, 0.4f, 1.0f);
		pVertices[5].position = XMFLOAT3(20.0f, 20.0f, -20.0f);
		pVertices[5].color = XMFLOAT4(0.1f, 0.1f, 0.4f, 1.0f);
		pVertices[6].position = XMFLOAT3(20.0f, 20.0f, 20.0f);
		pVertices[6].color = XMFLOAT4(0.1f, 0.1f, 0.4f, 1.0f);
		pVertices[7].position = XMFLOAT3(-20.0f, 20.0f, 20.0f);
		pVertices[7].color = XMFLOAT4(0.1f, 0.1f, 0.4f, 1.0f);


		pIndices[0] = 0; pIndices[1] = 1; pIndices[2] = 2;
		pIndices[3] = 0; pIndices[4] = 2; pIndices[5] = 3;
		pIndices[6] = 4; pIndices[7] = 7; pIndices[8] = 6;
		pIndices[9] = 4; pIndices[10] = 6; pIndices[11] = 5;
		pIndices[12] = 0; pIndices[13] = 4; pIndices[14] = 5;
		pIndices[15] = 0; pIndices[16] = 5; pIndices[17] = 1;
		pIndices[18] = 3; pIndices[19] = 6; pIndices[20] = 7;
		pIndices[21] = 3; pIndices[22] = 2; pIndices[23] = 6;
		pIndices[24] = 3; pIndices[25] = 7; pIndices[26] = 4;
		pIndices[27] = 3; pIndices[28] = 4; pIndices[29] = 0;
		pIndices[30] = 1; pIndices[31] = 5; pIndices[32] = 6;
		pIndices[33] = 1; pIndices[34] = 6; pIndices[35] = 2;
	

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