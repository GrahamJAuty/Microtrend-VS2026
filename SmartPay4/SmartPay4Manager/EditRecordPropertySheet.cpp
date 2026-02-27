//$$******************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRepositoryPurchaseControlStamp.h"
//$$******************************************************************
#include "EditRecordPropertySheet.h"
//$$******************************************************************

CEditRecordPropertySheet::CEditRecordPropertySheet(CSQLRowAccountFull& RowAccount, CSQLRowExternalAccount& RowExternal, int nMode, CWnd* pParentWnd, UINT iSelectPage)
	:CSSAutoShutdownPropertySheet(AutoShutdownHelper, "", pParentWnd, iSelectPage)
	, m_SQLRowAccount(RowAccount)
	, m_PageAccount(RowAccount)
	, m_PageBalance(RowAccount)
	, m_PageMaxSpend(RowAccount)
	, m_PagePurchaseControl(RowAccount)
	, m_PageExternal(RowAccount, RowExternal)
	, m_GroupShiftValidator(COleDateTime::GetCurrentTime(), RowAccount)
{
	m_pLeaversData = NULL;
	m_nMode = nMode;

	Server.Read(Filenames.GetServerDataFilename());
	m_EditAccountBuffer.m_strCardName = m_SQLRowAccount.GetUsername();
	m_EditAccountBuffer.m_nGroupNo = m_SQLRowAccount.GetGroupNo();
	m_EditAccountBuffer.m_bOwnMaxSpend = m_SQLRowAccount.GetOwnMaxSpend();

	m_bGotMaxSpendTab = FALSE;
	m_bGotPurchaseControlTab = FALSE;
	m_bGotExternalTab = FALSE;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	AddPage(&m_PageAccount);
	m_PageAccount.m_psp.dwFlags &= ~PSP_HASHELP;
	m_PageAccount.SetPointer(this, &m_EditAccountBuffer, nMode, &m_GroupShiftValidator);

	AddPage(&m_PageBalance);
	m_PageBalance.SetPointer(&m_EditAccountBuffer, nMode);
	m_PageBalance.m_psp.dwFlags &= ~PSP_HASHELP;

	m_PageMaxSpend.m_psp.dwFlags &= ~PSP_HASHELP;
	m_PageMaxSpend.SetPointer(nMode);

	InitialCheckOptionalTabs();

	if (System.GetExternalAccountFlag() == TRUE)
	{
		if (Passwords.IsEditExternalAccountLinkAllowed() == TRUE)
		{
			m_bGotExternalTab = TRUE;
		}
	}

	AddOptionalTabs();
}

//$$******************************************************************

CEditRecordPropertySheet::CEditRecordPropertySheet(CSQLRowAccountFull& RowAccount, CSQLRowExternalAccount& RowExternal, int nMode, CLeaversHandler* pLeaversData, CWnd* pParentWnd, UINT iSelectPage)
	:CSSAutoShutdownPropertySheet(AutoShutdownHelper, "", pParentWnd, iSelectPage)
	, m_SQLRowAccount(RowAccount)
	, m_PageAccount(RowAccount)
	, m_PageBalance(RowAccount)
	, m_PageMaxSpend(RowAccount)
	, m_PagePurchaseControl(RowAccount)
	, m_PageExternal(RowAccount, RowExternal)
	, m_GroupShiftValidator(COleDateTime::GetCurrentTime(), RowAccount)
{
	m_pLeaversData = pLeaversData;
	m_nMode = nMode;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	m_bGotMaxSpendTab = FALSE;
	m_bGotPurchaseControlTab = FALSE;
	m_bGotExternalTab = FALSE;

	AddPage(&m_PageAccount);
	m_PageAccount.m_psp.dwFlags &= ~PSP_HASHELP;
	m_PageAccount.SetPointer(this, &m_EditAccountBuffer, m_nMode, &m_GroupShiftValidator, pLeaversData);

	AddPage(&m_PageBalance);
	m_PageBalance.m_psp.dwFlags &= ~PSP_HASHELP;
	m_PageBalance.SetPointer(&m_EditAccountBuffer, m_nMode, pLeaversData);
}

//$$******************************************************************

CEditRecordPropertySheet::~CEditRecordPropertySheet()
{
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CEditRecordPropertySheet, CSSAutoShutdownPropertySheet)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CEditRecordPropertySheet::CSSAutoShutdownPostInitDialog()
{
	CString strTitle;
	if (m_pLeaversData == NULL)
	{
		strTitle.Format("Edit UserID %s", 
			(const char*) System.FormatCardNo( m_SQLRowAccount.GetUserIDString()));

		switch (m_SQLRowAccount.GetGroupShiftStatus())
		{
			case 1:
			case 2:
			{
				CString strExtra = "";
				strExtra.Format( "  (Group Shift %s to %s)",
					(const char*)m_SQLRowAccount.GetGroupShiftStartDate(),
					(const char*)m_SQLRowAccount.GetGroupShiftEndDate());

				strTitle += strExtra;

			}
			break;
		}

		if ((System.GetBioConsentTickNo() >= 1) && (System.GetBioConsentTickNo() <= 4) && (System.GetAssumeBioConsentFlag() == TRUE))
		{
			strTitle += " (BIO CONSENT SETUP MODE, Bio allowed for all accounts)";
		}
	}
	else
	{
		strTitle.Format("Edit Leaver %s ( %s )", 
			(const char*) System.FormatCardNo(m_SQLRowAccount.GetUserIDString()), 
			(const char*) m_pLeaversData->GetYear());
	}

	if ( m_SQLRowAccount.HasUserExpired() == TRUE)
	{
		strTitle += " - EXPIRED";
	}
	else if ( System.GetInterfaceType() == nINTERFACE_MIFAREv1 )
	{
		if (m_SQLRowAccount.IsAlertSet() == TRUE)
		{
			strTitle += " - HOTLISTED";
		}
	}

	if (m_nMode == DB_READONLY)
	{
		strTitle += " ( ReadOnly mode )";
	}

	SetWindowText ( strTitle );

	return TRUE;
}

//$$******************************************************************

void CEditRecordPropertySheet::InitialCheckOptionalTabs()
{
	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(m_EditAccountBuffer.m_nGroupNo);

	CSQLRepositoryGroup RepoGroup;
	if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		switch (RowGroup.GetMaxSpendType())
		{
		case nGRPSPENDTYPE_PERIOD_LOCKED:
		case nGRPSPENDTYPE_PERIOD_RECORD1:
		case nGRPSPENDTYPE_PERIOD_RECORD2:
			m_bGotMaxSpendTab = TRUE;
			break;
		}

		if (RowGroup.GetPurchaseControlFlag() == TRUE)
		{
			m_bGotPurchaseControlTab = TRUE;
		}
	}

	if (FALSE == m_bGotPurchaseControlTab)
	{
		CSQLRepositoryPurchaseControlStamp RepoStamp;
		m_bGotPurchaseControlTab = (RepoStamp.CountRowsByUserID(m_SQLRowAccount.GetUserID(), NULL).GetSQLResult() != 0);
	}
}

//$$******************************************************************

bool CEditRecordPropertySheet::AddMaxSpendTab()
{
	bool bResult = FALSE;
	if (FALSE == m_bGotMaxSpendTab)
	{
		m_bGotMaxSpendTab = TRUE;
		bResult = TRUE;
	}
	return bResult;
}

//$$******************************************************************

bool CEditRecordPropertySheet::RemoveMaxSpendTab()
{
	bool bResult = FALSE;
	if (TRUE == m_bGotMaxSpendTab)
	{
		m_bGotMaxSpendTab = FALSE;
		bResult = TRUE;
	}
	return bResult;
}

//$$******************************************************************

bool CEditRecordPropertySheet::AddPurchaseControlTab()
{
	bool bResult = FALSE;
	if (FALSE == m_bGotPurchaseControlTab)
	{
		m_bGotPurchaseControlTab = TRUE;
		bResult = TRUE;
	}
	return bResult;
}

//$$******************************************************************

bool CEditRecordPropertySheet::RemovePurchaseControlTab()
{
	bool bResult = FALSE;
	if (TRUE == m_bGotPurchaseControlTab)
	{
		m_bGotPurchaseControlTab = FALSE;
		bResult = TRUE;
	}
	return bResult;
}

//$$******************************************************************

void CEditRecordPropertySheet::AddOptionalTabs()
{
	if (NULL == m_pLeaversData)
	{
		while (GetPageCount() > 2)
		{
			RemovePage(GetPageCount() - 1);
		}

		if (m_bGotMaxSpendTab)
		{
			AddPage(&m_PageMaxSpend);
			m_PageMaxSpend.m_psp.dwFlags &= ~PSP_HASHELP;
		}

		if (m_bGotPurchaseControlTab)
		{
			AddPage(&m_PagePurchaseControl);
			m_PagePurchaseControl.m_psp.dwFlags &= ~PSP_HASHELP;
		}

		if (m_bGotExternalTab)
		{
			AddPage(&m_PageExternal);
			m_PageExternal.m_psp.dwFlags &= ~PSP_HASHELP;
		}
	}
}

//$$******************************************************************

bool CEditRecordPropertySheet::PromptBeforeCancel()
{
	bool bResult = TRUE;
	
	if (System.GetAccountCancelPromptFlag() == TRUE)
	{
		CString strMsg = "";
		strMsg += "You have chosen to CANCEL changes to this account";
		strMsg += "\n\n";
		strMsg += "Are you sure you want to do this?";
		bResult = (Prompter.YesNo(strMsg) == IDYES);
	}

	return bResult;
}

//$$******************************************************************
