//*******************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//*******************************************************
#include "..\SmartPay4Shared\SmartPayLockManager.h"
//*******************************************************
#include "StandardCombos.h"
//*******************************************************
#include "SetupTucasiDlg.h"
//*******************************************************

CSetupTucasiDlg::CSetupTucasiDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CSetupTucasiDlg::IDD, pParent)
{
	m_bKeepXmlFiles = FALSE;
	m_bKeepXmlLogin = FALSE;
	m_strSchoolID = "";
	m_strUserName = "";
	m_strPassword = "";
	m_strSoapVersion = "";
	m_nLastPaymentID = 0;
	m_bIncludePurse2 = FALSE;
	m_bImportPayments = FALSE;
	m_bExportUpdates = FALSE;
	m_bExportBalances = FALSE;
}

//*******************************************************

void CSetupTucasiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_CHECK_INCLUDEPURSE2, m_checkIncludePurse2);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLFILES, m_checkKeepXMLFiles);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLLOGIN, m_checkKeepXMLLogin);
	DDX_Control(pDX, IDC_EDIT_LASTPAYMENT, m_editLastPayment);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_SCHOOLID, m_strSchoolID);
	DDX_Text(pDX, IDC_EDIT_LASTPAYMENT, m_nLastPaymentID);
	DDX_Control(pDX, IDC_COMBO_SOAP, m_comboSoap);
	DDX_CBString(pDX, IDC_COMBO_SOAP, m_strSoapVersion);
	DDX_Check(pDX, IDC_CHECK_IMPORTPAYMENTS, m_bImportPayments);
	DDX_Check(pDX, IDC_CHECK_EXPORTUPDATES, m_bExportUpdates);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT1, m_comboAccountPurse);
	DDX_Check(pDX, IDC_CHECK_INCLUDEPURSE2, m_bIncludePurse2);
	DDX_Check(pDX, IDC_CHECK_KEEPXMLFILES, m_bKeepXmlFiles);
	DDX_Check(pDX, IDC_CHECK_EXPORTBALANCES, m_bExportBalances);
	DDX_Check(pDX, IDC_CHECK_KEEPXMLLOGIN, m_bKeepXmlLogin);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
}

//*******************************************************

BEGIN_MESSAGE_MAP(CSetupTucasiDlg, CSSAutoShutdownDialog)
	ON_STN_CLICKED(IDC_STATIC_PAYMENT1, OnClickStaticPayment1)
	ON_BN_CLICKED(IDC_CHECK_EXPORTBALANCES, OnToggleExportBalances)
	ON_BN_CLICKED(IDC_CHECK_IMPORTPAYMENTS, OnToggleImportPayments)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
END_MESSAGE_MAP()

//*******************************************************

void CSetupTucasiDlg::CSSAutoShutdownPreInitDialog()
{
	m_data.Read();
	m_bKeepXmlFiles = m_data.m_bKeepXmlFiles;
	m_bKeepXmlLogin = m_data.m_bKeepXmlLogin;
	m_strUserName = m_data.m_strUserName;
	m_strPassword = m_data.m_strPassword;
	m_strSchoolID = m_data.m_strSchoolID;
	m_strSoapVersion = m_data.m_strSoapVersion;
	m_nLastPaymentID = m_data.m_nLastPaymentID;
	m_bImportPayments = m_data.m_bImportPayments;
	m_bExportUpdates = m_data.m_bExportUpdates;
	m_bExportBalances = m_data.m_bExportBalances;
	m_bIncludePurse2 = m_data.m_bIncludePurse2;
}

//*******************************************************

BOOL CSetupTucasiDlg::CSSAutoShutdownPostInitDialog()
{
	int nAccountPurse = m_data.m_nAccountPurse;

	m_editLastPayment.EnableWindow(FALSE);

	CStandardCombos::FillSoapCombo( m_comboSoap, m_strSoapVersion );
	
	int nCurSel = 0;
	m_comboAccountPurse.AddString ( Account::Purse1.Label );
	
	if (System.GetEnablePurse3Flag() == TRUE)
	{
		m_comboAccountPurse.AddString(Account::Purse3.Label);
		if (nAccountPurse == 3)
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

	if ( m_bKeepXmlFiles == FALSE && m_bKeepXmlLogin == FALSE )
	{
		m_checkKeepXMLFiles.ShowWindow(SW_HIDE);
		m_checkKeepXMLLogin.ShowWindow(SW_HIDE);
		m_buttonDelete.ShowWindow(SW_HIDE);
	}

	return TRUE;
}

//*******************************************************

void CSetupTucasiDlg::OnToggleExportBalances()	
{ 
	SetDisplay();
}

//*******************************************************

void CSetupTucasiDlg::OnToggleImportPayments()
{ 
	SetDisplay(); 
}

//*******************************************************

void CSetupTucasiDlg::SetDisplay()
{
	if ( UpdateData() == TRUE )
	{
		BOOL bAllowed = (  m_comboAccountPurse.GetCount() > 1 ) ? m_bImportPayments : FALSE;
		m_comboAccountPurse.EnableWindow(bAllowed);

		m_checkIncludePurse2.EnableWindow(m_bExportBalances);
	}
}

//*******************************************************

void CSetupTucasiDlg::OnClickStaticPayment1()
{
	if ( UpdateData() == TRUE )
	{
		m_editLastPayment.EnableWindow(TRUE);
		m_editLastPayment.SetFocus();
	}
}

//*******************************************************

void CSetupTucasiDlg::OnButtonAltKey1()
{
	if ( UpdateData() == TRUE )
	{
		m_checkKeepXMLFiles.ShowWindow(SW_SHOW);
		m_checkKeepXMLLogin.ShowWindow(SW_SHOW);
		m_buttonDelete.ShowWindow(SW_SHOW);
	}
}

//*******************************************************

void CSetupTucasiDlg::OnButtonDelete()
{
	CSmartPayLockRequest Request;
	Request.m_bWantLockBgnd = TRUE;
	Request.m_bWantLockServerInner = TRUE;
	
	CSmartPayLockManager LockManager;
	int nLockError = LockManager.AcquireLocks( Request, this );

	if ( LOCKMANAGER_OK == nLockError )
	{
		CString strMask	 = System.GetWebPaymentPath ( "*.xml", nWEBPAYMENT_TUCASI );
		::DeleteWithConfirmation ( strMask, "Delete XML Files", this );
	}
}

//*******************************************************

void CSetupTucasiDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		CString strPurse = "";
		m_comboAccountPurse.GetLBText(m_comboAccountPurse.GetCurSel(), strPurse);
		int nAccountPurse = (strPurse == Account::Purse3.Label) ? 3 : 1;

		m_data.m_bKeepXmlFiles = (m_bKeepXmlFiles != 0);
		m_data.m_bKeepXmlLogin = (m_bKeepXmlLogin != 0);
		m_data.m_strUserName = m_strUserName;
		m_data.m_strPassword = m_strPassword;
		m_data.m_strSchoolID = m_strSchoolID;
		m_data.m_strSoapVersion = m_strSoapVersion;
		m_data.m_nLastPaymentID = m_nLastPaymentID;
		m_data.m_nAccountPurse = nAccountPurse;
		m_data.m_bImportPayments = (m_bImportPayments != 0);
		m_data.m_bExportUpdates = (m_bExportUpdates != 0);
		m_data.m_bExportBalances = (m_bExportBalances != 0);
		m_data.m_bIncludePurse2 = (m_bIncludePurse2 != 0);
		m_data.Save();

		EndDialog(IDOK);
	}
}

//*******************************************************
