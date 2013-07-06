#include "ModelMaterial.h"
#include "../external/DDSTextureLoader.h"

/** @file ModelMaterial.cpp
 ** ModelMaterial implementation.*/


namespace Ab2
{
	ModelMaterial::ModelMaterial()
		:	Material()
	{

	}

	ModelMaterial::~ModelMaterial()
	{

	}

	bool ModelMaterial::Initialize(ID3D11Device* a_pDevice, SceneSettings* a_pSceneSettings)
	{
		bool result;

		result = InitializeShader(a_pDevice, "../TexturedModelVS.cso", "../TexturedModelPS.cso");
		if(!result)
		{
			return false;
		}

		m_pSceneSettings = a_pSceneSettings;

		CreateDDSTextureFromFile(a_pDevice, L"../Tiny_skin.dds", NULL, &m_pTexture);

		return true;
	}

	bool ModelMaterial::Render(ID3D11DeviceContext* a_pDeviceContext, int a_nIndexCount, XMFLOAT4X4 a_mWorldMatrix, XMFLOAT4X4 a_mViewMatrix, XMFLOAT4X4 a_mProjectionMatrix)
	{
		bool result;

		result = SetShaderParameters(a_pDeviceContext, a_mWorldMatrix, a_mViewMatrix, a_mProjectionMatrix);
		if(!result)
		{
			return false;
		}

		RenderShader(a_pDeviceContext, a_nIndexCount);

		return true;
	}

	bool ModelMaterial::InitializeInputLayout(ID3D11Device* a_pDevice, char* a_pVertexCsoBuffer, size_t a_nVertexCsoSize)
	{
		HRESULT result;
		D3D11_INPUT_ELEMENT_DESC descPolygonLayout[2];
		unsigned int nElements;
		D3D11_BUFFER_DESC descMatrixBuffer;
		D3D11_SAMPLER_DESC descSampler;

		descPolygonLayout[0].SemanticName = "POSITION";
		descPolygonLayout[0].SemanticIndex = 0;
		descPolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		descPolygonLayout[0].InputSlot = 0;
		descPolygonLayout[0].AlignedByteOffset = 0;
		descPolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		descPolygonLayout[0].InstanceDataStepRate = 0;

		descPolygonLayout[1].SemanticName = "TEXCOORD";
		descPolygonLayout[1].SemanticIndex = 0;
		descPolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		descPolygonLayout[1].InputSlot = 0;
		descPolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		descPolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		descPolygonLayout[1].InstanceDataStepRate = 0;

		nElements = sizeof(descPolygonLayout) / sizeof(descPolygonLayout[0]);

		result = a_pDevice->CreateInputLayout(descPolygonLayout, nElements, a_pVertexCsoBuffer, a_nVertexCsoSize, &m_pInputLayout);
		if(FAILED(result))
		{
			return false;
		}

		descMatrixBuffer.Usage = D3D11_USAGE_DYNAMIC;
		descMatrixBuffer.ByteWidth = sizeof(MatrixBuffer);
		descMatrixBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		descMatrixBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		descMatrixBuffer.MiscFlags = 0;
		descMatrixBuffer.StructureByteStride = 0;

		result = a_pDevice->CreateBuffer(&descMatrixBuffer, NULL, &m_pConstantBuffer);
		if(FAILED(result))
		{
			return false;
		}

		descSampler.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		descSampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		descSampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		descSampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		descSampler.MipLODBias = 0.0f;
		descSampler.MaxAnisotropy = 1;
		descSampler.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		descSampler.BorderColor[0] = 0;
		descSampler.BorderColor[1] = 0;
		descSampler.BorderColor[2] = 0;
		descSampler.BorderColor[3] = 0;
		descSampler.MinLOD = 0;
		descSampler.MaxLOD = D3D11_FLOAT32_MAX;

		result = a_pDevice->CreateSamplerState(&descSampler, &m_pSamplerState);
		if(FAILED(result))
		{
			return false;
		}

		return true;
	}

	bool ModelMaterial::SetShaderParameters(ID3D11DeviceContext* a_pDeviceContext, XMFLOAT4X4 a_mWorldMatrix, XMFLOAT4X4 a_mViewMatrix, XMFLOAT4X4 a_mProjectionMatrix)
	{
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		MatrixBuffer* pData;
		unsigned int nBuffer;

		XMStoreFloat4x4(&a_mWorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&a_mWorldMatrix)));
		XMStoreFloat4x4(&a_mViewMatrix, XMMatrixTranspose(XMLoadFloat4x4(&a_mViewMatrix)));
		XMStoreFloat4x4(&a_mProjectionMatrix, XMMatrixTranspose(XMLoadFloat4x4(&a_mProjectionMatrix)));

		result = a_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if(FAILED(result))
		{
			return false;
		}

		pData = (MatrixBuffer*)mappedResource.pData;

		pData->world = a_mWorldMatrix;
		pData->view = a_mViewMatrix;
		pData->projection = a_mProjectionMatrix;
		pData->ambientIntensity = m_pSceneSettings->AmbientIntensity;

		a_pDeviceContext->Unmap(m_pConstantBuffer, 0);

		nBuffer = 0;

		a_pDeviceContext->VSSetConstantBuffers(nBuffer, 1, &m_pConstantBuffer);
		a_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
		a_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);

		return true;
	}

	void ModelMaterial::Shutdown()
	{
		Material::Shutdown();

		if(m_pSamplerState)
		{
			m_pSamplerState->Release();
			m_pSamplerState = NULL;
		}

		if(m_pTexture)
		{
			m_pTexture->Release();
			m_pTexture = NULL;
		}
	}

}