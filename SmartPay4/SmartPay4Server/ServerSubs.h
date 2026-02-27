#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//**********************************************************************

class CServerSubs
{
public:
	//ACCOUNT SHOULD COME FROM GLOBAL OBJECT FOR SERVER
	//GROUP MAY BE DAILY OR ACCOUNT DEFAULT, SO MUST BE SPECIFIED
	static bool IsPurseAllowed(int nPurseNo, CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup, CSSDate& date, CSSTime& time);
	static void GetMaxSpendFlags(CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup, bool& bDaily, bool& bGroup);
};

//**********************************************************************
