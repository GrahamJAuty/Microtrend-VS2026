//**************************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//**************************************************************************
#include "SetupParentPay2Dlg.h"
//**************************************************************************

static const char* szERROR1		= "Conflict between UPN and Roll number database UserText fields!";
static const char* szERROR2		= "Conflict between MIS ID and Roll number database UserText fields!";
static const char* szERROR3		= "Conflict between MIS ID and UPN database UserText fields!";
static const char* szERROR4		= "MIS ID is being used to create new accounts!";
static const char* szERROR5		= "Roll number is being used to create new accounts!";

static const char* szNOTDEFINED	= "Not defined";

//**************************************************************************

CSetupParentPay2Dlg::CSetupParentPay2Dlg(CParentPayData* pData, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetupParentPay2Dlg::IDD, pParent)
{
	m_pData = pData;
	m_bAddUnknowns = m_pData->m_bAddUnknowns;
	m_nMisIDInfoIndex = m_pData->m_nMisIDInfoIndex;
	m_nUPNInfoIndex = m_pData->m_nUPNInfoIndex;
	m_nRollNoInfoIndex = m_pData->m_nRollNoInfoIndex;
	m_nYearInfoIndex = m_pData->m_nYearInfoIndex;
	m_nRegInfoIndex = m_pData->m_nRegInfoIndex;
	m_nDobInfoIndex = m_pData->m_nDobInfoIndex;
	m_nNewIntakeAccount = m_pData->m_nNewIntakeAccount;			// 0=misID, 1=RollNo, 2=NextFree
	m_nLegacyLocateBy = m_pData->m_nLegacyLocateBy;
	m_strStartFromUserID = m_pData->m_strStartFromUserID;
}

//**************************************************************************

void CSetupParentPay2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ACCOUNTID, m_staticAccountID);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_editCardNo);
	DDX_Control(pDX, IDC_STATIC_LOCATEBY, m_staticLegacyLocateBy);
	DDX_Control(pDX, IDC_COMBO_LOCATEBY, m_comboLegacyLocateBy);
	DDX_CBIndex(pDX, IDC_COMBO_LOCATEBY, m_nLegacyLocateBy);
	DDX_Control(pDX, IDC_COMBO_UPN, m_comboUPN);
	DDX_CBIndex(pDX, IDC_COMBO_UPN, m_nUPNInfoIndex);
	DDX_Control(pDX, IDC_COMBO_SIMS, m_comboSIMS);
	DDX_CBIndex(pDX, IDC_COMBO_SIMS, m_nRollNoInfoIndex);
	DDX_Control(pDX, IDC_COMBO_PID, m_comboPID);
	DDX_CBIndex(pDX, IDC_COMBO_PID, m_nMisIDInfoIndex);
	DDX_Control(pDX, IDC_COMBO_YEAR, m_comboYear);
	DDX_CBIndex(pDX, IDC_COMBO_YEAR, m_nYearInfoIndex);
	DDX_Control(pDX, IDC_COMBO_REG, m_comboReg);
	DDX_CBIndex(pDX, IDC_COMBO_REG, m_nRegInfoIndex);
	DDX_Control(pDX, IDC_COMBO_DOB, m_comboDob);
	DDX_CBIndex(pDX, IDC_COMBO_DOB, m_nDobInfoIndex);
	DDX_CBIndex(pDX, IDC_COMBO_NEWINTAKE, m_nNewIntakeAccount);
	DDX_Control(pDX, IDC_COMBO_NEWINTAKE, m_comboNewIntake);
	DDX_Check(pDX, IDC_CHECK_ADDUNKNOWNS, m_bAddUnknowns);
	DDX_Text(pDX, IDC_EDIT_CARDNO, m_strStartFromUserID);
}

//**************************************************************************

BEGIN_MESSAGE_MAP(CSetupParentPay2Dlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_CHECK_ADDUNKNOWNS, OnToggleAddUnknowns)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWINTAKE, OnSelectNewIntake)
END_MESSAGE_MAP()

//******************************************************************

BOOL CSetupParentPay2Dlg::CSSAutoShutdownPostInitDialog()
{
// user 
	CSQLRepositoryUserText RepoText;
	m_staticAccountID.SetWindowText (RepoText.GetDisplayMemberText());

	m_comboPID.AddString ( szNOTDEFINED );
	m_comboUPN.AddString ( szNOTDEFINED );
	m_comboSIMS.AddString ( szNOTDEFINED );
	m_comboYear.AddString ( szNOTDEFINED );
	m_comboReg.AddString ( szNOTDEFINED );
	m_comboDob.AddString ( szNOTDEFINED );

// add info texts

	for ( int n = 1 ; n <= nMAX_INFOTEXTS ; n++ )
	{
		CString strDisplayText = RepoText.GetDisplayInfoText(n);

		m_comboPID.AddString ( strDisplayText );
		m_comboUPN.AddString ( strDisplayText );
		m_comboSIMS.AddString ( strDisplayText );
		m_comboYear.AddString ( strDisplayText );
		m_comboReg.AddString ( strDisplayText );
	}

// dob no

	m_comboDob.AddString ( Account::DOB.Label );

//
	m_comboPID.SetCurSel ( m_nMisIDInfoIndex );
	m_comboUPN.SetCurSel ( m_nUPNInfoIndex );
	m_comboSIMS.SetCurSel ( m_nRollNoInfoIndex );
	m_comboYear.SetCurSel ( m_nYearInfoIndex );
	m_comboReg.SetCurSel ( m_nRegInfoIndex );
	m_comboDob.SetCurSel ( m_nDobInfoIndex );

//
	ShowAndEnableWindow(&m_staticLegacyLocateBy, FALSE);
	ShowAndEnableWindow(&m_comboLegacyLocateBy, FALSE);
	//m_comboLegacyLocateBy.AddString ( m_pData->GetLocateByText ( nPARENTPAY_LOCATEBY_ROLLNO_INDEX ) );		// Roll noo
	//m_comboLegacyLocateBy.AddString ( m_pData->GetLocateByText ( nPARENTPAY_LOCATEBY_UPN_INDEX ) );			// UPN
	//m_comboLegacyLocateBy.AddString ( m_pData->GetLocateByText ( nPARENTPAY_LOCATEBY_MISID_INDEX ) );		// Person_id
	//m_comboLegacyLocateBy.SetCurSel ( m_nLocateBy );

	m_comboNewIntake.AddString ( m_pData->GetNewIntakeAddedByText ( nPARENTPAY_NEWINTAKE_MISID_INDEX ) );		// Person_id
	m_comboNewIntake.AddString ( m_pData->GetNewIntakeAddedByText ( nPARENTPAY_NEWINTAKE_ROLLNO_INDEX ) );		// roll no
	m_comboNewIntake.AddString ( m_pData->GetNewIntakeAddedByText ( nPARENTPAY_NEWINTAKE_NEXTFREE_INDEX ) );	// Next free
	m_comboNewIntake.SetCurSel ( m_nNewIntakeAccount );

	SetDisplay();

	return TRUE;
}

//******************************************************************

void CSetupParentPay2Dlg::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		if ( m_nRollNoInfoIndex == 0 && m_nUPNInfoIndex == 0 && m_nMisIDInfoIndex == 0 )
		{
			if ( Prompter.YesNo( "No Database UserText fields have been defined for use by MIS ID, UPN or Roll number, is this correct?", "Verification") == IDNO )
			{
				m_comboPID.SetFocus();
				return;
			}
		}

		/*FIDDLE
		if ( m_nLegacyLocateBy == nPARENTPAY_LOCATEBY_ROLLNO_INDEX && m_nRollNoInfoIndex == 0 || m_nLegacyLocateBy == nPARENTPAY_LOCATEBY_UPN_INDEX && m_nUPNInfoIndex == 0 || m_nLegacyLocateBy == nPARENTPAY_LOCATEBY_MISID_INDEX && m_nMisIDInfoIndex == 0 )
		{
			Prompter.Error( "Cannot locate unknown accounts as the specified field is currently set to 'Not defined'!", "Entry Error" );
			m_comboLegacyLocateBy.SetFocus();
			return;
		}
		*/

		CString strError = "";

		if ( m_nNewIntakeAccount == nPARENTPAY_NEWINTAKE_MISID_INDEX && m_nMisIDInfoIndex != 0 )	
		{
			strError = szERROR4;								// "MIS ID used to save new accounts"
			m_comboPID.SetFocus();
		}
	
		if ( m_nNewIntakeAccount == nPARENTPAY_NEWINTAKE_ROLLNO_INDEX && m_nRollNoInfoIndex != 0 )	
		{
			strError = szERROR5;								// "Roll number being used to save new accounts"
			m_comboSIMS.SetFocus();
		}

		if ( m_nRollNoInfoIndex != 0 )
		{
			if ( m_nRollNoInfoIndex == m_nUPNInfoIndex )
			{
				strError = szERROR1;								// "Conflict between SIMS Adno and UPN Database UserText fields!"
				m_comboSIMS.SetFocus();
			}
	
			else if ( m_nRollNoInfoIndex == m_nMisIDInfoIndex )
			{
				strError = szERROR2;								// "Conflict between SIMS Adno and Person_id Database UserText fields!"
				m_comboSIMS.SetFocus();
			}
		}

		if ( m_nUPNInfoIndex != 0 )
		{
			if ( m_nUPNInfoIndex == m_nRollNoInfoIndex )
			{
				strError = szERROR1;									// "Conflict between SIMS Adno and UPN Database UserText fields!";
				m_comboUPN.SetFocus();
			}
	
			else if ( m_nUPNInfoIndex == m_nMisIDInfoIndex )
			{
				strError = szERROR3;								// "Conflict between UPN and Person_id Database UserText fields!"
				m_comboUPN.SetFocus();
			}
		}

		if ( m_nMisIDInfoIndex != 0 )
		{
			if ( m_nMisIDInfoIndex == m_nRollNoInfoIndex )
			{
				strError = szERROR2;								// "Conflict between SIMS Adno and Person_id Database UserText fields!"
				m_comboPID.SetFocus();
			}
	
			else if ( m_nMisIDInfoIndex == m_nUPNInfoIndex )
			{
				strError = szERROR3;								// "Conflict between UPN Adno and Perosn_id Database UserText fields!";
				m_comboPID.SetFocus();
			}
		}

		if ( m_nYearInfoIndex != 0 && ( m_nYearInfoIndex == m_nRegInfoIndex ) )
		{
			strError = "Conflict between Year and Reg fields specified!";
			m_comboYear.SetFocus();
		}

		if ( m_bAddUnknowns == TRUE && m_nNewIntakeAccount == nPARENTPAY_NEWINTAKE_NEXTFREE_INDEX )
		{
			if ( System.IsValidCardNo ( m_strStartFromUserID ) == FALSE )
			{
				strError = "Invalid 'Start from' UserID specified!";
				m_editCardNo.SetFocus();
			}
		}

		if ( strError != "" )
		{
			Prompter.Error( strError, "Entry Error" );
			return;
		}

		m_pData->m_bAddUnknowns			= ( m_bAddUnknowns != 0 );
		m_pData->m_nMisIDInfoIndex		= m_nMisIDInfoIndex;
		m_pData->m_nUPNInfoIndex		= m_nUPNInfoIndex;
		m_pData->m_nRollNoInfoIndex		= m_nRollNoInfoIndex;
		m_pData->m_nYearInfoIndex		= m_nYearInfoIndex;
		m_pData->m_nRegInfoIndex		= m_nRegInfoIndex;
		m_pData->m_nDobInfoIndex		= m_nDobInfoIndex;
		m_pData->m_nNewIntakeAccount	= m_nNewIntakeAccount;
		m_pData->m_nLegacyLocateBy		= m_nLegacyLocateBy;
		m_pData->m_strStartFromUserID	= m_strStartFromUserID;

		EndDialog(IDOK);
	}
}

//******************************************************************

void CSetupParentPay2Dlg::OnToggleAddUnknowns()
{
	SetDisplay();
	m_comboNewIntake.SetFocus();
}

//******************************************************************

void CSetupParentPay2Dlg::OnSelectNewIntake()	
{ 
	SetDisplay(); 
}

//******************************************************************

void CSetupParentPay2Dlg::SetDisplay()
{
	if ( UpdateData() == TRUE )
	{
		m_editCardNo.EnableWindow( m_bAddUnknowns && (nPARENTPAY_NEWINTAKE_NEXTFREE_INDEX == m_nNewIntakeAccount));
		m_comboNewIntake.EnableWindow(m_bAddUnknowns);

	}
}

//**************************************************************************
