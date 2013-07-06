#pragma once

#include <DirectXMath.h>

/** @file Camera.h
 ** Holds the Camera class.*/

using namespace DirectX;

#define MOVEMENT_CONSTANT 0.01f
#define ROTATION_CONSTANT 0.0001f

namespace Ab2
{
	/** A generic camera class holding the view matrix and updating it according to inputs.*/
	class Camera
	{
	public:
		/** Default constructor.*/
		Camera();

		/** Destructor.*/
		~Camera();

		/** Setter for camera position.*/
		void SetPosition(XMFLOAT3 a_vPosition);

		/** Setter for camera rotation.*/
		void SetRotation(XMFLOAT3 a_vRotation);

		/** Getter for camera position.*/
		XMFLOAT3 GetPosition();

		/** Getter for camera rotation.*/
		XMFLOAT3 GetRotation();

		/** Prepares the view matrix.*/
		void Render(float a_fDeltaTime);

		/** Getter for the view matrix.*/
		void GetViewMatrix(XMFLOAT4X4& a_mViewMatrix);

		/** Booleans for controlling the camera movement.
		 ** Meant to be set by the renderer.*/
		void SetMoveForward(bool a_bValue);
		void SetMoveBackward(bool a_bValue);
		void SetMoveUp(bool a_bValue);
		void SetMoveDown(bool a_bValue);
		void SetMoveLeft(bool a_bValue);
		void SetMoveRight(bool a_bValue);
		void SetLookHorizontal(int a_nValue);
		void SetLookVertical(int a_nValue);

	private:
		XMFLOAT3 m_vPosition;
		XMFLOAT3 m_vUp;
		XMFLOAT3 m_vRight;
		XMFLOAT3 m_vForward;
		XMFLOAT4X4 m_mViewMatrix;

		bool m_bMoveForward;
		bool m_bMoveBackward;
		bool m_bMoveUp;
		bool m_bMoveDown;
		bool m_bMoveLeft;
		bool m_bMoveRight;

		int m_nLookHorizontal;
		int m_nLookVertical;
	};
}