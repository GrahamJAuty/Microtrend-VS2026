#pragma once
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SPosLoyalty4Shared\AlertText_base.h"
//$$******************************************************************

class CAlertText : public CAlertText_base
{
public:
	CAlertText(){}
	
public:
	static bool ValidateRecord(CSQLRowAccountFull& RowAccount, CString strAlertFilter);
	static void ConvertFilterStringToArray(CString str, CArray<int, int>& array);

private:
	static CString CreateAlertStatusString(CSQLRowAccountFull& RowAccount);
};

//$$******************************************************************
