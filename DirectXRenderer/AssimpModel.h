#pragma once

/** @file AssimpModel.h
 ** Contains the AssimpModel class.*/

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "Model.h"
#include <vector>
#include <string>

using namespace DirectX;
using namespace std;

namespace Ab2
{
	/** Encapsulates loading a model and creating vertex and index buffers.*/
	class AssimpModel : public Model
	{
	public:
		/** Default constructor.*/
		AssimpModel();

		/** Destructor.*/
		~AssimpModel();

		/** Initializes the vertex and index buffers associated with the model.*/
		bool InitializeBuffers(ID3D11Device* a_pDevice);
		void RenderBuffers(ID3D11DeviceContext* a_pDeviceContext);
		int GetIndexCount();

	private:
		struct Mesh
		{
			vector<XMFLOAT3> vertices;
			vector<UINT> indices;
			vector<XMFLOAT4> vertexColors;
			vector<XMFLOAT2> textureCoordinates;
			vector<XMFLOAT3> normals;
			string name;
			UINT numFaces;
			UINT numVertices;
			UINT numIndices;

			ID3D11Buffer* vertexBuffer;
			ID3D11Buffer* indexBuffer;
		};

		/** Vertex format.*/
		struct VertexType
		{
			XMFLOAT3 position;
			//XMFLOAT4 color;
			XMFLOAT2 texCoord;
			//XMFLOAT3 normal;
		};

		vector<Mesh*> m_vMeshes;
	};
}