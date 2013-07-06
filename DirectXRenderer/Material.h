#pragma once

/** @file Material.h
 ** Holds the Material class.*/

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "SceneSettings.h"

using namespace DirectX;

namespace Ab2
{
	/** Encapsulates creating vertex and pixel shaders and setting parameters for a simple color shader.*/
	class Material
	{
	public:
		/** Default Constructor.*/
		Material();

		/** Destructor.*/
		virtual ~Material();

		/** Initializes the components of the shader class.*/
		bool virtual Initialize(ID3D11Device* a_pDevice, SceneSettings* a_pSceneSettings) = 0;

		/** Shuts down the components of the shader class.*/
		void virtual Shutdown();

		/** Given a deviceContext, set the shader parameters and set the pipeline to use the ColorShader vs and ps.*/
		bool virtual Render(ID3D11DeviceContext* a_pDeviceContext, int a_nIndexCount, XMFLOAT4X4 a_mWorldMatrix, XMFLOAT4X4 a_mViewMatrix, XMFLOAT4X4 a_mProjectionMatrix) = 0;

	protected:

		/** Loads the .cso files and creates the shaders on the device.*/
		bool InitializeShader(ID3D11Device* a_pDevice, char* a_wsVSFilename, char* a_wsPSFilename);
		
		/** Customizable Helper function to set up input layouts.*/
		bool virtual InitializeInputLayout(ID3D11Device* a_pDevice, char* a_pVertexCsoBuffer, size_t a_nVertexCsoSize) = 0;

		/** Cleans up the shaders.*/
		void ShutdownShader();

		/** Given a deviceContext sets the constant buffers for the shader.*/
		bool SetShaderParameters(ID3D11DeviceContext* a_pDeviceContext, XMFLOAT4X4 a_mWorldMatrix, XMFLOAT4X4 a_mViewMatrix, XMFLOAT4X4 a_mProjectionMatrix);
		
		/** Sets the device context to use its own vs and ps.*/
		void RenderShader(ID3D11DeviceContext* a_pDeviceContext, int a_nIndexCount);

		ID3D11VertexShader* m_pVertexShader;
		ID3D11PixelShader* m_pPixelShader;
		ID3D11InputLayout* m_pInputLayout;
		ID3D11Buffer* m_pConstantBuffer;

		SceneSettings* m_pSceneSettings;
	};
}