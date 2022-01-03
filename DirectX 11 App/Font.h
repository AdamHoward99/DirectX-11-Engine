#pragma once
#include <d3d11.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <spriteFont.h>
#include "StringConverter.h"

class Font
{
public:
	///Default Constructor
	Font();
	///Constructor to create SpriteFont & SpriteBatch using default engine font (Arial-16)
	Font(const ID3D11Device* pDevice, const ID3D11DeviceContext* pDeviceContext);
	///Constructor to create SpriteFont & SpriteBatch given a font file
	Font(const ID3D11Device* pDevice, const ID3D11DeviceContext* pDeviceContext, const wchar_t* fileName);
	///Copy Constructor Operator
	Font(const Font& otherFont);
	///Default Destructor
	~Font();
	///Copy Assignment Operator
	Font& operator=(const Font& fontA);
	///Function that uses SpriteFont::DrawString(), only passes string to be outputted
	const void DrawString(const CString& text);
	///Overload function of DrawString() which passes desired position, colour, and origin
	const void DrawString(const CString& text, const DirectX::XMFLOAT2A& position, const DirectX::XMVECTOR& textColour = DirectX::Colors::White,
		const DirectX::XMFLOAT2A& textOrigin = DirectX::XMFLOAT2A(0.f, 0.f));

private:
	DirectX::SpriteFont* font;
	DirectX::SpriteBatch* spBatch;

	DirectX::XMVECTOR fontColour;
	DirectX::XMFLOAT2A fontOrigin;

	/*
	fontsize
	fontscale
	fontrotation?

	spriteeffects?
	fontBorder?
	fontOutline
	*/
};