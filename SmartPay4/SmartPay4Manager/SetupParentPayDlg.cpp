//$$******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SmartPayLockManager.h"
//$$******************************************************************
#include "SetupParentPay2Dlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "SetupParentPayDlg.h"
//$$******************************************************************

CSetupParentPayDlg::CSetupParentPayDlg(CWnd* pParent)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CSetupParentPayDlg::IDD, pParent)
{
	m_bKeepXmlFiles = FALSE;
	m_bKeepXmlLogin = FALSE;
	m_strSchoolID = "";
	m_strUsername = "";
	m_strPassword = "";
	m_strSoapVersion = "";
	m_strPosID = "";
	m_nLastDinerSnapshotID = 0;
	m_nLastBalanceSnapshotID = 0;
	m_nLastPosTxnID = 0;
	m_nNextPosTxnID = 0;
	m_bImportPayments1 = FALSE;
	m_nServiceID1= 0;
	m_nLastPaymentID1 = 0;
	m_nMealSession1 = 0;
	m_bImportPayments2 = FALSE;
	m_nServiceID2= 0;
	m_nLastPaymentID2 = 0;
	m_nMealSession2 = 0;

	m_nAccount1Purse = 1;
	m_nAccount2Purse = 1;
}

//$$******************************************************************

CSetupParentPayDlg::~CSetupParentPayDlg()
{
}

//$$******************************************************************

void CSetupParentPayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MEALSESSION1, m_editMealSession1 );
	DDX_Control(pDX, IDC_EDIT_MEALSESSION2, m_editMealSession2 );
	DDX_Control(pDX, IDC_EDIT_SNAPSHOTID, m_editSnapShotID1 );
	DDX_Control(pDX, IDC_EDIT_SNAPSHOTID2, m_editSnapShotID2 );
	DDX_Control(pDX, IDC_EDIT_SNAPSHOTID3, m_editSnapShotID3 );
	DDX_Control(pDX, IDC_EDIT_POSID, m_editPosID );
	DDX_Control(pDX, IDC_EDIT_NEXTID, m_editNextID );
	DDX_Control(pDX, IDC_EDIT_LASTPAYMENTID, m_editLastPaymentID1 );
	DDX_Control(pDX, IDC_EDIT_LASTPAYMENTID2, m_editLastPaymentID2 );
	DDX_Control(pDX, IDC_EDIT_SERVICEID, m_editServiceID1 );
	DDX_Control(pDX, IDC_EDIT_SERVICEID2, m_editServiceID2 );
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete );
	DDX_Control(pDX, IDC_CHECK_KEEPXMLFILES, m_checkKeepXMLFiles );
	DDX_Control(pDX, IDC_CHECK_KEEPXMLLOGIN, m_checkKeepXMLLogin );
	DDX_Control(pDX, IDC_CHECK_EXPORTPURSE2, m_checkExportPurse2Sales );
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1 );

	DDX_Control(pDX, IDC_COMBO_ACCOUNT1, m_comboAccount1Purse);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT2, m_comboAccount2Purse);
	DDX_Control(pDX, IDC_COMBO_SOAP, m_comboSoap);
	DDX_CBString(pDX, IDC_COMBO_SOAP, m_strSoapVersion);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUsername);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_SCHOOLID, m_strSchoolID);
	DDX_Text(pDX, IDC_EDIT_POSID, m_strPosID);
	DDX_Text(pDX, IDC_EDIT_SERVICEID, m_nServiceID1);
	DDX_Text(pDX, IDC_EDIT_LASTPAYMENTID, m_nLastPaymentID1);
	DDX_Text(pDX, IDC_EDIT_SNAPSHOTID, m_nLastPosTxnID);
	DDX_Text(pDX, IDC_EDIT_NEXTID, m_nNextPosTxnID);
	DDX_Text(pDX, IDC_EDIT_SNAPSHOTID2, m_nLastDinerSnapshotID);
	DDX_Text(pDX, IDC_EDIT_SNAPSHOTID3, m_nLastBalanceSnapshotID);
	DDX_Text(pDX, IDC_EDIT_MEALSESSION1, m_nMealSession1);
	DDX_Check(pDX, IDC_CHECK_IMPORTPAYMENTS, m_bImportPayments1);
	DDX_Text(pDX, IDC_EDIT_SERVICEID2, m_nServiceID2);
	DDX_Text(pDX, IDC_EDIT_LASTPAYMENTID2, m_nLastPaymentID2);
	DDX_Check(pDX, IDC_CHECK_IMPORTPAYMENTS2, m_bImportPayments2);
	DDX_Text(pDX, IDC_EDIT_MEALSESSION2, m_nMealSession2);
	DDX_Check(pDX, IDC_CHECK_KEEPXMLFILES, m_bKeepXmlFiles);
	DDX_Check(pDX, IDC_CHECK_KEEPXMLLOGIN, m_bKeepXmlLogin);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSetupParentPayDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_CHECK_IMPORTPAYMENTS, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_IMPORTPAYMENTS2, SetDisplay)
	ON_STN_CLICKED(IDC_STATIC_LASTPAYMENTID, OnClickStaticPayment1)
	ON_STN_CLICKED(IDC_STATIC_LASTPAYMENTID2, OnClickStaticPayment2)
	ON_STN_CLICKED(IDC_STATIC_SNAPSHOT, OnClickStaticSnapshot)
	ON_STN_CLICKED(IDC_STATIC_SNAPSHOT2, OnClickStaticSnapshot2)
	ON_STN_CLICKED(IDC_STATIC_SNAPSHOT3, OnClickStaticSnapshot3)
	ON_STN_CLICKED(IDC_STATIC_POSID, OnClickStaticPosid)
	ON_STN_CLICKED(IDC_STATIC_NEXTID, OnClickStaticNextid)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	ON_BN_CLICKED(IDC_BUTTON_SETUP, OnButtonSetup)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
END_MESSAGE_MAP()

//$$******************************************************************

void CSetupParentPayDlg::CSSAutoShutdownPreInitDialog()
{
	m_data.Read();
	m_bKeepXmlFiles = m_data.m_bKeepXmlFiles;
	m_bKeepXmlLogin = m_data.m_bKeepXmlLogin;
	m_strSchoolID = m_data.m_strSchoolID;
	m_strUsername = m_data.m_strUsername;
	m_strPassword = m_data.m_strPassword;
	m_strSoapVersion = m_data.m_strSoapVersion;
	m_strPosID = m_data.m_strPosID;
	m_bImportPayments1 = m_data.m_bImportAccount1;
	m_nLastPaymentID1 = m_data.m_nLastAccount1;
	m_nServiceID1 = m_data.m_nAccountID1;
	m_nMealSession1 = m_data.m_nMealSession1;
	m_nAccount1Purse = m_data.m_nAccount1Purse;
	m_bImportPayments2 = m_data.m_bImportAccount2;
	m_nAccount2Purse = m_data.m_nAccount2Purse;
	m_nLastPaymentID2 = m_data.m_nLastAccount2;
	m_nServiceID2 = m_data.m_nAccountID2;
	m_nMealSession2 = m_data.m_nMealSession2;
	m_nLastDinerSnapshotID = m_data.m_nLastDinerSnapshotID;
	m_nLastBalanceSnapshotID = m_data.m_nLastBalanceSnapshotID;
	m_nLastPosTxnID = m_data.GetLastPosTxnID();
	m_nNextPosTxnID = m_data.GetNextPosTxnID(FALSE);
}

//$$******************************************************************

BOOL CSetupParentPayDlg::CSSAutoShutdownPostInitDialog()
{
	m_editMealSession1.EnableWindow(FALSE);

	m_editPosID.EnableWindow(FALSE);
	m_editNextID.EnableWindow(FALSE);				// next transaction
	m_editSnapShotID1.EnableWindow(FALSE);			// last sent transaction id
	m_editSnapShotID2.EnableWindow(FALSE);			// last diner snapshot id
	m_editSnapShotID3.EnableWindow(FALSE);			// last balance snapshot id
	m_editLastPaymentID1.EnableWindow(FALSE);
	m_editLastPaymentID2.EnableWindow(FALSE);

	CStandardCombos::FillSoapCombo( m_comboSoap, m_strSoapVersion );

// account 1

	int nCurSel = 0;
	m_comboAccount1Purse.AddString ( Account::Purse1.Label );
	
	if ( System.GetEnablePurse3Flag() == TRUE )
	{
		m_comboAccount1Purse.AddString ( Account::Purse3.Label );
		if ( m_nAccount1Purse == 3 )	nCurSel = 1;
	}
	m_comboAccount1Purse.SetCurSel ( nCurSel );

	if ( m_comboAccount1Purse.GetCount() < 2 )
		m_comboAccount1Purse.EnableWindow(FALSE);

/// account 2

	nCurSel = 0;
	m_comboAccount2Purse.AddString ( Account::Purse1.Label );
	
	if ( System.GetEnablePurse3Flag() == TRUE )
	{
		m_comboAccount2Purse.AddString ( Account::Purse3.Label );
		if ( m_nAccount2Purse == 3 )	nCurSel = 1;
	}
	m_comboAccount2Purse.SetCurSel ( nCurSel );

	if ( m_comboAccount2Purse.GetCount() < 2 )
		m_comboAccount2Purse.EnableWindow(FALSE);

	SetDisplay();

	if ( m_bKeepXmlFiles == FALSE && m_bKeepXmlLogin == FALSE )
	{
		m_checkKeepXMLFiles.ShowWindow(SW_HIDE);
		m_checkKeepXMLLogin.ShowWindow(SW_HIDE);
		m_buttonDelete.ShowWindow(SW_HIDE);
	}

	m_checkExportPurse2Sales.SetCheck( m_data.GetExportPurse2SalesFlag() );

	m_buttonAltKey1.ShowWindow(SW_HIDE);

	return TRUE;
}

//$$******************************************************************

void CSetupParentPayDlg::SetDisplay()
{
	if ( UpdateData() == TRUE )
	{
		m_editServiceID1.EnableWindow(m_bImportPayments1);
		m_editMealSession1.EnableWindow(m_bImportPayments1);

		if ( m_comboAccount1Purse.GetCount() > 1 )
			m_comboAccount1Purse.EnableWindow(m_bImportPayments1);

		m_editServiceID2.EnableWindow(m_bImportPayments2);
		m_editMealSession2.EnableWindow(m_bImportPayments2);

		if ( m_comboAccount2Purse.GetCount() > 1 )
			m_comboAccount2Purse.EnableWindow(m_bImportPayments2);
	}
}

//$$******************************************************************

void CSetupParentPayDlg::OnClickStaticPayment1()
{
	if ( UpdateData() == TRUE )
	{
		m_editLastPaymentID1.EnableWindow(m_bImportPayments1);
		m_editLastPaymentID1.SetFocus();
	}
}

//$$******************************************************************

void CSetupParentPayDlg::OnClickStaticPayment2()
{
	if ( UpdateData() == TRUE )
	{
		m_editLastPaymentID2.EnableWindow(m_bImportPayments2);
		m_editLastPaymentID2.SetFocus();
	}
}

//void CSetupParentPayDlg::OnStnClickedStaticMealsession()
//{
//	if ( UpdateData() == TRUE )
//	{
//		m_editMealSession1.EnableWindow(m_bImportPayments1);
//		m_editMealSession1.SetFocus();
//	}
//}

//$$******************************************************************

void CSetupParentPayDlg::OnButtonSetup()
{
	CSetupParentPay2Dlg dlg ( &m_data );
	dlg.DoModal();
}

//$$******************************************************************

void CSetupParentPayDlg::OnClickStaticSnapshot()
{
	m_editSnapShotID1.EnableWindow(TRUE);		// last transaction id
	m_editSnapShotID1.SetFocus();
}

//$$******************************************************************

void CSetupParentPayDlg::OnClickStaticSnapshot2()
{
	if ( UpdateData() == TRUE )
	{
		m_editSnapShotID2.EnableWindow(TRUE);		// last diner snapshot id
		m_editSnapShotID2.SetFocus();
	}
}

//$$******************************************************************

void CSetupParentPayDlg::OnClickStaticSnapshot3()
{
	if ( UpdateData() == TRUE )
	{
		m_editSnapShotID3.EnableWindow(TRUE);		// last balance snapshot id
		m_editSnapShotID3.SetFocus();
	}
}

//$$******************************************************************

void CSetupParentPayDlg::OnClickStaticPosid()
{
	m_editPosID.EnableWindow(TRUE);					// last balance snapshot id
	m_editPosID.SetFocus();
}

//$$******************************************************************

void CSetupParentPayDlg::OnClickStaticNextid()
{
	m_editNextID.EnableWindow(TRUE);				// next transction ID
	m_editNextID.SetFocus();
}

//$$******************************************************************

void CSetupParentPayDlg::OnButtonAltKey1()
{	
	m_checkKeepXMLFiles.ShowWindow(SW_SHOW);
	m_checkKeepXMLLogin.ShowWindow(SW_SHOW);
	m_buttonDelete.ShowWindow(SW_SHOW);
}

//$$******************************************************************

void CSetupParentPayDlg::OnButtonDelete()
{
	CSmartPayLockRequest Request;
	Request.m_bWantLockBgnd = TRUE;
	Request.m_bWantLockServerInner = TRUE;
	
	CSmartPayLockManager LockManager;
	int nLockError = LockManager.AcquireLocks( Request, this );

	if ( LOCKMANAGER_OK == nLockError )						
	{
		CString strMask	 = System.GetWebPaymentPath ( "*.xml", nWEBPAYMENT_PARENTPAY );
		::DeleteWithConfirmation ( strMask, "Delete XML Files", this );
	}
}

//$$******************************************************************

void CSetupParentPayDlg::OnOK() 
{
	if ( UpdateData() == TRUE )
	{
		CString strErrorMsg = "";

		CString strPurse;

		m_comboAccount1Purse.GetLBText ( m_comboAccount1Purse.GetCurSel(), strPurse );
		int nAccount1Purse = ( strPurse == Account::Purse3.Label ) ? 3 : 1;

		m_comboAccount2Purse.GetLBText ( m_comboAccount2Purse.GetCurSel(), strPurse );
		int nAccount2Purse = ( strPurse == Account::Purse3.Label ) ? 3: 1;

//
		if ( m_bImportPayments1 == TRUE && m_bImportPayments2 == TRUE )
		{
			if ( m_nMealSession1 == m_nMealSession2 )
			{
				strErrorMsg = "Two Services cannot use the same Meal Session!";
				m_editMealSession2.SetFocus();
			}

			if ( nAccount1Purse == nAccount2Purse )
			{
				strErrorMsg = "Two Services cannot use the same account purse!";
				m_comboAccount2Purse.SetFocus();
			}
		}

		if ( m_bImportPayments2 == TRUE )
		{
			if ( m_nServiceID2 == 0  )
			{
				strErrorMsg = "Service2 ID has not been set!";
				m_editServiceID2.SetFocus();
			}
			else
			{
				if ( m_nMealSession2 == 0 )
				{
					strErrorMsg = "Service2 cannot use Meal Session 0!";
					m_editMealSession2.SetFocus();
				}
			}
		}
		else
		{
			if ( m_nMealSession2 != 0 )
			{
				strErrorMsg = "Service2 has not been enabled - the Meal Session must be set to 0!";
				m_editMealSession2.EnableWindow(TRUE);
				m_editMealSession2.SetFocus();
			}
		}

		if ( m_bImportPayments1 == TRUE )
		{
			if ( m_nServiceID1 == 0 )
			{
				strErrorMsg = "Service1 ID has not been set!";
				m_editServiceID1.SetFocus();
			}
			else
			{
				if ( m_nMealSession1 == 0 )
				{
					strErrorMsg = "Service1 cannot use Meal Session 0!";
					m_editMealSession1.SetFocus();
				}
			}
		}
		else
		{
			if ( m_nMealSession1 != 0 )
			{
				strErrorMsg = "Service1 has not been enabled - the Meal Session must be set to 0!";
				m_editMealSession1.EnableWindow(TRUE);
				m_editMealSession1.SetFocus();
			}
		}
//
		if ( strErrorMsg != "" )
		{
			Prompter.Error( strErrorMsg, "Invalid entry" );
			return;
		}

		m_data.m_bKeepXmlFiles			= ( m_bKeepXmlFiles != 0 );
		m_data.m_bKeepXmlLogin			= ( m_bKeepXmlLogin != 0 );
		m_data.m_strSchoolID			= m_strSchoolID;
		m_data.m_strUsername			= m_strUsername;
		m_data.m_strPassword			= m_strPassword;
		m_data.m_strSoapVersion			= m_strSoapVersion;
		m_data.m_bImportAccount1		= ( m_bImportPayments1 != 0 );
		m_data.m_nAccountID1			= m_nServiceID1;
		m_data.m_nMealSession1			= m_nMealSession1;
		m_data.m_nLastAccount1			= m_nLastPaymentID1;
		m_data.m_nAccount1Purse			= nAccount1Purse;
		m_data.m_nAccount2Purse			= nAccount2Purse;
		m_data.m_bImportAccount2		= ( m_bImportPayments2 != 0 );
		m_data.m_nAccountID2			= m_nServiceID2;
		m_data.m_nMealSession2			= m_nMealSession2;
		m_data.m_nLastAccount2			= m_nLastPaymentID2;
		m_data.m_nLastDinerSnapshotID	= m_nLastDinerSnapshotID;
		m_data.m_nLastBalanceSnapshotID	= m_nLastBalanceSnapshotID;
		m_data.m_strPosID				= m_strPosID;

		m_data.SetLastPosTxnID ( m_nLastPosTxnID );
		m_data.SetNextPosTxnID ( m_nNextPosTxnID );

		m_data.SetExportPurse2SalesFlag( m_checkExportPurse2Sales.GetCheck() != 0 );

		m_data.Save();

		EndDialog(IDOK);
	}
}

//$$******************************************************************
