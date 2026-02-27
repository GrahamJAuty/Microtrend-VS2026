/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLDb.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\EmailWeeklyPoints.h"
/**********************************************************************/

CEmailWeeklyPoints::CEmailWeeklyPoints()
{
	Init();
}

/**********************************************************************/

void CEmailWeeklyPoints::Execute()
{
	int nDayNo = COleDateTime::GetCurrentTime().GetDayOfWeek() - 1;		// range = 1-7 (1=Sunday, 2= Momday

	if ( nDayNo == m_data.m_nWeeklyPointsDay )
	{
		ScanDatabase();
	}
}

//*******************************************************************

void CEmailWeeklyPoints::ScanDatabase()
{
	CSQLRecordSetAccountFull RecordSet(NULL, 
		RSParams_AccountFull_NoParams{}, FALSE);

	CSQLRowAccountFull RowAccount;
	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (m_data.IsValidWeeklyPointsUpdateRecord(RowAccount) == TRUE)
		{
			if (CheckNewEmailRequired(RowAccount) == TRUE)						// check against last email sent
			{
				CString strError = SendEmail(RowAccount);						// compile email message & send to outbox
			}
		}
	}
}

//*******************************************************************
