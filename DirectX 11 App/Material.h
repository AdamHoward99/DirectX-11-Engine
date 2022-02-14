#pragma once
#include <string>
#include <vector>
#include "ConstantBufferData.h"
#include "Texture.h"

struct Material
{
	std::string matName;
	MaterialData matData;
	std::vector<Texture> matTextures;
};
