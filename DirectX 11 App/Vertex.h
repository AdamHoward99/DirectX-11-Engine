#pragma once
#include <DirectXMath.h>

class Vertex
{
public:
	Vertex(){}
	Vertex(DirectX::XMFLOAT2 vertPos, DirectX::XMFLOAT3 vertColour);

	DirectX::XMFLOAT2 vertexPosition;
	DirectX::XMFLOAT3 vertexColour;
};