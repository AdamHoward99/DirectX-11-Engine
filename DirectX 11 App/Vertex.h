#pragma once
#include <DirectXMath.h>

class Vertex
{
public:
	Vertex(){}
	Vertex(DirectX::XMFLOAT3 vertPos, DirectX::XMFLOAT3 vertColour);

	DirectX::XMFLOAT3 vertexPosition;
	DirectX::XMFLOAT3 vertexColour;
};