#pragma once
#include <DirectXMath.h>

struct VS_CB_DATA		///Notice: Needs to be 16-bit aligned to work
{
	float xOff = 0;
	float yOff = 0;
	long long int byteOffset = 0;		//Only being used for byte offset of 16
};