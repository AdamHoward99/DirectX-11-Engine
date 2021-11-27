#pragma once
#include <DirectXMath.h>

class Vertex
{
public:
	Vertex(){}
	Vertex(float, float);

	DirectX::XMFLOAT2 vertexPosition;
};