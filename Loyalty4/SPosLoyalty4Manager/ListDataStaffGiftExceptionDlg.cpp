/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLConnectionPool.h"
#include "..\SPOSLoyalty4Shared\SQLDb.h"
#include "..\SPOSLoyalty4Shared\SQLDefines.h"
#include "..\SPOSLoyalty4Shared\SQLHelpers.h"
#include "..\SPOSLoyalty4Shared\SQLNamespaces.h"
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPOSLoyalty4Shared\SQLTable_StaffCard\SQLRepositoryStaffCard.h"
#include "..\SPOSLoyalty4Shared\SQLTable_StaffGiftException\SQLRepositoryStaffGiftException.h"
/**********************************************************************/
#include "FindDlg.h"
#include "SQLTranRefundStaffGift.h"
#include "SQLTranRetryStaffGift.h"
/**********************************************************************/
#include "ListDataStaffGiftExceptionDlg.h"
/**********************************************************************/

CSingleListContentHelpersStaffGiftException::CSingleListContentHelpersStaffGiftException(CSQLRowSetStaffGiftException& RowSet)
	: CSingleListContentHelpers(), m_RowSet(RowSet)
{
}

/**********************************************************************/

int CSingleListContentHelpersStaffGiftException::GetArraySize()
{
	return m_RowSet.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersStaffGiftException::PrepareLine(int nIndex, CString& strLine)
{
	CSQLRowStaffGiftException RowSC;
	m_RowSet.GetRow(nIndex, RowSC);

	CString strError = "Unknown";

	switch (RowSC.GetErrorCode())
	{
	case 0:
		strError = "No error detected, can retry";
		break;

	case 1:
		strError = "Invalid SPOS Server number";
		break;

	case 2:
		strError = "No mapped account for SPOS Server";
		break;

	case 3:
		strError = "SPOS Server mapped to invalid account ID";
		break;

	case 4:
		strError = "SPOS Server mapped to Customer account";
		break;

	case 5:
		strError = "Unable to access SPOS Server account";
		break;
	}

	CCSV csvLine('\t');
	csvLine.Add(RowSC.GetDate());
	csvLine.Add(RowSC.GetTime());
	csvLine.Add(RowSC.GetDisplayCustomerID());
	csvLine.Add(RowSC.GetDisplayCustomerName());
	csvLine.Add(RowSC.GetDisplayServerID());
	csvLine.Add(RowSC.GetDisplayServerName());
	csvLine.Add(RowSC.GetValue(), 2);
	csvLine.Add(strError);

	strLine = csvLine.GetLine();

	return TRUE;
}

/**********************************************************************/

void CSingleListContentHelpersStaffGiftException::DeleteDatabaseEntry( int nIndex )
{
	if ((nIndex >= 0) && (nIndex < m_RowSet.GetSize()))
	{
		m_RowSet.RemoveRow(nIndex);
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersStaffGiftException::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;
}

/**********************************************************************/

void CSingleListContentHelpersStaffGiftException::SetColumns(CSSListCtrlVirtual& list, CWnd* pParent)
{
	list.SubclassDlgItem(IDC_LIST, pParent);
	list.InsertColumn(0, "Date", LVCFMT_LEFT, 100);
	list.InsertColumn(1, "Time", LVCFMT_LEFT, 60);
	list.InsertColumn(2, "Customer ID", LVCFMT_LEFT, 100);
	list.InsertColumn(3, "Name", LVCFMT_LEFT, 150);
	list.InsertColumn(5, "Server ID", LVCFMT_LEFT, 150);
	list.InsertColumn(6, "Name", LVCFMT_LEFT, 150);
	list.InsertColumn(7, "Value", LVCFMT_LEFT, 60);
	list.InsertColumn(8, "Error", LVCFMT_LEFT, 270);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataStaffGiftExceptionDlg::CListDataStaffGiftExceptionDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_strContinuePrompt = "Are you sure you wish to continue ?";

	m_RowSet.LoadExceptions();

	m_pContentHelpers = new CSingleListContentHelpersStaffGiftException(m_RowSet);
	m_pDisplayHelpers = new CSingleListDisplayHelpers(m_listData, *m_pContentHelpers);
	
	m_pDisplayHelpers->SetTopSingleText("staff gift exception");
	m_pDisplayHelpers->SetTopPluralText("staff gift exceptions");
	m_pDisplayHelpers->SetBottomSingleText("staff gift exception");
	m_pDisplayHelpers->SetBottomPluralText("staff gift exceptions");
}

/**********************************************************************/

CListDataStaffGiftExceptionDlg::~CListDataStaffGiftExceptionDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataStaffGiftExceptionDlg::SetTitle()
{
	SetWindowText ( "Process Staff Gift Exceptions" );
}

/**********************************************************************/

void CListDataStaffGiftExceptionDlg::SetButtonTexts()
{
	m_button1.SetWindowText("&Refund\nCustomer");
	m_button2.SetWindowText("Retry\n&Gift");
	m_button3.SetWindowText("Redirect\n&Gift");
	ShowAndEnableWindow(&m_button4,FALSE);
	ShowAndEnableWindow(&m_button5,FALSE);
	m_button6.SetWindowText("Recheck\n&Errors");

	m_buttonOK.ShowWindow(SW_HIDE);
	m_buttonCancel.SetWindowText("Close");
}

/**********************************************************************/

void CListDataStaffGiftExceptionDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
	
	MoveControl(&m_listData, 7, 7, 720, 285);
	MoveControl(&m_button1, 743, 7);
	MoveControl(&m_button2, 743, 52);
	MoveControl(&m_button3, 743, 97);
	MoveControl(&m_button4, 743, 147);
	MoveControl(&m_button5, 743, 147);
	MoveControl(&m_button6, 743, 187);
	MoveControl(&m_buttonOK, 743, 262);
	MoveControl(&m_buttonCancel, 743, 278);

	ResizeDialog(800, 299);
}

/**********************************************************************/

void CListDataStaffGiftExceptionDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if (m_RowSet.GetSize() != 0)
	{
		SelectLine(0);
	}
}

/**********************************************************************/

void CListDataStaffGiftExceptionDlg::HandleButton1()
{
	m_pDisplayHelpers->UpdateSelectedItemArray();

	int nSelectCount = m_pDisplayHelpers->GetSelectedArraySize();
	
	if (0 == nSelectCount)
	{
		return;
	}

	CString strMessage = "";
	CString strTitle = "Refund Failed Staff Gift";

	if (1 == nSelectCount)
	{
		int nIndex = m_pDisplayHelpers->GetSelectedLine(0);
		CSQLRowStaffGiftException RowSC;
		m_RowSet.GetRow(nIndex, RowSC);

		strMessage.Format("This option will attempt the following refund\n\n£%.2f to Purse1 of account %s, %s\n\n%s",
			RowSC.GetValue(),
			(const char*)RowSC.GetDisplayCustomerID(),
			(const char*)RowSC.GetDisplayCustomerName(),
			(const char*)m_strContinuePrompt);
	}
	else
	{
		strMessage.Format("This option will attempt to refund the %d selected gifts\n\n%s",
			m_pDisplayHelpers->GetSelectedArraySize(),
			(const char*)m_strContinuePrompt);

		strTitle += "s";
	}

	if (Prompter.YesNo(strMessage, strTitle) == IDYES)
	{
		int nDeleted = 0;
		int nFailed = 0;

		for (int n = 0; n < m_pDisplayHelpers->GetSelectedArraySize(); n++)
		{
			int nIndex = m_pDisplayHelpers->GetSelectedLine(n) - nDeleted;

			CSQLRowStaffGiftException RowSC;
			m_RowSet.GetRow(nIndex, RowSC);

			CSQLTranRefundStaffGift Tran;
			Tran.BeginTrans();
			Tran.DoWork(RowSC);

			switch (Tran.EndTrans())
			{
			case SQLTRAN_STATE_COMMITOK:
				Tran.AfterTransaction();
				m_RowSet.RemoveRow(nIndex);
				m_listData.SetItemCountEx(m_RowSet.GetSize());
				SelectLine(min(nIndex, m_RowSet.GetSize() - 1));
				nDeleted++;
				break;

			default:
				if (1 == nSelectCount)
				{
					Tran.ShowErrorMessage("Refund Staff Gift");
				}
				nFailed++;
				break;
			}
		}

		if (1 != nSelectCount)
		{
			CString strMsg = "";
			strMsg.Format("%d of %d selected gifts were refunded",
				nDeleted,
				nDeleted + nFailed);

			Prompter.Info(strMsg);
		}
	}
}

/**********************************************************************/

void CListDataStaffGiftExceptionDlg::HandleButton2()
{
	m_pDisplayHelpers->UpdateSelectedItemArray();

	int nSelectCount = m_pDisplayHelpers->GetSelectedArraySize();

	if (0 == nSelectCount)
	{
		return;
	}

	if (1 == nSelectCount)
	{
		int nIndex = m_pDisplayHelpers->GetSelectedLine(0);
		CSQLRowStaffGiftException RowSG;
		m_RowSet.GetRow(nIndex, RowSG);

		if (RowSG.GetErrorCode() == 1)
		{
			CString strError = "";
			strError += "This gift is for an invalid server number, and cannot be retried.\n\n";
			strError += "Please use one of the other available options to:\n\n";
			strError += "(1) Refund the gift to Purse1 of the customer OR\n\n";
			strError += "(2) Redirect the gift to another account\n\n";
			Prompter.Error(strError);
			return;
		}
	}

	bool bCanRetry = TRUE;

	if (nSelectCount > 1)
	{
		CString strMessage = "";
		strMessage.Format("This option will retry the %d selected gifts\n\n%s",
			m_pDisplayHelpers->GetSelectedArraySize(),
			(const char*)m_strContinuePrompt);

		bCanRetry = (Prompter.YesNo(strMessage, "Retry Failed Staff Gifts") == IDYES);
	}

	if ( TRUE == bCanRetry )
	{
		int nDeleted = 0;
		int nFailed = 0;

		for (int n = 0; n < nSelectCount; n++)
		{
			int nIndex = m_pDisplayHelpers->GetSelectedLine(n) - nDeleted;

			CSQLRowStaffGiftException RowSC;
			m_RowSet.GetRow(nIndex, RowSC);

			CString strError = "";

			switch (RowSC.GetErrorCode())
			{
			case 0:
			case 2:
			case 3:
			case 4:
				if (RetryGift(nIndex, RowSC, strError, ( 1 == nSelectCount) ) == TRUE)
				{
					nDeleted++;
				}
				else
				{
					if ( (1 == nSelectCount) && ( strError != "" ) )
					{
						Prompter.Error(strError);
					}
					nFailed++;
				}
				break;

			default:
				nFailed++;
				break;
			}
		}

		if (nSelectCount > 1)
		{
			CString strMsg = "";
			strMsg.Format("%d of %d selected gifts were successfully retried",
				nDeleted,
				nDeleted + nFailed);

			Prompter.Info(strMsg);
		}
	}
}

/**********************************************************************/

bool CListDataStaffGiftExceptionDlg::RetryGift(int nIndex, CSQLRowStaffGiftException& RowSG, CString& strError, bool bSingleLine)
{
	strError = "This gift cannot be retried because:\n\n";

	CSQLRowStaffCard RowSC;
	RowSC.SetFolderIndexDbNo(RowSG.GetFolderIndexDbNo());
	RowSC.SetFolderIndexSetNo(RowSG.GetFolderIndexSetNo());
	RowSC.SetServerNo(RowSG.GetServerNo());

	CSQLRepositoryStaffCard repoCard;
	if (repoCard.SelectRow(RowSC, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		RowSC.SetFolderIndexSetNo(0);

		if (repoCard.SelectRow(RowSC, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			RowSC.SetFolderIndexDbNo(0);

			if (repoCard.SelectRow(RowSC, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				strError += "The SPOS Server is not mapped to an account.";
				return FALSE;
			}
		}
	}

	__int64 nUserID = RowSC.GetUserID();
	if (nUserID <= 0)
	{
		strError += "The SPOS Server is mapped to an invalid account ID.";
		return FALSE;
	}

	if (nUserID == RowSG.GetCustomerID())
	{
		strError += "The SPOS Server is mapped to the customer's account.";
		return FALSE;
	}

	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nUserID);
	CSQLRepositoryAccount repoAccount;
	if (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		strError += "The mapped account does not exist, or could not be accessed.";
		return FALSE;
	}

	bool bCanRetry = TRUE;
	
	if ( TRUE == bSingleLine )
	{
		CString strMessage = "";
		strMessage.Format("This option will retry the following gift\n\n£%.2f to Purse1 for %s, %s\n\n%s",
			RowSG.GetValue(),
			(const char*)System.FormatCardNo(RowAccount.GetUserID(), TRUE),
			(const char*)RowAccount.GetFullname(),
			(const char*)m_strContinuePrompt);

		bCanRetry = (Prompter.YesNo(strMessage, "Retry Failed Staff Gift") == IDYES);
	}

	bool bResult = FALSE;

	if (TRUE == bCanRetry)
	{
		CSQLTranRetryStaffGift Tran;
		Tran.BeginTrans();
		Tran.DoWork(RowSG, RowAccount);

		switch (Tran.EndTrans())
		{
		case SQLTRAN_STATE_COMMITOK:
			Tran.AfterTransaction();
			m_RowSet.RemoveRow(nIndex);
			m_listData.SetItemCountEx(m_RowSet.GetSize());
			SelectLine(min(nIndex, m_RowSet.GetSize() - 1));
			bResult = TRUE;
			break;

		default:
			if (TRUE == bSingleLine)
			{
				Tran.ShowErrorMessage("Retry Staff Gift");
				strError = "";
			}
			break;
		}
	}

	return bResult;
}

/**********************************************************************/

void CListDataStaffGiftExceptionDlg::HandleButton3()
{
	m_pDisplayHelpers->UpdateSelectedItemArray();

	int nSelectCount = m_pDisplayHelpers->GetSelectedArraySize();
	if (0 == nSelectCount)
	{
		return;
	}

	CString strPrompt = "";
	if (1 == nSelectCount)
	{
		strPrompt = "Choose Card To Redirect Gift";
	}
	else
	{
		strPrompt.Format("Choose Card To Redirect the %d Selected Gifts",
			nSelectCount);
	}

	CFindDlg dlg{};
	dlg.SetTitle(strPrompt);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	if (dlg.m_nRequestedCardNo > 0)
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(dlg.m_nRequestedCardNo);
		CSQLRepositoryAccount repoAccount;
		if (repoAccount.SelectRow(RowAccount, NULL).GetSQLResult() != SQLCRUD_ERR_NONE)
		{
			Prompter.Error("Unable to access selected account for Staff Gift");
			return;
		}

		CString strMessage = "";
		CString strTitle = "Redirect Failed Staff Gift";

		if (nSelectCount > 1)
		{
			double dTotal = 0.0;
			for (int n = 0; n < nSelectCount; n++)
			{
				int nIndex = m_pDisplayHelpers->GetSelectedLine(0);
				CSQLRowStaffGiftException RowSG;
				m_RowSet.GetRow(nIndex, RowSG);
				dTotal += RowSG.GetValue();
			}

			strMessage.Format("This option will redirect the %d selected gifts\n\ntotalling £%.2f to Purse1 of account %s, %s\n\n%s",
				nSelectCount,
				dTotal,
				(const char*)System.FormatCardNo(RowAccount.GetUserID(), TRUE),
				(const char*)RowAccount.GetFullname(),
				(const char*)m_strContinuePrompt);

			strTitle += "s";
		}
		else
		{
			int nIndex = m_pDisplayHelpers->GetSelectedLine(0);
			CSQLRowStaffGiftException RowSG;
			m_RowSet.GetRow(nIndex, RowSG);

			strMessage.Format("This option will redirect the following gift\n\n£%.2f to Purse1 of account %s, %s\n\n%s",
				RowSG.GetValue(),
				(const char*)System.FormatCardNo(RowAccount.GetUserID(), TRUE),
				(const char*)RowAccount.GetFullname(),
				(const char*)m_strContinuePrompt);
		}

		if (Prompter.YesNo(strMessage, strTitle) != IDYES)
		{
			return;
		}

		int nDeleted = 0;
		int nFailed = 0;

		for (int n = 0; n < nSelectCount; n++)
		{
			int nIndex = m_pDisplayHelpers->GetSelectedLine(n) - nDeleted;
			CSQLRowStaffGiftException RowSG;
			m_RowSet.GetRow(nIndex, RowSG);

			CSQLTranRetryStaffGift Tran;
			Tran.BeginTrans();
			Tran.DoWork(RowSG, RowAccount);

			switch (Tran.EndTrans())
			{
			case SQLTRAN_STATE_COMMITOK:
				Tran.AfterTransaction();
				m_RowSet.RemoveRow(nIndex);
				m_listData.SetItemCountEx(m_RowSet.GetSize());
				SelectLine(min(nIndex, m_RowSet.GetSize() - 1));
				nDeleted++;
				break;

			default:
				if (1 == nSelectCount)
				{
					Tran.ShowErrorMessage("Redirect Staff Gift");
				}
				nFailed++;
				break;
			}
		}

		if (nSelectCount > 1)
		{
			CString strMsg = "";
			strMsg.Format("%d of %d selected gifts were successfully redirected",
				nDeleted,
				nDeleted + nFailed);

			Prompter.Info(strMsg);
		}
	}
}

/**********************************************************************/

void CListDataStaffGiftExceptionDlg::HandleButton6()
{
	for (int nIndex = 0; nIndex < m_RowSet.GetSize(); nIndex++)
	{
		CSQLRowStaffGiftException RowSG;
		m_RowSet.GetRow(nIndex, RowSG);
		
		switch (RowSG.GetErrorCode())
		{
		case 2:
		case 3:
		case 4:
		case 5:
			RefreshRow(RowSG);
			RowSG.SetDisplayUserInfo();
			m_RowSet.UpdateRow(nIndex, RowSG);
			break;
		}
	}
	m_listData.Invalidate();

	Prompter.Info("The exceptions have been rechecked using current system settings.\n\nPlease refer to the list for results");
}

/**********************************************************************/

void CListDataStaffGiftExceptionDlg::RefreshRow(CSQLRowStaffGiftException& RowSG)
{
	CSQLRowStaffCard RowSC;
	RowSC.SetFolderIndexDbNo(RowSG.GetFolderIndexDbNo());
	RowSC.SetFolderIndexSetNo(RowSG.GetFolderIndexSetNo());
	RowSC.SetServerNo(RowSG.GetServerNo());

	CSQLRepositoryStaffCard repoCard;
	if (repoCard.SelectRow(RowSC, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		RowSC.SetFolderIndexSetNo(0);

		if (repoCard.SelectRow(RowSC, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			RowSC.SetFolderIndexDbNo(0);

			if (repoCard.SelectRow(RowSC, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				RowSG.SetErrorCode(2);
				RowSG.SetServerID(0);
				return;
			}
		}
	}

	__int64 nUserID = RowSC.GetUserID();
	if (nUserID <= 0)
	{
		RowSG.SetErrorCode(3);
		RowSG.SetServerID(0);
		return;
	}

	RowSG.SetServerID(nUserID);

	if (nUserID == RowSG.GetCustomerID())
	{
		RowSG.SetErrorCode(4);
		return;
	}

	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nUserID);
	CSQLRepositoryAccount repoAccount;
	if (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		RowSG.SetErrorCode(5);
		return;
	}
	else
	{
		RowSG.SetErrorCode(0);
		return;
	}
}

/**********************************************************************/

bool CListDataStaffGiftExceptionDlg::HandleOK()
{
	return TRUE;
}

/**********************************************************************/

bool CListDataStaffGiftExceptionDlg::HandleCancel()
{
	return TRUE;
}

/**********************************************************************/

