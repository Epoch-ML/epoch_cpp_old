#include "STBImage.h"

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