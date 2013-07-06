#include "AssimpModel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/** @file AssimpModel.cpp
 ** AssimpModel implementation.*/

namespace Ab2
{
	AssimpModel::AssimpModel()
		:	Model()
	{
		
	}

	AssimpModel::~AssimpModel()
	{

	}

	bool AssimpModel::InitializeBuffers(ID3D11Device* a_pDevice)
	{
		Assimp::Importer importer;
		VertexType* pVertices;
		D3D11_BUFFER_DESC descVertexBuffer, descIndexBuffer;
		D3D11_SUBRESOURCE_DATA dataVertex, dataIndex;
		HRESULT result;



		const aiScene* pScene = importer.ReadFile(	"../tiny.x", 
													   aiProcess_CalcTangentSpace       | 
													   aiProcess_Triangulate            |
													   aiProcess_JoinIdenticalVertices  |
													   aiProcess_SortByPType            |
													   aiProcess_MakeLeftHanded         |
													   aiProcess_GenNormals             |
													   aiProcess_GenUVCoords            |
													   aiProcess_TransformUVCoords      |
													   aiProcess_FlipWindingOrder       |
													   aiProcess_FlipUVs    );   

		if(!pScene)
		{
			return false;
		}

		if(pScene->HasMeshes())
		{
			for(int i = 0; i < pScene->mNumMeshes; ++i)
			{
				aiMesh* pCurrentAiMesh;
				Mesh* pCurrentMesh; 

				pCurrentAiMesh = pScene->mMeshes[i];
				pCurrentMesh = new Mesh();

				pCurrentMesh->numFaces = pCurrentAiMesh->mNumFaces;
				pCurrentMesh->name = string(pCurrentAiMesh->mName.C_Str());
				pCurrentMesh->numVertices = pCurrentAiMesh->mNumVertices;
				pCurrentMesh->numIndices = pCurrentAiMesh->mNumFaces * 3;

				if(pCurrentAiMesh->HasFaces())
				{
					for(int j = 0; j < pCurrentAiMesh->mNumFaces; ++j)
					{
						pCurrentMesh->indices.push_back(pCurrentAiMesh->mFaces[j].mIndices[0]);
						pCurrentMesh->indices.push_back(pCurrentAiMesh->mFaces[j].mIndices[1]);
						pCurrentMesh->indices.push_back(pCurrentAiMesh->mFaces[j].mIndices[2]);
					}
				}
			
				for(int j = 0; j < pCurrentAiMesh->mNumVertices; ++j)
				{
					if(pCurrentAiMesh->HasPositions())
					{
						pCurrentMesh->vertices.push_back(XMFLOAT3(pCurrentAiMesh->mVertices[j].x, pCurrentAiMesh->mVertices[j].y, pCurrentAiMesh->mVertices[j].z));
					}
					if(pCurrentAiMesh->HasVertexColors(0))
					{
						pCurrentMesh->vertexColors.push_back(XMFLOAT4(pCurrentAiMesh->mColors[0][j].r, pCurrentAiMesh->mColors[0][j].g, pCurrentAiMesh->mColors[0][j].b, pCurrentAiMesh->mColors[0][j].a));
					}
					if(pCurrentAiMesh->HasTextureCoords(0))
					{
						pCurrentMesh->textureCoordinates.push_back(XMFLOAT2(pCurrentAiMesh->mTextureCoords[0][j].x, pCurrentAiMesh->mTextureCoords[0][j].y));
					}
					if(pCurrentAiMesh->HasNormals())
					{
						pCurrentMesh->normals.push_back(XMFLOAT3(pCurrentAiMesh->mNormals[j].x, pCurrentAiMesh->mNormals[j].y, pCurrentAiMesh->mNormals[j].z));
					}
				}

				m_vMeshes.push_back(pCurrentMesh);
		
				pVertices = new VertexType[pCurrentMesh->numVertices];
				if(!pVertices)
				{
					return false;
				}

				for(int i = 0; i < pCurrentMesh->numVertices; ++i)
				{
					pVertices[i].position = pCurrentMesh->vertices[i];
					pVertices[i].texCoord = pCurrentMesh->textureCoordinates[i];
					//pVertices[i].color = pCurrentMesh->vertexColors[i];
					//pVertices[i].color = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
					//pVertices[i].normal = pCurrentMesh->normals[i];
				}

				descVertexBuffer.Usage = D3D11_USAGE_DEFAULT;
				descVertexBuffer.ByteWidth = sizeof(VertexType) * pCurrentMesh->numVertices;
				descVertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				descVertexBuffer.CPUAccessFlags = 0;
				descVertexBuffer.MiscFlags = 0;
				descVertexBuffer.StructureByteStride = 0;

				dataVertex.pSysMem = pVertices;
				dataVertex.SysMemPitch = 0;
				dataVertex.SysMemSlicePitch = 0;

				result = a_pDevice->CreateBuffer(&descVertexBuffer, &dataVertex, &pCurrentMesh->vertexBuffer);
				if(FAILED(result))
				{
					return false;
				}

				descIndexBuffer.Usage = D3D11_USAGE_DEFAULT;
				descIndexBuffer.ByteWidth = sizeof(unsigned long) * pCurrentMesh->numIndices;
				descIndexBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
				descIndexBuffer.CPUAccessFlags = 0;
				descIndexBuffer.MiscFlags = 0;
				descIndexBuffer.StructureByteStride = 0;

				dataIndex.pSysMem = pCurrentMesh->indices.data();
				dataIndex.SysMemPitch = 0;
				dataIndex.SysMemSlicePitch = 0;

				result = a_pDevice->CreateBuffer(&descIndexBuffer, &dataIndex, &pCurrentMesh->indexBuffer);
				if(FAILED(result))
				{
					return false;
				}
				
				delete[] pVertices;
				pVertices = NULL;
			}
		}



		return true;
	}

	void AssimpModel::RenderBuffers(ID3D11DeviceContext* a_pDeviceContext)
	{
		unsigned int nStride;
		unsigned int nOffset;

		nStride = sizeof(VertexType);
		nOffset = 0;

		for(int i = 0; i < m_vMeshes.size(); ++i)
		{
			a_pDeviceContext->IASetVertexBuffers(0, 1, &m_vMeshes[i]->vertexBuffer, &nStride, &nOffset);
			a_pDeviceContext->IASetIndexBuffer(m_vMeshes[i]->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			a_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
	}

	int AssimpModel::GetIndexCount()
	{
		unsigned int nIndices = 0;
		for(int i = 0; i < m_vMeshes.size(); ++i)
		{
			nIndices += m_vMeshes[i]->numIndices;
		}

		return nIndices;
	}
}