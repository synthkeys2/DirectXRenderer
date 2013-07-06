#pragma once

/** @file CubeModel.h
 ** Contains the CubeModel class.*/

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "Model.h"

using namespace DirectX;

namespace Ab2
{
	/** Encapsulates loading a model and creating vertex and index buffers.*/
	class CubeModel : public Model
	{
	public:
		/** Default constructor.*/
		CubeModel();

		/** Destructor.*/
		~CubeModel();

		/** Initializes the vertex and index buffers associated with the model.*/
		bool InitializeBuffers(ID3D11Device* a_pDevice);
	};
}