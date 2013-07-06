#include "AmbientLighting.h"

/** @file AmbientLighting.cpp
 ** AmbientLighting implementation.*/


namespace Ab2
{
	AmbientLighting::AmbientLighting()
		:	Material()
	{

	}

	AmbientLighting::~AmbientLighting()
	{

	}

	bool AmbientLighting::Initialize(ID3D11Device* a_pDevice, SceneSettings* a_pSceneSettings)
	{
		bool result;

		result = InitializeShader(a_pDevice, "../AmbientVS.cso", "../AmbientPS.cso");
		if(!result)
		{
			return false;
		}

		m_pSceneSettings = a_pSceneSettings;

		return true;
	}

	bool AmbientLighting::Render(ID3D11DeviceContext* a_pDeviceContext, int a_nIndexCount, XMFLOAT4X4 a_mWorldMatrix, XMFLOAT4X4 a_mViewMatrix, XMFLOAT4X4 a_mProjectionMatrix)
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

	bool AmbientLighting::InitializeInputLayout(ID3D11Device* a_pDevice, char* a_pVertexCsoBuffer, size_t a_nVertexCsoSize)
	{
		HRESULT result;
		D3D11_INPUT_ELEMENT_DESC descPolygonLayout[2];
		unsigned int nElements;
		D3D11_BUFFER_DESC descMatrixBuffer;

		descPolygonLayout[0].SemanticName = "POSITION";
		descPolygonLayout[0].SemanticIndex = 0;
		descPolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		descPolygonLayout[0].InputSlot = 0;
		descPolygonLayout[0].AlignedByteOffset = 0;
		descPolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		descPolygonLayout[0].InstanceDataStepRate = 0;

		descPolygonLayout[1].SemanticName = "COLOR";
		descPolygonLayout[1].SemanticIndex = 0;
		descPolygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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

		return true;
	}

	bool AmbientLighting::SetShaderParameters(ID3D11DeviceContext* a_pDeviceContext, XMFLOAT4X4 a_mWorldMatrix, XMFLOAT4X4 a_mViewMatrix, XMFLOAT4X4 a_mProjectionMatrix)
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

		return true;
	}

}