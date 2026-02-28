//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRepositoryOfferStamp.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLDefines.h"
//$$******************************************************************
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "PasswordData.h"
//$$******************************************************************
#include "EditAccountPropertySheet.h"
//$$******************************************************************

CEditAccountPropertySheet::CEditAccountPropertySheet(CSQLRowAccountFull& RowAccount, CSQLRowExternalAccount& RowExternal, CPhotoIDFilenameStack& PhotoIDFilenameStack, CWnd* pParentWnd, UINT iSelectPage)
	:CSSPropertySheet("", pParentWnd, iSelectPage), 
	m_PhotoIDFilenameStack(PhotoIDFilenameStack), 
	m_RowAccount(RowAccount),
	m_PageDetail(RowAccount),
	m_PageBalance(RowAccount),
	m_PageExternal(RowAccount,RowExternal),
	m_PageOfferStamp(RowAccount)
{
	m_bGotStampOfferTab = FALSE;
	m_bGotExternalTab = FALSE;

	m_PhotoIDFilenameStack.Initialise(SimpleFormatInt64Value(RowAccount.GetUserID()));
	m_EditAccountBuffer.m_strCardName = RowAccount.GetFullname();
	m_EditAccountBuffer.m_nGroupNo = RowAccount.GetGroupNo();

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	//ACCOUNT DETAILS
	AddPage(&m_PageDetail);
	m_PageDetail.m_psp.dwFlags &= ~PSP_HASHELP;
	m_PageDetail.SetPointer(this, &m_EditAccountBuffer);
	m_PageDetail.SetPhotoIDFilenameStack(&m_PhotoIDFilenameStack);

	//BALANCES
	AddPage(&m_PageBalance);
	m_PageBalance.m_psp.dwFlags &= ~PSP_HASHELP;
	m_PageBalance.SetPointer( &m_EditAccountBuffer);
	
	m_PageOfferStamp.m_psp.dwFlags &= ~PSP_HASHELP;
	InitialCheckStampOfferTab();

	//EXTERNAL ACCOUNT
	if (System.GetExternalAccountFlag() == TRUE)
	{
		if (Passwords.IsEditExternalAccountLinkAllowed() == TRUE)
		{
			AddPage(&m_PageExternal);
			m_PageExternal.m_psp.dwFlags &= ~PSP_HASHELP;
			m_bGotExternalTab = TRUE;
		}
	}
}

//$$******************************************************************

CEditAccountPropertySheet::~CEditAccountPropertySheet()
{
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CEditAccountPropertySheet, CPropertySheet)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CEditAccountPropertySheet::OnInitDialog()
{
	CPropertySheet::OnInitDialog();
	UpdateTitle( m_RowAccount );
	return TRUE;
}

//$$******************************************************************

void CEditAccountPropertySheet::UpdateTitle( CSQLRowAccountFull& RowAccount )
{
	CString strTitle = "";

	if (RowAccount.IsCardHotlisted() == TRUE)
	{
		strTitle = "HOTLISTED ";
	}
	else if (RowAccount.HasCardExpired() == TRUE)
	{
		strTitle = "EXPIRED ";
	}

	strTitle += "Account ";
	strTitle += System.FormatCardNo(SimpleFormatInt64Value(RowAccount.GetUserID()));
	
	{
		CString strName = RowAccount.GetFullname();
		SolutionGlobalFunctions::TrimSpacesFromString(strName, FALSE);

		if (strName != "")
		{
			strTitle += " ";
			strTitle += strName;
			strTitle += " ";
		}
	}
		
	SetWindowText(strTitle);
}

//$$******************************************************************

void CEditAccountPropertySheet::InitialCheckStampOfferTab()
{
	bool bShowStampOfferTab = FALSE;

	{
		CSQLRowGroup RowGroup;
		RowGroup.SetGroupNo(m_EditAccountBuffer.m_nGroupNo);

		CSQLRepositoryGroup repoGroup;
		if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if (RowGroup.GetStampOfferFlag() == TRUE)
			{
				bShowStampOfferTab = TRUE;
			}
		}
	}

	if (FALSE == bShowStampOfferTab)
	{
		CSQLRepositoryOfferStamp repoStamp;
		bShowStampOfferTab = ( repoStamp.CountRowsByUserID(m_RowAccount.GetUserID(), NULL).GetSQLResult() != 0 );
	}

	if (TRUE == bShowStampOfferTab)
	{
		AddStampOfferTab();
	}
}

//$$******************************************************************

void CEditAccountPropertySheet::AddStampOfferTab()
{
	if (FALSE == m_bGotStampOfferTab)
	{
		if (TRUE == m_bGotExternalTab)
		{
			RemovePage(2);
		}

		AddPage(&m_PageOfferStamp);
		m_bGotStampOfferTab = TRUE;

		if (TRUE == m_bGotExternalTab)
		{
			AddPage(&m_PageExternal);
		}
	}
}

//$$******************************************************************

void CEditAccountPropertySheet::RemoveStampOfferTab()
{
	if (TRUE == m_bGotStampOfferTab)
	{
		RemovePage(2);
		m_bGotStampOfferTab = FALSE;
	}
}

//$$******************************************************************

bool CEditAccountPropertySheet::PromptBeforeCancel()
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