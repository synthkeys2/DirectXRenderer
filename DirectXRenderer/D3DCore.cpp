#include "D3DCore.h"

/** @file D3DCore.cpp
 ** D3DCore implementation.*/

namespace Ab2
{
	D3DCore::D3DCore()
		:	m_pSwapChain(NULL), m_pDevice(NULL), m_pDeviceContext(NULL), m_pRenderTargetView(NULL),
			m_pDepthStencilBuffer(NULL), m_pDepthStencilState(NULL), m_pDepthStencilView(NULL),
			m_pRasterizerState(NULL)
	{

	}

	D3DCore::~D3DCore()
	{

	}

	bool D3DCore::Initialize(int a_nScreenWidth, int a_nScreenHeight, bool a_bVSync, HWND a_hWindow, bool a_bFullScreen, float a_fNearPlane, float a_fFarPlane)
	{
		HRESULT result;
		IDXGIFactory* pFactory;
		IDXGIAdapter* pAdapter;
		IDXGIOutput* pAdapterOutput;
		unsigned int nModes, i, nNumerator, nDenominator, nStringLength;
		DXGI_MODE_DESC* pDisplayModeList;
		DXGI_ADAPTER_DESC descAdapter;
		int iError;
		DXGI_SWAP_CHAIN_DESC descSwapChain;
		D3D_FEATURE_LEVEL featureLevel;
		ID3D11Texture2D* pBackBuffer;
		D3D11_TEXTURE2D_DESC descDepthBuffer;
		D3D11_DEPTH_STENCIL_DESC descDepthStencil;
		D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView;
		D3D11_RASTERIZER_DESC descRasterizer;
		D3D11_VIEWPORT viewport;
		float fFOV, fAspectRatio;

		m_bVSyncEnabled = a_bVSync;

		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
		if(FAILED(result))
		{
			return false;
		}

		result = pFactory->EnumAdapters(0, &pAdapter);
		if(FAILED(result))
		{
			return false;
		}

		result = pAdapter->EnumOutputs(0, &pAdapterOutput);
		if(FAILED(result))
		{
			return false;
		}

		result = pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nModes, NULL);
		if(FAILED(result))
		{
			return false;
		}

		pDisplayModeList = new DXGI_MODE_DESC[nModes];
		if(!pDisplayModeList)
		{
			return false;
		}

		result = pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nModes, pDisplayModeList);
		if(FAILED(result))
		{
			return false;
		}

		for(i = 0; i < nModes; ++i)
		{
			if(pDisplayModeList[i].Width == (unsigned int)a_nScreenWidth && pDisplayModeList[i].Height == (unsigned int)a_nScreenHeight)
			{
				nNumerator = pDisplayModeList[i].RefreshRate.Numerator;
				nDenominator = pDisplayModeList[i].RefreshRate.Denominator;
			}
		}

		result = pAdapter->GetDesc(&descAdapter);
		if(FAILED(result))
		{
			return false;
		}

		m_nVideoCardMemory = (int)(descAdapter.DedicatedVideoMemory / 1024 / 1024);

		iError = wcstombs_s(&nStringLength, m_sVideoCardDescription, 128, descAdapter.Description, 128);
		if(iError != 0)
		{
			return false;
		}

		delete pDisplayModeList;
		pDisplayModeList = NULL;

		pAdapterOutput->Release();
		pAdapterOutput = NULL;

		pAdapter->Release();
		pAdapter = NULL;

		pFactory->Release();
		pFactory = NULL;

		ZeroMemory(&descSwapChain, sizeof(descSwapChain));
		descSwapChain.BufferCount = 1;
		descSwapChain.BufferDesc.Width = a_nScreenWidth;
		descSwapChain.BufferDesc.Height = a_nScreenHeight;
		descSwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if(m_bVSyncEnabled)
		{
			descSwapChain.BufferDesc.RefreshRate.Numerator = nNumerator;
			descSwapChain.BufferDesc.RefreshRate.Denominator = nDenominator;
		}
		else
		{
			descSwapChain.BufferDesc.RefreshRate.Numerator = 0;
			descSwapChain.BufferDesc.RefreshRate.Denominator = 1;
		}

		descSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		descSwapChain.OutputWindow = a_hWindow;
		descSwapChain.SampleDesc.Count = 1;
		descSwapChain.SampleDesc.Quality = 0;

		if(a_bFullScreen)
		{
			descSwapChain.Windowed = false;
		}
		else
		{
			descSwapChain.Windowed = true;
		}

		descSwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		descSwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		descSwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		featureLevel = D3D_FEATURE_LEVEL_11_0;

		result = D3D11CreateDeviceAndSwapChain(	NULL,
												D3D_DRIVER_TYPE_HARDWARE,
												NULL,
												0,
												&featureLevel,
												1,
												D3D11_SDK_VERSION,
												&descSwapChain,
												&m_pSwapChain,
												&m_pDevice,
												NULL,
												&m_pDeviceContext);

		if(FAILED(result))
		{
			return false;
		}

		result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if(FAILED(result))
		{
			return false;
		}

		result = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
		if(FAILED(result))
		{
			return false;
		}

		pBackBuffer->Release();
		pBackBuffer= NULL;

		ZeroMemory(&descDepthBuffer, sizeof(descDepthBuffer));

		descDepthBuffer.Width = a_nScreenWidth;
		descDepthBuffer.Height = a_nScreenHeight;
		descDepthBuffer.MipLevels = 1;
		descDepthBuffer.ArraySize = 1;
		descDepthBuffer.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepthBuffer.SampleDesc.Count = 1;
		descDepthBuffer.SampleDesc.Quality = 0;
		descDepthBuffer.Usage = D3D11_USAGE_DEFAULT;
		descDepthBuffer.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		result = m_pDevice->CreateTexture2D(&descDepthBuffer, NULL, &m_pDepthStencilBuffer);
		if(FAILED(result))
		{
			return false;
		}

		ZeroMemory(&descDepthStencil, sizeof(descDepthStencil));

		descDepthStencil.DepthEnable = true;
		descDepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDepthStencil.DepthFunc = D3D11_COMPARISON_LESS;
		descDepthStencil.StencilEnable = true;
		descDepthStencil.StencilReadMask = 0xFF;
		descDepthStencil.StencilWriteMask = 0xFF;
		descDepthStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDepthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDepthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDepthStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDepthStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDepthStencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		result = m_pDevice->CreateDepthStencilState(&descDepthStencil, &m_pDepthStencilState);
		if(FAILED(result))
		{
			return false;
		}

		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

		ZeroMemory(&descDepthStencilView, sizeof(descDepthStencilView));

		descDepthStencilView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDepthStencilView.Texture2D.MipSlice = 0;

		result = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &descDepthStencilView, &m_pDepthStencilView);

		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

		ZeroMemory(&descRasterizer, sizeof(descRasterizer));

		descRasterizer.AntialiasedLineEnable = false;
		descRasterizer.CullMode = D3D11_CULL_BACK;
		descRasterizer.DepthBias = 0;
		descRasterizer.DepthBiasClamp = 0.0f;
		descRasterizer.DepthClipEnable = true;
		descRasterizer.FillMode = D3D11_FILL_SOLID;
		descRasterizer.FrontCounterClockwise = false;
		descRasterizer.MultisampleEnable = false;
		descRasterizer.ScissorEnable = false;
		descRasterizer.SlopeScaledDepthBias = 0.0f;

		result = m_pDevice->CreateRasterizerState(&descRasterizer, &m_pRasterizerState);
		if(FAILED(result))
		{
			return false;
		}

		m_pDeviceContext->RSSetState(m_pRasterizerState);

		viewport.Width = (float)a_nScreenWidth;
		viewport.Height = (float)a_nScreenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 0.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		m_pDeviceContext->RSSetViewports(1, &viewport);

		fFOV = XM_PI / 4.0f;
		fAspectRatio = (float)a_nScreenWidth / (float)a_nScreenHeight;

		XMStoreFloat4x4(&m_mProjection, XMMatrixPerspectiveFovLH(fFOV, fAspectRatio, a_fNearPlane, a_fFarPlane));
		XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());
		XMStoreFloat4x4(&m_mOrtho, XMMatrixOrthographicLH((float)a_nScreenWidth, (float)a_nScreenHeight, a_fNearPlane, a_fFarPlane));

		return true;
	}

	void D3DCore::Shutdown()
	{
		if(m_pSwapChain)
		{
			m_pSwapChain->SetFullscreenState(false, NULL);
		}
		if(m_pRasterizerState)
		{
			m_pRasterizerState->Release();
			m_pRasterizerState = NULL;
		}
		if(m_pDepthStencilView)
		{
			m_pDepthStencilView->Release();
			m_pDepthStencilView = NULL;
		}
		if(m_pDepthStencilState)
		{
			m_pDepthStencilState->Release();
			m_pDepthStencilState = NULL;
		}
		if(m_pRenderTargetView)
		{
			m_pRenderTargetView->Release();
			m_pRenderTargetView = NULL;
		}
		if(m_pDeviceContext)
		{
			m_pDeviceContext->Release();
			m_pDeviceContext = NULL;
		}
		if(m_pDevice)
		{
			m_pDevice->Release();
			m_pDevice = NULL;
		}
		if(m_pSwapChain)
		{
			m_pSwapChain->Release();
			m_pSwapChain = NULL;
		}
	}

	void D3DCore::BeginScene(float a_fRed, float a_fGreen, float a_fBlue, float a_fAlpha)
	{
		const FLOAT color[4] = {a_fRed, a_fGreen, a_fBlue, a_fAlpha};
		
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void D3DCore::EndScene()
	{
		if(m_bVSyncEnabled)
		{
			m_pSwapChain->Present(1, 0);
		}
		else
		{
			m_pSwapChain->Present(0, 0);
		}
	}

	ID3D11Device* D3DCore::GetDevice()
	{
		return m_pDevice;
	}

	ID3D11DeviceContext* D3DCore::GetDeviceContext()
	{
		return m_pDeviceContext;
	}

	void D3DCore::GetProjectionMatrix(XMFLOAT4X4& a_rProjection)
	{
		a_rProjection = m_mProjection;
	}

	void D3DCore::GetWorldMatrix(XMFLOAT4X4& a_rWorld)
	{
		a_rWorld = m_mWorld;
	}

	void D3DCore::GetOrthoMatrix(XMFLOAT4X4& a_rOrtho)
	{
		a_rOrtho = m_mOrtho;
	}

	void D3DCore::GetVideoCardInfo(char* a_sCardName, int& a_rMemory)
	{
		strcpy_s(a_sCardName, 128, m_sVideoCardDescription);
		a_rMemory = m_nVideoCardMemory;
	}
}