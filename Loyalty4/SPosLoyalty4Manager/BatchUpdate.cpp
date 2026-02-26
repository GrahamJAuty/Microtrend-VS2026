//*******************************************************************
#include "..\SPosLoyalty4Shared\RedeemPoints.h"
#include "..\SPosLoyalty4Shared\SQLResultInfo.h"
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRecordSetAccountFull.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//*******************************************************************
#include "AuditRecord.h"
#include "BatchPropertySheet.h"
#include "CardPickerDlg.h"
#include "DbExportHandler.h"
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "PasswordData.h"
#include "SQLTranBatchUpdate.h"
#include "WorkingDlg.h"
//*******************************************************************
#include "BatchUpdate.h"
//*******************************************************************

CBatchUpdate::CBatchUpdate( int nUpdateType, CSQLRowAccountFull& RowAccount, CWnd* pParent ) : m_RowAccount( RowAccount)
{	
	m_nUpdateType = nUpdateType;
	m_pParent = pParent;

	m_strFinalMessage = "";

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

CBatchUpdate::~CBatchUpdate()
{
}

//*******************************************************************

const char* CBatchUpdate::DoUpdate()
{
	int nStatus = 0;
	int nCount = 0;
	m_strFinalMessage = "";

	CBatchAccountBalanceMemory BalanceMemory;
	CBatchAccountDetailMemory DetailMemory;
	CBatchPropertySheet prop ( m_strTitle, m_nUpdateType, m_RowAccount, BalanceMemory, DetailMemory, m_pParent );
	
	while ( TRUE )
	{
		if (prop.DoModal() == IDCANCEL)
		{
			break;
		}

		if ( prop.HaveRecords() == FALSE )
		{
			Prompter.FatalError( "No record types have been selected.", m_strTitle );
			prop.SetActivePage(0);
			continue;
		}

		if ( prop.HaveUpdates() == FALSE )
		{
			Prompter.FatalError( "No batch update information has been set.", m_strTitle );
			prop.SetActivePage(1);
			continue;
		}

		// return number of possible records to update
		int nPossibleCount = CreateUpdateList(&prop);
		if ( nPossibleCount > 0 )	
		{ 
			nStatus = 1;									// update list has been created
			if (ValidateList(&prop) == TRUE)
			{
				nStatus = 2;								// list has been validated
				nCount = Update(&prop, nPossibleCount, BalanceMemory, DetailMemory);
			}

			break;
		}
		else
		{
			CString strMsg = "";
			if (m_nUpdateType == nBATCHUPDATE_REDEEM)
			{
				strMsg = "No records found in the specified range with enough points to redeem.";
			}
			else
			{
				strMsg = "No records found in the specified range.";
			}

			Prompter.Info( strMsg, m_strTitle );
		}
	}

	if ( nStatus != 0 )
	{
		CString strMsg = "";
		if (m_nUpdateType == nBATCHUPDATE_DELETE)
		{
			if (nCount == 0)
			{
				m_strFinalMessage = "Delete cancelled - No records have been deleted.";
			}
			else
			{
				m_strFinalMessage.Format("Delete finished - %d records have been deleted", nCount);
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

int CBatchUpdate::CreateUpdateList ( CBatchPropertySheet* pProp )
{
	//ADD DUMMY ROW AS PLACEHOLDER IN CASE SELECTING ZERO ITEMS
	m_arraySourceItems.RemoveAll();
	CSQLRowAccountList RowDummy;
	m_arraySourceItems.Add(RowDummy);

	CWorkingDlg WorkingDlg ( "Creating the batch update record list" );
	WorkingDlg.Create();

	CDbExportHandler Handler;
	pProp->GetSQLConditions(Handler);
	pProp->GetSQLFilters(Handler);

	CString strWhere = "";
	CSQLBuilderBind BuilderBind{};
	CSQLRepositoryAccount repoAccount;
	repoAccount.PrepareExportWhere(Handler, -1, strWhere, BuilderBind);

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ExportByParams{&BuilderBind, strWhere});
	
	int nChecked = 0;

	CSQLRowAccountFull RowAccount;

	while( RecordSet.StepSelectAll(RowAccount) == TRUE )
	{
		if ((++nChecked % 10) == 0)
		{
			if (WorkingDlg.SetCaption2RecordsChecked(++nChecked) == TRUE)
			{
				m_arraySourceItems.RemoveAll();
				break;
			}
		}

		if ( pProp->NonSQLFilter(RowAccount) == TRUE )						// see if valid card for updating
		{
			bool bOK = TRUE;

			if (nBATCHUPDATE_REDEEM == m_nUpdateType)
			{
				int nTrip = Server.GetRedeemPointsTrip();							// get default trip
				bool bAllowed = (Server.GetRedeemPointsType() == nREDEEM_FAMILY) ? FALSE : TRUE;

				{
					CSQLRowGroup RowGroup;
					RowGroup.SetGroupNo(RowAccount.GetGroupNo());

					CSQLRepositoryGroup repoGroup;
					if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
					{
						if (RowGroup.GetRedeemFlag() == TRUE)						// group specifc redeem points
						{
							if (RowGroup.GetRedeemDisable() == TRUE)
							{
								bAllowed = FALSE;
							}
							else
							{
								nTrip = RowGroup.GetRedeemTrip();
								bAllowed = (RowGroup.GetRedeemType() != nREDEEM_FAMILY);
							}
						}
					}
				}

				if (FALSE == bAllowed)
				{
					bOK = FALSE;
				}
				else
				{
					bOK = ( RowAccount.GetPoints() >= nTrip);
				}
			}

			if ( bOK == TRUE )
			{
				CSQLRowAccountList RowList;
				RowList.SetACListUserID(RowAccount.GetUserID());
				RowList.SetACListFullname(RowAccount.GetFullname());
				RowList.SetACListGroupNo(RowAccount.GetGroupNo());
				RowList.SetACListExpiryDate(RowAccount.GetExpiryDate());
				RowList.SetACListInfo1(RowAccount.GetInfo1());
				RowList.SetACListAddress1(RowAccount.GetAddress1());
				RowList.SetACListInactive(RowAccount.GetInactive());
				RowList.SetACListHotlistCode(RowAccount.GetHotlistCode());
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

	if (m_nUpdateType == nBATCHUPDATE_DELETE)
	{
		dlg.m_strBoxText = "Please verify the selection of accounts that will be deleted";
	}
	else
	{
		dlg.m_strBoxText = "Please verify the selection of accounts that will be updated";
	}

	if (dlg.DoModal() == IDCANCEL)
	{
		return FALSE;
	}

	CSQLRepositoryAccount repoAccount;
	__int64 nMaxRecords = repoAccount.CountRows(NULL).GetSQLResult();
	
	CString strMsg = "";
	CStringArray Warning{};

	strMsg.Format ( "%s - Confirmation required", 
		(const char*) m_strTitle );

	Warning.Add ( strMsg );

	switch ( m_nUpdateType )
	{
	case nBATCHUPDATE_UPDATE:
		strMsg.Format ( "A possible %d out of %I64d accounts will be changed.", 
			dlg.GetCount(), 
			nMaxRecords );

		Warning.Add ( strMsg );
		break;

	case nBATCHUPDATE_DELETE:
		strMsg.Format ( "%d out of %I64d database account records will be deleted.", 
			dlg.GetCount(), 
			nMaxRecords );

		Warning.Add ( strMsg );

		if (Server.GetPurchaseHistoryFlag() == TRUE)
		{
			Warning.Add("Any associated Purchase History files will be deleted.");
		}

		Warning.Add ( "Any associated Photo ID files will be deleted." );
		break;

	case nBATCHUPDATE_REDEEM:
		strMsg.Format ( "A possible %d out of %I64d accounts may be changed.", 
			dlg.GetCount(), 
			nMaxRecords );

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
		nAppNo = APPNO_BATCHDELETE;
		break;

	case nBATCHUPDATE_REDEEM:
		strTitle = "Redeeming Points";
		strAction = "Batch Redeem";
		nAppNo = APPNO_BATCHREDEEM;
		break;
	}

	int nCount = 0;

	CSQLTranBatchUpdate Tran;
	Tran.BeginTrans();
	
	Tran.DoWork(m_nUpdateType, m_arraySourceItems, BalanceMemory, DetailMemory, pProp->GetComment());
	
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

