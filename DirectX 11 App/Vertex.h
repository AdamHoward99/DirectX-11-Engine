#pragma once
#include <DirectXMath.h>

class Vertex
{
	Vertex(){}
	Vertex(float, float, float);

	DirectX::XMFLOAT2 vertexPosition;
};