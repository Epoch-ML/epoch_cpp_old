#include "STBImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

RESULT STBImage::Initialize() {
	RESULT r = R::OK;

	EPString<char> strFilePath = EPString<char>(EPOCH_TEXTURES_DIR) + "/" + m_strImageFilename;

	m_pPixelData = stbi_load(
		strFilePath.c_str(),
		&m_width, 
		&m_height, 
		&m_channels, 
		STBI_rgb_alpha
	);

	// STBI_rgb_alpha creates an alpha channel for us automagically 
	m_channels = 4;

	CNM(m_pPixelData, "Failed to load %s", strFilePath.c_str());

	m_bytedepth = 1;

Error:
	return r;
}

EPRef<STBImage> STBImage::InternalMake(
	const EPString<char>& strImageFilename
) {
	RESULT r = R::OK;
	EPRef<STBImage> pSTBImage = nullptr;

	pSTBImage = new STBImage(strImageFilename);
	CNM(pSTBImage, "Failed to allocate image");

	CRM(pSTBImage->Initialize(), "Failed to initialize image");

Success:
	return pSTBImage;

Error:
	pSTBImage = nullptr;
	return nullptr;
}

STBImage::~STBImage() {

	if (m_pPixelData != nullptr) {
		stbi_image_free(m_pPixelData);
		m_pPixelData = nullptr;
	}

}