/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/**********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\SQLConnectionPool.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "ImportRecordsDlg.h"
#include "ImportStatsDlg.h"
/**********************************************************************/
#include "DatabaseAuditImport.h"
/**********************************************************************/

CDatabaseAuditImport::CDatabaseAuditImport ( CWnd* pParent )
{
	m_pParent = pParent;
	m_nRead = 0;
	m_nUsed = 0;
	m_nAdded = 0;
	m_nDeleted = 0;
	m_nIgnored = 0;
	m_nImportMethod = 0;
	m_data.Read();
	m_strTitle  = "Import Audit Balances";
}

//********************************************************************

void CDatabaseAuditImport::ProcessAudit()
{
	bool bReply = FALSE;

	CImportRecordsDlg dlg(m_strTitle, m_data.m_strDbaseAuditFilename, m_pParent);
	dlg.m_nImportMethod = m_data.m_nDbaseImportMethod;
	dlg.m_bDeleteFile = FALSE;
	dlg.m_bShowDeleteFile = FALSE;
	dlg.m_bEnableSelective = TRUE;
	dlg.m_strFileType = "dat";

	if (dlg.DoModal() == IDOK)
	{
		CString strAuditFile = dlg.m_strImportFilename;
		m_nImportMethod = dlg.m_nImportMethod;

		CSmartPayAuditFile auditFile(strAuditFile);
		if (auditFile.ValidateFile() == nREPORT_NOERROR)
		{
			CSmartPayLockRequest Request;
			Request.m_bWantLockBgnd = TRUE;
			Request.m_bWantLockServerInner = TRUE;

			CSmartPayLockManager LockManager;
			int nLockError = LockManager.AcquireLocks(Request, m_pParent);

			if (LOCKMANAGER_OK == nLockError)
			{
				Import(&auditFile);
				m_data.m_bEnableDbaseAuditImport = FALSE;
				m_data.Save();
			}
		}
		else
		{
			CString strMsg = "";
			strMsg.Format("Error in opening audit file ' %s ' ", 
				(const char*) strAuditFile);

			Prompter.Error(strMsg, m_strTitle);
		}
	}
}

//************************************************************************

void CDatabaseAuditImport::Import(CSmartPayAuditFile* pAuditFile)
{
	int nConnection = -1;
	CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

	if (nConnection != -1)
	{
		pDatabase->BeginTrans();

		if (ImportInternal(pDatabase, pAuditFile) == TRUE)
		{
			pDatabase->CommitTrans();
			ShowStatistics();
		}
		else
		{
			pDatabase->Rollback();
			Prompter.Info("Import cancel - no database records have been updated", m_strTitle);
		}

		SQLConnectionPool.FreeConnection(nConnection);
	}
}

//************************************************************************

bool CDatabaseAuditImport::ImportInternal( CDatabase* pDatabase, CSmartPayAuditFile* pAuditFile)
{
	bool bCancelButton = FALSE;

	if (pAuditFile->Open() == TRUE)
	{
		CWorkingDlg dlgWorking("Scanning Audit File");
		dlgWorking.Create();

		CSQLAuditRecord atc;
		while (TRUE)
		{
			if (dlgWorking.SetCaption2PercentDone(pAuditFile->GetPercentPosition(), TRUE) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}

			if (pAuditFile->ReadLine(&atc) == FALSE)
			{
				break;
			}

			UpdateRecord(pDatabase, &atc);					// use audit line to update account database
		}
	}

	pAuditFile->Close();										// close audit file open
	return ( FALSE == bCancelButton);
}

//**********************************************************************

void CDatabaseAuditImport::UpdateRecord (CDatabase* pDatabase, CSQLAuditRecord* pAtc )
{
	int nSource	= pAtc->GetSourceType();
	if (nSource == AUDIT_EODBALANCE)
	{
		return;
	}

	++m_nRead;

	CString strUserID	= pAtc->GetUserID();
	
	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserIDString(strUserID);

	CSQLRepositoryAccount RepoAccount;
	bool bExists = (RepoAccount.SelectRow(RowAccount, pDatabase).GetSQLError() == SQLCRUD_ERR_NONE);
	
	if ( (FALSE == bExists) && (nIMPORT_METHOD_EXISTONLY == m_nImportMethod ) )
	{
		++m_nIgnored;
		return;
	}

	if ( bExists == TRUE && m_nImportMethod == nIMPORT_METHOD_NEWONLY )
	{
		++m_nIgnored;
		return;
	}

	if ( bExists == FALSE )
	{
		RowAccount.SetUsername ( pAtc->GetUserName() );
		RowAccount.SetGroupNo ( pAtc->GetGroupNo() );

		RepoAccount.InsertRow(RowAccount,pDatabase);
		++m_nAdded;
	}

	bool bOk	= TRUE;										// assume line can be used
	int nAppNo	= pAtc->GetApplicationNo();

	switch ( nAppNo )
	{
	case APPNO_EDIT:					
	case APPNO_ADDCASH:					
	case APPNO_ADDPOINTS:				
	case APPNO_REVALUEBONUSCASH:		
	case APPNO_REVALUEBONUSPOINTS:		
	case APPNO_SALE:					
	case APPNO_MEAL:					
	case APPNO_REDEEM:
	case APPNO_BATCHREDEEM:
	case APPNO_REWARDBONUSCASH:		
	case APPNO_REWARDBONUSPOINTS:		
	case APPNO_XFEROUT:					// transfer out
	case APPNO_XFERIN:					// transfer in
	case APPNO_HOTXFEROUT:				// hotlist replacment transfer
	case APPNO_HOTXFERIN:				
	case APPNO_REFUNDSALE:				
	case APPNO_REFUNDREWARDBONUSCASH:	
	case APPNO_REFUNDREWARDBONUSPOINTS:
	case APPNO_IMPORTUPDATE:			
	case APPNO_BATCHUPDATE:				
	case APPNO_PARENTPAY:				
	case APPNO_WISEPAY:					
	case APPNO_TUCASI:					
	case APPNO_SCHOOLCOMMS:
	case APPNO_BROMCOM:
	case APPNO_PEBBLE_TOPUP:
	case APPNO_PEBBLE_TOPUP_REFUND:
	case APPNO_PEBBLE_ORDER:
	case APPNO_PEBBLE_ORDER_REFUND:
	case APPNO_IMPORTTOPUP:				
	case APPNO_TUCASIv2:				
	case APPNO_SQUID:					
	case APPNO_PARENTMAIL:				
	case APPNO_WPOPENINGBALANCE:	  	// WebPayment opening balance (ParentMail)
	case APPNO_IMPORTCUSTOM:			// custom csv import data
										break;

	case APPNO_INSERT:
	case APPNO_IMPORTINSERT:
	case APPNO_HOTLISTREPLACE:
	case APPNO_IMPORTREPLACE:			// auto replace due to import of new account for existing recotd
	case APPNO_INSERTRECORDSIMS:
	case APPNO_INSERTRECORDPPAY:		
	case APPNO_INSERTRECORDWISEPAY:
	case APPNO_INSERTRECORDWONDE:	// Wonde csv import data
										break;

	case APPNO_REFRESH:					RowAccount.SetPurse2RefreshedDate ( pAtc->GetDateFiled() );
										RowAccount.SetPurse2RefreshedTime ( pAtc->GetTimeFiled() );
										RowAccount.SetPurse2Balance ( pAtc->GetCurrentPurse2Balance() );
										RowAccount.SetPurse2LastSpend(0);
										++m_nUsed;
										bOk = FALSE;								// no update as already done
										break;

	case APPNO_HOTLIST:					
	case APPNO_IMPORTHOTLIST:			// auto hotlist on importing new account
										RowAccount.SetAlertCode ( atoi ( pAtc->GetAuditLineExtraText() ) );
										RowAccount.SetAlertDate ( pAtc->GetDateFiled() );
										RowAccount.SetAlertTime ( pAtc->GetTimeFiled() );
										break;

	case APPNO_UNHOTLIST:				// clear hotlist
										RowAccount.SetUserAlert ( 0 );
										break;

	case APPNO_DELETE:
	case APPNO_BATCHDELETE:				
	case APPNO_DELETELEAVER:
	case APPNO_IMPORTDELETE:
	case APPNO_BATCHLEAVER:				// batch records moved to leavers database
	{
		RepoAccount.DeleteRow(RowAccount, pDatabase);
		++m_nUsed;
		++m_nDeleted;
		bOk = FALSE;										// no update reqd
	}
	break;

	case APPNO_BALANCE:					// only used by EOD_Audit ( so far)
	case APPNO_IMPORTLEAVER:			// imported record to move to leavers database
	default:							bOk = FALSE;
										++m_nIgnored;
										break;
	}

// update database record if required

	if ( bOk == TRUE )											// check ok to use audit data
	{
		++m_nUsed;

// balances
		RowAccount.SetUsername ( pAtc->GetUserName() );		// name may change
		RowAccount.SetGroupNo ( pAtc->GetGroupNo() );		// group may have changed

		RowAccount.SetPurse1Liability ( pAtc->GetCurrentPurse1Liability() );
		RowAccount.SetPurse1Credit(pAtc->GetCurrentPurse1Credit());
		RowAccount.SetPurse2Balance ( pAtc->GetCurrentPurse2Balance() );
		RowAccount.SetPurse3Liability ( pAtc->GetCurrentPurse3Liability() );
		RowAccount.SetPurse3Credit(pAtc->GetCurrentPurse3Credit());
		RowAccount.SetCashSpendToDate ( pAtc->GetToDateCashSpend() );
		RowAccount.SetPurse1SpendToDate ( pAtc->GetToDatePurse1Spend() );
		RowAccount.SetPurse2SpendToDate ( pAtc->GetToDatePurse2Spend() );
		RowAccount.SetPurse3SpendToDate ( pAtc->GetToDatePurse3Spend() );
		RowAccount.SetPoints ( pAtc->GetCurrentPoints() );
		RowAccount.SetPointsToDate ( pAtc->GetToDatePoints() );

// last used stuff

		if ( nAppNo == APPNO_SALE || nAppNo == APPNO_MEAL  )
		{
			if ( pAtc->GetTransactionPurse1Total() < 0.0 )					// amount deducted from purse1 in transaction
			{
				bool bSetLastUsed = FALSE;

				CSSDate dbDate (RowAccount.GetPurse1LastSpendDate() );
				if (pAtc->m_dateLastUsed > dbDate)
				{
					bSetLastUsed = TRUE;
				}
				else if ( pAtc->m_dateLastUsed == dbDate )  
				{
					CSSTime dbTime (RowAccount.GetPurse1LastSpendTime() );
					
					if (pAtc->m_timeLastUsed >= dbTime)
					{
						bSetLastUsed = TRUE;
					}
				}
				
				if ( TRUE == bSetLastUsed )
				{
					RowAccount.SetPurse1LastSpendDate ( pAtc->GetDateLastUsed() );
					RowAccount.SetPurse1LastSpendTime ( pAtc->GetTimeLastUsed () );
					RowAccount.SetPurse1LastSpend	( -(pAtc->GetTransactionPurse1Total()) );
				}
			}

			if ( pAtc->GetTransactionPurse2Balance() < 0.0 )				// amount deducted from purse2 in transaction
			{
				bool bSetLastUsed = FALSE;

				CSSDate dbDate ( RowAccount.GetPurse2LastSpendDate() );
				if (pAtc->m_dateLastUsed > dbDate)
				{
					bSetLastUsed = TRUE;
				}
				else if ( pAtc->m_dateLastUsed == dbDate )  
				{
					CSSTime dbTime ( RowAccount.GetPurse2LastSpendTime() );
					
					if (pAtc->m_timeLastUsed >= dbTime)
					{
						bSetLastUsed = TRUE;
					}
				}
				
				if ( TRUE == bSetLastUsed )
				{
					RowAccount.SetPurse2LastSpendDate ( pAtc->GetDateLastUsed() );
					RowAccount.SetPurse2LastSpendTime ( pAtc->GetTimeLastUsed () );
					RowAccount.SetPurse2LastSpend	( -(pAtc->GetTransactionPurse2Balance()) );
				}
			}

			if ( pAtc->GetTransactionPurse3Total() < 0.0	)				// amount deducted from purse3 in transaction
			{
				bool bSetLastUsed = FALSE;

				CSSDate dbDate ( RowAccount.GetPurse3LastSpendDate() );
				if (pAtc->m_dateLastUsed > dbDate)
				{
					bSetLastUsed = TRUE;
				}
				else if ( pAtc->m_dateLastUsed == dbDate )  
				{
					CSSTime dbTime ( RowAccount.GetPurse3LastSpendTime() );
					
					if (pAtc->m_timeLastUsed >= dbTime)
					{
						bSetLastUsed = TRUE;
					}
				}
				
				if ( TRUE == bSetLastUsed )
				{
					RowAccount.SetPurse3LastSpendDate ( pAtc->GetDateLastUsed() );
					RowAccount.SetPurse3LastSpendTime ( pAtc->GetTimeLastUsed () );
					RowAccount.SetPurse3LastSpend	( -(pAtc->GetTransactionPurse3Total()) );
				}
			}
		}

		RepoAccount.UpdateRow(RowAccount, pDatabase);
	
//		if ( nSource == AUDIT_SERVER )
//		{
//		}
//
//		else if ( nSource == AUDIT_PC )
//		{
//		}
//
//		else if ( nSource == AUDIT_POS )
//		{
//		}
//		else if ( nSource == AUDIT_KIOSKLINK )
//		{
//		}
//		else if ( nSource == AUDIT_VENDING )
//		{
//		}
	}
}
//
//***************************************************************

void CDatabaseAuditImport::ShowStatistics()
{
	CImportStatsDlgConfig Config;
	Config.m_nLinesRead = m_nRead;
	Config.m_nLinesIgnored = m_nIgnored;
	Config.m_nLinesUsed = m_nUsed;
	Config.m_nRecordsAdded = m_nAdded;
	Config.m_nRecordsDeleted = m_nDeleted;
	Config.m_nLinesReadPos = 0;
	Config.m_nLinesIgnoredPos = 1;
	Config.m_nLinesUsedPos = 2;
	Config.m_nRecordsAddedPos = 4;
	Config.m_nRecordsDeletedPos = 5;
	Config.m_nLinesInvalidPos = -1;
	Config.m_nLastLinePos = 5;
	Config.m_strCaption = m_strTitle;

	CImportStatsDlg dlg ( Config );
	dlg.DoModal();
}

//***************************************************************
