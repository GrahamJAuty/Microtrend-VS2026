//*******************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//*******************************************************
#include "SetupWisePay2Dlg.h"
//*******************************************************
static const char* szNOTDEFINED	= "Not defined";
//*******************************************************

CSetupWisePay2Dlg::CSetupWisePay2Dlg(CWisePayData* pData, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetupWisePay2Dlg::IDD, pParent)
{
	m_pData = pData;
	m_bAddUnknowns = m_pData->m_bAddUnknowns;
	m_nEmailInfoIndex = m_pData->m_nEmailInfoIndex;			// 0=email field, else info 1-8
	m_bSaveEmail = m_pData->m_bSaveEmail;
	m_nYearInfoIndex = m_pData->m_nYearInfoIndex;
	m_nRegInfoIndex = m_pData->m_nRegInfoIndex;
	m_nDobInfoIndex = m_pData->m_nDobInfoIndex;
	m_nNewIntakeAccount = m_pData->m_nNewIntakeAccount;			// 0=uID, 1=NextFree
	m_strBalanceText = m_pData->m_strBalanceText;
	m_strStartFromUserID = m_pData->m_strStartFromUserID;
}

//*******************************************************

void CSetupWisePay2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_editCardNo);
	DDX_Control(pDX, IDC_STATIC_ACCOUNTID, m_staticAccountID);
	DDX_Check(pDX, IDC_CHECK_SAVEEMAIL, m_bSaveEmail);
	DDX_Control(pDX, IDC_COMBO_EMAIL, m_comboEmail);
	DDX_CBIndex(pDX, IDC_COMBO_EMAIL, m_nEmailInfoIndex);
	DDX_Control(pDX, IDC_COMBO_YEAR, m_comboYear);
	DDX_CBIndex(pDX, IDC_COMBO_YEAR, m_nYearInfoIndex);
	DDX_Control(pDX, IDC_COMBO_REG, m_comboReg);
	DDX_CBIndex(pDX, IDC_COMBO_REG, m_nRegInfoIndex);
	DDX_Control(pDX, IDC_COMBO_DOB, m_comboDob);
	DDX_CBIndex(pDX, IDC_COMBO_DOB, m_nDobInfoIndex);
	DDX_CBIndex(pDX, IDC_COMBO_NEWINTAKE, m_nNewIntakeAccount);
	DDX_Control(pDX, IDC_COMBO_NEWINTAKE, m_comboNewIntake);
	DDX_Check(pDX, IDC_CHECK_ADDUNKNOWNS, m_bAddUnknowns);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_strBalanceText);
	DDX_Text(pDX, IDC_EDIT_CARDNO, m_strStartFromUserID);
}

//*******************************************************

BEGIN_MESSAGE_MAP(CSetupWisePay2Dlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_CHECK_ADDUNKNOWNS, OnToggleAddUnknowns)
	ON_BN_CLICKED(IDC_CHECK_SAVEEMAIL, OnToggleSaveEmail)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWINTAKE, OnSelectNewIntake)
END_MESSAGE_MAP()

//*******************************************************

BOOL CSetupWisePay2Dlg::CSSAutoShutdownPostInitDialog()
{
	CSQLRepositoryUserText RepoText;
	CString strDisplayText = RepoText.GetDisplayMemberText();
	m_staticAccountID.SetWindowText ( strDisplayText );

	m_comboEmail.AddString ( Account::Email.Label );				// 0=email field, else Info1 - 8
	m_comboYear.AddString ( szNOTDEFINED );
	m_comboReg.AddString ( szNOTDEFINED );
	m_comboDob.AddString ( szNOTDEFINED );

// add info texts

	for ( int n = 1 ; n <= nMAX_INFOTEXTS ; n++ )
	{
		strDisplayText = RepoText.GetDisplayInfoText(n);

		m_comboEmail.AddString ( strDisplayText );
		m_comboYear.AddString ( strDisplayText );
		m_comboReg.AddString ( strDisplayText );
	}

// dob no

	m_comboDob.AddString ( Account::DOB.Label );

//
	m_comboEmail.SetCurSel ( m_nEmailInfoIndex );
	m_comboYear.SetCurSel ( m_nYearInfoIndex );
	m_comboReg.SetCurSel ( m_nRegInfoIndex );
	m_comboDob.SetCurSel ( m_nDobInfoIndex );

//

	m_comboNewIntake.AddString ( m_pData->GetNewIntakeAddedByText ( nWISEPAY_NEWINTAKE_MISID_INDEX ) );		// uID
	m_comboNewIntake.AddString ( m_pData->GetNewIntakeAddedByText ( nWISEPAY_NEWINTAKE_NEXTFREE_INDEX ) );	// Next free
	m_comboNewIntake.SetCurSel ( m_nNewIntakeAccount );

//

	SetDisplay();

	return TRUE;
}

//******************************************************************

void CSetupWisePay2Dlg::OnSelectNewIntake()	
{ 
	SetDisplay(); 
}

//******************************************************************

void CSetupWisePay2Dlg::OnToggleSaveEmail()
{
	SetDisplay();
	m_comboEmail.SetFocus();
}

//******************************************************************

void CSetupWisePay2Dlg::OnToggleAddUnknowns()
{
	SetDisplay();
	m_comboNewIntake.SetFocus();
}

//******************************************************************

void CSetupWisePay2Dlg::SetDisplay()
{
	if ( UpdateData() == TRUE )
	{
		bool bAllowed = ( m_bAddUnknowns == TRUE && m_nNewIntakeAccount == nWISEPAY_NEWINTAKE_NEXTFREE_INDEX ) ? TRUE : FALSE;
		m_editCardNo.EnableWindow(bAllowed);

		m_comboEmail.EnableWindow(m_bSaveEmail);
		m_comboNewIntake.EnableWindow(m_bAddUnknowns);
	}
}

//******************************************************************

void CSetupWisePay2Dlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		if (m_bAddUnknowns == TRUE && m_nNewIntakeAccount == nWISEPAY_NEWINTAKE_NEXTFREE_INDEX)
		{
			if (System.IsValidCardNo(m_strStartFromUserID) == FALSE)
			{
				Prompter.Error("Invalid 'Start from' UserID specified!", "Input Error");
				m_editCardNo.SetFocus();
				return;
			}
		}

		m_pData->m_bAddUnknowns = (m_bAddUnknowns != 0);
		m_pData->m_bSaveEmail = (m_bSaveEmail != 0);
		m_pData->m_nEmailInfoIndex = m_nEmailInfoIndex;
		m_pData->m_nYearInfoIndex = m_nYearInfoIndex;
		m_pData->m_nRegInfoIndex = m_nRegInfoIndex;
		m_pData->m_nDobInfoIndex = m_nDobInfoIndex;
		m_pData->m_nNewIntakeAccount = m_nNewIntakeAccount;
		m_pData->m_strBalanceText = m_strBalanceText;
		m_pData->m_strStartFromUserID = m_strStartFromUserID;

		EndDialog(IDOK);
	}
}

//******************************************************************
