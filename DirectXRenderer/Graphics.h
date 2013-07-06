#pragma once

/** @file Graphics.h
 ** Contains the Graphics class.*/

#include <Windows.h>
#include "D3DCore.h"
#include "Camera.h"
#include "TriangleModel.h"
#include "CubeModel.h"
#include "AssimpModel.h"
#include "ColorShader.h"
#include "AmbientLighting.h"
#include "SkyBox.h"
#include "ModelMaterial.h"
#include "SceneSettings.h"

namespace Ab2
{
	const bool FULL_SCREEN = false;
	const unsigned int SCREEN_WIDTH = 800;
	const unsigned int SCREEN_HEIGHT = 600;
	const bool VSYNC_ENABLED = true;
	const float FAR_PLANE = 1000.0f;
	const float NEAR_PLANE = 0.1f;

	/** Contains the D3D subsystem, camera, and manages models and shaders.*/
	class Graphics
	{
	public:
		/** Default constructor.*/
		Graphics();

		/** Destructor.*/
		~Graphics();

		/** Initializes all Graphics components.*/
		bool Initialize(int a_nScreenWidth, int a_nScreenHeight, HWND a_hWindow);
		
		/** Cleans up all Graphics components.*/
		void Shutdown();

		/** Attempts to render by setting up the camera, matrices, and any states or shaders.*/
		bool ProcessFrame(float a_fDeltaTime);

		/** Getter for the camera.*/
		Camera* GetCamera();

	private:
		bool Render(float a_fDeltaTime);

		D3DCore* m_pD3DCore;
		Camera* m_pCamera;
		Model* m_pTriangleModel;
		Model* m_pSkyBoxModel;
		Model* m_pAssimpModel;
		Material* m_pSkyBoxMaterial;
		Material* m_pTriangleMaterial;
		Material* m_pAssimpMaterial;

		ID3D11RasterizerState* m_pRSWireFrame;

		SceneSettings* m_pSceneSettings;

	};
}