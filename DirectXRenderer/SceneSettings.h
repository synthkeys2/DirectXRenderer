#pragma once

/** @file SceneSettings.h
 ** Holds the SceneSettings struct.*/

#include <DirectXMath.h>

using namespace DirectX;

namespace Ab2
{
	struct SceneSettings
	{
	public:
		XMFLOAT4 DefaultColor;
		XMFLOAT4 BackgroundColor;
		XMFLOAT4 AmbientIntensity;
	};
}