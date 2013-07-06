#pragma once

/** @file Timer.h
 ** Holds the Timer class.*/

#include <Windows.h>

namespace Ab2
{
	/** Encapsulates reading system time.*/
	class Timer
	{
	public:
		/** Default constructor.*/
		Timer();

		/** Destructor.*/
		~Timer();

		/** Read the system time info.*/
		bool Initialize();

		/** Compute the time since last frame.*/
		void ProcessFrame();

		/** Get the current frame time.*/
		float GetTime();
	
	private:
		INT64 m_nFrequency;
		float m_fTicksPerMs;
		INT64 m_nStartTime;
		float m_fFrameTime;
	};
}