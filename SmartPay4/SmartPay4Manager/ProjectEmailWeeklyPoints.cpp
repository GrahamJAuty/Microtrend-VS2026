/**********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\EmailWeeklyPoints.h"
/**********************************************************************/

CEmailWeeklyPoints::CEmailWeeklyPoints (CWnd* pParent )
{
	m_pParent = pParent;
	m_bCancelButton = FALSE;
	Init();
}

/**********************************************************************/

void CEmailWeeklyPoints::Execute()
{
	CString strMsg = "";

	int nDayNo = COleDateTime::GetCurrentTime().GetDayOfWeek() - 1;		// range = 1-7 (1=Sunday, 2= Momday

	if ( nDayNo != m_data.m_nWeeklyPointsDay )
	{
		strMsg = "CAUTION:\n\nThis function is not scheduled for use today!\n\n";
	}

	if ( m_data.m_bPromptPurchaseAlerts == TRUE )
	{
		strMsg += "Please confirm - email weekly points updates now?";
		MessageBeep ( MB_ICONQUESTION );

		if ( Prompter.YesNo( strMsg, m_strTitle, TRUE ) == IDNO )
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

		strMsg.Format ( "%s\n\nNumber of emails generated = %d", (const char*) strBuf, m_nExportCount );
		Prompter.Info( strMsg, m_strTitle );	
	}
}

//*******************************************************************

void CEmailWeeklyPoints::ScanDatabase()
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

		if (m_data.IsValidWeeklyPointsUpdateRecord(RowAccount) == TRUE)
		{
			if (CheckNewEmailRequired(RowAccount) == TRUE)					// check against last email sent
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

//*******************************************************************
