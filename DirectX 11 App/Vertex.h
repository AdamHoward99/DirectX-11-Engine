#pragma once
#include <DirectXMath.h>

class Vertex
{
public:
	Vertex(){}
	Vertex(DirectX::XMFLOAT3 vertPos, DirectX::XMFLOAT2 texCoord, DirectX::XMFLOAT3 normPos);

	DirectX::XMFLOAT3 vertexPosition;
	DirectX::XMFLOAT2 textureCoordinates;
	DirectX::XMFLOAT3 normalCoordinates;
	DirectX::XMFLOAT3 tangentCoordinates;
};