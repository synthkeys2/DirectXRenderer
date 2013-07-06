#include "Material.h"
#include <fstream>

/** @file Material.cpp
 ** Material implementation.*/


namespace Ab2
{
	Material::Material()
		:	m_pVertexShader(NULL), m_pPixelShader(NULL), m_pInputLayout(NULL), m_pConstantBuffer(NULL)
	{

	}

	Material::~Material()
	{

	}


	void Material::Shutdown()
	{
		ShutdownShader();
	}

	bool Material::InitializeShader(ID3D11Device* a_pDevice, char* a_wsVSFilename, char* a_wsPSFilename)
	{
		HRESULT result;
		ID3D10Blob* pErrorMessage;
		ID3D10Blob* pVertexShaderBuffer;
		ID3D10Blob* pPixelShaderBuffer;


		pErrorMessage = NULL;
		pVertexShaderBuffer = NULL;
		pPixelShaderBuffer = NULL;

		std::ifstream vertexCso;
		size_t vertexCsoSize;
		vertexCso.open(a_wsVSFilename, std::ios::binary | std::ios::ate);
		if(!vertexCso.is_open())
		{
			return false;
		}
		vertexCsoSize = (size_t)vertexCso.tellg();

		std::ifstream pixelCso;
		size_t pixelCsoSize;
		pixelCso.open(a_wsPSFilename, std::ios::binary | std::ios::ate);
		if(!pixelCso.is_open())
		{
			return false;
		}
		pixelCsoSize = (size_t)pixelCso.tellg();
		
		char* vertexCsoBuffer = new char[vertexCsoSize];
		char* pixelCsoBuffer = new char[pixelCsoSize];
		vertexCso.seekg(0, std::ios::beg);
		pixelCso.seekg(0, std::ios::beg);
		vertexCso.read(vertexCsoBuffer, vertexCsoSize);
		pixelCso.read(pixelCsoBuffer, pixelCsoSize);
		vertexCso.close();
		pixelCso.close();
		
		result = a_pDevice->CreateVertexShader(vertexCsoBuffer, vertexCsoSize, NULL, &m_pVertexShader);
		if(FAILED(result))
		{
			return false;
		}

		result = a_pDevice->CreatePixelShader(pixelCsoBuffer, pixelCsoSize, NULL, &m_pPixelShader);
		if(FAILED(result))
		{
			return false;
		}

		InitializeInputLayout(a_pDevice, vertexCsoBuffer, vertexCsoSize);

		delete[] vertexCsoBuffer;
		delete[] pixelCsoBuffer;

		return true;
	}

	void Material::ShutdownShader()
	{
		if(m_pConstantBuffer)
		{
			m_pConstantBuffer->Release();
			m_pConstantBuffer = NULL;
		}

		if(m_pInputLayout)
		{
			m_pInputLayout->Release();
			m_pInputLayout = NULL;
		}

		if(m_pPixelShader)
		{
			m_pPixelShader->Release();
			m_pPixelShader = NULL;
		}

		if(m_pVertexShader)
		{
			m_pVertexShader->Release();
			m_pVertexShader = NULL;
		}
	}

	void Material::RenderShader(ID3D11DeviceContext* a_pDeviceContext, int a_nIndexCount)
	{
		a_pDeviceContext->IASetInputLayout(m_pInputLayout);
		a_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
		a_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
		a_pDeviceContext->DrawIndexed(a_nIndexCount, 0, 0);
	}
}