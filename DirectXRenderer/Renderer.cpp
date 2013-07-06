#include "Renderer.h"
#include <windowsx.h>
#include <WinUser.h>

/** @file Renderer.cpp
 ** Renderer Implementation.*/

namespace Ab2
{
	Renderer::Renderer()
		:	m_pInput(NULL), m_pGraphics(NULL)
	{

	}

	Renderer::~Renderer()
	{

	}

	bool Renderer::Initialize()
	{
		int screenWidth = 0, screenHeight = 0;
		bool result;

		InitializeWindows(screenWidth, screenHeight);

		m_pInput = new Input();
		if(!m_pInput)
		{
			return false;
		}

		result = m_pInput->Initialize();
		if(!result)
		{
			return false;
		}

		m_pGraphics = new Graphics();
		if(!m_pGraphics)
		{
			return false;
		}

		result = m_pGraphics->Initialize(screenWidth, screenHeight, m_hWindow);
		if(!result)
		{
			return false;
		}

		m_pTimer = new Timer();
		if(!m_pTimer)
		{
			return false;
		}

		result = m_pTimer->Initialize();
		if(!result)
		{
			return false;
		}


		return true;
	}

	void Renderer::Shutdown()
	{
		if(m_pTimer)
		{
			delete m_pTimer;
			m_pTimer = NULL;
		}

		if(m_pGraphics)
		{
			m_pGraphics->Shutdown();
			delete m_pGraphics;
			m_pGraphics = NULL;
		}

		if(m_pInput)
		{
			m_pInput->Shutdown();
			delete m_pInput;
			m_pInput = NULL;
		}

		ShutdownWindows();
	}

	void Renderer::Run()
	{
		MSG message;
		bool result;

		ZeroMemory(&message, sizeof(MSG));

		while(true)
		{
			if(PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			if(message.message == WM_QUIT)
			{
				break;
			}
			else
			{
				result = ProcessFrame();
				if(!result)
				{
					break;
				}
			}
		}
	}

	LRESULT CALLBACK Renderer::MessageHandler(HWND a_hWindow, UINT a_uMessage, WPARAM a_wParam, LPARAM a_lParam)
	{
		switch(a_uMessage)
		{
			case WM_KEYDOWN:
				m_pInput->KeyDown((unsigned int)a_wParam);
				return NULL;
			case WM_KEYUP:
				m_pInput->KeyUp((unsigned int)a_wParam);
				return NULL;
			case WM_MOUSEMOVE:
				m_pInput->SetMousePosition(GET_X_LPARAM(a_lParam), GET_Y_LPARAM(a_lParam));
				return NULL;
			case WM_RBUTTONDOWN:
				m_pInput->SetRightMouseDown(true);
				return NULL;
			case WM_RBUTTONUP:
				m_pInput->SetRightMouseDown(false);
				return NULL;
			default:
				return DefWindowProc(a_hWindow, a_uMessage, a_wParam, a_lParam);
		}
	}

	void Renderer::InitializeWindows(int& a_nScreenWidth, int& a_nScreenHeight)
	{
		WNDCLASSEX windowClass;
		DEVMODE screenSettings;
		int xPos, yPos;

		g_sApplicationHandle = this;
		m_hInstance = GetModuleHandle(NULL);
		m_wsApplicationName = "Renderer";

		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		windowClass.lpfnWndProc = WndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = m_hInstance;
		windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		windowClass.hIconSm = windowClass.hIcon;
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = m_wsApplicationName;

		RegisterClassEx(&windowClass);

		a_nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		a_nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		if(FULL_SCREEN)
		{
			memset(&screenSettings, 0, sizeof(screenSettings));
			screenSettings.dmSize = sizeof(screenSettings);
			screenSettings.dmPelsWidth = (unsigned long)a_nScreenWidth;
			screenSettings.dmPelsHeight = (unsigned long)a_nScreenHeight;
			screenSettings.dmBitsPerPel = 32;
			screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);

			xPos = 0;
			yPos = 0;
		}
		else //(!FULLSCREEN)
		{
			a_nScreenWidth = SCREEN_WIDTH;
			a_nScreenHeight = SCREEN_HEIGHT;

			xPos = (GetSystemMetrics(SM_CXSCREEN) - a_nScreenWidth) / 2;
			yPos = (GetSystemMetrics(SM_CYSCREEN) - a_nScreenHeight) / 2;
		}

		m_hWindow = CreateWindowEx(	WS_EX_APPWINDOW,
									m_wsApplicationName,
									m_wsApplicationName,
									WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
									xPos,
									yPos,
									a_nScreenWidth,
									a_nScreenHeight,
									NULL,
									NULL,
									m_hInstance,
									NULL);

		ShowWindow(m_hWindow, SW_SHOW);
		SetForegroundWindow(m_hWindow);
		SetFocus(m_hWindow);
	}

	void Renderer::ShutdownWindows()
	{
		ShowCursor(true);

		if(FULL_SCREEN)
		{
			ChangeDisplaySettings(NULL, 0);
		}

		DestroyWindow(m_hWindow);
		m_hWindow = NULL;

		UnregisterClass(m_wsApplicationName, m_hInstance);
		m_hInstance = NULL;

		g_sApplicationHandle = NULL;
	}

	bool Renderer::ProcessFrame()
	{
		bool result;

		m_pTimer->ProcessFrame();
		m_pInput->ProcessFrame();

		if(m_pInput->IsKeyDown(VK_ESCAPE))
		{
			return false;
		}
		if(m_pInput->IsKeyDown('W'))
		{
			m_pGraphics->GetCamera()->SetMoveForward(true);
		}
		if(m_pInput->IsKeyDown('A'))
		{
			m_pGraphics->GetCamera()->SetMoveLeft(true);
		}
		if(m_pInput->IsKeyDown('S'))
		{
			m_pGraphics->GetCamera()->SetMoveBackward(true);
		}
		if(m_pInput->IsKeyDown('D'))
		{
			m_pGraphics->GetCamera()->SetMoveRight(true);
		}
		if(m_pInput->IsKeyDown('Q'))
		{
			m_pGraphics->GetCamera()->SetMoveUp(true);
		}
		if(m_pInput->IsKeyDown('E'))
		{
			m_pGraphics->GetCamera()->SetMoveDown(true);
		}
		if(!m_pInput->IsKeyDown('W'))
		{
			m_pGraphics->GetCamera()->SetMoveForward(false);
		}
		if(!m_pInput->IsKeyDown('A'))
		{
			m_pGraphics->GetCamera()->SetMoveLeft(false);
		}
		if(!m_pInput->IsKeyDown('S'))
		{
			m_pGraphics->GetCamera()->SetMoveBackward(false);
		}
		if(!m_pInput->IsKeyDown('D'))
		{
			m_pGraphics->GetCamera()->SetMoveRight(false);
		}
		if(!m_pInput->IsKeyDown('Q'))
		{
			m_pGraphics->GetCamera()->SetMoveUp(false);
		}
		if(!m_pInput->IsKeyDown('E'))
		{
			m_pGraphics->GetCamera()->SetMoveDown(false);
		}

		if(m_pInput->IsRightMouseDown())
		{
			m_pGraphics->GetCamera()->SetLookHorizontal(m_pInput->GetMouseDeltaX());
			m_pGraphics->GetCamera()->SetLookVertical(m_pInput->GetMouseDeltaY());
		}

		result = m_pGraphics->ProcessFrame(m_pTimer->GetTime());
		if(!result)
		{
			return false;
		}

		return true;
	}

	LRESULT CALLBACK WndProc(HWND a_hWindow, UINT a_uMessage, WPARAM a_wParam, LPARAM a_lParam)
	{
		switch(a_uMessage)
		{
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			case WM_CLOSE:
				PostQuitMessage(0);
				return 0;
			default:
				return g_sApplicationHandle->MessageHandler(a_hWindow, a_uMessage, a_wParam, a_lParam);
		}
	}
}