#pragma once

/** @file Input.h
 ** Contains the Input class.*/

namespace Ab2
{
	/** Holds all input state.*/
	class Input
	{
	public:
		/** Default Constructor.*/
		Input();

		/** Destructor.*/
		~Input();

		/** Initialies Input components.*/
		bool Initialize();

		/** Cleans up Input components.*/
		void Shutdown();

		/** Sets a specific key's state to down.*/
		void KeyDown(unsigned int a_nKey);

		/** Sets a specific key's state to up.*/
		void KeyUp(unsigned int a_nKey);

		/** Checks the state of a key.*/
		bool IsKeyDown(unsigned int a_nKey);

		/** Checks the state of the left mouse button.*/
		bool IsLeftMouseDown();

		/** Checks the state of the right mouse button.*/
		bool IsRightMouseDown();

		/** Sets the position of the mouse, used for calculating the mouse delta position.*/
		void SetMousePosition(int a_nMousePositionX, int a_nMousePositionY);

		/** Sets the state of the left mouse button.*/
		void SetLeftMouseDown(bool a_bValue);

		/** Sets the state of the right mouse button.*/
		void SetRightMouseDown(bool a_bValue);

		/** Gets the amount that the mouse has moved horizontally in the last frame.*/
		int GetMouseDeltaX();

		/** Gets the amount that the mouse has moved vertically in the last frame.*/
		int GetMouseDeltaY();

		/** Calculates any frame based data like mouse delta.*/
		bool ProcessFrame();

	private:
		bool m_aKeys[256];
		bool m_bLeftMouseDown;
		bool m_bRightMouseDown;
		int m_nLastMousePosX;
		int m_nLastMousePosY;
		int m_nCurrentMousePosX;
		int m_nCurrentMousePosY;
		int m_nMouseDeltaThisFrameX;
		int m_nMouseDeltaThisFrameY;
	};
}