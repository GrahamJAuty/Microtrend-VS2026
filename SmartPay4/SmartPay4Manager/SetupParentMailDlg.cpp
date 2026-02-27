//*******************************************************
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//*******************************************************
#include "StandardCombos.h"
//*******************************************************
#include "SetupParentMailDlg.h"
//*******************************************************
static const char* szNOTDEFINED	= "Not defined";
//*******************************************************

CSetupParentMailDlg::CSetupParentMailDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CSetupParentMailDlg::IDD, pParent)
{
	m_bKeepXmlFiles = FALSE;
	m_bKeepXmlLogin = FALSE;
	m_strSchoolID = "";
	m_strUsername = "";
	m_strPassword = "";
	m_strSoapVersion = "";
	m_nLastPaymentID = 0;
	m_nFormNameRefField = 0;
	m_nAdditionalRefField = 0;
	m_nInternalSnapshotID = 0;
	m_nMaxTransactionLines = 0;
	m_strStaffGroupList = "";
	m_nAccountPurse = 1;
}

//*******************************************************

void CSetupParentMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SOAP, m_comboSoap);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT1, m_comboAccountPurse);
	DDX_Control(pDX, IDC_COMBO_FORMNAME, m_comboFormName);
	DDX_Control(pDX, IDC_COMBO_ADDITIONALREFFIELD, m_comboAdditionalRefField);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLFILES, m_checkKeepXMLFiles);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLLOGIN, m_checkKeepXMLLogin);
	DDX_Control(pDX, IDC_EDIT_LASTPAYMENTID, m_editLastPaymentID);
	DDX_Control(pDX, IDC_EDIT_SNAPSHOTID, m_editSnapshotID);
	DDX_Control(pDX, IDC_EDIT_STAFFGROUPLIST, m_editStaffGroupList);
	DDX_Text(pDX, IDC_EDIT_SCHOOLID, m_strSchoolID);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUsername);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_CBString(pDX, IDC_COMBO_SOAP, m_strSoapVersion);
	DDX_Text(pDX, IDC_EDIT_LASTPAYMENTID, m_nLastPaymentID);
	DDX_CBIndex(pDX, IDC_COMBO_FORMNAME, m_nFormNameRefField);
	DDX_Text(pDX, IDC_EDIT_SNAPSHOTID, m_nInternalSnapshotID);
	DDX_Text(pDX, IDC_EDIT_TRANSACTIONBLOCKSIZE, m_nMaxTransactionLines);
	DDX_Check(pDX, IDC_CHECK_KEEPXMLFILES, m_bKeepXmlFiles);
	DDX_Check(pDX, IDC_CHECK_KEEPXMLLOGIN, m_bKeepXmlLogin);
	DDX_CBIndex(pDX, IDC_COMBO_ADDITIONALREFFIELD, m_nAdditionalRefField);
	DDX_Text(pDX, IDC_EDIT_STAFFGROUPLIST, m_strStaffGroupList);
	
}

//*******************************************************

BEGIN_MESSAGE_MAP(CSetupParentMailDlg, CSSAutoShutdownDialog)
	ON_STN_CLICKED(IDC_STATIC_SNAPSHOT, OnClickStaticSnapshot)
	ON_STN_CLICKED(IDC_STATIC_PAYMENT, OnClickStaticPayment)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
END_MESSAGE_MAP()

//*******************************************************

void CSetupParentMailDlg::CSSAutoShutdownPreInitDialog()
{
	m_data.Read();
	m_bKeepXmlFiles = m_data.m_bKeepXmlFiles;
	m_bKeepXmlLogin = m_data.m_bKeepXmlLogin;
	m_strUsername = m_data.m_strUsername;
	m_strPassword = m_data.m_strPassword;
	m_strSchoolID = m_data.m_strSchoolID;
	m_strSoapVersion = m_data.m_strSoapVersion;
	m_nLastPaymentID = m_data.m_nLastPaymentID;
	m_nInternalSnapshotID = m_data.m_nLastTransactionID;
	m_nAccountPurse = m_data.m_nAccountPurse;
	m_nFormNameRefField = m_data.m_nFormNameRefField;
	m_nAdditionalRefField = m_data.m_nAdditionalRefField;
	m_nMaxTransactionLines = m_data.m_nMaxTransactionLines;
	m_strStaffGroupList = m_data.m_strStaffGroupList;
}

//*******************************************************

BOOL CSetupParentMailDlg::CSSAutoShutdownPostInitDialog()
{
	CStandardCombos::FillSoapCombo( m_comboSoap, m_strSoapVersion );

	m_editLastPaymentID.EnableWindow(FALSE);
	m_editSnapshotID.EnableWindow(FALSE);

// add info texts

	m_comboFormName.AddString ( szNOTDEFINED );
	m_comboAdditionalRefField.AddString ( szNOTDEFINED );

	for ( int n = 1 ; n <= nMAX_INFOTEXTS ; n++ )
	{
		CSQLRepositoryUserText RepoText;
		m_comboFormName.AddString ( RepoText.GetDisplayInfoText(n) );
		m_comboAdditionalRefField.AddString (RepoText.GetDisplayInfoText(n) );
	}

	m_comboFormName.SetCurSel ( m_nFormNameRefField );
	m_comboAdditionalRefField.SetCurSel ( m_nAdditionalRefField );

//
	int nCurSel = 0;
	m_comboAccountPurse.AddString ( Account::Purse1.Label );
	
	if ( System.GetEnablePurse3Flag() == TRUE )
	{
		m_comboAccountPurse.AddString ( Account::Purse3.Label );
		if ( m_nAccountPurse == 3 )	nCurSel = 1;
	}
	m_comboAccountPurse.SetCurSel ( nCurSel );

	if (m_comboAccountPurse.GetCount() < 2)
	{
		m_comboAccountPurse.EnableWindow(FALSE);
	}

	if ( m_bKeepXmlFiles == FALSE && m_bKeepXmlLogin == FALSE )
	{
		m_checkKeepXMLFiles.ShowWindow(SW_HIDE);
		m_checkKeepXMLLogin.ShowWindow(SW_HIDE);
		m_buttonDelete.ShowWindow(SW_HIDE);
	}

	m_buttonAltKey1.ShowWindow( SW_HIDE );

	return TRUE;
}

//*******************************************************

void CSetupParentMailDlg::OnClickStaticPayment()
{
	if ( UpdateData() == TRUE )
	{
		m_editLastPaymentID.EnableWindow(TRUE);
		m_editLastPaymentID.SetFocus();
	}
}

//*******************************************************

void CSetupParentMailDlg::OnClickStaticSnapshot()
{
	if ( UpdateData() == TRUE )
	{
		m_editSnapshotID.EnableWindow(TRUE);
		m_editSnapshotID.SetFocus();
	}
}

//*******************************************************

void CSetupParentMailDlg::OnButtonAltKey1()
{
	if ( UpdateData() == TRUE )
	{
		m_checkKeepXMLFiles.ShowWindow(SW_SHOW);
		m_checkKeepXMLLogin.ShowWindow(SW_SHOW);
		m_buttonDelete.ShowWindow(SW_SHOW);
	}
}

//*******************************************************

void CSetupParentMailDlg::OnButtonDelete()
{
	CSmartPayLockRequest Request;
	Request.m_bWantLockBgnd = TRUE;
	Request.m_bWantLockServerInner = TRUE;
	
	CSmartPayLockManager LockManager;
	int nLockError = LockManager.AcquireLocks( Request, this );

	if ( LOCKMANAGER_OK == nLockError )						
	{
		CString strMask	 = System.GetWebPaymentPath ( "*.xml", nWEBPAYMENT_PARENTMAIL );
		::DeleteWithConfirmation ( strMask, "Delete XML Files", this );
	}
}

//*******************************************************

void CSetupParentMailDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		if (m_nFormNameRefField != 0 && (m_nFormNameRefField == m_nAdditionalRefField))
		{
			Prompter.FatalError("Cannot use the same field for both FormName and additional link reference!", "Verification");
			{
				m_comboAdditionalRefField.SetFocus();
				return;
			}
		}

		if (m_data.ValidateStaffList(m_strStaffGroupList) == FALSE)
		{
			m_data.DisplayStaffListError(this);
			m_editStaffGroupList.SetFocus();
			return;
		}

		CString strPurse = "";
		m_comboAccountPurse.GetLBText(m_comboAccountPurse.GetCurSel(), strPurse);
		int nAccountPurse = (strPurse == Account::Purse3.Label) ? 3 : 1;

		m_data.m_bKeepXmlFiles = (m_bKeepXmlFiles != 0);
		m_data.m_bKeepXmlLogin = (m_bKeepXmlLogin != 0);
		m_data.m_strUsername = m_strUsername;
		m_data.m_strPassword = m_strPassword;
		m_data.m_strSchoolID = m_strSchoolID;
		m_data.m_strSoapVersion = m_strSoapVersion;
		m_data.m_nAccountPurse = nAccountPurse;
		m_data.m_nLastPaymentID = m_nLastPaymentID;
		m_data.m_nLastTransactionID = m_nInternalSnapshotID;
		m_data.m_nFormNameRefField = m_nFormNameRefField;
		m_data.m_nAdditionalRefField = m_nAdditionalRefField;
		m_data.m_nMaxTransactionLines = m_nMaxTransactionLines;
		m_data.m_strStaffGroupList = m_strStaffGroupList;
		m_data.Save();

		EndDialog(IDOK);
	}
}

//*******************************************************
