#pragma once
/**********************************************************************/
#include "TLogRecord_base.h"
#include "TLogDatabase.h"
/**********************************************************************/

class CTLogRecord : public CTLogRecord_base
{
public:
	CTLogRecord();
	CTLogRecord(const char* szLine);

	bool ConvertRecord(CTLogDatabase* pDb, bool bFullConvert);

	CString GetXmlDate(bool bMilliSecondsReqd = FALSE);
	void GetCombinedValues(CString& strSalesText, double& dSalesValue);

private:
	CString MakeLabel(int nNo);
	void ConvertDataV1(CTLogDatabase* pDb, bool bFullConvert);
	void ConvertDataV2(CTLogDatabase* pDb, bool bFullConvert);
	void ConvertDataV3(CTLogDatabase* pDb, bool bFullConvert);
	void ConvertDataV4(CTLogDatabase* pDb, bool bFullConvert);
	void ConvertDataV5(CTLogDatabase* pDb, bool bFullConvert);
	void ConvertDataV6(CTLogDatabase* pDb, bool bFullConvert);
};

/**********************************************************************/
