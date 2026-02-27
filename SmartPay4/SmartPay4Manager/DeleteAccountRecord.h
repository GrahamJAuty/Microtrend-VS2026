#pragma once
//********************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//********************************************************************

class CDeleteAccountRecord
{
public:
	CDeleteAccountRecord ( CWnd* pParent );
	
public:
	void DeleteRecord ( __int64 nUserID );

private:
	CWnd* m_pParent;
};

//********************************************************************
