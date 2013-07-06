#pragma once

/** @file Renderer.h
 ** Contains the Renderer class.*/

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "Graphics.h"
#include "Input.h"
#include "Timer.h"

namespace Ab2
{
	/** A Rendering application.*/
	class Renderer
	{
	public:
		/** Default Constructor.*/
		Renderer();

		/** Destructor.*/
		~Renderer();

		/** Initializes all components of the renderer.*/
		bool Initialize();

		/** Shuts down all components of the renderer.*/
		void Shutdown();

		/** Holds the main loop.*/
		void Run();

		/** A custom windows event handler, handles all types of key presses.*/
		LRESULT CALLBACK MessageHandler(HWND a_hWindow, UINT a_uMessage, WPARAM a_wParam, LPARAM a_lParam);

	private:
		/** Initializes windows in order to run the renderer on top of it.*/
		void InitializeWindows(int& a_nScreenWidth, int& a_nScreenHeight);

		/** Cleans up any components dealing with windows.*/
		void ShutdownWindows();

		/** Main update function.*/
		bool ProcessFrame();

		LPCTSTR m_wsApplicationName;
		HINSTANCE m_hInstance;
		HWND m_hWindow;

		Input *m_pInput;
		Graphics* m_pGraphics;
		Timer* m_pTimer;
	};

	static LRESULT CALLBACK WndProc(HWND a_hWindow, UINT a_uMessage, WPARAM a_wParam, LPARAM a_lParam);
	static Renderer* g_sApplicationHandle = NULL;
}