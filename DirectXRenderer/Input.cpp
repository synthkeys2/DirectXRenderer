#include "Input.h"

/** @file Input.cpp
 ** Input implementation.*/

namespace Ab2
{
	Input::Input()
		:	m_nLastMousePosX(0), m_nLastMousePosY(0), m_nCurrentMousePosX(0), m_nCurrentMousePosY(0),
			m_bLeftMouseDown(false), m_bRightMouseDown(false)
	{

	}

	Input::~Input()
	{

	}

	bool Input::Initialize()
	{
		for(int i = 0; i < 256; ++i)
		{
			m_aKeys[i] = false;
		}

		return true;
	}

	void Input::Shutdown()
	{

	}

	void Input::KeyDown(unsigned int a_nKey)
	{
		m_aKeys[a_nKey] = true;
	}

	void Input::KeyUp(unsigned int a_nKey)
	{
		m_aKeys[a_nKey] = false;
	}

	bool Input::IsKeyDown(unsigned int a_nKey)
	{
		return m_aKeys[a_nKey];
	}

	void Input::SetMousePosition(int a_nMousePositionX, int a_nMousePositionY)
	{
		m_nCurrentMousePosX = a_nMousePositionX;
		m_nCurrentMousePosY = a_nMousePositionY;
	}

	bool Input::ProcessFrame()
	{
		m_nMouseDeltaThisFrameX = m_nCurrentMousePosX - m_nLastMousePosX;
		m_nMouseDeltaThisFrameY = m_nCurrentMousePosY - m_nLastMousePosY;
		m_nLastMousePosX = m_nCurrentMousePosX;
		m_nLastMousePosY = m_nCurrentMousePosY;

		return true;
	}

	int Input::GetMouseDeltaX()
	{
		return m_nMouseDeltaThisFrameX;
	}

	int Input::GetMouseDeltaY()
	{
		return m_nMouseDeltaThisFrameY;
	}

	bool Input::IsLeftMouseDown()
	{
		return m_bLeftMouseDown;
	}

	bool Input::IsRightMouseDown()
	{
		return m_bRightMouseDown;
	}

	void Input::SetLeftMouseDown(bool a_bValue)
	{
		m_bLeftMouseDown = a_bValue;
	}

	void Input::SetRightMouseDown(bool a_bValue)
	{
		m_bRightMouseDown = a_bValue;
	}
}