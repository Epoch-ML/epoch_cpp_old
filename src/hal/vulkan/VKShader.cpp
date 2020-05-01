#include "VKShader.h"

#include <fstream>

RESULT VKShader::Initialize() {
	RESULT r = R::OK;
	EPVector<char> buffer;
	size_t fileShader_n = 0;

	EPString<char> strFilenamePath = EPString<char>(EPOCHROOTDIR) + m_strFilename;

	std::ifstream fileShader(m_strFilename.c_str(), std::ios::ate | std::ios::binary);
	CBM(fileShader.is_open(), "Failed to open %s", m_strFilename.c_str());

	// ingest file contents
	fileShader_n = (size_t)fileShader.tellg();
	buffer = EPVector<char>(fileShader_n, true);
	fileShader.seekg(0);
	fileShader.read(buffer.data(), fileShader_n);

	fileShader.close();
	CBM(fileShader.is_open() == false, "Failed to close %s", m_strFilename.c_str());

	// Do stuff with buffer

Error:
	return r;
}

RESULT VKShader::Kill() {
	RESULT r = R::OK;

	//

Error:
	return r;
}

EPRef<VKShader> VKShader::InternalMake(const EPString<char> &strFilename) {
	RESULT r = R::OK;
	EPRef<VKShader> pVKShader = nullptr;

	pVKShader = new VKShader(strFilename);
	CNM(pVKShader, "Failed to allocate shader");

	CRM(pVKShader->Initialize(), "Failed to initialize VK shader");

Success:
	return pVKShader;

Error:
	pVKShader = nullptr;
	return nullptr;
}