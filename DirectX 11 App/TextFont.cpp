#include "TextFont.h"

TextFont::TextFont()
	:pFont(nullptr), pFontBatch(nullptr), fontColour(DirectX::Colors::White), fontRotation(0.0f), fontPosition(0.0f, 0.0f),
	fontOrigin(0.0f, 0.0f), fontScale(0.0f), fontLayerDepth(0.0f)
{}

TextFont::TextFont(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	pFont = new DirectX::SpriteFont(pDevice, L"Fonts\\Arial_16.spritefont");
	pFontBatch = new DirectX::SpriteBatch(pDeviceContext);
}

TextFont::TextFont(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const CWString fileName)
{
	pFont = new DirectX::SpriteFont(pDevice, fileName);
	pFontBatch = new DirectX::SpriteBatch(pDeviceContext);
}

TextFont::TextFont(const TextFont& otherFont)
{
	///Copy Constructor
	pFont = otherFont.pFont;
	pFontBatch = otherFont.pFontBatch;
	fontColour = otherFont.fontColour;
	fontRotation = otherFont.fontRotation;
	fontPosition = otherFont.fontPosition;
	fontOrigin = otherFont.fontOrigin;
	fontScale = otherFont.fontScale;
	fontLayerDepth = otherFont.fontLayerDepth;
}

TextFont::~TextFont()
{
	delete pFont;
	delete pFontBatch;

	pFont = nullptr;
	pFontBatch = nullptr;
}

TextFont& TextFont::operator=(const TextFont& fontA)
{
	///Copy Assignment Operator
	///Ensure this is not a self-assignment
	if (this != &fontA)
	{
		this->pFont = fontA.pFont;
		this->pFontBatch = fontA.pFontBatch;
		this->fontColour = fontA.fontColour;
		this->fontRotation = fontA.fontRotation;
		this->fontPosition = fontA.fontPosition;
		this->fontOrigin = fontA.fontOrigin;
		this->fontScale = fontA.fontScale;
		this->fontLayerDepth = fontA.fontLayerDepth;
	}

	return *this;
}

const void TextFont::DrawString(const CString& text)
{
	pFontBatch->Begin();
	pFont->DrawString(pFontBatch, text, DirectX::XMFLOAT2A(0, 0), DirectX::Colors::White, 0.0f);
	pFontBatch->End();
}

const void TextFont::DrawString(const CString& text, const DirectX::XMFLOAT2A& position, const DirectX::XMVECTOR& textColour, const float textRotation)
{
	pFontBatch->Begin();
	pFont->DrawString(pFontBatch, text, position, textColour, textRotation);
	pFontBatch->End();
}

void* TextFont::operator new(size_t i)
{
	///Aligns to 16-bits for unique_ptr creation
	return _mm_malloc(i, 16);
}

void TextFont::operator delete(void* p)
{
	_mm_free(p);
}

const DirectX::XMFLOAT2A& TextFont::GetFontPosition() const
{
	return fontPosition;
}

const void TextFont::SetFontPosition(const DirectX::XMFLOAT2A& newPosition)
{
	fontPosition = newPosition;
}

const float TextFont::GetFontRotation() const
{
	return fontRotation;
}

const void TextFont::SetFontRotation(const float newRotation)
{
	fontRotation = newRotation;
}

const DirectX::XMFLOAT2A& TextFont::GetFontOrigin() const
{
	return fontOrigin;
}

const void TextFont::SetFontOrigin(const DirectX::XMFLOAT2A& newOrigin)
{
	fontOrigin = newOrigin;
}

const float TextFont::GetFontScale() const
{
	return fontScale;
}

const void TextFont::SetFontScale(const float newScale)
{
	fontScale = newScale;
}

const float TextFont::GetFontLayerDepth() const
{
	return fontLayerDepth;
}

const void TextFont::SetFontLayerDepth(const float newLayerDepth)
{
	fontLayerDepth = newLayerDepth;
}

const DirectX::XMVECTOR& TextFont::GetFontColour() const
{
	return fontColour;
}

const void TextFont::SetFontColour(const DirectX::XMVECTOR& newColour)
{
	fontColour = newColour;
}
