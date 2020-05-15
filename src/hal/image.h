#ifndef IMAGE_H_
#define IMAGE_H_

#include "core/ehm/result.h"

// epoch hal image
// epoch/src/hal/image.h

#include "core/types/EPObj.h"

#include "core/types/EPString.h"

class image :
	public EPObj
{
public:
	
	// TODO: Is this the best way
	image() = default;

	image(uint32_t width, uint32_t height, uint32_t channels = 4, uint32_t bytedepth = 1) :
		m_width(width),
		m_height(height),
		m_channels(channels),
		m_bytedepth(bytedepth)
	{
		// 
	}

	image(const EPString<char>& strImageFilename) :
		m_strImageFilename(strImageFilename)
	{}

	~image() = default;

	virtual RESULT Initialize() = 0;
	virtual RESULT Kill() { return R::NOT_IMPLEMENTED; }

	static EPRef<image> InternalMake(const EPString<char>& strImageFilename);

	size_t byte_size() const {
		return m_width * m_height * m_channels * m_bytedepth;
	}

	virtual void* data() const {
		return nullptr;
	}

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

protected:
	EPString<char> m_strImageFilename;

	int m_width = 0;
	int m_height = 0;
	int m_channels = 0;
	int m_bytedepth = 0;
};


#endif // ! IMAGE_H_