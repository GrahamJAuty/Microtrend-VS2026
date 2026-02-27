#pragma once
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************

class CDeleteRecord
{
public:
	CDeleteRecord ( CSQLRowAccountFull& SQLRowAccount, CString strComment, CString strLeaversYear, CWnd* pParent );
};

//*******************************************************************
