#pragma once

/** @file Model.h
 ** Contains the Model class.*/

#include <d3d11_1.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace Ab2
{
	/** Encapsulates loading a model and creating vertex and index buffers.*/
	class Model
	{
	public:
		/** Default constructor.*/
		Model();

		/** Destructor.*/
		virtual ~Model();

		/** Initializes the vertex and index buffers associated with the model.*/
		bool Initialize(ID3D11Device* a_pDevice);

		/** Cleans up the vertex and index buffers associated with the model.*/
		void Shutdown();

		/** Tells the input assembler to use this models vertex and index buffers.*/
		void Render(ID3D11DeviceContext* a_pDeviceContext);

		/** Get the number of indices.*/
		int virtual GetIndexCount();

	protected:
		/** Vertex format.*/
		struct VertexType
		{
			XMFLOAT3 position;
			XMFLOAT4 color;
		};

		bool virtual InitializeBuffers(ID3D11Device* a_pDevice) = 0;
		void virtual RenderBuffers(ID3D11DeviceContext* a_pDeviceContext);
		void ShutdownBuffers();

		ID3D11Buffer* m_pVertexBuffer;
		ID3D11Buffer* m_pIndexBuffer;
		int m_nVertexCount;
		int m_nIndexCount;
	};
}