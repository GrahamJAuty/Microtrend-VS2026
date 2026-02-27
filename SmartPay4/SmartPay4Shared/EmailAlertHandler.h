#pragma once
//*******************************************************************
#include "EmailAlertData.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************

class CEmailAlertHandler :	public CEmailAlertData
{
public:
	CEmailAlertHandler();

	bool IsValidLowBalanceRecord(CSQLRowAccountFull& RowAccount);
	bool IsValidPurchaseRecord(CSQLRowAccountFull& RowAccount);
	bool IsValidPointsAchievementRecord(CSQLRowAccountFull& RowAccount);
	bool IsValidWeeklyPointsUpdateRecord(CSQLRowAccountFull& RowAccount);

	CString GetEmailAddress(CSQLRowAccountFull& RowAccount);

private:
	bool IsValid (CSQLRowAccountFull& RowAccount);
};

//*******************************************************************
