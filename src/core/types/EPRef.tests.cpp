#include "TypesTestSuite.h"

#include "core/types/EPRef.h"

class dum {
public:
	dum() { DEBUG_LINEOUT("construct dum %d", ++dum_count); }
	virtual ~dum() { DEBUG_LINEOUT("destruct dum %d", --dum_count); }
	virtual RESULT Print() { DEBUG_LINEOUT("dum class"); return R::OK; }
public:
	static long dum_count;
};

class dumdum : public dum {
public:
	dumdum() { DEBUG_LINEOUT("construct dumdum %d", ++dum_count); }
	virtual ~dumdum() override { DEBUG_LINEOUT("destruct dumdum %d", --dum_count); }
	virtual RESULT Print() override { DEBUG_LINEOUT("dumdum class"); return R::OK; }
};

class dumdumdum : public dumdum {
public:
	dumdumdum() { DEBUG_LINEOUT("construct dumdumdum %d", ++dum_count); }
	virtual ~dumdumdum() override { DEBUG_LINEOUT("destruct dumdumdum %d", --dum_count); }
	virtual RESULT Print() override { DEBUG_LINEOUT("dumdumdum class"); return R::OK; }
};

long dum::dum_count = 0;

RESULT TypesTestSuite::TestEPRef(EPTestBase* pEPTestBase) {
	RESULT r = R::OK;

	dum* pDum = nullptr;
	dumdum* pDumDum = nullptr;
	dumdumdum* pDumDumDum = nullptr;

	EPTest<RESULT(EPTestBase*)>* pEPTest = dynamic_cast<EPTest<RESULT(EPTestBase*)>*>(pEPTestBase);
	CNM(pEPTest, "EPTest is nullptr");

	// Case - Create ref to each in scope and make sure memory gets dealloced 
	{
		EPRef<dum> eprDum = new dum();
		EPRef<dumdum> eprDumDum = new dumdum();
		EPRef<dumdumdum> eprDumDumDum = new dumdumdum();

		pDum = eprDum.get();
		pDumDum = eprDumDum.get();
		pDumDumDum = eprDumDumDum.get();
	}

	CBM(dum::dum_count == 0, "Dum count %d should be zero", dum::dum_count);

	// Case - Create a epr<dum> of a dumdum*
	{
		EPRef<dum> eprDum = new dumdum();
		eprDum->Print();
	}

	CBM(dum::dum_count == 0, "Dum count %d should be zero", dum::dum_count);

	// Case - Create a epr<dum> from a epr<dumdum>
	{
		EPRef<dumdum> eprDumDum = new dumdum();
		EPRef<dum> eprDum = eprDumDum;
		eprDum->Print();
	}

	CBM(dum::dum_count == 0, "Dum count %d should be zero", dum::dum_count);

	// Case - Construct epr<dumdum> from epr<dum> to dumdum object
	{
		EPRef<dum> eprDum = nullptr;
		eprDum = new dumdum();
		EPRef<dumdum> eprDumDum(eprDum);
		
		eprDumDum->Print();
	}

	CBM(dum::dum_count == 0, "Dum count %d should be zero", dum::dum_count);

Error:
	return r;
}