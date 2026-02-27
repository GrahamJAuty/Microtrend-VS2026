//$$******************************************************************
#include "..\SmartPay4Shared\AESHelpers.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
//$$******************************************************************
#include "DefEditDlg.h"
#include "EditRecordPropertySheet.h"
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "EditRecordPropertyPageExternal.h"
//$$******************************************************************

CEditRecordPropertyPageExternal::CEditRecordPropertyPageExternal(CSQLRowAccountFull& RowAccount, CSQLRowExternalAccount& RowExternal)
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CEditRecordPropertyPageExternal::IDD)
	, m_SQLRowAccount(RowAccount)
	, m_SQLRowExternal(RowExternal)
{
	m_strNewPassword = "";
	m_SQLRowExternalOld.SetAESUsername(m_SQLRowExternal.GetAESUsername());
	m_SQLRowExternalOld.SetPasswordHash(m_SQLRowExternal.GetPasswordHash());
	m_SQLRowExternalOld.SetAESUserID(m_SQLRowExternal.GetAESUserID());
}

//$$******************************************************************

CEditRecordPropertyPageExternal::~CEditRecordPropertyPageExternal()
{
}

//$$******************************************************************

void CEditRecordPropertyPageExternal::DoDataExchange(CDataExchange* pDX)
{
	CSSPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUserName);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_BUTTON_PASSWORD, m_buttonPassword);
	DDX_Control(pDX, IDC_BUTTON_CHECK, m_buttonCheck);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEditRecordPropertyPageExternal, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_PASSWORD, OnButtonPassword)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, OnButtonCheckPassword)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CEditRecordPropertyPageExternal::OnInitDialog()
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

void CEditRecordPropertyPageExternal::OnButtonPassword()
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

void CEditRecordPropertyPageExternal::OnButtonCheckPassword()
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

BOOL CEditRecordPropertyPageExternal::OnKillActive()
{
	//COPY NEW ACCOUNT SETTINGS INTO HOLDING RECORD

	CString strAESHex = "";
	CString strUserName = GetEditBoxText(m_editUserName);
	TrimSpacesFromString(strUserName, FALSE);
	strUserName.MakeLower();

	if (strUserName != "")
	{
		CAESHelpers::PlainTextToAESHex(strUserName, strAESHex);
	}

	m_SQLRowExternal.SetAESUsername(strAESHex);

	if (m_strNewPassword != "")
	{
		m_SQLRowExternal.SetPasswordHash(CSSMD5::CalcMD5(m_strNewPassword));
	}

	bool bInUse = FALSE;
	if ( ( strAESHex != "") && ( strAESHex != m_SQLRowExternalOld.GetAESUsername()))
	{
		CSQLRowExternalAccount SQLRowExternalCheck;
		SQLRowExternalCheck.SetAESUsername(strAESHex);
		
		CSQLRepositoryExternalAccount RepoExt;
		switch (RepoExt.SelectRow(SQLRowExternalCheck, NULL).GetSQLError())
		{
		case SQLCRUD_ERR_NONE:
		case SQLCRUD_ERR_DUPLICATE:
			bInUse = TRUE;
			break;
		} 
	}

	if (TRUE == bInUse)
	{
		Prompter.Error("The selected user name is already assigned to another user");
		return FALSE;
	}

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************

void CEditRecordPropertyPageExternal::OnOK()
{
	CSSPropertyPage::OnOK();
}

//$$******************************************************************

BOOL CEditRecordPropertyPageExternal::OnQueryCancel()
{
	return CEditRecordPropertySheet::PromptBeforeCancel();
}

//********************************************************************

void CEditRecordPropertyPageExternal::OnCancel()
{
	CSSPropertyPage::OnCancel();
}

//$$******************************************************************
