//******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDB.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//******************************************************************
#include "..\SmartPay4ManagerBgnd\EmailLowBalances.h"
//******************************************************************

CEmailLowBalances::CEmailLowBalances (CWnd* pParent )
{
	m_pParent = pParent;
	m_bCancelButton = FALSE;
	Init();
}

//******************************************************************

void CEmailLowBalances::Execute()
{
	if ( m_data.m_bPromptLowBalanceAlerts == TRUE )
	{
		if ( Prompter.YesNo( "Please confirm - email low balance alerts now?", m_strTitle, TRUE ) == IDNO )
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

		strMsg.Format ( "%s\n\nNumber of new alerts generated = %d", 
			(const char*)strBuf, 
			m_nExportCount );

		Prompter.Info( strMsg, m_strTitle );	
	}
}

//*******************************************************************

void CEmailLowBalances::ScanDatabase()
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

		if (m_data.IsValidLowBalanceRecord(RowAccount) == TRUE)
		{
			if (CheckLowBalances(RowAccount) == TRUE)								// 1 or more purse is low
			{
				if (CheckNewEmailRequired(RowAccount) == TRUE)						// check against last low balance email sent
				{
					CString strError = SendEmail(RowAccount);							// compile email message & send to outbox

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
