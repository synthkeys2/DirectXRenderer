#include "Timer.h"

/** @file Timer.cpp
 ** Timer implementation.*/

namespace Ab2
{
	Timer::Timer()
	{

	}

	Timer::~Timer()
	{

	}

	bool Timer::Initialize()
	{
		QueryPerformanceFrequency((LARGE_INTEGER*)&m_nFrequency);
		if(m_nFrequency == 0)
		{
			return false;
		}

		m_fTicksPerMs = (float)(m_nFrequency / 1000);

		QueryPerformanceCounter((LARGE_INTEGER*)&m_nStartTime);

		return true;
	}

	void Timer::ProcessFrame()
	{
		INT64 nCurrentTime;
		float fDifference;

		QueryPerformanceCounter((LARGE_INTEGER*)&nCurrentTime);
		fDifference = (float)(nCurrentTime - m_nStartTime);
		m_fFrameTime = fDifference / m_fTicksPerMs;
		m_nStartTime = nCurrentTime;
	}

	float Timer::GetTime()
	{
		return m_fFrameTime;
	}
}