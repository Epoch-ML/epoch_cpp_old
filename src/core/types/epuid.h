#ifndef EP_UID_H_
#define EP_UID_H_

#include "core/ehm/result.h"

// epoch types: epuid
// epoch/src/core/types/epuid.h

// Simple encapsulation of a epoch UID

class epuid {
private:
	// Initialized in the source 
	static uint64_t ms_EPUIDCount;

	static uint64_t GetLocalEPUIDValue();
	
	// GUID
	RESULT GetGlobalEPUIDValue();
	RESULT epuid::InitializeGUIDImpl() {

public:
	epuid();
	~epuid();

private:
	uint64_t m_localUID = 0;

	struct guid {
		uint8_t bytes[16] = { 0 };
	} m_globalUID = { 0 };
};

#endif // EP_UID_H_