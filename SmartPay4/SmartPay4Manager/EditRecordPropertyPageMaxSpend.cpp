//*******************************************************
#include "resource.h"
//*******************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//*******************************************************
#include "..\SmartPay4Shared\GroupSetText.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//*******************************************************
#include "EditRecordPropertySheet.h"
#include "StandardCombos.h"
//*******************************************************
#include "EditRecordPropertyPageMaxSpend.h"
//*******************************************************

CEditRecordPropertyPageMaxSpend::CEditRecordPropertyPageMaxSpend(CSQLRowAccountFull& Row )
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CEditRecordPropertyPageMaxSpend::IDD)
	,m_SQLRowAccount(Row)
{
	m_bReadOnly = FALSE;
	m_bUseRecordSettings = FALSE;
	/*****/
	SQLRowSetAuditPeriod.LoadPeriods();
	/*****/
	m_nControlIDPeriodText[1] = IDC_EDIT_PERIODTEXT1;
	m_nControlIDPeriodText[2] = IDC_EDIT_PERIODTEXT2;
	m_nControlIDPeriodText[3] = IDC_EDIT_PERIODTEXT3;
	m_nControlIDPeriodText[4] = IDC_EDIT_PERIODTEXT4;
	m_nControlIDPeriodText[5] = IDC_EDIT_PERIODTEXT5;
	m_nControlIDPeriodText[6] = IDC_EDIT_PERIODTEXT6;
	/*****/
	m_nControlIDPeriodStart[1] = IDC_STATIC_STARTPERIOD1;
	m_nControlIDPeriodStart[2] = IDC_STATIC_STARTPERIOD2;
	m_nControlIDPeriodStart[3] = IDC_STATIC_STARTPERIOD3;
	m_nControlIDPeriodStart[4] = IDC_STATIC_STARTPERIOD4;
	m_nControlIDPeriodStart[5] = IDC_STATIC_STARTPERIOD5;
	m_nControlIDPeriodStart[6] = IDC_STATIC_STARTPERIOD6;
	/*****/
	m_nControlIDPeriodAllow[1][1] = IDC_CHECK_P1_PURSE1;
	m_nControlIDPeriodAllow[1][2] = IDC_CHECK_P1_PURSE2;
	m_nControlIDPeriodAllow[1][3] = IDC_CHECK_P1_PURSE3;
	m_nControlIDPeriodAllow[2][1] = IDC_CHECK_P2_PURSE1;
	m_nControlIDPeriodAllow[2][2] = IDC_CHECK_P2_PURSE2;
	m_nControlIDPeriodAllow[2][3] = IDC_CHECK_P2_PURSE3;
	m_nControlIDPeriodAllow[3][1] = IDC_CHECK_P3_PURSE1;
	m_nControlIDPeriodAllow[3][2] = IDC_CHECK_P3_PURSE2;
	m_nControlIDPeriodAllow[3][3] = IDC_CHECK_P3_PURSE3;
	m_nControlIDPeriodAllow[4][1] = IDC_CHECK_P4_PURSE1;
	m_nControlIDPeriodAllow[4][2] = IDC_CHECK_P4_PURSE2;
	m_nControlIDPeriodAllow[4][3] = IDC_CHECK_P4_PURSE3;
	m_nControlIDPeriodAllow[5][1] = IDC_CHECK_P5_PURSE1;
	m_nControlIDPeriodAllow[5][2] = IDC_CHECK_P5_PURSE2;
	m_nControlIDPeriodAllow[5][3] = IDC_CHECK_P5_PURSE3;
	m_nControlIDPeriodAllow[6][1] = IDC_CHECK_P6_PURSE1;
	m_nControlIDPeriodAllow[6][2] = IDC_CHECK_P6_PURSE2;
	m_nControlIDPeriodAllow[6][3] = IDC_CHECK_P6_PURSE3;
	/*****/
	m_nControlIDMaxSpend[1] = IDC_EDIT_MAXSPEND1;
	m_nControlIDMaxSpend[2] = IDC_EDIT_MAXSPEND2;
	m_nControlIDMaxSpend[3] = IDC_EDIT_MAXSPEND3;
	m_nControlIDMaxSpend[4] = IDC_EDIT_MAXSPEND4;
	m_nControlIDMaxSpend[5] = IDC_EDIT_MAXSPEND5;
	m_nControlIDMaxSpend[6] = IDC_EDIT_MAXSPEND6;
	/*****/
	m_nControlIDMaxSpendP3[1] = IDC_EDIT_MAXSPEND1P3;
	m_nControlIDMaxSpendP3[2] = IDC_EDIT_MAXSPEND2P3;
	m_nControlIDMaxSpendP3[3] = IDC_EDIT_MAXSPEND3P3;
	m_nControlIDMaxSpendP3[4] = IDC_EDIT_MAXSPEND4P3;
	m_nControlIDMaxSpendP3[5] = IDC_EDIT_MAXSPEND5P3;
	m_nControlIDMaxSpendP3[6] = IDC_EDIT_MAXSPEND6P3;
	/*****/
	m_nControlIDOverspend[1] = IDC_EDIT_OVERSPEND1;
	m_nControlIDOverspend[2] = IDC_EDIT_OVERSPEND2;
	m_nControlIDOverspend[3] = IDC_EDIT_OVERSPEND3;
	m_nControlIDOverspend[4] = IDC_EDIT_OVERSPEND4;
	m_nControlIDOverspend[5] = IDC_EDIT_OVERSPEND5;
	m_nControlIDOverspend[6] = IDC_EDIT_OVERSPEND6;
	/*****/
	m_nControlIDOverspendP3[1] = IDC_EDIT_OVERSPEND1P3;
	m_nControlIDOverspendP3[2] = IDC_EDIT_OVERSPEND2P3;
	m_nControlIDOverspendP3[3] = IDC_EDIT_OVERSPEND3P3;
	m_nControlIDOverspendP3[4] = IDC_EDIT_OVERSPEND4P3;
	m_nControlIDOverspendP3[5] = IDC_EDIT_OVERSPEND5P3;
	m_nControlIDOverspendP3[6] = IDC_EDIT_OVERSPEND6P3;	
}

//*******************************************************

CEditRecordPropertyPageMaxSpend::~CEditRecordPropertyPageMaxSpend()
{
}

//*******************************************************

void CEditRecordPropertyPageMaxSpend::SetPointer ( int nMode, CLeaversHandler* pLeaversData )
{
	m_bReadOnly = ( pLeaversData != NULL ) || ( DB_READONLY == nMode );
}

//*******************************************************************

void CEditRecordPropertyPageMaxSpend::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_CHECK_OWNMAXSPEND, m_checkOwnMaxSpend);
	DDX_Control(pDX, IDC_STATIC_GROUPMAXSPEND, m_staticGroupMaxSpend);
	DDX_Control(pDX, IDC_BUTTON_COPYGROUP, m_buttonCopyGroup);
	DDX_Control(pDX, IDC_STATIC_PERIOD, m_staticPeriod);
	DDX_Control(pDX, IDC_STATIC_START, m_staticStart);
	DDX_Control(pDX, IDC_STATIC_PURSE1, m_staticPurse1);
	DDX_Control(pDX, IDC_STATIC_MAXSPEND, m_staticMaxSpend);
	DDX_Control(pDX, IDC_STATIC_OVERDRAFT, m_staticOverdraft);
	DDX_Control(pDX, IDC_STATIC_PURSE2, m_staticPurse2);
	DDX_Control(pDX, IDC_STATIC_PURSE3, m_staticPurse3);
	DDX_Control(pDX, IDC_STATIC_MAXSPENDP3, m_staticMaxSpendP3);
	DDX_Control(pDX, IDC_STATIC_OVERDRAFTP3, m_staticOverdraftP3);
		
	for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
	{
		DDX_Control(pDX, m_nControlIDPeriodText[nPeriod], m_editPeriodText[nPeriod]);
		DDX_Control(pDX, m_nControlIDPeriodStart[nPeriod], m_staticPeriodStart[nPeriod]);

		for ( int nPurse = 1; nPurse <= 3; nPurse++ )
		{
			DDX_Control(pDX, m_nControlIDPeriodAllow[nPeriod][nPurse], m_checkPeriodAllow[nPeriod][nPurse] );
		}
	}

	for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
	{
		DDX_Text(pDX, m_nControlIDMaxSpend[nPeriod], m_dMaxSpend[nPeriod]);
		DDX_Text(pDX, m_nControlIDMaxSpendP3[nPeriod], m_dMaxSpendP3[nPeriod]);
		DDX_Text(pDX, m_nControlIDOverspend[nPeriod], m_dOverspend[nPeriod]);
		DDX_Text(pDX, m_nControlIDOverspendP3[nPeriod], m_dOverspendP3[nPeriod]);
	}

	if (TRUE == pDX->m_bSaveAndValidate)
	{
		for (int nPeriod = 1; nPeriod <= 6; nPeriod++)
		{
			if (ValidateMaxSpend(
				GetEditMaxSpend(nPeriod), 
				nPeriod, 
				Group::MaxSpendPurse1Period1.Min, 
				Group::MaxSpendPurse1Period1.Max, 
				"Purse1 Max Spend") 
				== FALSE )
			{
				pDX->Fail();
				return;
			}

			if (ValidateMaxSpend(
				GetEditMaxSpendP3(nPeriod),
				nPeriod,
				Group::MaxSpendPurse3Period1.Min,
				Group::MaxSpendPurse3Period1.Max,
				"Purse3 Max Spend")
				== FALSE)
			{
				pDX->Fail();
				return;
			}

			if (ValidateMaxSpend(
				GetEditOverspend(nPeriod),
				nPeriod,
				Group::MaxOverdraftPurse1Period1.Min,
				Group::MaxOverdraftPurse1Period1.Max,
				"Purse1 Max Overspend")
				== FALSE)
			{
				pDX->Fail();
				return;
			}

			if (ValidateMaxSpend(
				GetEditOverspendP3(nPeriod),
				nPeriod,
				Group::MaxOverdraftPurse3Period1.Min,
				Group::MaxOverdraftPurse3Period1.Max,
				"Purse3 Max Overspend")
				== FALSE)
			{
				pDX->Fail();
				return;
			}
		}
	}
}

//*******************************************************

bool CEditRecordPropertyPageMaxSpend::ValidateMaxSpend(CEdit* pEdit, int nPeriod, double dMin, double dMax, CString strType)
{
	bool bResult = TRUE;

	if (pEdit->IsWindowEnabled() == TRUE)
	{
		double dValue = GetEditBoxDouble(*pEdit);

		if ((dValue < dMin) || (dValue > dMax))
		{
			CString strError = "";
			strError.Format("The %s value for Period %d is out of range.\n\nPlease enter a value between %.2f and %.2f.",
				(const char*)strType,
				nPeriod,
				dMin,
				dMax);

			Prompter.Error(strError, "Error");
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
			bResult = FALSE;
		}
	}

	return bResult;
}

//*******************************************************

BEGIN_MESSAGE_MAP(CEditRecordPropertyPageMaxSpend, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_OWNMAXSPEND, OnToggleMaxSpend)
	ON_BN_CLICKED(IDC_CHECK_P1_PURSE1, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P1_PURSE2, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P1_PURSE3, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P2_PURSE1, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P2_PURSE2, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P2_PURSE3, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P3_PURSE1, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P3_PURSE2, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P3_PURSE3, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P4_PURSE1, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P4_PURSE2, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P4_PURSE3, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P5_PURSE1, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P5_PURSE2, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P5_PURSE3, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P6_PURSE1, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P6_PURSE2, SetEnables)
	ON_BN_CLICKED(IDC_CHECK_P6_PURSE3, SetEnables)
	ON_BN_CLICKED(IDC_BUTTON_COPYGROUP, OnButtonCopyGroup)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//*******************************************************

BOOL CEditRecordPropertyPageMaxSpend::OnInitDialog()
{
	CSSPropertyPage::OnInitDialog();
	
	MoveControl( &m_staticGroupMaxSpend, 45, 24 );

	for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
	{
		SubclassEdit ( m_nControlIDMaxSpend[nPeriod],		SS_NUM_SDP, 9, "%.2f" );
		SubclassEdit ( m_nControlIDMaxSpendP3[nPeriod],		SS_NUM_SDP, 9, "%.2f" );
		SubclassEdit ( m_nControlIDOverspend[nPeriod],		SS_NUM_SDP, 9, "%.2f" );
		SubclassEdit ( m_nControlIDOverspendP3[nPeriod],	SS_NUM_SDP, 9, "%.2f" );
	}

	for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
	{
		m_editPeriodText[nPeriod].SetWindowText( SQLRowSetAuditPeriod.GetPeriodName(nPeriod - 1) );
		m_staticPeriodStart[nPeriod].SetWindowText( SQLRowSetAuditPeriod.FormatStartTime(nPeriod - 1) );
	}
	
	return TRUE;
}

//*******************************************************
CEdit* CEditRecordPropertyPageMaxSpend::GetEditMaxSpend( int n ){ return GetEdit( m_nControlIDMaxSpend[n] ); }
CEdit* CEditRecordPropertyPageMaxSpend::GetEditMaxSpendP3( int n ){ return GetEdit( m_nControlIDMaxSpendP3[n] ); }
CEdit* CEditRecordPropertyPageMaxSpend::GetEditOverspend( int n ){ return GetEdit( m_nControlIDOverspend[n] ); }
CEdit* CEditRecordPropertyPageMaxSpend::GetEditOverspendP3( int n ){ return GetEdit( m_nControlIDOverspendP3[n] ); }
//*******************************************************

BOOL CEditRecordPropertyPageMaxSpend::OnSetActive()
{
	m_SQLRowGroup.Reset();
	m_SQLRowGroup.SetGroupNo(m_SQLRowAccount.GetGroupNo());
	CSQLRepositoryGroup RepoGroup;
	RepoGroup.SelectRow(m_SQLRowGroup, NULL);

	m_checkOwnMaxSpend.SetCheck( m_SQLRowAccount.GetOwnMaxSpend() );
	ToggleMaxSpend( TRUE );
	return CSSPropertyPage::OnSetActive();
}

//*******************************************************

void CEditRecordPropertyPageMaxSpend::OnToggleMaxSpend()
{
	ToggleMaxSpend( FALSE );
}

//*******************************************************

void CEditRecordPropertyPageMaxSpend::ToggleMaxSpend( bool bForceUpdate )
{
	{
		bool bShowButton = FALSE;
		bool bShowStatic = FALSE;
		bool bShowCheck = FALSE;

		int nMaxSpendType = m_SQLRowGroup.GetMaxSpendType(); 
		switch( nMaxSpendType )
		{
		case nGRPSPENDTYPE_PERIOD_RECORD1:
			m_bUseRecordSettings = TRUE;
			bShowButton = TRUE;
			break;
		
		case nGRPSPENDTYPE_PERIOD_RECORD2:
			m_bUseRecordSettings = ( m_checkOwnMaxSpend.GetCheck() != 0 );
			bShowCheck = TRUE;
			bShowButton = TRUE;
			break;

		case nGRPSPENDTYPE_PERIOD_LOCKED:
		default:
			m_bUseRecordSettings = FALSE;
			bShowStatic = TRUE;
			break;
		}

		ShowAndEnableWindow( &m_buttonCopyGroup, bShowButton, m_bUseRecordSettings && ( FALSE == m_bReadOnly ) );
		ShowAndEnableWindow( &m_staticGroupMaxSpend, bShowStatic );
		ShowAndEnableWindow( &m_checkOwnMaxSpend, bShowCheck, ( FALSE == m_bReadOnly ) );
	}

	if (bForceUpdate || (FALSE == m_bUseRecordSettings))
	{
		SetPeriodDisplay();
	}
	
	SetEnables();
}

//*******************************************************

BOOL CEditRecordPropertyPageMaxSpend::OnKillActive()
{
	BOOL bResult = TRUE;

	if ( FALSE == m_bReadOnly )
	{
		m_SQLRowAccount.SetOwnMaxSpend( m_checkOwnMaxSpend.GetCheck() != 0 );
	}

	if ( ( TRUE == m_bUseRecordSettings ) && ( FALSE == m_bReadOnly ) )
	{
		if ( UpdateData() == TRUE )
		{
			for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
			{
				for ( int nPurse = 1; nPurse <= 3; nPurse++ )
				{
					m_SQLRowAccount.SetPurseAllowed( nPurse, nPeriod - 1, m_checkPeriodAllow[nPeriod][nPurse].GetCheck() != 0 ); 
				}
			}

			m_SQLRowAccount.SetMaxSpendPurse1Period1 ( m_dMaxSpend[1] );
			m_SQLRowAccount.SetMaxSpendPurse1Period2 ( m_dMaxSpend[2] );
			m_SQLRowAccount.SetMaxSpendPurse1Period3 ( m_dMaxSpend[3] );
			m_SQLRowAccount.SetMaxSpendPurse1Period4 ( m_dMaxSpend[4] );
			m_SQLRowAccount.SetMaxSpendPurse1Period5 ( m_dMaxSpend[5] );
			m_SQLRowAccount.SetMaxSpendPurse1Period6 ( m_dMaxSpend[6] );

			m_SQLRowAccount.SetMaxSpendPurse3Period1 ( m_dMaxSpendP3[1] );
			m_SQLRowAccount.SetMaxSpendPurse3Period2 ( m_dMaxSpendP3[2] );
			m_SQLRowAccount.SetMaxSpendPurse3Period3 ( m_dMaxSpendP3[3] );
			m_SQLRowAccount.SetMaxSpendPurse3Period4 ( m_dMaxSpendP3[4] );
			m_SQLRowAccount.SetMaxSpendPurse3Period5 ( m_dMaxSpendP3[5] );
			m_SQLRowAccount.SetMaxSpendPurse3Period6 ( m_dMaxSpendP3[6] );

			m_SQLRowAccount.SetMaxOverdraftPurse1Period1 ( m_dOverspend[1] );
			m_SQLRowAccount.SetMaxOverdraftPurse1Period2 ( m_dOverspend[2] );
			m_SQLRowAccount.SetMaxOverdraftPurse1Period3 ( m_dOverspend[3] );
			m_SQLRowAccount.SetMaxOverdraftPurse1Period4 ( m_dOverspend[4] );
			m_SQLRowAccount.SetMaxOverdraftPurse1Period5 ( m_dOverspend[5] );
			m_SQLRowAccount.SetMaxOverdraftPurse1Period6 ( m_dOverspend[6] );

			m_SQLRowAccount.SetMaxOverdraftPurse3Period1 ( m_dOverspendP3[1] );
			m_SQLRowAccount.SetMaxOverdraftPurse3Period2 ( m_dOverspendP3[2] );
			m_SQLRowAccount.SetMaxOverdraftPurse3Period3 ( m_dOverspendP3[3] );
			m_SQLRowAccount.SetMaxOverdraftPurse3Period4 ( m_dOverspendP3[4] );
			m_SQLRowAccount.SetMaxOverdraftPurse3Period5 ( m_dOverspendP3[5] );
			m_SQLRowAccount.SetMaxOverdraftPurse3Period6 ( m_dOverspendP3[6] );
		}
		else 
		{
			bResult = FALSE;
		}
	}
	
	if (TRUE == bResult)
	{
		bResult = CSSPropertyPage::OnKillActive();
	}

	return bResult;
}

//*******************************************************

bool CEditRecordPropertyPageMaxSpend::IsPurseAllowed( int nPurse, int nPeriod )
{
	if (TRUE == m_bUseRecordSettings)
	{
		return m_SQLRowAccount.IsPurseAllowed(nPurse, nPeriod - 1);
	}
	else
	{
		return m_SQLRowGroup.IsPurseAllowed(nPurse, nPeriod - 1);
	}

	return FALSE;
}

//*******************************************************

double CEditRecordPropertyPageMaxSpend::GetMaxSpend( int nPeriod )
{
	double dResult = 0.0;

	if ( TRUE == m_bUseRecordSettings )
	{
		switch( nPeriod )
		{
		case 1:	dResult = m_SQLRowAccount.GetMaxSpendPurse1Period1();	break;
		case 2:	dResult = m_SQLRowAccount.GetMaxSpendPurse1Period2();	break;
		case 3:	dResult = m_SQLRowAccount.GetMaxSpendPurse1Period3();	break;
		case 4:	dResult = m_SQLRowAccount.GetMaxSpendPurse1Period4();	break;
		case 5:	dResult = m_SQLRowAccount.GetMaxSpendPurse1Period5();	break;
		case 6:	dResult = m_SQLRowAccount.GetMaxSpendPurse1Period6();	break;
		}
	}
	else
	{
		switch( nPeriod )
		{
		case 1:	dResult = m_SQLRowGroup.GetMaxSpendPurse1Period1();	break;
		case 2:	dResult = m_SQLRowGroup.GetMaxSpendPurse1Period2();	break;
		case 3:	dResult = m_SQLRowGroup.GetMaxSpendPurse1Period3();	break;
		case 4:	dResult = m_SQLRowGroup.GetMaxSpendPurse1Period4();	break;
		case 5:	dResult = m_SQLRowGroup.GetMaxSpendPurse1Period5();	break;
		case 6:	dResult = m_SQLRowGroup.GetMaxSpendPurse1Period6();	break;
		}
	}

	return dResult;
}

//*******************************************************

double CEditRecordPropertyPageMaxSpend::GetOverspend( int nPeriod )
{
	double dResult = 0.0;

	if ( TRUE == m_bUseRecordSettings )
	{
		switch( nPeriod )
		{
		case 1:	dResult = m_SQLRowAccount.GetMaxOverdraftPurse1Period1();	break;
		case 2:	dResult = m_SQLRowAccount.GetMaxOverdraftPurse1Period1();	break;
		case 3:	dResult = m_SQLRowAccount.GetMaxOverdraftPurse1Period3();	break;
		case 4:	dResult = m_SQLRowAccount.GetMaxOverdraftPurse1Period4();	break;
		case 5:	dResult = m_SQLRowAccount.GetMaxOverdraftPurse1Period5();	break;
		case 6:	dResult = m_SQLRowAccount.GetMaxOverdraftPurse1Period6();	break;
		}
	}
	else
	{
		switch( nPeriod )
		{
		case 1:	dResult = m_SQLRowGroup.GetMaxOverdraftPurse1Period1();	break;
		case 2:	dResult = m_SQLRowGroup.GetMaxOverdraftPurse1Period2();	break;
		case 3:	dResult = m_SQLRowGroup.GetMaxOverdraftPurse1Period3();	break;
		case 4:	dResult = m_SQLRowGroup.GetMaxOverdraftPurse1Period4();	break;
		case 5:	dResult = m_SQLRowGroup.GetMaxOverdraftPurse1Period5();	break;
		case 6:	dResult = m_SQLRowGroup.GetMaxOverdraftPurse1Period6();	break;
		}
	}

	return dResult;
}

//*******************************************************

double CEditRecordPropertyPageMaxSpend::GetMaxSpendP3( int nPeriod )
{
	double dResult = 0.0;

	if ( TRUE == m_bUseRecordSettings )
	{
		switch( nPeriod )
		{
		case 1:	dResult = m_SQLRowAccount.GetMaxSpendPurse3Period1();	break;
		case 2:	dResult = m_SQLRowAccount.GetMaxSpendPurse3Period2();	break;
		case 3:	dResult = m_SQLRowAccount.GetMaxSpendPurse3Period3();	break;
		case 4:	dResult = m_SQLRowAccount.GetMaxSpendPurse3Period4();	break;
		case 5:	dResult = m_SQLRowAccount.GetMaxSpendPurse3Period5();	break;
		case 6:	dResult = m_SQLRowAccount.GetMaxSpendPurse3Period6();	break;
		}
	}
	else
	{
		switch( nPeriod )
		{
		case 1:	dResult = m_SQLRowGroup.GetMaxSpendPurse3Period1();	break;
		case 2:	dResult = m_SQLRowGroup.GetMaxSpendPurse3Period2();	break;
		case 3:	dResult = m_SQLRowGroup.GetMaxSpendPurse3Period3();	break;
		case 4:	dResult = m_SQLRowGroup.GetMaxSpendPurse3Period4();	break;
		case 5:	dResult = m_SQLRowGroup.GetMaxSpendPurse3Period5();	break;
		case 6:	dResult = m_SQLRowGroup.GetMaxSpendPurse3Period6();	break;
		}
	}

	return dResult;
}

//*******************************************************

double CEditRecordPropertyPageMaxSpend::GetOverspendP3( int nPeriod )
{
	double dResult = 0.0;

	if ( TRUE == m_bUseRecordSettings )
	{
		switch( nPeriod )
		{
		case 1:	dResult = m_SQLRowAccount.GetMaxOverdraftPurse3Period1();	break;
		case 2:	dResult = m_SQLRowAccount.GetMaxOverdraftPurse3Period2();	break;
		case 3:	dResult = m_SQLRowAccount.GetMaxOverdraftPurse3Period3();	break;
		case 4:	dResult = m_SQLRowAccount.GetMaxOverdraftPurse3Period4();	break;
		case 5:	dResult = m_SQLRowAccount.GetMaxOverdraftPurse3Period5();	break;
		case 6:	dResult = m_SQLRowAccount.GetMaxOverdraftPurse3Period6();	break;
		}
	}
	else
	{
		switch( nPeriod )
		{
		case 1:	dResult = m_SQLRowGroup.GetMaxOverdraftPurse3Period1();	break;
		case 2:	dResult = m_SQLRowGroup.GetMaxOverdraftPurse3Period2();	break;
		case 3:	dResult = m_SQLRowGroup.GetMaxOverdraftPurse3Period3();	break;
		case 4:	dResult = m_SQLRowGroup.GetMaxOverdraftPurse3Period4();	break;
		case 5:	dResult = m_SQLRowGroup.GetMaxOverdraftPurse3Period5();	break;
		case 6:	dResult = m_SQLRowGroup.GetMaxOverdraftPurse3Period6();	break;
		}
	}

	return dResult;
}

//*******************************************************

void CEditRecordPropertyPageMaxSpend::OnButtonCopyGroup()
{
	m_bUseRecordSettings = FALSE;
	SetPeriodDisplay();
	m_bUseRecordSettings = TRUE;
	SetEnables();
}

//*******************************************************

void CEditRecordPropertyPageMaxSpend::SetPeriodDisplay()
{
	for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
	{
		m_dMaxSpend[nPeriod] = GetMaxSpend(nPeriod);
		m_dOverspend[nPeriod] = GetOverspend(nPeriod);
		m_dMaxSpendP3[nPeriod] = GetMaxSpendP3(nPeriod);
		m_dOverspendP3[nPeriod] = GetOverspendP3(nPeriod);
	}

	UpdateData( FALSE );

	for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
	{	
		m_checkPeriodAllow[nPeriod][1].SetCheck( IsPurseAllowed( 1, nPeriod ) );
		m_checkPeriodAllow[nPeriod][2].SetCheck( IsPurseAllowed( 2, nPeriod ) );
		m_checkPeriodAllow[nPeriod][3].SetCheck( IsPurseAllowed( 3, nPeriod ) );
	}
}

//*******************************************************

void CEditRecordPropertyPageMaxSpend::SetEnables()
{
	bool bPurse2Allow = ( m_SQLRowGroup.GetRefreshType() == nREFRESHTYPE_DISABLED );
	bool bPurse3Allow = System.GetEnablePurse3Flag();
	bool bOverspendAllow = Server.GetAllowOverdraftFlag();

	for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
	{
		bool bPeriodAllow = SQLRowSetAuditPeriod.IsPeriodSet( nPeriod - 1 );
	
		m_checkPeriodAllow[nPeriod][1].EnableWindow( bPeriodAllow & m_bUseRecordSettings & ( FALSE == m_bReadOnly ) );
		m_checkPeriodAllow[nPeriod][2].EnableWindow( bPeriodAllow & bPurse2Allow & m_bUseRecordSettings & ( FALSE == m_bReadOnly ) );
		m_checkPeriodAllow[nPeriod][3].EnableWindow( bPeriodAllow & bPurse3Allow & m_bUseRecordSettings & ( FALSE == m_bReadOnly ) );	
	}
	
	for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
	{
		bool bPurse1Allow = ( m_checkPeriodAllow[nPeriod][1].GetCheck() != 0 );
		bool bPurse3Allow = ( m_checkPeriodAllow[nPeriod][3].GetCheck() != 0 );
	
		GetEditMaxSpend(nPeriod) -> EnableWindow( bPurse1Allow & m_bUseRecordSettings );
		GetEditOverspend(nPeriod) -> EnableWindow( bPurse1Allow & bOverspendAllow & m_bUseRecordSettings & ( FALSE == m_bReadOnly ) );
		GetEditMaxSpendP3(nPeriod) -> EnableWindow( bPurse3Allow  & m_bUseRecordSettings );
		GetEditOverspendP3(nPeriod) -> EnableWindow( bPurse3Allow & bOverspendAllow & m_bUseRecordSettings & ( FALSE == m_bReadOnly ) );
	}
}

//*******************************************************

BOOL CEditRecordPropertyPageMaxSpend::OnQueryCancel()
{
	return CEditRecordPropertySheet::PromptBeforeCancel();
}

//********************************************************************
