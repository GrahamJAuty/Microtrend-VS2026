//$$******************************************************************
#include "..\SPOSLoyalty4Shared\AESHelpers.h"
#include "..\SPOSLoyalty4Shared\SQLDefines.h"
#include "..\SPOSLoyalty4Shared\SQLNamespaces.h"
#include "..\SPOSLoyalty4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
//$$******************************************************************
#include "DefEditDlg.h"
#include "EditAccountPropertySheet.h"
//$$******************************************************************
#include "EditAccountPropertyPage4.h"
//$$******************************************************************

CEditAccountPropertyPage4::CEditAccountPropertyPage4(CSQLRowAccountFull& RowAccount, CSQLRowExternalAccount& RowExternal)
	: CSSPropertyPage(CEditAccountPropertyPage4::IDD)
	, m_SQLRowAccount(RowAccount)
	, m_SQLRowExternal(RowExternal)
{
	m_strNewPassword = "";
	m_SQLRowExternalOld.SetAESUsername(m_SQLRowExternal.GetAESUsername());
	m_SQLRowExternalOld.SetPasswordHash(m_SQLRowExternal.GetPasswordHash());
	m_SQLRowExternalOld.SetAESUserID(m_SQLRowExternal.GetAESUserID());
}

//$$******************************************************************

CEditAccountPropertyPage4::~CEditAccountPropertyPage4()
{
}

//$$******************************************************************

void CEditAccountPropertyPage4::DoDataExchange(CDataExchange* pDX)
{
	CSSPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUserName);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_BUTTON_PASSWORD, m_buttonPassword);
	DDX_Control(pDX, IDC_BUTTON_CHECK, m_buttonCheck);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEditAccountPropertyPage4, CSSPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_PASSWORD, OnButtonPassword)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, OnButtonCheckPassword)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CEditAccountPropertyPage4::OnInitDialog()
{
	CSSPropertyPage::OnInitDialog();

	CString strUsername = "";
	CString strAESUsername = m_SQLRowExternalOld.GetAESUsername();
	
	if (strAESUsername != "")
	{
		CAESHelpers::AESHexToPlainText(strAESUsername, strUsername);
	}
	
	m_editUserName.SetWindowText(strUsername);
	
	if (m_SQLRowExternalOld.GetPasswordHash() != "" )
	{
		m_editPassword.SetWindowText("<The password has been set>");
	}
	else
	{
		m_editPassword.SetWindowText("<No password set>");
	}

#ifndef _DEBUG
	ShowAndEnableWindow(&m_buttonCheck, FALSE);
#endif

	return TRUE;
}

//$$******************************************************************

void CEditAccountPropertyPage4::OnButtonPassword()
{
	CDefEditDlg dlg("Set External Account Password", "", ExternalAccount::Password.Max, "Password", this);
	
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_strName != "")
		{
			m_strNewPassword = dlg.m_strName;

			if ( m_SQLRowExternalOld.GetPasswordHash() != "" )
			{
				m_editPassword.SetWindowText("<The password has been changed>");
			}
			else
			{
				m_editPassword.SetWindowText("<The password has been set>");
			}
		}
	}
}

//$$******************************************************************

void CEditAccountPropertyPage4::OnButtonCheckPassword()
{
#ifdef _DEBUG
	
	CDefEditDlg dlg("Check Account Password", "", ExternalAccount::Password.Max, "Password", this);
	
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_strName != "")
		{
			CString strMD5 = CSSMD5::CalcMD5(dlg.m_strName);

			if (strMD5 == m_SQLRowExternalOld.GetPasswordHash())
			{
				Prompter.Info("Match");
			}
			else
			{
				Prompter.Info("No Match");
			}
		}
	}

#endif

}

//$$******************************************************************

BOOL CEditAccountPropertyPage4::OnKillActive()
{
	CString strAESHex = "";
	CString strUserName = GetEditBoxText(m_editUserName);
	TrimSpacesFromString(strUserName, FALSE);
	strUserName.MakeLower();

	if (strUserName != "")
	{
		CAESHelpers::PlainTextToAESHex(strUserName, strAESHex);
	}

	bool bCanSaveChanges = TRUE;
	if ((strAESHex != "") && (strAESHex != m_SQLRowExternalOld.GetAESUsername()))
	{
		CSQLRowExternalAccount SQLRowExternalCheck;
		SQLRowExternalCheck.SetAESUsername(strAESHex);

		CSQLRepositoryExternalAccount repoExt;

		switch (repoExt.SelectRow(SQLRowExternalCheck, NULL).GetSQLError())
		{
		case SQLCRUD_ERR_NONE:
		case SQLCRUD_ERR_DUPLICATE:
			Prompter.Error("The selected user name is already assigned to another user");
			return FALSE;

		case SQLCRUD_ERR_EXCEPT_DB:
		case SQLCRUD_ERR_EXCEPT_MISC:
			Prompter.Error("Unable to check database for existing user name\nChanges will not be saved");
			bCanSaveChanges = TRUE;
			break;
		}
	}

	if (TRUE == bCanSaveChanges)
	{
		m_SQLRowExternal.SetAESUsername(strAESHex);

		if (m_strNewPassword != "")
		{
			m_SQLRowExternal.SetPasswordHash(CSSMD5::CalcMD5(m_strNewPassword));
		}
	}

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************

void CEditAccountPropertyPage4::OnOK()
{
	CSSPropertyPage::OnOK();
}

//$$******************************************************************

BOOL CEditAccountPropertyPage4::OnQueryCancel()
{
	return CEditAccountPropertySheet::PromptBeforeCancel();
}

//********************************************************************

void CEditAccountPropertyPage4::OnCancel()
{
	CSSPropertyPage::OnCancel();
}

//$$******************************************************************
