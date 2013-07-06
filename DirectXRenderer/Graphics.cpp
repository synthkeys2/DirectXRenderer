#include "Graphics.h"

/** @file Graphics.cpp
 ** Graphics implementation.*/

namespace Ab2
{
	Graphics::Graphics()
		:	m_pD3DCore(NULL), m_pCamera(NULL), m_pTriangleModel(NULL), m_pSkyBoxMaterial(NULL), m_pSceneSettings(NULL)
	{

	}

	Graphics::~Graphics()
	{

	}

	bool Graphics::Initialize(int a_nScreenWidth, int a_nScreenHeight, HWND a_hWindow)
	{
		bool result;

		m_pD3DCore = new D3DCore();
		if(!m_pD3DCore)
		{
			return false;
		}

		result = m_pD3DCore->Initialize(	a_nScreenWidth, 
											a_nScreenHeight, 
											VSYNC_ENABLED, 
											a_hWindow,
											FULL_SCREEN, NEAR_PLANE, FAR_PLANE);
		if(!result)
		{
			MessageBoxA(a_hWindow, "Failed to initialize D3D.", "Error", MB_OK);
			return false;
		}

		m_pCamera = new Camera();
		if(!m_pCamera)
		{
			return false;
		}

		m_pCamera->SetPosition(XMFLOAT3(0.0f, 0.0f, 500.0f));

		m_pTriangleModel = new TriangleModel();
		if(!m_pTriangleModel)
		{
			return false;
		}

		result = m_pTriangleModel->Initialize(m_pD3DCore->GetDevice());
		if(!result)
		{
			MessageBoxA(a_hWindow, "Failed to initialize Model.", "Error", MB_OK);
			return false;
		}

		m_pSkyBoxModel = new CubeModel();
		if(!m_pSkyBoxModel)
		{
			return false;
		}

		result = m_pSkyBoxModel->Initialize(m_pD3DCore->GetDevice());
		if(!result)
		{
			return false;
		}

		m_pAssimpModel = new AssimpModel();
		if(!m_pAssimpModel)
		{
			return false;
		}

		result = m_pAssimpModel->Initialize(m_pD3DCore->GetDevice());
		if(!result)
		{
			MessageBoxA(a_hWindow, "Failed to initialize Model.", "Error", MB_OK);
			return false;
		}

		m_pSkyBoxMaterial = new SkyBox();
		if(!m_pSkyBoxMaterial)
		{
			return false;
		}

		m_pTriangleMaterial = new AmbientLighting();
		if(!m_pTriangleMaterial)
		{
			return false;
		}

		m_pAssimpMaterial = new ModelMaterial();
		if(!m_pAssimpMaterial)
		{
			return false;
		}

		m_pSceneSettings = new SceneSettings();
		if(!m_pSceneSettings)
		{
			return false;
		}

		m_pSceneSettings->DefaultColor = XMFLOAT4(0.1f, 0.1f, 0.4f, 1.0f);
		m_pSceneSettings->BackgroundColor = XMFLOAT4(0.7f, 0.5f, 0.2f, 1.0f);
		m_pSceneSettings->AmbientIntensity = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

		result = m_pSkyBoxMaterial->Initialize(m_pD3DCore->GetDevice(), m_pSceneSettings);
		if(!result)
		{
			MessageBoxA(a_hWindow, "Failed to initialize Material.", "Error", MB_OK);
			return false;
		}

		result = m_pTriangleMaterial->Initialize(m_pD3DCore->GetDevice(), m_pSceneSettings);
		if(!result)
		{
			return false;
		}

		result = m_pAssimpMaterial->Initialize(m_pD3DCore->GetDevice(), m_pSceneSettings);
		if(!result)
		{
			return false;
		}

		D3D11_RASTERIZER_DESC rd;

		// Create wireframe/no cull mode for debug
		ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
		rd.FillMode = D3D11_FILL_WIREFRAME;
		rd.CullMode = D3D11_CULL_NONE;
		rd.FrontCounterClockwise = false;
		rd.DepthClipEnable = true;

		m_pD3DCore->GetDevice()->CreateRasterizerState(&rd, &m_pRSWireFrame);

		return true;
	}

	void Graphics::Shutdown()
	{
		if(m_pTriangleMaterial)
		{
			m_pTriangleMaterial->Shutdown();
			delete m_pTriangleMaterial;
			m_pSkyBoxMaterial = NULL;
		}

		if(m_pSkyBoxMaterial)
		{
			m_pSkyBoxMaterial->Shutdown();
			delete m_pSkyBoxMaterial;
			m_pSkyBoxMaterial = NULL;
		}

		if(m_pSceneSettings)
		{
			delete m_pSceneSettings;
			m_pSceneSettings = NULL;
		}

		if(m_pSkyBoxModel)
		{
			m_pSkyBoxModel->Shutdown();
			delete m_pSkyBoxModel;
			m_pSkyBoxModel = NULL;
		}

		if(m_pTriangleModel)
		{
			m_pTriangleModel->Shutdown();
			delete m_pTriangleModel;
			m_pTriangleModel = NULL;
		}

		if(m_pCamera)
		{
			delete m_pCamera;
			m_pCamera = NULL;
		}

		if(m_pD3DCore)
		{
			m_pD3DCore->Shutdown();
			delete m_pD3DCore;
			m_pD3DCore = NULL;
		}
	}

	bool Graphics::ProcessFrame(float a_fDeltaTime)
	{
		bool result;

		result = Render(a_fDeltaTime);
		if(!result)
		{
			return false;
		}

		return true;
	}

	Camera* Graphics::GetCamera()
	{
		return m_pCamera;
	}

	bool Graphics::Render(float a_fDeltaTime)
	{
		bool result;
		XMFLOAT4X4 viewMatrix, projectionMatrix, worldMatrix;

		m_pD3DCore->BeginScene(m_pSceneSettings->BackgroundColor.x, m_pSceneSettings->BackgroundColor.y, m_pSceneSettings->BackgroundColor.z, m_pSceneSettings->BackgroundColor.w);

		m_pCamera->Render(a_fDeltaTime);
		
		m_pCamera->GetViewMatrix(viewMatrix);
		m_pD3DCore->GetWorldMatrix(worldMatrix);
		m_pD3DCore->GetProjectionMatrix(projectionMatrix);

		//m_pD3DCore->GetDeviceContext()->RSSetState(m_pRSWireFrame);

		//m_pD3DCore->GetDeviceContext()->OMSetDepthStencilState(0, 0);
		//m_pD3DCore->GetDeviceContext()->RSSetState(0);

		//m_pTriangleModel->Render(m_pD3DCore->GetDeviceContext());
		//result = m_pTriangleMaterial->Render(m_pD3DCore->GetDeviceContext(), m_pTriangleModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
		//if(!result)
		//{
		//	return false;
		//}

		//m_pSkyBoxModel->Render(m_pD3DCore->GetDeviceContext());
		//result = m_pSkyBoxMaterial->Render(m_pD3DCore->GetDeviceContext(), m_pSkyBoxModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
		//if(!result)
		//{
		//	return false;
		//}

		m_pAssimpModel->Render(m_pD3DCore->GetDeviceContext());
		m_pAssimpMaterial->Render(m_pD3DCore->GetDeviceContext(), m_pAssimpModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);

		m_pD3DCore->GetDeviceContext()->OMSetDepthStencilState(0, 0);
		m_pD3DCore->GetDeviceContext()->RSSetState(0);

		//ID3D11ShaderResourceView* resourceViews[1] = {0};
		//m_pD3DCore->GetDeviceContext()->PSSetShaderResources(0, 1, resourceViews);


		m_pD3DCore->EndScene();

		return true;
	}
}