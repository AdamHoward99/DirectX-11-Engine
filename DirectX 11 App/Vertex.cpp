#include "Vertex.h"

Vertex::Vertex(DirectX::XMFLOAT3 vertexPos, DirectX::XMFLOAT2 texCoord, DirectX::XMFLOAT3 normalPos)
{
	vertexPosition = vertexPos;
	textureCoordinates = texCoord;
	normalCoordinates = normalPos;
}