#include "image.h"

RESULT image::Initialize() {
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

EPRef<image> image::InternalMake(
	const EPString<char>& strImageFilename
) {
	RESULT r = R::OK;
	EPRef<image> pImage = nullptr;

	pImage = new image(strImageFilename);
	CNM(pImage, "Failed to allocate image");

	CRM(pImage->Initialize(), "Failed to initialize image");

Success:
	return pImage;

Error:
	pImage = nullptr;
	return nullptr;
}

image::~image() {

	if (m_pPixelData != nullptr) {
		stbi_image_free(m_pPixelData);
		m_pPixelData = nullptr;
	}

}