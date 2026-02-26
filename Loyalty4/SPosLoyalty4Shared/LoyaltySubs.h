//*******************************************************************
//*** exact same file used by LoyaltyManager / LoyaltyServer ********
//*******************************************************************
#pragma once
//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRowGroup.h"
//**********************************************************************

class CLoyaltySubs
{
public:
	static bool IsRefreshPurse2Reqd(CSQLRowAccountFull& RowAccount, CSQLRowGroup& RowGroup, CSSDate& dateToday);
	static double GetPurse2NextSpendBalance(CSQLRowAccountFull& RowAccount, CSQLRowGroup& RowGroup, bool bForceLoadGroup = FALSE);
};

//**********************************************************************
extern CLoyaltySubs LoyaltySubs;
//**********************************************************************
