#include "texture.h"

// TODO: More robust pattern
#include "STBImage.h"

texture::texture(const EPString<char>& strFilename) :
	m_strFilename(strFilename)
{
	//
}

texture::~texture() {
	m_pImage = nullptr;
}

RESULT texture::LoadImage() {
	RESULT r = R::OK;

	m_pImage = STBImage::make(m_strFilename);
	CNM(m_pImage, "Failed to load image");

	m_width = static_cast<uint32_t>(m_pImage->GetWidth());
	m_height = static_cast<uint32_t>(m_pImage->GetHeight());

Error:
	return r;
}