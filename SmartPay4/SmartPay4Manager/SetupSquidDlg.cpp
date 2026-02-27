//*******************************************************
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//*******************************************************
#include "StandardCombos.h"
//*******************************************************
#include "SetupSquidDlg.h"
//*******************************************************
#define szNOTDEFINED		"Not defined"
//*******************************************************

CSetupSquidDlg::CSetupSquidDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetupSquidDlg::IDD, pParent)
{
	m_data.Read();
	m_nAccountPurse = 1;
	m_bKeepLinkLog = FALSE;
	m_bKeepXmlFiles = FALSE;
	m_bKeepXmlLogin = FALSE;
	m_nMaxTransactionLines = 0;
	m_nSquidCVVField = 0;
	m_nSquidCardField = 0;
	m_nSquidCardHolderField = 0;
	m_nSquidExternalRefField = 0;
}

//*******************************************************

void CSetupSquidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLFILES, m_checkKeepXMLFiles);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLLOGIN, m_checkKeepXMLLogin);
	DDX_Text(pDX, IDC_EDIT_SCHOOLID, m_strMerchantID );
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strIssuerID);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Control(pDX, IDC_COMBO_SOAP, m_comboSoap);
	DDX_CBString(pDX, IDC_COMBO_SOAP, m_strSoapVersion);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT1, m_comboAccountPurse);
	DDX_Text(pDX, IDC_EDIT_TRANSACTIONBLOCKSIZE, m_nMaxTransactionLines);
	DDX_Check(pDX, IDC_CHECK_KEEPXMLFILES, m_bKeepXmlFiles);
	DDX_Check(pDX, IDC_CHECK_KEEPXMLLOGIN, m_bKeepXmlLogin);
	DDX_Text(pDX, IDC_EDIT_IPADDRESS, m_strIPAddress);
	DDX_Control(pDX, IDC_COMBO_SQUIDCARD, m_comboSquidCard);
	DDX_Control(pDX, IDC_COMBO_SQUIDCVV, m_comboSquidCVV);
	DDX_CBIndex(pDX, IDC_COMBO_SQUIDCARD, m_nSquidCardField);
	DDX_CBIndex(pDX, IDC_COMBO_SQUIDCVV, m_nSquidCVVField);
	DDX_Control(pDX, IDC_COMBO_SQUIDPUPILID, m_comboSquidPupilID);
	DDX_CBIndex(pDX, IDC_COMBO_SQUIDPUPILID, m_nSquidCardHolderField);
	DDX_Control(pDX, IDC_COMBO_SQUIDEXTREFERENCE, m_comboExternalRef);
	DDX_CBIndex(pDX, IDC_COMBO_SQUIDEXTREFERENCE, m_nSquidExternalRefField);
	DDX_Check(pDX, IDC_CHECK_KEEPLINKLOG, m_bKeepLinkLog);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
}

//*******************************************************

BEGIN_MESSAGE_MAP(CSetupSquidDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
END_MESSAGE_MAP()

//*******************************************************

void CSetupSquidDlg::CSSAutoShutdownPreInitDialog()
{
	m_bKeepXmlFiles = m_data.m_bKeepXmlFiles;
	m_bKeepXmlLogin = m_data.m_bKeepXmlLogin;
	m_bKeepLinkLog = m_data.m_bKeepLinkLog;
	m_strIPAddress = m_data.m_strIPAddress;
	m_strSoapVersion = m_data.m_strSoapVersion;
	m_strMerchantID = m_data.m_strMerchantID;
	m_strIssuerID = m_data.m_strIssuerID;
	m_strPassword = m_data.m_strPassword;
	m_nAccountPurse = m_data.m_nAccountPurse;
	m_nMaxTransactionLines = m_data.m_nMaxTransactionLines;
	m_nSquidCardHolderField = m_data.m_nSquidCardHolderField;
	m_nSquidExternalRefField = m_data.m_nSquidExtRefField;
	m_nSquidCardField = m_data.m_nSquidCardField;
	m_nSquidCVVField = m_data.m_nSquidCVVField;
}

//*******************************************************

BOOL CSetupSquidDlg::CSSAutoShutdownPostInitDialog()
{
	CStandardCombos::FillSoapCombo(m_comboSoap, m_strSoapVersion);

	int nCurSel = 0;
	m_comboAccountPurse.AddString(Account::Purse1.Label);

	if (System.GetEnablePurse3Flag() == TRUE)
	{
		m_comboAccountPurse.AddString(Account::Purse3.Label);
		if (m_nAccountPurse == 3)
		{
			nCurSel = 1;
		}
	}
	m_comboAccountPurse.SetCurSel(nCurSel);

	if (m_comboAccountPurse.GetCount() < 2)
	{
		m_comboAccountPurse.EnableWindow(FALSE);
	}

	// add info texts

	m_comboSquidCard.AddString(szNOTDEFINED);
	m_comboSquidCVV.AddString(szNOTDEFINED);
	m_comboSquidPupilID.AddString(szNOTDEFINED);
	m_comboExternalRef.AddString(szNOTDEFINED);

	for (int n = 1; n <= nMAX_INFOTEXTS; n++)
	{
		CSQLRepositoryUserText RepoText;
		CString strDisplayText = RepoText.GetDisplayInfoText(n);

		m_comboSquidCard.AddString(strDisplayText);
		m_comboSquidCVV.AddString(strDisplayText);
		m_comboSquidPupilID.AddString(strDisplayText);
		m_comboExternalRef.AddString(strDisplayText);
	}

	m_comboSquidCard.SetCurSel(m_nSquidCardField);
	m_comboSquidCVV.SetCurSel(m_nSquidCVVField);
	m_comboSquidPupilID.SetCurSel(m_nSquidCardHolderField);
	m_comboExternalRef.SetCurSel(m_nSquidExternalRefField);

	SetDisplay();

	if ((FALSE == m_bKeepXmlFiles) && (FALSE == m_bKeepXmlLogin))
	{
		m_checkKeepXMLFiles.ShowWindow(SW_HIDE);
		m_checkKeepXMLLogin.ShowWindow(SW_HIDE);
		m_buttonDelete.ShowWindow(SW_HIDE);
	}

	SetDisplay();

	m_buttonAltKey1.ShowWindow(SW_HIDE);

	return TRUE;
}

//*******************************************************

void CSetupSquidDlg::SetDisplay()
{
	if ( UpdateData() == TRUE )
	{
		BOOL bAllowed = (  m_comboAccountPurse.GetCount() > 1 ) ? TRUE : FALSE;
		m_comboAccountPurse.EnableWindow(bAllowed);
	}
}

//*******************************************************

void CSetupSquidDlg::OnButtonAltKey1()
{
	if ( UpdateData() == TRUE )
	{
		m_checkKeepXMLFiles.ShowWindow(SW_SHOW);
		m_checkKeepXMLLogin.ShowWindow(SW_SHOW);
		m_buttonDelete.ShowWindow(SW_SHOW);
	}
}

//*******************************************************

void CSetupSquidDlg::OnButtonDelete()
{
	CSmartPayLockRequest Request;
	Request.m_bWantLockBgnd = TRUE;
	Request.m_bWantLockServerInner = TRUE;
	
	CSmartPayLockManager LockManager;
	int nLockError = LockManager.AcquireLocks( Request, this );

	if ( LOCKMANAGER_OK == nLockError )						
	{
		CString strMask	 = System.GetWebPaymentPath ( "*.xml", nWEBPAYMENT_SQUID );
		::DeleteWithConfirmation ( strMask, "Delete XML Files", this );
	}
}

//*******************************************************

void CSetupSquidDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		if (m_nSquidCardHolderField == 0 && m_nSquidExternalRefField == 0)
		{
			Prompter.FatalError("No sQuid link reference fields have been specified!", "Invalid Entry");
			m_comboSquidPupilID.SetFocus();
			return;
		}

		if (m_nSquidCardField == 0)
		{
			if (Prompter.YesNo("A sQuid card number field has not been specified!\n\nAre you sure you do not wish to save it against the relevant account?", "Please confirm") == IDNO)
			{
				m_comboSquidCard.SetFocus();
				return;
			}
		}

		if (m_nSquidCVVField == 0)
		{
			if (Prompter.YesNo("A sQuid card CVV field has not been specified!\n\nAre you sure you do not wish to save it against the relevant account?", "Please confirm") == IDNO)
			{
				m_comboSquidCVV.SetFocus();
				return;
			}
		}

		if (m_nSquidCardHolderField == 0 && m_nSquidExternalRefField == 0)
		{
			Prompter.FatalError("A sQuid link reference field has not been specified!", "Invalid Entry");
			m_comboSquidPupilID.SetFocus();
			return;
		}

		if (m_nSquidCardField == m_nSquidCVVField && m_nSquidCardField != 0)
		{
			Prompter.FatalError("Cannot use the same database field for both sQuid Card Number & sQuid Card CVV!", "Invalid Entry");
			m_comboSquidCVV.SetFocus();
			return;
		}

		if (m_nSquidCardHolderField != 0 && m_nSquidCardField == m_nSquidCardHolderField || m_nSquidExternalRefField != 0 && m_nSquidCardField == m_nSquidExternalRefField)
		{
			Prompter.FatalError("Cannot use the same database field for both sQuid Card Number & sQuid link reference!", "Invalid Entry");
			m_comboSquidCard.SetFocus();
			return;
		}

		if (m_nSquidCardHolderField != 0 && m_nSquidCVVField == m_nSquidCardHolderField || m_nSquidExternalRefField != 0 && m_nSquidCVVField == m_nSquidExternalRefField)
		{
			Prompter.FatalError("Cannot use the same database field for both sQuid Card CVV & sQuid link reference!", "Invalid Entry");
			m_comboSquidCVV.SetFocus();
			return;
		}

		CString strPurse;
		m_comboAccountPurse.GetLBText(m_comboAccountPurse.GetCurSel(), strPurse);
		int nAccountPurse = (strPurse == Account::Purse3.Label) ? 3 : 1;

		m_data.m_bKeepXmlFiles = (m_bKeepXmlFiles != 0);
		m_data.m_bKeepXmlLogin = (m_bKeepXmlLogin != 0);
		m_data.m_bKeepLinkLog = (m_bKeepLinkLog != 0);
		m_data.m_strIPAddress = m_strIPAddress;
		m_data.m_strSoapVersion = m_strSoapVersion;
		m_data.m_strMerchantID = m_strMerchantID;
		m_data.m_strIssuerID = m_strIssuerID;
		m_data.m_strPassword = m_strPassword;
		m_data.m_nAccountPurse = nAccountPurse;
		m_data.m_nMaxTransactionLines = m_nMaxTransactionLines;
		m_data.m_nSquidCardHolderField = m_nSquidCardHolderField;
		m_data.m_nSquidExtRefField = m_nSquidExternalRefField;
		m_data.m_nSquidCardField = m_nSquidCardField;
		m_data.m_nSquidCVVField = m_nSquidCVVField;
		m_data.Save();

		EndDialog(IDOK);
	}
}

//*******************************************************
