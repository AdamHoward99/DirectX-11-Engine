#pragma once
#include <d3d11.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <spriteFont.h>
#include "StringConverter.h"

class TextFont
{
public:
	///Default Constructor
	TextFont();
	///Constructor to create SpriteFont & SpriteBatch using default engine font (Arial-16)
	TextFont(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	///Constructor to create SpriteFont & SpriteBatch given a font file
	TextFont(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const CWString fileName);
	///Copy Constructor Operator
	TextFont(const TextFont& otherFont);
	///Default Destructor
	~TextFont();
	///Copy Assignment Operator
	TextFont& operator=(const TextFont& fontA);
	///Function that uses SpriteFont::DrawString(), only passes string to be outputted
	const void DrawString(const CString& text);
	///Overload function of DrawString() which passes desired position, colour, and origin
	const void DrawString(const CString& text, const DirectX::XMFLOAT2A& position, const DirectX::XMVECTOR& textColour = DirectX::Colors::White,
		const float textRotation = 0.0f);

	///Operators required for 16-bit alignment
	void* operator new(size_t i);
	void operator delete(void* p);

	///Encapsulation functions for private font component variables
	const float GetFontRotation() const;
	const float GetFontScale() const;
	const float GetFontLayerDepth() const;
	const DirectX::XMFLOAT2A& GetFontPosition() const;
	const DirectX::XMFLOAT2A& GetFontOrigin() const;
	const DirectX::XMVECTOR& GetFontColour() const;

	const void SetFontRotation(const float newRotation);
	const void SetFontScale(const float newScale);
	const void SetFontLayerDepth(const float newLayerDepth);
	const void SetFontPosition(const DirectX::XMFLOAT2A& newPosition);
	const void SetFontOrigin(const DirectX::XMFLOAT2A& newOrigin);
	const void SetFontColour(const DirectX::XMVECTOR& newColour);

private:
	DirectX::SpriteFont* pFont;
	DirectX::SpriteBatch* pFontBatch;

	///Variables to edit components of Font
	float fontRotation;
	float fontScale;
	float fontLayerDepth;
	DirectX::XMFLOAT2A fontPosition;
	DirectX::XMFLOAT2A fontOrigin;
	DirectX::XMVECTOR fontColour;

	/*
	fontsize
	fontBorder?
	fontOutline
	*/
};