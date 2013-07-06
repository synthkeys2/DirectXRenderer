#pragma once

/** @file ModelMaterial.h
 ** Holds the ModelMaterial class.*/

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "Material.h"

using namespace DirectX;

namespace Ab2
{
	/** Encapsulates creating vertex and pixel shaders and setting parameters for a simple color shader.*/
	class ModelMaterial : public Material
	{
	public:
		/** Default Constructor.*/
		ModelMaterial();

		/** Destructor.*/
		~ModelMaterial();

		/** Initializes the components of the shader class.*/
		bool Initialize(ID3D11Device* a_pDevice, SceneSettings* a_pSceneSettings);

		void Shutdown();

		/** Given a deviceContext, set the shader parameters and set the pipeline to use the ColorShader vs and ps.*/
		bool Render(ID3D11DeviceContext* a_pDeviceContext, int a_nIndexCount, XMFLOAT4X4 a_mWorldMatrix, XMFLOAT4X4 a_mViewMatrix, XMFLOAT4X4 a_mProjectionMatrix);

	private:
		/** Houses the shader parameter format.*/
		struct MatrixBuffer
		{
			XMFLOAT4X4 world;
			XMFLOAT4X4 view;
			XMFLOAT4X4 projection;
			XMFLOAT4 ambientIntensity;
		};

		/** Sets up a custom input layout.*/
		bool virtual InitializeInputLayout(ID3D11Device* a_pDevice, char* a_pVertexCsoBuffer, size_t a_nVertexCsoSize);

		/** Given a deviceContext sets the constant buffers for the shader.*/
		bool SetShaderParameters(ID3D11DeviceContext* a_pDeviceContext, XMFLOAT4X4 a_mWorldMatrix, XMFLOAT4X4 a_mViewMatrix, XMFLOAT4X4 a_mProjectionMatrix);

		ID3D11ShaderResourceView* m_pTexture;
		ID3D11SamplerState* m_pSamplerState;
	};
}