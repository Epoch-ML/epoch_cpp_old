#include "epuid.h"

#include "core/ehm/ehm.h"

// Initialize the static value
uint64_t epuid::ms_EPUIDCount = (uint64_t)(0);

epuid::epuid() :
	m_localUID(GetLocalEPUIDValue())
{
	GetGlobalEPUIDValue();
}

epuid::~epuid() {
	// TODO: release local and global IDs potentially
}

// 
uint64_t epuid::GetLocalEPUIDValue() {
#ifndef EP_ALLOW_ZERO_LOCAL_UID
	return ++ms_EPUIDCount;
#else
	return ++ms_EPUIDCount;
#endif
}

#if defined(_WIN32) || defined(_WIN64)

#include <combaseapi.h>

RESULT epuid::InitializeGUIDImpl() {
	
	GUID tempWindowsGUID;

	CoCreateGuid(&tempWindowsGUID);

	uint8_t guidBytes[16] = {
		(unsigned char)((tempWindowsGUID.Data1 >> 24) & 0xFF),
		(unsigned char)((tempWindowsGUID.Data1 >> 16) & 0xFF),
		(unsigned char)((tempWindowsGUID.Data1 >> 8) & 0xFF),
		(unsigned char)((tempWindowsGUID.Data1) & 0xff),

		(unsigned char)((tempWindowsGUID.Data2 >> 8) & 0xFF),
		(unsigned char)((tempWindowsGUID.Data2) & 0xff),

		(unsigned char)((tempWindowsGUID.Data3 >> 8) & 0xFF),
		(unsigned char)((tempWindowsGUID.Data3) & 0xFF),

		(unsigned char)tempWindowsGUID.Data4[0],
		(unsigned char)tempWindowsGUID.Data4[1],
		(unsigned char)tempWindowsGUID.Data4[2],
		(unsigned char)tempWindowsGUID.Data4[3],
		(unsigned char)tempWindowsGUID.Data4[4],
		(unsigned char)tempWindowsGUID.Data4[5],
		(unsigned char)tempWindowsGUID.Data4[6],
		(unsigned char)tempWindowsGUID.Data4[7]
	};

	memcpy(&m_globalUID.bytes, guidBytes, (sizeof(m_globalUID.bytes) / sizeof(m_globalUID.bytes[0])));

	return R::OK;
}
#endif

RESULT epuid::GetGlobalEPUIDValue() {
	RESULT r = R::OK;

	// Allocate a GUID

#if defined(_WIN32) || defined(_WIN64)

#else
	// TODO: this
	//#error GUID functionality not implemented for this platform
	CBRM(false, R::PLATFORM_NOT_SUPPORTED, "GetGlobalEPUIDValue not supported on this platfom");
#endif

Error:
	return r;
}