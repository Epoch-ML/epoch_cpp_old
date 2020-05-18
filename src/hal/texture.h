#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "core/ehm/ehm.h"

// epoch hal texture
// epoch/src/hal/texture.h

#include "core/types/EPObj.h"
#include "core/types/EPRef.h"
#include "core/types/EPString.h"

//#include "image.h"

// TODO: Fix this
class STBImage;

class texture : 
	public EPObj
{
public:
	texture(const EPString<char>& strFilename);
	virtual ~texture();

public:
	virtual RESULT Initialize() = 0;
	virtual RESULT Kill() = 0;

protected:
	RESULT LoadImage();

protected:
	EPString<char> m_strFilename;
	EPRef<STBImage> m_pImage = nullptr;

	uint32_t m_width = 0;
	uint32_t m_height = 0;
};


#endif // ! TEXTURE_H_