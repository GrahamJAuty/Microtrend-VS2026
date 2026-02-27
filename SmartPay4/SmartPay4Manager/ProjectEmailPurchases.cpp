/**********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\EmailPurchases.h"
/**********************************************************************/

CEmailPurchases::CEmailPurchases (CWnd* pParent )
{
	m_pParent = pParent;
	m_bCancelButton = FALSE;
	Init();
}

/**********************************************************************/

void CEmailPurchases::Execute()
{
	int nDayNo = COleDateTime::GetCurrentTime().GetDayOfWeek() - 1;		// range = 1-7 (1=Sunday, 2= Momday

	CString strMsg = "";
	if ( nDayNo != m_data.m_nPurchaseDay )
	{
		strMsg = "CAUTION:\n\nThis function is not scheduled for use today!\n\n";
	}

	if ( m_data.m_bPromptPurchaseAlerts == TRUE )
	{
		strMsg += "Please confirm - email itemised purchase information now?";
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

		strMsg.Format ( "%s\n\nNumber of emails generated = %d", 
			(const char*) strBuf,
			m_nExportCount );

		Prompter.Info( strMsg, m_strTitle );	
	}
}

//*******************************************************************

void CEmailPurchases::ScanDatabase ()
{
	CWorkingDlg progress ( szMSG_SCANNINGDATABASE );							// "Scanning database"
	progress.Create();

	CEmailLogDatabase db;
	if ( db.Open ( Filenames.GetEmailLogDatabasePathname(2), DB_READWRITE ) == DB_ERR_NONE )
	{
		int nCount = 0;
		CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NoParams{}, FALSE);

		CSQLRowAccountFull RowAccount;
		while (RecordSet.StepSelectAll(RowAccount) == TRUE)
		{
			if ( progress.SetCaption2RecordsChecked(++nCount) == TRUE )
			{
				m_bCancelButton = TRUE;
				break;
			}

			CString strError = "";
			CreateEmail( RowAccount, db, m_bCancelButton, strError );

			if ( TRUE == m_bCancelButton )
			{
				MessageBeep ( MB_ICONSTOP );
				Prompter.FatalError( strError, strError );
				m_bCancelButton = TRUE;
				break;
			}
		}
		
		db.Close();
	}
}

//*******************************************************************
