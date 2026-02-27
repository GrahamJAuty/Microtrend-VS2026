//*******************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//*******************************************************
#include "..\SmartPay4Shared\SmartPayLockManager.h"
//*******************************************************
#include "StandardCombos.h"
//*******************************************************
#include "SetupSchoolcommsDlg.h"
//*******************************************************

CSetupSchoolcommsDlg::CSetupSchoolcommsDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CSetupSchoolcommsDlg::IDD, pParent)
{
	m_bKeepXmlFiles = FALSE;
	m_bKeepXmlLogin = FALSE;
	m_strSchoolID = "";
	m_strUserName = "";
	m_strPassword = "";
	m_strMemorable = "";
	m_strSoapVersion = "";
	m_nLastPaymentID = 0;
	m_nMaxTransactionLines = 0;
	m_bIncludePurse2 = FALSE;
	m_nAccountPurse = 1;
}

//*******************************************************

void CSetupSchoolcommsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SCHOOLID, m_strSchoolID);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_MEMORABLE, m_strMemorable);
	DDX_CBString(pDX, IDC_COMBO_SOAP, m_strSoapVersion);
	DDX_Text(pDX, IDC_EDIT_TRANSACTIONBLOCKSIZE, m_nMaxTransactionLines);
	DDX_Text(pDX, IDC_EDIT_LASTPAYMENTID, m_nLastPaymentID);
	DDX_Check(pDX, IDC_CHECK_INCLUDEPURSE2, m_bIncludePurse2);
	DDX_Check(pDX, IDC_CHECK_KEEPXMLFILES, m_bKeepXmlFiles);
	DDX_Check(pDX, IDC_CHECK_KEEPXMLLOGIN, m_bKeepXmlLogin);
	/*****/
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLFILES, m_checkKeepXmlFiles);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLLOGIN, m_checkKeepXmlLogin);
	DDX_Control(pDX, IDC_COMBO_SOAP, m_comboSoap);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT1, m_comboAccountPurse);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_EDIT_LASTPAYMENTID, m_editLastPaymentID);
}

//*******************************************************

BEGIN_MESSAGE_MAP(CSetupSchoolcommsDlg, CSSAutoShutdownDialog)
	ON_STN_CLICKED(IDC_STATIC_PAYMENT, OnClickStaticPayment)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
END_MESSAGE_MAP()

//*******************************************************

void CSetupSchoolcommsDlg::CSSAutoShutdownPreInitDialog()
{
	m_data.Read();
	m_bKeepXmlFiles = m_data.m_bKeepXmlFiles;
	m_bKeepXmlLogin = m_data.m_bKeepXmlLogin;
	m_strSoapVersion = m_data.m_strSoapVersion;
	m_strSchoolID = m_data.m_strSchoolID;
	m_strUserName = m_data.m_strUsername;
	m_strPassword = m_data.m_strPassword;
	m_strMemorable = m_data.m_strMemorable;
	m_nLastPaymentID = m_data.m_nLastPaymentID;
	m_nAccountPurse = m_data.m_nAccountPurse;
	m_nMaxTransactionLines = m_data.m_nMaxTransactionLines;
	m_bIncludePurse2 = m_data.m_bIncludePurse2;
}

//*******************************************************

BOOL CSetupSchoolcommsDlg::CSSAutoShutdownPostInitDialog()
{
	CStandardCombos::FillSoapCombo( m_comboSoap, m_strSoapVersion );
	m_editLastPaymentID.EnableWindow(FALSE);

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

//	SetDisplay();

	if ( m_bKeepXmlFiles == FALSE &&  m_bKeepXmlLogin == FALSE )
	{
		m_checkKeepXmlFiles.ShowWindow(SW_HIDE);
		m_checkKeepXmlLogin.ShowWindow(SW_HIDE);
		m_buttonDelete.ShowWindow(SW_HIDE);
	}

	m_buttonAltKey1.ShowWindow( SW_HIDE );

	return TRUE;
}

//*******************************************************

void CSetupSchoolcommsDlg::OnClickStaticPayment()
{
	m_editLastPaymentID.EnableWindow(TRUE);
	m_editLastPaymentID.SetFocus();
}

//*******************************************************************

void CSetupSchoolcommsDlg::OnButtonAltKey1()
{
	m_checkKeepXmlFiles.ShowWindow(SW_SHOW);
	m_checkKeepXmlLogin.ShowWindow(SW_SHOW);
	m_buttonDelete.ShowWindow(SW_SHOW);
}

//*******************************************************

void CSetupSchoolcommsDlg::OnButtonDelete()
{
	CSmartPayLockRequest Request;
	Request.m_bWantLockBgnd = TRUE;
	Request.m_bWantLockServerInner = TRUE;

	CSmartPayLockManager LockManager;
	int nLockError = LockManager.AcquireLocks( Request, this );

	if ( LOCKMANAGER_OK == nLockError )						
	{
		CString strMask	 = System.GetWebPaymentPath ( "*.xml", nWEBPAYMENT_SCHOOLCOMMS );
		::DeleteWithConfirmation ( strMask, "Delete XML Files", this );
	}
}

//*******************************************************

void CSetupSchoolcommsDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		CString strPurse;	m_comboAccountPurse.GetLBText(m_comboAccountPurse.GetCurSel(), strPurse);
		int nAccountPurse = (strPurse == Account::Purse3.Label) ? 3 : 1;

		m_data.m_bKeepXmlFiles = (m_bKeepXmlFiles != 0);
		m_data.m_bKeepXmlLogin = (m_bKeepXmlLogin != 0);
		m_data.m_strUsername = m_strUserName;
		m_data.m_strPassword = m_strPassword;
		m_data.m_strSchoolID = m_strSchoolID;
		m_data.m_strMemorable = m_strMemorable;
		m_data.m_strSoapVersion = m_strSoapVersion;
		m_data.m_nLastPaymentID = m_nLastPaymentID;
		m_data.m_nAccountPurse = nAccountPurse;
		m_data.m_nMaxTransactionLines = m_nMaxTransactionLines;
		m_data.m_bIncludePurse2 = (m_bIncludePurse2 != 0);
		m_data.Save();

		EndDialog(IDOK);
	}
}

//*******************************************************
