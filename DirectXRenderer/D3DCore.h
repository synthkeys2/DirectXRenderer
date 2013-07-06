#pragma once

/** @file D3DCore.h
 ** Holds the D3DCore class.*/

#include <dxgi.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace Ab2
{
	/** Abstracts the D3D initialization and device.*/
	class D3DCore
	{
	public:
		/** Default constructor.*/
		D3DCore();

		/** Destructor.*/
		~D3DCore();

		/** Handles all the D3D intialization.*/
		bool Initialize(int a_nScreenWidth, int a_nScreenHeight, bool a_bVSync, HWND a_hWindow, bool a_bFullscreen, float a_fNearPlane, float a_fFarPlane);
		
		/** Cleans up all D3D objects.*/
		void Shutdown();

		/** Set the clear color and clear the buffers.*/
		void BeginScene(float a_fRed, float a_fGreen, float a_fBlue, float a_fAlpha);
		
		/** Present what we have rendered to the swap chain.*/
		void EndScene();

		/** Getter for the D3D device.*/
		ID3D11Device* GetDevice();

		/** Getter for the D3D device context.*/
		ID3D11DeviceContext* GetDeviceContext();

		/** Getter for the Projection Matrix.*/
		void GetProjectionMatrix(XMFLOAT4X4& a_rProjection);

		/** Getter for the World Matrix.*/
		void GetWorldMatrix(XMFLOAT4X4& a_rWorld);

		/** Getter for the Orthographic Matrix.*/
		void GetOrthoMatrix(XMFLOAT4X4& a_rOrtho);

		/** Getter for the video card info.*/
		void GetVideoCardInfo(char* a_sCardName, int& a_rMemory);

	private:
		bool m_bVSyncEnabled;
		int m_nVideoCardMemory;
		char m_sVideoCardDescription[128];
		
		IDXGISwapChain* m_pSwapChain;
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		ID3D11RenderTargetView* m_pRenderTargetView;
		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilState* m_pDepthStencilState;
		ID3D11DepthStencilView* m_pDepthStencilView;
		ID3D11RasterizerState* m_pRasterizerState;

		XMFLOAT4X4 m_mProjection;
		XMFLOAT4X4 m_mWorld;
		XMFLOAT4X4 m_mOrtho;
	};
}