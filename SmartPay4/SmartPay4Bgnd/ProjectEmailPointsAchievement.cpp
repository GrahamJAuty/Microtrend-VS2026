/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLResultInfo.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\EmailPointsAchievement.h"
/**********************************************************************/

CEmailPointsAchievement::CEmailPointsAchievement() 
{
	Init();
}

/**********************************************************************/

void CEmailPointsAchievement::Execute()
{
	ScanDatabase();
}

//*******************************************************************

void CEmailPointsAchievement::ScanDatabase()
{
	int nCount = 0;

	CSQLRecordSetAccountFull RecordSet(NULL, 
		RSParams_AccountFull_NoParams{}, FALSE);

	CSQLRowAccountFull RowAccount;
	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (m_data.IsValidPointsAchievementRecord(RowAccount) == TRUE)
		{
			m_nPointsThreshold = 0;

			CSQLRowGroupFull RowGroup;
			RowGroup.SetGroupNo(RowAccount.GetGroupNo());

			CSQLRepositoryGroup RepoGroup;
			if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				m_nPointsThreshold = RowGroup.GetPointsAchievement();
			}

			if (m_nPointsThreshold != 0)
			{
				if (CheckNewEmailRequired(RowAccount) == TRUE)			// check against last points achievement email sent
				{
					CString strError = SendEmail(RowAccount);			// compile email message & send to outbox
				}
			}
		}
	}
}

//*******************************************************************
