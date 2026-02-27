//*******************************************************************
#include "..\SmartPay4ManagerBgnd\DeleteRecord.h"
#include "..\SmartPay4ManagerBgnd\LeaversHandler.h"
//*******************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\RedeemPoints.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//*******************************************************************
#include "BatchPropertySheet.h"
#include "CardPickerDlg.h"
#include "DeleteLeaverDlg.h"
#include "SQLTranBatchUpdate.h"
//*******************************************************************
#include "BatchUpdate.h"
//*******************************************************************

CBatchUpdate::CBatchUpdate( int nUpdateType, CWnd* pParent )
{
	m_nUpdateType = nUpdateType;
	m_pParent = pParent;
	m_strLeaversYear = "";

	switch (m_nUpdateType)
	{
	case nBATCHUPDATE_UPDATE:
		m_strTitle = "Batch Update Records";
		break;

	case nBATCHUPDATE_DELETE:
		m_strTitle = "Batch Delete Records";
		break;

	case nBATCHUPDATE_REDEEM:
		m_strTitle = "Batch Redeem Points";
		break;
	}

	Server.Read ( Filenames.GetServerDataFilename() );
}

//*******************************************************************

const char* CBatchUpdate::DoUpdate()
{
	int nStatus				= 0;
	int nCount				= 0;
	CString strAuditComment	= "";
	m_strFinalMessage		= "";

	if (m_nUpdateType == nBATCHUPDATE_DELETE)									// see if batch delete
	{
		if (System.GetKeepLeaversDatabaseFlag() == TRUE)
		{
			CDeleteLeaverDlg dlg(m_strTitle, m_pParent);
			dlg.m_strMessage = "Remove records from current database";

			if (dlg.DoModal() == IDCANCEL)
			{
				return m_strFinalMessage;
			}

			strAuditComment = dlg.m_strComment;					// set audit comment
			m_strLeaversYear = dlg.m_strLeaversYear;				// set leavers year folder name (if used)

			if (m_strLeaversYear != "")
			{
				m_strTitle.Format("Move Records to %s Leavers", 
					(const char*) m_strLeaversYear);
			}
		}
	}

	CBatchAccountBalanceMemory BalanceMemory;
	CBatchAccountDetailMemory DetailMemory;
	CBatchPropertySheet prop ( m_strTitle, m_nUpdateType, BalanceMemory, DetailMemory, m_pParent );
	
	if (strAuditComment != "")
	{
		prop.SetComment(strAuditComment);						// set audit comment
	}

	while ( TRUE )
	{
		if (prop.DoModal() == IDCANCEL)
		{
			break;
		}

		if ( prop.HaveRecords() == FALSE )
		{
			Prompter.Error( "No record types have been selected.", m_strTitle );
			prop.SetActivePage(0);
			continue;
		}

		if ( prop.HaveUpdates() == FALSE )
		{
			Prompter.Error( "No batch update information have been set.", m_strTitle );
			prop.SetActivePage(1);
			continue;
		}

		// return number of possible records to update
		int nPossibleCount = CreateUpdateList(&prop);
		if ( nPossibleCount > 0 )	
		{ 
			nStatus = 1;									// update list has been created
			if ( ValidateList ( &prop ) == TRUE )
			{
				nStatus = 2;								// list has been validated
				nCount = Update ( &prop, nPossibleCount, BalanceMemory, DetailMemory );
			}

			break;
		}
		else
		{
			Prompter.Info("No records found in the specified range.", m_strTitle);
		}
	}

	if ( nStatus != 0 )
	{
		if ( m_nUpdateType == nBATCHUPDATE_DELETE )
		{
			if (nCount == 0)
			{
				m_strFinalMessage = "Delete cancelled - No records have been removed from the current database.";
			}
			else
			{
				if (m_strLeaversYear == "")
				{
					m_strFinalMessage.Format("Delete finished - %d records have been removed.", nCount);
				}
				else
				{
					m_strFinalMessage.Format("Move finished - %d records have been moved to %s Leavers.", 
						nCount, 
						(const char*) m_strLeaversYear);
				}
			}
		}
		else
		{
			if (nCount == 0)
			{
				m_strFinalMessage = "Update cancelled - No records have been modified.";
			}
			else
			{
				m_strFinalMessage.Format("Update finished - %d records have been modified", nCount);
			}
		}
	}

	return m_strFinalMessage;
}

//*******************************************************************

int CBatchUpdate::CreateUpdateList(CBatchPropertySheet* pProp)
{
	//ADD DUMMY ROW AS PLACEHOLDER IN CASE SELECTING ZERO ITEMS
	m_arraySourceItems.RemoveAll();
	CSQLRowAccountList RowDummy;
	m_arraySourceItems.Add(RowDummy);

	CWorkingDlg progress("Creating the batch update record list");
	progress.Create();

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NoParams{}, FALSE);

	int nChecked = 0;

	CSQLRowAccountFull RowAccount;
	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if ((++nChecked % 10) == 0)
		{
			if (progress.SetCaption2RecordsChecked(++nChecked) == TRUE)
			{
				m_arraySourceItems.RemoveAll();
				break;
			}
		}

		if (pProp->IsValid(RowAccount) == TRUE)						// see if valid card for updating
		{
			bool bOK = TRUE;

			if (m_nUpdateType == nBATCHUPDATE_REDEEM)
			{
				int nTrip = Server.GetActiveRedeemPointsTrip();							// get default trip
				bool bAllowed = TRUE;

				CSQLRowGroupFull RowGroup;
				RowGroup.SetGroupNo(RowAccount.GetGroupNo());

				CSQLRepositoryGroup RepoGroup;
				if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					if (RowGroup.GetActiveRedeemFlag() == TRUE)						// group specifc redeem points
					{
						if (RowGroup.GetRedeemDisable() == TRUE)
						{
							bAllowed = FALSE;
						}
						else
						{
							nTrip = RowGroup.GetRedeemTrip();
						}
					}
				}

				if (bAllowed == FALSE)
				{
					bOK = FALSE;
				}
				else
				{
					bOK = (RowAccount.GetPoints() < nTrip) ? FALSE : TRUE;
				}
			}

			if (bOK == TRUE)
			{
				CSQLRowAccountList RowList;
				RowList.SetUserID(RowAccount.GetUserID());
				RowList.SetGroupNo(RowAccount.GetGroupNo());
				RowList.SetUsername(RowAccount.GetUsername());
				RowList.SetForename(RowAccount.GetForename());
				RowList.SetExpiryDate(RowAccount.GetExpiryDate());
				RowList.SetInactive(RowAccount.GetInactive());
				RowList.SetAlertCode(RowAccount.GetAlertCode());
				RowList.SetMemberID(RowAccount.GetMemberID());
				m_arraySourceItems.Add(RowList);
			}
		}
	}

	return m_arraySourceItems.GetSize() - 1;
}

//*******************************************************************

bool CBatchUpdate::ValidateList ( CBatchPropertySheet* pProp )
{
	CCardPickerDlg dlg(m_strTitle, "", m_arraySourceItems, m_pParent);
	dlg.DisablePreviousSelection();

	dlg.m_bGroupsAllowed = ( pProp->m_Page1.m_nSingleGroupNo == 0 ) ? TRUE : FALSE;

	if (m_nUpdateType == nBATCHUPDATE_DELETE)
	{
		dlg.m_strBoxText = "Please verify the selection of records that will be removed";
	}
	else
	{
		dlg.m_strBoxText = "Please verify the selection of records that will be updated";
	}

	if (dlg.DoModal() == IDCANCEL)
	{
		return FALSE;
	}

	CString strMsg = "";
	CStringArray Warning;

	strMsg.Format ( "%s - Confirmation required",
		(const char*) m_strTitle );

	Warning.Add ( strMsg );

	CSQLRepositoryAccount RepoAccount;
	__int64 nTotalRecords = RepoAccount.CountRows(NULL).GetSQLResult();
	
	switch ( m_nUpdateType )
	{
	case nBATCHUPDATE_UPDATE:
		strMsg.Format ( "A possible %d out of %I64d records will be changed.", 
			dlg.GetCount(), 
			nTotalRecords );

		Warning.Add ( strMsg );
		Warning.Add ( "+Each record will be updated as follows:-" );

		pProp->AddWarnings ( &Warning );
		break;

	case nBATCHUPDATE_DELETE:
		if (m_strLeaversYear == "")
		{
			strMsg.Format("%d out of %I64d database records will be deleted.", 
				dlg.GetCount(), 
				nTotalRecords);
		}
		else
		{
			strMsg.Format("%d out of %I64d database records will be moved to %s Leavers.", 
				dlg.GetCount(), 
				nTotalRecords, 
				(const char*) m_strLeaversYear);
		}
		
		Warning.Add ( strMsg );

		if (Server.GetActiveEnablePurchaseHistoryFlag() == TRUE)
		{
			Warning.Add("Any associated Purchase History files will be deleted.");
		}

		Warning.Add ( "Any associated Photo ID files will be deleted." );
		break;

	case nBATCHUPDATE_REDEEM:
		strMsg.Format ( "A possible %d out of %I64d records could be changed.", dlg.GetCount(), nTotalRecords);
		Warning.Add ( strMsg );
		break;
	}

	if (Prompter.Warning(Warning) == IDNO)
	{
		return FALSE;
	}

	return TRUE;
}

//*******************************************************************

int CBatchUpdate::Update(CBatchPropertySheet* pProp, int nPossibleCount, CBatchAccountBalanceMemory& BalanceMemory, CBatchAccountDetailMemory& DetailMemory)
{
	CString strAuditComment = pProp->GetComment();

	int nAppNo = 0;
	CString strTitle = "";
	CString strAction = "";

	switch (m_nUpdateType)
	{
	case nBATCHUPDATE_UPDATE:
		strTitle = "Updating records";
		strAction = "Batch Update";
		nAppNo = APPNO_BATCHUPDATE;
		break;

	case nBATCHUPDATE_DELETE:
		strTitle = "Deleting records";
		strAction = "Batch Delete";
		nAppNo = (m_strLeaversYear == "") ? APPNO_BATCHDELETE : APPNO_BATCHLEAVER;
		break;

	case nBATCHUPDATE_REDEEM:
		strTitle = "Redeeming Points";
		strTitle = "Batch Redeem";
		nAppNo = APPNO_BATCHREDEEM;
		break;
	}

	int nCount = 0;

	CSQLTranBatchUpdate Tran;
	Tran.BeginTrans();
	Tran.DoWork(m_nUpdateType, m_arraySourceItems, BalanceMemory, DetailMemory, strAuditComment, m_strLeaversYear);

	switch (Tran.EndTrans())
	{
	case SQLTRAN_STATE_COMMITOK:
		nCount = Tran.GetCount();
		Tran.AfterTransaction();
		break;

	default:
		Tran.ShowErrorMessage(strAction);
		break;
	}

	return nCount;
}

//*******************************************************************

