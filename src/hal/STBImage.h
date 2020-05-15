#ifndef STB_IMAGE_H_
#define STB_IMAGE_H_

#include "core/ehm/result.h"

// epoch hal stb image
// epoch/src/hal/STBImage.h

#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

#include "image.h"
#include "stb_image.h"

class STBImage : 
	public image,
	public EPFactoryMethod<STBImage,
		const EPString<char>&
	>
{
public:
	STBImage(const EPString<char>& strImageFilename) :
		image(strImageFilename)
	{
		//
	}

	~STBImage();

	virtual RESULT Initialize() override;

	static EPRef<STBImage> InternalMake(const EPString<char>& strImageFilename);

	virtual void* data() const override {
		return static_cast<void*>(m_pPixelData);
	}

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

private:
	EPString<char> m_strImageFilename;

	int m_width = 0;
	int m_height = 0;
	int m_channels = 0;
	int m_bytedepth = 0;

	stbi_uc* m_pPixelData = nullptr;
};


#endif // ! STB_IMAGE_H_