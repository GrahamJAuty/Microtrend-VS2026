/**********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\EmailPointsAchievement.h"
/**********************************************************************/

CEmailPointsAchievement::CEmailPointsAchievement ( CWnd* pParent )
{	
	m_pParent = pParent;
	m_bCancelButton = FALSE;
	Init();
}

/**********************************************************************/

void CEmailPointsAchievement::Execute()
{
	if ( m_data.m_bPromptPointsAchievement == TRUE )
	{
		if ( Prompter.YesNo( "Please confirm - email points achievements now?", m_strTitle, TRUE ) == IDNO )
		{
			return;
		}
	}
	
	ScanDatabase();

	{
		CString strBuf = "";
		CString strMsg = "";

		if (m_bCancelButton == FALSE)
		{
			strBuf = szMSG_SCANCOMPLETE;				// "Database scan complete"
		}
		else
		{
			strBuf = szMSG_SCANCANCELLED;				// "Database scan terminated!
		}

		strMsg.Format ( "%s\n\nNumber of new emails generated = %d", 
			(const char*) strBuf, 
			m_nExportCount );

		Prompter.Info( strMsg, m_strTitle );	
	}
}

//*******************************************************************

void CEmailPointsAchievement::ScanDatabase()
{
	CWorkingDlg progress(szMSG_SCANNINGDATABASE);								// "Scanning database";
	progress.Create();

	int nCount = 0;

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NoParams{}, FALSE);

	CSQLRowAccountFull RowAccount;
	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (progress.SetCaption2RecordsChecked(++nCount) == TRUE)
		{
			m_bCancelButton = TRUE;
			break;
		}

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
				if (CheckNewEmailRequired(RowAccount) == TRUE)					// check against last points achievement email sent
				{
					CString strError = SendEmail(RowAccount);						// compile email message & send to outbox

					if (strError != "")
					{
						MessageBeep(MB_ICONSTOP);
						Prompter.FatalError(strError, strError);
						m_bCancelButton = TRUE;
						break;
					}
				}
			}
		}
	}
}

//*******************************************************************
