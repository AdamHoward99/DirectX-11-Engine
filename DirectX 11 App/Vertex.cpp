#include "Vertex.h"

Vertex::Vertex(DirectX::XMFLOAT3 vertexPos, DirectX::XMFLOAT2 texCoord)
{
	vertexPosition = vertexPos;
	textureCoordinates = texCoord;
}