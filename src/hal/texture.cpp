#include "texture.h"

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

	m_pImage = image::make(m_strFilename);
	CNM(m_pImage, "Failed to load image");

Error:
	return r;
}