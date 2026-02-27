//******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRecordSetAccountFull.h"
//******************************************************************
#include "..\SmartPay4Shared\SQLDb.h"
//******************************************************************
#include "..\SmartPay4ManagerBgnd\EmailLowBalances.h"
//******************************************************************

CEmailLowBalances::CEmailLowBalances()
{
	Init();
}

//******************************************************************

void CEmailLowBalances::Execute()
{
	ScanDatabase();
}

//*******************************************************************

void CEmailLowBalances::ScanDatabase()
{
	CSQLRecordSetAccountFull RecordSet(NULL, 
		RSParams_AccountFull_NoParams{}, FALSE);

	CSQLRowAccountFull RowAccount;
	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (m_data.IsValidLowBalanceRecord(RowAccount) == TRUE)
		{
			if (CheckLowBalances(RowAccount) == TRUE)								// 1 or more purse is low
			{
				if (CheckNewEmailRequired(RowAccount) == TRUE)						// check against last low balance email sent
				{
					CString strError = SendEmail(RowAccount);						// compile email message & send to outbox
				}
			}
		}
	}
}

//*******************************************************************