//*******************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//*******************************************************
#include "..\SmartPay4Shared\SmartPayLockManager.h"
//*******************************************************
#include "SetupWisePay2Dlg.h"
#include "StandardCombos.h"
//*******************************************************
#include "SetupWisePayDlg.h"
//*******************************************************

CSetupWisePayDlg::CSetupWisePayDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CSetupWisePayDlg::IDD, pParent)
{
	m_nAccount1Purse = 1;
	m_nAccount2Purse = 1;
	m_bKeepXmlFiles = FALSE;
	m_bKeepXmlLogin = FALSE;
	m_strSchoolID = "";
	m_strSoapVersion = "";
	m_bImportAccount1 = FALSE;
	m_nAddPurse1ID = 0;
	m_nLastAccount1 = 0;
	m_bImportAccount2 = FALSE;
	m_nAddPurse2ID = 0;
	m_nLastAccount2 = 0;
	m_bIncludePurse2 = FALSE;
}

//*******************************************************

void CSetupWisePayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_ADDPURSE1, m_editAddPurse1);
	DDX_Control(pDX, IDC_EDIT_ADDPURSE2, m_editAddPurse2);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_CHECK_INCLUDEPURSE2, m_checkIncludePurse2);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLFILES, m_checkKeepXMLFiles);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLLOGIN, m_checkKeepXMLLogin);
	DDX_Control(pDX, IDC_EDIT_LASTPAYMENT, m_editLastPayment);
	DDX_Control(pDX, IDC_EDIT_LASTPAYMENT2, m_editLastPayment2);
	DDX_Text(pDX, IDC_EDIT_SCHOOLID, m_strSchoolID);
	DDX_Control(pDX, IDC_COMBO_SOAP, m_comboSoap);
	DDX_CBString(pDX, IDC_COMBO_SOAP, m_strSoapVersion);
	DDX_Check(pDX, IDC_CHECK_ACCOUNT1, m_bImportAccount1);
	DDX_Text(pDX, IDC_EDIT_ADDPURSE1, m_nAddPurse1ID);
	DDX_Text(pDX, IDC_EDIT_LASTPAYMENT, m_nLastAccount1);
	DDX_Check(pDX, IDC_CHECK_ACCOUNT2, m_bImportAccount2);
	DDX_Text(pDX, IDC_EDIT_ADDPURSE2, m_nAddPurse2ID);
	DDX_Text(pDX, IDC_EDIT_LASTPAYMENT2, m_nLastAccount2);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT1, m_comboAccount1Purse);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT2, m_comboAccount2Purse);
	DDX_Check(pDX, IDC_CHECK_INCLUDEPURSE2, m_bIncludePurse2);
	DDX_Check(pDX, IDC_CHECK_KEEPXMLFILES, m_bKeepXmlFiles);
	DDX_Check(pDX, IDC_CHECK_KEEPXMLLOGIN, m_bKeepXmlLogin);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
}

//*******************************************************

BEGIN_MESSAGE_MAP(CSetupWisePayDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_CHECK_ACCOUNT1, OnToggleAccount1)
	ON_BN_CLICKED(IDC_CHECK_ACCOUNT2, OnToggleAccount2)
	ON_STN_CLICKED(IDC_STATIC_PAYMENT1, OnClickStaticPayment1)
	ON_STN_CLICKED(IDC_STATIC_PAYMENT2, OnClickStaticPayment2)
	ON_BN_CLICKED(IDC_BUTTON_SETUP, OnButtonSetup)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
END_MESSAGE_MAP()

//*******************************************************

void CSetupWisePayDlg::CSSAutoShutdownPreInitDialog()
{
	m_data.Read();
	m_bKeepXmlFiles = m_data.m_bKeepXmlFiles;
	m_bKeepXmlLogin = m_data.m_bKeepXmlLogin;
	m_strSchoolID = m_data.m_strSchoolID;
	m_strSoapVersion = m_data.m_strSoapVersion;
	m_bImportAccount1 = m_data.m_bImportAccount1;
	m_nAddPurse1ID = m_data.m_nAccountID1;
	m_nLastAccount1 = m_data.m_nLastAccount1;
	m_bImportAccount2 = m_data.m_bImportAccount2;
	m_nAddPurse2ID = m_data.m_nAccountID2;
	m_nLastAccount2 = m_data.m_nLastAccount2;
	m_nAccount1Purse = m_data.m_nAccount1Purse;			// 1 or 3
	m_nAccount2Purse = m_data.m_nAccount2Purse;			// 1 or 3
	m_bIncludePurse2 = m_data.m_bIncludePurse2;
}

//*******************************************************

BOOL CSetupWisePayDlg::CSSAutoShutdownPostInitDialog()
{
	CStandardCombos::FillSoapCombo( m_comboSoap, m_strSoapVersion );

	m_editLastPayment.EnableWindow(FALSE);
	m_editLastPayment2.EnableWindow(FALSE);

// account 1

	int nCurSel = 0;
	m_comboAccount1Purse.AddString ( Account::Purse1.Label );
	
	if ( System.GetEnablePurse3Flag() == TRUE )
	{
		m_comboAccount1Purse.AddString ( Account::Purse3.Label );
		if (m_nAccount1Purse == 3)
		{
			nCurSel = 1;
		}
	}
	m_comboAccount1Purse.SetCurSel ( nCurSel );

	if (m_comboAccount1Purse.GetCount() < 2)
	{
		m_comboAccount1Purse.EnableWindow(FALSE);
	}

/// account 2

	nCurSel = 0;
	m_comboAccount2Purse.AddString ( Account::Purse1.Label );
	m_comboAccount2Purse.AddString ( Account::Purse2.Label );			// allow purse2 ( WhiftGift don;t done FSM)
	
	if (m_nAccount2Purse == 2)
	{
		nCurSel = 1;
	}
	
	if ( System.GetEnablePurse3Flag() == TRUE )
	{
		m_comboAccount2Purse.AddString ( Account::Purse3.Label );
		
		if (m_nAccount2Purse == 3)
		{
			nCurSel = 2;
		}
	}
	
	m_comboAccount2Purse.SetCurSel ( nCurSel );

	if (m_comboAccount2Purse.GetCount() < 2)
	{
		m_comboAccount2Purse.EnableWindow(FALSE);
	}

	SetDisplay();

	if ( m_bKeepXmlFiles == FALSE && m_bKeepXmlLogin == FALSE  )
	{
		m_checkKeepXMLFiles.ShowWindow(SW_HIDE);
		m_checkKeepXMLLogin.ShowWindow(SW_HIDE);
		m_buttonDelete.ShowWindow(SW_HIDE);
	}

	m_buttonAltKey1.ShowWindow(SW_HIDE);

	return TRUE;
}

//*******************************************************

void CSetupWisePayDlg::SetDisplay()
{
	if (UpdateData() == TRUE)
	{
		m_editAddPurse1.EnableWindow(m_bImportAccount1);
		m_checkIncludePurse2.EnableWindow(m_bImportAccount1);

		if (m_comboAccount1Purse.GetCount() > 1)
		{
			m_comboAccount1Purse.EnableWindow(m_bImportAccount1);
		}

		m_editAddPurse2.EnableWindow(m_bImportAccount2);

		if (m_comboAccount2Purse.GetCount() > 1)
		{
			m_comboAccount2Purse.EnableWindow(m_bImportAccount2);
		}
	}
}

//*******************************************************

void CSetupWisePayDlg::OnToggleAccount1()
{
	SetDisplay();
	m_editAddPurse1.SetFocus();
}

//*******************************************************

void CSetupWisePayDlg::OnToggleAccount2()
{
	SetDisplay();
	m_editAddPurse2.SetFocus();
}

//*******************************************************

void CSetupWisePayDlg::OnClickStaticPayment1()
{
	if ( UpdateData() == TRUE )
	{
		m_editLastPayment.EnableWindow(m_bImportAccount1);
		m_editLastPayment.SetFocus();
	}
}

//*******************************************************

void CSetupWisePayDlg::OnClickStaticPayment2()
{
	if ( UpdateData() == TRUE )
	{
		m_editLastPayment2.EnableWindow(m_bImportAccount2);
		m_editLastPayment2.SetFocus();
	}

}

//*******************************************************

void CSetupWisePayDlg::OnButtonSetup()
{
	CSetupWisePay2Dlg dlg ( &m_data, this );
	dlg.DoModal();
}

//*******************************************************

void CSetupWisePayDlg::OnButtonAltKey1()
{
	if ( UpdateData() == TRUE )
	{
		m_checkKeepXMLFiles.ShowWindow(SW_SHOW);
		m_checkKeepXMLLogin.ShowWindow(SW_SHOW);
		m_buttonDelete.ShowWindow(SW_SHOW);
	}
}

//*******************************************************

void CSetupWisePayDlg::OnButtonDelete()
{
	CSmartPayLockRequest Request;
	Request.m_bWantLockBgnd = TRUE;
	Request.m_bWantLockServerInner = TRUE;
	
	CSmartPayLockManager LockManager;
	int nLockError = LockManager.AcquireLocks( Request, this );

	if ( LOCKMANAGER_OK == nLockError )
	{
		CString strMask	 = System.GetWebPaymentPath ( "*.xml", nWEBPAYMENT_WISEPAY );
		::DeleteWithConfirmation ( strMask, "Delete XML Files", this );
	}
}

//*******************************************************

void CSetupWisePayDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		int nAccount1Purse = 1;									// assume purse1 for Service 1
		int nAccount2Purse = 3;									// assume purse 3 for Service 2

		CString strPurse = "";
		m_comboAccount1Purse.GetLBText(m_comboAccount1Purse.GetCurSel(), strPurse);
		
		if (strPurse == Account::Purse3.Label)
		{
			nAccount1Purse = 3;
		}

		m_comboAccount2Purse.GetLBText(m_comboAccount2Purse.GetCurSel(), strPurse);
		
		if (strPurse == Account::Purse1.Label)
		{
			nAccount2Purse = 1;
		}
		else if (strPurse == Account::Purse2.Label)
		{
			nAccount2Purse = 2;
		}

		if (m_bImportAccount1 == TRUE && m_bImportAccount2 == TRUE)
		{
			if (nAccount1Purse == nAccount2Purse)
			{
				Prompter.Error("Cannot use the same purse for 2 Product ID's!", "Invalid entry");
				m_comboAccount2Purse.SetFocus();
				return;
			}
		}

		if (m_bImportAccount1 == TRUE && m_bIncludePurse2 == TRUE && m_bImportAccount2 == TRUE && nAccount2Purse == 2)
		{
			Prompter.Error("Purse2 is already being used by Service 1!", "Invalid entry");
			m_comboAccount2Purse.SetFocus();
			return;
		}

		m_data.m_bKeepXmlFiles = (m_bKeepXmlFiles != 0);
		m_data.m_bKeepXmlLogin = (m_bKeepXmlLogin != 0);
		m_data.m_strSchoolID = m_strSchoolID;
		m_data.m_strSoapVersion = m_strSoapVersion;
		m_data.m_nAccount1Purse = nAccount1Purse;
		m_data.m_nAccount2Purse = nAccount2Purse;
		m_data.m_bImportAccount1 = (m_bImportAccount1 != 0);
		m_data.m_nAccountID1 = m_nAddPurse1ID;
		m_data.m_nLastAccount1 = m_nLastAccount1;
		m_data.m_bImportAccount2 = (m_bImportAccount2 != 0);
		m_data.m_nAccountID2 = m_nAddPurse2ID;
		m_data.m_nLastAccount2 = m_nLastAccount2;
		m_data.m_bIncludePurse2 = (m_bIncludePurse2 != 0);
		m_data.Save();

		EndDialog(IDOK);
	}
}

//*******************************************************
