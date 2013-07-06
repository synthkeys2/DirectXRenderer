#include "Camera.h"

/** @file Camera.cpp
 ** Camera Implementation.*/

namespace Ab2
{
	Camera::Camera()
		:	m_vPosition(0.0f, 0.0f, 0.0f), m_vUp(0, 1, 0), m_vForward(0, 0, 1),
			m_bMoveBackward(false), m_bMoveDown(false), m_bMoveForward(false), m_bMoveLeft(false), 
			m_bMoveRight(false), m_bMoveUp(false), m_nLookHorizontal(0), m_nLookVertical(0)
	{

	}

	Camera::~Camera()
	{

	}

	void Camera::SetPosition(XMFLOAT3 a_vPosition)
	{
		m_vPosition = a_vPosition;
	}

	XMFLOAT3 Camera::GetPosition()
	{
		return m_vPosition;
	}

	void Camera::Render(float a_fDeltaTime)
	{
		XMVECTOR U = XMLoadFloat3(&m_vUp);
		XMVECTOR F = XMLoadFloat3(&m_vForward);
		XMVECTOR P = XMLoadFloat3(&m_vPosition);	

		F = XMVector3Normalize(F);
		U = XMVector3Normalize(U);
		XMVECTOR R = XMVector3Cross(U, F);

		XMMATRIX R1 = XMMatrixRotationAxis(R, a_fDeltaTime * ROTATION_CONSTANT * m_nLookVertical);
		XMMATRIX R2 = XMMatrixRotationY(a_fDeltaTime * ROTATION_CONSTANT * m_nLookHorizontal);

		U = XMVector3TransformNormal(U, R1);
		U = XMVector3TransformNormal(U, R2);
		F = XMVector3TransformNormal(F, R1);
		F = XMVector3TransformNormal(F, R2);

		R = XMVector3Cross(U, F);


		if(m_bMoveForward)
		{
			P += a_fDeltaTime * MOVEMENT_CONSTANT * F;
		}
		if(m_bMoveBackward)
		{
			P -= a_fDeltaTime * MOVEMENT_CONSTANT * F;
		}
		if(m_bMoveUp)
		{
			P += a_fDeltaTime * MOVEMENT_CONSTANT * U;
		}
		if(m_bMoveDown)
		{
			P -= a_fDeltaTime * MOVEMENT_CONSTANT * U;
		}
		if(m_bMoveLeft)
		{
			P -= a_fDeltaTime * MOVEMENT_CONSTANT * R;
		}
		if(m_bMoveRight)
		{
			P += a_fDeltaTime * MOVEMENT_CONSTANT * R;
		}


		m_nLookHorizontal = 0;
		m_nLookVertical = 0;


		float x = -XMVectorGetX(XMVector3Dot(P, R));
		float y = -XMVectorGetX(XMVector3Dot(P, U));
		float z = -XMVectorGetX(XMVector3Dot(P, F));

		XMStoreFloat3(&m_vRight, R);
		XMStoreFloat3(&m_vUp, U);
		XMStoreFloat3(&m_vForward, F);
		XMStoreFloat3(&m_vPosition, P);

		m_mViewMatrix(0,0) = m_vRight.x; 
		m_mViewMatrix(1,0) = m_vRight.y; 
		m_mViewMatrix(2,0) = m_vRight.z; 
		m_mViewMatrix(3,0) = x;   

		m_mViewMatrix(0,1) = m_vUp.x;
		m_mViewMatrix(1,1) = m_vUp.y;
		m_mViewMatrix(2,1) = m_vUp.z;
		m_mViewMatrix(3,1) = y;  

		m_mViewMatrix(0,2) = m_vForward.x; 
		m_mViewMatrix(1,2) = m_vForward.y; 
		m_mViewMatrix(2,2) = m_vForward.z; 
		m_mViewMatrix(3,2) = z;   

		m_mViewMatrix(0,3) = 0.0f;
		m_mViewMatrix(1,3) = 0.0f;
		m_mViewMatrix(2,3) = 0.0f;
		m_mViewMatrix(3,3) = 1.0f;
	}

	void Camera::GetViewMatrix(XMFLOAT4X4& a_mViewMatrix)
	{
		a_mViewMatrix = m_mViewMatrix;
	}

	void Camera::SetMoveUp(bool a_bValue)
	{
		m_bMoveUp = a_bValue;
	}

	void Camera::SetMoveDown(bool a_bValue)
	{
		m_bMoveDown = a_bValue;
	}

	void Camera::SetMoveForward(bool a_bValue)
	{
		m_bMoveForward = a_bValue;
	}

	void Camera::SetMoveBackward(bool a_bValue)
	{
		m_bMoveBackward = a_bValue;
	}

	void Camera::SetMoveLeft(bool a_bValue)
	{
		m_bMoveLeft = a_bValue;
	}

	void Camera::SetMoveRight(bool a_bValue)
	{
		m_bMoveRight = a_bValue;
	}

	void Camera::SetLookHorizontal(int a_nValue)
	{
		m_nLookHorizontal = a_nValue;
		if(m_nLookHorizontal > 0)
		{
			m_nLookHorizontal++;
		}
	}

	void Camera::SetLookVertical(int a_nValue)
	{
		m_nLookVertical = a_nValue;
	}
}