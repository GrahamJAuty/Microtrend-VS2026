//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLFieldValidatorAccount.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "AlertTextSelectDlg.h"
#include "GroupComboHelpers.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "BatchPropertyPage5.h"
//$$******************************************************************

CBatchPropertyPage5::CBatchPropertyPage5(CBatchAccountDetailMemory& DetailMemory)
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CBatchPropertyPage5::IDD),
	m_Memory(DetailMemory)
{
	Server.Read ( Filenames.GetServerDataFilename() );
	m_alertText.Open ( Filenames.GetAlertFilename() );
}

//$$******************************************************************

void CBatchPropertyPage5::SetPointer ( CSQLRowAccountFull& AccountRow )
{
	m_Memory.m_nGroupNo = AccountRow.GetGroupNo();
}

//$$******************************************************************

void CBatchPropertyPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SETGROUP, m_checkSetGroup);
	DDX_Control(pDX, IDC_CHECK_SETEXPIRY, m_checkSetExpiry);
	DDX_Control(pDX, IDC_CHECK_SETDOB, m_checkSetDOB);
	DDX_Control(pDX, IDC_CHECK_MAXSPEND, m_checkSetMaxSpend);
	DDX_Control(pDX, IDC_CHECK_OVERDRAFT, m_checkSetOverSpend);
	DDX_Control(pDX, IDC_CHECK_SETALERT, m_checkSetAlert);
	DDX_Control(pDX, IDC_CHECK_SETNAME, m_checkSetName);
	DDX_Control(pDX, IDC_CHECK_CLEARGROUP, m_checkClearGroup);
	DDX_Control(pDX, IDC_CHECK_CLEAREXPIRY, m_checkClearExpiry);
	DDX_Control(pDX, IDC_CHECK_CLEARDOB, m_checkClearDOB);
	DDX_Control(pDX, IDC_CHECK_CLEARMAXSPEND, m_checkClearMaxSpend);
	DDX_Control(pDX, IDC_CHECK_CLEAROVERDRAFT, m_checkClearOverSpend);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_tabcomboGroup);
	DDX_Control(pDX, IDC_DATETIMEPICKER_EXPIRY, m_DatePickerExpiry);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DOB, m_DatePickerDOB);
	DDX_Control(pDX, IDC_BUTTON_ALERT, m_buttonAlert);
	DDX_Control(pDX, IDC_EDIT_ALERTTEXT, m_editAlert);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CBatchPropertyPage5, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_SETGROUP, OnToggleSetGroup)
	ON_BN_CLICKED(IDC_CHECK_SETEXPIRY, OnToggleSetExpiry)
	ON_BN_CLICKED(IDC_CHECK_SETDOB, OnToggleSetDob)
	ON_BN_CLICKED(IDC_CHECK_MAXSPEND, OnToggleSetMaxSpend)
	ON_BN_CLICKED(IDC_CHECK_OVERDRAFT, OnToggleSetOverSpend)
	ON_BN_CLICKED(IDC_CHECK_SETALERT, OnToggleSetAlert)
	ON_BN_CLICKED(IDC_CHECK_CLEARGROUP, OnToggleClearGroup)
	ON_BN_CLICKED(IDC_CHECK_CLEAREXPIRY, OnToggleClearExpiry)
	ON_BN_CLICKED(IDC_CHECK_CLEARDOB, OnToggleClearDob)
	ON_BN_CLICKED(IDC_CHECK_CLEARMAXSPEND, OnToggleClearMaxSpend)
	ON_BN_CLICKED(IDC_CHECK_CLEAROVERDRAFT, OnToggleClearOverSpend)
	ON_BN_CLICKED(IDC_BUTTON_ALERT, OnButtonAlert)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CBatchPropertyPage5::OnInitDialog()
{
	CSSPropertyPage::OnInitDialog();

	{
		CSQLRepositoryUserText RepoText;
		CString strDOBText = RepoText.GetDOBText(Account::DOB.Label);

		CString strMsg = "";
		strMsg.Format ( "Set %s", (const char*) strDOBText );
		m_checkSetDOB.SetWindowText ( strMsg ); 

		strMsg.Format ( "Clear %s", (const char*)strDOBText );
		m_checkClearDOB.SetWindowText ( strMsg ); 
	}

	m_Memory.TidyOptions();

	m_nAlertCode = m_Memory.m_nAlertCode;

	m_checkSetGroup.SetCheck( m_Memory.m_bSetGroup );
	m_checkSetExpiry.SetCheck( m_Memory.m_bSetExpiry );
	m_checkSetDOB.SetCheck( m_Memory.m_bSetDob );
	m_checkSetMaxSpend.SetCheck( m_Memory.m_bSetMaxSpend );
	m_checkSetOverSpend.SetCheck( m_Memory.m_bSetOverSpend );
	m_checkSetAlert.SetCheck( m_Memory.m_bSetAlert );
	m_checkSetName.SetCheck( m_Memory.m_bSetName );
	
	m_checkClearGroup.SetCheck( m_Memory.m_bClearGroup );
	m_checkClearExpiry.SetCheck( m_Memory.m_bClearExpiry );
	m_checkClearDOB.SetCheck( m_Memory.m_bClearDob );
	m_checkClearMaxSpend.SetCheck( m_Memory.m_bClearMaxSpend );
	m_checkClearOverSpend.SetCheck( m_Memory.m_bClearOverSpend );

	SubclassEdit ( IDC_EDIT_OVERSPEND,		SS_NUM_SDP, 11, "%.2f" );
	SubclassEdit ( IDC_EDIT_MAXSPEND,		SS_NUM_SDP, 11, "%.2f" );

	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup, m_Memory.m_nGroupNo, FALSE);

	m_tabcomboGroup.EnableWindow( m_Memory.m_bSetGroup );

	//DOB
	{
		COleDateTime oleMin = CSQLFieldValidatorAccount::GetMinDob();
		COleDateTime oleMax = CSQLFieldValidatorAccount::GetMaxDob();
		m_DatePickerDOB.SetRange(&oleMin, &oleMax);

		bool bGotDOB = FALSE;
		if (m_Memory.m_bSetDob)
		{
			bGotDOB = FALSE;
			if (CSQLFieldValidatorAccount::ValidateDOB(m_Memory.m_dateDob) == TRUE)
			{
				m_checkSetDOB.SetCheck(TRUE);
				m_DatePickerDOB.SetTime(m_Memory.m_dateDob);
				m_DatePickerDOB.EnableWindow(TRUE);
				bGotDOB = TRUE;
			}
		}

		if (FALSE == bGotDOB)
		{
			m_checkSetDOB.SetCheck(FALSE);
			m_DatePickerDOB.SetTime(COleDateTime::GetCurrentTime());
			m_DatePickerDOB.EnableWindow(FALSE);
		}
	}

	//EXPIRY
	{
		COleDateTime oleMin = CSQLFieldValidatorAccount::GetMinExpiry();
		COleDateTime oleMax = CSQLFieldValidatorAccount::GetMaxExpiry();
		m_DatePickerExpiry.SetRange(&oleMin, &oleMax);

		bool bGotExpiry = FALSE;
		if (m_Memory.m_bSetExpiry)
		{
			bGotExpiry = FALSE;
			if (CSQLFieldValidatorAccount::ValidateExpiry(m_Memory.m_dateExpiry) == TRUE)
			{
				m_checkSetExpiry.SetCheck(TRUE);
				m_DatePickerExpiry.SetTime(m_Memory.m_dateExpiry);
				m_DatePickerExpiry.EnableWindow(TRUE);
				bGotExpiry = TRUE;
			}
		}

		if (FALSE == bGotExpiry)
		{
			m_checkSetExpiry.SetCheck(FALSE);
			m_DatePickerExpiry.SetTime(COleDateTime::GetCurrentTime());
			m_DatePickerExpiry.EnableWindow(FALSE);
		}
	}

	GetEditMaxSpend() -> EnableWindow( m_Memory.m_bSetMaxSpend );
	SetEditDouble( GetEditMaxSpend(), m_Memory.m_dMaxSpend );

	GetEditOverSpend() -> EnableWindow( m_Memory.m_bSetOverSpend );
	SetEditDouble( GetEditOverSpend(), m_Memory.m_dOverSpend );

	if ( Server.GetAllowOverdraftFlag() == FALSE)
	{
		GetEditOverSpend() -> ShowWindow(SW_HIDE);
		m_checkSetOverSpend.ShowWindow(SW_HIDE);
		m_checkClearOverSpend.ShowWindow(SW_HIDE);
	}

	if ( TRUE == m_Memory.m_bSetAlert )
	{
		m_buttonAlert.EnableWindow( TRUE );
		m_editAlert.SetWindowText( GetAlertText() );
	}
	else
	{
		m_buttonAlert.EnableWindow( FALSE );
		m_editAlert.SetWindowText( "" );
	}

	return TRUE;
}

//$$******************************************************************
CEdit* CBatchPropertyPage5::GetEditMaxSpend(){ return GetEdit( IDC_EDIT_MAXSPEND ); }
CEdit* CBatchPropertyPage5::GetEditOverSpend(){ return GetEdit( IDC_EDIT_OVERSPEND ); }
//$$******************************************************************

void CBatchPropertyPage5::SetEditDouble( CEdit* pEdit, double d )
{
	CString str = "";
	str.Format( "%.2f", d );
	pEdit -> SetWindowText( str );
}

//$$******************************************************************

void CBatchPropertyPage5::SetEditInt( CEdit* pEdit, int n )
{
	CString str = "";
	str.Format( "%d", n );
	pEdit -> SetWindowText( str );
}

//$$******************************************************************

double CBatchPropertyPage5::GetEditDouble( CEdit* pEdit )
{
	CString str = "";
	pEdit -> GetWindowText( str );
	return atof( str );
}

//$$******************************************************************

int CBatchPropertyPage5::GetEditInt( CEdit* pEdit )
{
	CString str = "";
	pEdit -> GetWindowText( str );
	return atoi( str );
}

//$$******************************************************************

void CBatchPropertyPage5::UpdateMemory()
{
	m_Memory.m_bSetGroup = (IsTicked(m_checkSetGroup));
	m_Memory.m_bClearGroup = (IsTicked(m_checkClearGroup));

	m_Memory.m_nGroupNo = Group::GroupNo.Min;
	if (m_Memory.m_bSetGroup == TRUE)
	{
		int nSel = m_tabcomboGroup.GetCurSel();
		if ((nSel >= 0) && (nSel < m_tabcomboGroup.GetCount()))
		{
			m_Memory.m_nGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup, 0);
		}
	}

	m_Memory.m_bSetExpiry = FALSE;
	if (IsTicked(m_checkSetExpiry))
	{
		COleDateTime dateExpiry;
		m_DatePickerExpiry.GetTime(dateExpiry);
		if (CSQLFieldValidatorAccount::ValidateExpiry(dateExpiry))
		{
			m_Memory.m_dateExpiry = dateExpiry;
			m_Memory.m_bSetExpiry = TRUE;
		}
	}

	m_Memory.m_bSetDob = FALSE;
	if (IsTicked(m_checkSetDOB))
	{
		COleDateTime dateDOB;
		m_DatePickerDOB.GetTime(dateDOB);
		if (CSQLFieldValidatorAccount::ValidateDOB(dateDOB))
		{
			m_Memory.m_dateDob = dateDOB;
			m_Memory.m_bSetDob = TRUE;
		}
	}

	if (IsTicked(m_checkClearExpiry))
	{
		m_Memory.m_bClearExpiry = TRUE;
		m_Memory.m_bSetExpiry = FALSE;
	}
	else
	{
		m_Memory.m_bClearExpiry = FALSE;
	}

	if (IsTicked(m_checkClearDOB))
	{
		m_Memory.m_bClearDob = TRUE;
		m_Memory.m_bSetDob = FALSE;
	}
	else
	{
		m_Memory.m_bClearDob = FALSE;
	}
	
	m_Memory.m_bSetMaxSpend = ( m_checkSetMaxSpend.GetCheck() != 0 );
	m_Memory.m_bClearMaxSpend = ( m_checkClearMaxSpend.GetCheck() != 0 );
	m_Memory.m_dMaxSpend = GetEditDouble( GetEditMaxSpend() );

	m_Memory.m_bSetOverSpend = ( m_checkSetOverSpend.GetCheck() != 0 );
	m_Memory.m_bClearOverSpend = ( m_checkClearOverSpend.GetCheck() != 0 );
	m_Memory.m_dOverSpend = GetEditDouble( GetEditOverSpend() );

	m_Memory.m_bSetAlert = ( m_checkSetAlert.GetCheck() != 0 );
	m_Memory.m_nAlertCode = m_nAlertCode;

	m_Memory.m_bSetName = ( m_checkSetName.GetCheck() != 0 );

	m_Memory.TidyOptions();
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetGroup()
{
	m_checkClearGroup.SetCheck( FALSE );
	m_tabcomboGroup.EnableWindow( m_checkSetGroup.GetCheck() != 0 );
	m_tabcomboGroup.SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleClearGroup()
{
	m_checkSetGroup.SetCheck( FALSE );
	m_tabcomboGroup.EnableWindow( FALSE );
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetExpiry()
{
	m_DatePickerExpiry.EnableWindow( m_checkSetExpiry.GetCheck() != 0 );
	m_DatePickerExpiry.SetFocus();
	m_checkClearExpiry.SetCheck( FALSE );
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleClearExpiry()
{
	m_checkSetExpiry.SetCheck( FALSE );
	m_DatePickerExpiry.EnableWindow( FALSE );
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetDob()
{
	m_DatePickerDOB.EnableWindow(m_checkSetDOB.GetCheck() != 0);
	m_DatePickerDOB.SetFocus();
	m_checkClearDOB.SetCheck(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleClearDob()
{
	m_checkSetDOB.SetCheck(FALSE);
	m_DatePickerDOB.EnableWindow(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetMaxSpend()
{
	m_checkClearMaxSpend.SetCheck( FALSE );
	GetEditMaxSpend() -> EnableWindow( m_checkSetMaxSpend.GetCheck() != 0 );
	GetEditMaxSpend() -> SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleClearMaxSpend()
{
	m_checkSetMaxSpend.SetCheck( FALSE );
	SetEditDouble( GetEditMaxSpend(), 0.0 );
	GetEditMaxSpend() -> EnableWindow( FALSE );
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetOverSpend()
{
	m_checkClearOverSpend.SetCheck( FALSE );
	GetEditOverSpend() -> EnableWindow( m_checkSetOverSpend.GetCheck() != 0 );
	GetEditOverSpend() -> SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleClearOverSpend()
{
	m_checkSetOverSpend.SetCheck( FALSE );
	SetEditDouble( GetEditOverSpend(), 0.0 );
	GetEditOverSpend() -> EnableWindow( FALSE );
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetAlert()
{
	m_buttonAlert.EnableWindow(m_checkSetAlert.GetCheck() != 0);
	m_buttonAlert.SetFocus();

	if (m_checkSetAlert.GetCheck() == 0)
	{
		m_editAlert.SetWindowText("");
	}
	else
	{
		CString strAlertText = "Clear";
		if (m_nAlertCode != 0)
		{
			strAlertText = m_alertText.GetText(m_nAlertCode);
		}

		m_editAlert.SetWindowText(strAlertText);
	}
}

//$$******************************************************************

void CBatchPropertyPage5::OnButtonAlert()
{
	CAlertTextSelectDlg dlg;

	dlg.m_nAlertCode = m_nAlertCode == -1 ? 0 : m_nAlertCode;
	if ( dlg.DoModal() == IDOK )
	{
		m_nAlertCode = dlg.m_nAlertCode;
		m_editAlert.SetWindowText ( GetAlertText() ); 
	}
}

//$$******************************************************************

bool CBatchPropertyPage5::HaveUpdates()
{
	return m_Memory.HaveUpdates();
}

//$$******************************************************************

bool CBatchPropertyPage5::UpdateRecord(CSQLRowAccountFull& AccountRow)
{
	return m_Memory.UpdateRecord( AccountRow );
}

//$$******************************************************************

void CBatchPropertyPage5::AddWarnings ( CStringArray* pArray )
{
	m_Memory.AddWarnings( pArray, m_alertText );
}

//$$******************************************************************

CString CBatchPropertyPage5::GetAlertText()
{
	CString strAlertText = "";
	if (m_checkSetAlert.GetCheck() != 0)
	{
		if (m_nAlertCode == 0)
		{
			strAlertText = "Clear";
		}
		else if (m_nAlertCode > 0)
		{
			strAlertText = m_alertText.GetText(m_nAlertCode);
		}
	}

	return strAlertText;
}

//$$******************************************************************

BOOL CBatchPropertyPage5::OnKillActive()
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	UpdateMemory();
	return bResult;
}

//$$******************************************************************

