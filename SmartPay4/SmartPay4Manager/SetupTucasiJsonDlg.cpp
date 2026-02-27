//*******************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//*******************************************************
#include "..\SmartPay4Shared\SmartPayLockManager.h"
//*******************************************************
#include "SetupTucasiJsonDlg.h"
//*******************************************************

CSetupTucasiJsonDlg::CSetupTucasiJsonDlg(CWnd* pParent)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CSetupTucasiJsonDlg::IDD, pParent)
{
	m_nAccountPurse = 1;
	m_bKeepXmlFiles = FALSE;
	m_strIPAddress = "";
	m_strSchoolID = "";
	m_strUserName = "";
	m_strPassword = "";
	m_nLastPaymentID = 0;
	m_nMaxTransactionLines = 0;
	m_bIncludePurse2 = FALSE;
	m_bExportBalances = FALSE;
}

//*******************************************************

void CSetupTucasiJsonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DELETEJSON, m_buttonDeleteJSON);
	DDX_Control(pDX, IDC_CHECK_INCLUDEPURSE2, m_checkIncludePurse2);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLFILES, m_checkKeepXMLFiles);
	DDX_Control(pDX, IDC_EDIT_LASTPAYMENT, m_editLastPayment);
	DDX_Text(pDX, IDC_EDIT_IPADDRESS, m_strIPAddress);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_SCHOOLID, m_strSchoolID);
	DDX_Text(pDX, IDC_EDIT_TRANSACTIONBLOCKSIZE, m_nMaxTransactionLines);
	DDX_Text(pDX, IDC_EDIT_LASTPAYMENT, m_nLastPaymentID);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT1, m_comboAccountPurse);
	DDX_Check(pDX, IDC_CHECK_INCLUDEPURSE2, m_bIncludePurse2);
	DDX_Check(pDX, IDC_CHECK_KEEPXMLFILES, m_bKeepXmlFiles);
	DDX_Check(pDX, IDC_CHECK_EXPORTBALANCES, m_bExportBalances);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
}

//*******************************************************

BEGIN_MESSAGE_MAP(CSetupTucasiJsonDlg, CSSAutoShutdownDialog)
	ON_STN_CLICKED(IDC_STATIC_PAYMENT1, OnClickStaticPayment1)
	ON_BN_CLICKED(IDC_CHECK_EXPORTBALANCES, OnToggleExportBalances)
	ON_BN_CLICKED(IDC_BUTTON_DELETEJSON, OnButtonDeleteJSON)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
END_MESSAGE_MAP()

//**************************************************************************

void CSetupTucasiJsonDlg::CSSAutoShutdownPreInitDialog()
{
	m_data.Read();
	m_bKeepXmlFiles = m_data.m_bKeepJsonFile;
	m_strIPAddress = m_data.m_strIPAddress;
	m_strUserName = m_data.m_strUsername;
	m_strPassword = m_data.m_strPassword;
	m_strSchoolID = m_data.m_strSchoolID;
	m_nLastPaymentID = m_data.m_nLastPaymentID;
	m_nMaxTransactionLines = m_data.m_nMaxTransactionLines;
	m_nAccountPurse = m_data.m_nAccountPurse;
	m_bExportBalances = m_data.m_bExportBalances;
	m_bIncludePurse2 = m_data.m_bIncludePurse2;
}

//**************************************************************************

BOOL CSetupTucasiJsonDlg::CSSAutoShutdownPostInitDialog()
{
	m_editLastPayment.EnableWindow(FALSE);

	int nCurSel = 0;
	m_comboAccountPurse.AddString ( Account::Purse1.Label );
	
	if (System.GetEnablePurse3Flag() == TRUE)
	{
		m_comboAccountPurse.AddString(Account::Purse3.Label);
		if (m_nAccountPurse == 3)
		{
			nCurSel = 1;
		}
	}
	m_comboAccountPurse.SetCurSel ( nCurSel );

	if (m_comboAccountPurse.GetCount() < 2)
	{
		m_comboAccountPurse.EnableWindow(FALSE);
	}

	SetDisplay();

	if ( m_bKeepXmlFiles == FALSE )
	{
		m_checkKeepXMLFiles.ShowWindow(SW_HIDE);
		m_buttonDeleteJSON.ShowWindow(SW_HIDE);
	}

	m_buttonAltKey1.ShowWindow(SW_HIDE);
	return TRUE;
}

//**************************************************************************

void CSetupTucasiJsonDlg::OnToggleExportBalances()	
{ 
	SetDisplay();
}

//**************************************************************************

void CSetupTucasiJsonDlg::SetDisplay()
{
	if ( UpdateData() == TRUE )
	{
		BOOL bAllowed = (  m_comboAccountPurse.GetCount() > 1 ) ? TRUE : FALSE;
		m_comboAccountPurse.EnableWindow(bAllowed);

		m_checkIncludePurse2.EnableWindow(m_bExportBalances);
	}
}

//******************************************************************

void CSetupTucasiJsonDlg::OnClickStaticPayment1()
{
	if ( UpdateData() == TRUE )
	{
		m_editLastPayment.EnableWindow(TRUE);
		m_editLastPayment.SetFocus();
	}
}

//*******************************************************************

void CSetupTucasiJsonDlg::OnButtonAltKey1()
{
	if ( UpdateData() == TRUE )
	{
		m_checkKeepXMLFiles.ShowWindow(SW_SHOW);
		m_buttonDeleteJSON.ShowWindow(SW_SHOW);
	}
}

//******************************************************************

void CSetupTucasiJsonDlg::OnButtonDeleteJSON()
{
	CSmartPayLockRequest Request;
	Request.m_bWantLockBgnd = TRUE;
	Request.m_bWantLockServerInner = TRUE;
	
	CSmartPayLockManager LockManager;
	int nLockError = LockManager.AcquireLocks( Request, this );

	if ( LOCKMANAGER_OK == nLockError )
	{
		CString strMask	 = System.GetWebPaymentPath ( "*.json", nWEBPAYMENT_TUCASIv2 );
		::DeleteWithConfirmation ( strMask, "Delete JSON Files", this );
	}
}

//******************************************************************

void CSetupTucasiJsonDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		CString strPurse = "";
		m_comboAccountPurse.GetLBText(m_comboAccountPurse.GetCurSel(), strPurse);
		int nAccountPurse = (strPurse == Account::Purse3.Label) ? 3 : 1;

		m_data.m_bKeepJsonFile = (m_bKeepXmlFiles != 0);
		m_data.m_strIPAddress = m_strIPAddress;
		m_data.m_strUsername = m_strUserName;
		m_data.m_strPassword = m_strPassword;
		m_data.m_strSchoolID = m_strSchoolID;
		m_data.m_nMaxTransactionLines = m_nMaxTransactionLines;
		m_data.m_nLastPaymentID = m_nLastPaymentID;
		m_data.m_nAccountPurse = nAccountPurse;
		m_data.m_bExportBalances = (m_bExportBalances != 0);
		m_data.m_bIncludePurse2 = (m_bIncludePurse2 != 0);
		m_data.Save();

		EndDialog(IDOK);
	}
}

//******************************************************************
