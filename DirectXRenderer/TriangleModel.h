#pragma once

/** @file TriangleModel.h
 ** Contains the TriangleModel class.*/

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "Model.h"

using namespace DirectX;

namespace Ab2
{
	/** Encapsulates loading a model and creating vertex and index buffers.*/
	class TriangleModel : public Model
	{
	public:
		/** Default constructor.*/
		TriangleModel();

		/** Destructor.*/
		~TriangleModel();

		/** Initializes the vertex and index buffers associated with the model.*/
		bool InitializeBuffers(ID3D11Device* a_pDevice);
	};
}