#ifndef IMAGE_H_
#define IMAGE_H_

#include "core/ehm/result.h"

// epoch hal image
// epoch/src/hal/image.h

#include "core/types/EPObj.h"

#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

// TODO: Currently hard coded to STB
// but should also support FreeImage in the future

#include "stb_image.h"

class image : 
	public EPObj,
	public EPFactoryMethod<image,
		const EPString<char>&
	>
{
public:
	image(const EPString<char>& strImageFilename) :
		m_strImageFilename(strImageFilename)
	{
		//
	}

	~image();

	RESULT Initialize();

	static EPRef<image> InternalMake(const EPString<char>& strImageFilename);

	size_t byte_size() const {
		return m_width * m_height * m_channels * m_bytedepth;
	}

	void* data() const {
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


#endif // ! IMAGE_H_