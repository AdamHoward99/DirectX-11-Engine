#include "Font.h"

TextFont::TextFont()
	:pFont(nullptr), pFontBatch(nullptr), fontColour(DirectX::Colors::White), fontRotation(0.0f)
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