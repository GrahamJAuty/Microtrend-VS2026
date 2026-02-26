#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\TLogRecord_base.h"
//*******************************************************************
#include "TLogDatabase.h"
//**********************************************************************

class CTLogRecord : public CTLogRecord_base
{
public:
	CTLogRecord(void);
	CTLogRecord(CString strLine);

	bool ConvertRecord(CTLogDatabase* pDb);
	bool GetCombinedValues(CString& strSalesText, double& dSalesValue);

private:
	void ConvertDataV1(CTLogDatabase* pDb);
	void ConvertDataV2(CTLogDatabase* pDb);
	void ConvertDataV3(CTLogDatabase* pDb);
	void ConvertDataV4(CTLogDatabase* pDb);
	void ConvertDataV5(CTLogDatabase* pDb);
	void ConvertDataV6(CTLogDatabase* pDb);
	void ConvertDataV7(CTLogDatabase* pDb);

private:
	CString MakeLabel(int nNo);
	void ConvertItemData(CTLogDatabase* pDb, int nCount, int nBaseField, int nItemFields);
};

//**********************************************************************
