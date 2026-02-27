//*******************************************************
#include "resource.h"
//*******************************************************
#include "..\SmartPay4Shared\GroupSetText.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//*******************************************************
#include "StandardCombos.h"
//*******************************************************
#include "GroupPropertyPage7.h"
//*******************************************************

CGroupPropertyPage7::CGroupPropertyPage7(CSQLRowGroupFull& RowGroup)
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CGroupPropertyPage7::IDD)
	, m_RowGroup(RowGroup)
{
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

CGroupPropertyPage7::~CGroupPropertyPage7()
{
}

//*******************************************************

void CGroupPropertyPage7::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_COMBO_DAILYSPEND, m_comboMaxSpendType);
	DDX_Control(pDX, IDC_COMBO_OVERRIDE, m_comboMaxSpendOverride);
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
			if ( ValidateMaxSpend(
				GetEditMaxSpend(nPeriod), 
				nPeriod, 
				Group::MaxSpendPurse1Period1.Min,
				Group::MaxSpendPurse1Period1.Max, 
				"Purse1 Max Spend") == FALSE)
			{
				pDX->Fail();
				return;
			}

			if (ValidateMaxSpend(
				GetEditMaxSpendP3(nPeriod),
				nPeriod,
				Group::MaxSpendPurse3Period1.Min,
				Group::MaxSpendPurse3Period1.Max,
				"Purse3 Max Spend") == FALSE)
			{
				pDX->Fail();
				return;
			}

			if (ValidateMaxSpend(
				GetEditOverspend(nPeriod),
				nPeriod,
				Group::MaxOverdraftPurse1Period1.Min,
				Group::MaxOverdraftPurse1Period1.Max,
				"Purse1 Max Overspend") == FALSE)
			{
				pDX->Fail();
				return;
			}

			if (ValidateMaxSpend(
				GetEditOverspendP3(nPeriod),
				nPeriod,
				Group::MaxOverdraftPurse3Period1.Min,
				Group::MaxOverdraftPurse3Period1.Max,
				"Purse3 Max Overspend") == FALSE)
			{
				pDX->Fail();
				return;
			}
		}
	}
}

//*******************************************************

bool CGroupPropertyPage7::ValidateMaxSpend(CEdit* pEdit, int nPeriod, double dMin, double dMax, CString strType)
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

BEGIN_MESSAGE_MAP(CGroupPropertyPage7, CSSAutoShutdownPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_DAILYSPEND, OnSelectMaxSpendType)
	ON_BN_CLICKED(IDC_CHECK_P1_PURSE1, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P1_PURSE2, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P1_PURSE3, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P2_PURSE1, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P2_PURSE2, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P2_PURSE3, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P3_PURSE1, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P3_PURSE2, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P3_PURSE3, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P4_PURSE1, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P4_PURSE2, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P4_PURSE3, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P5_PURSE1, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P5_PURSE2, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P5_PURSE3, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P6_PURSE1, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P6_PURSE2, SetPeriodDisplay)
	ON_BN_CLICKED(IDC_CHECK_P6_PURSE3, SetPeriodDisplay)
END_MESSAGE_MAP()

//*******************************************************

BOOL CGroupPropertyPage7::OnInitDialog()
{
	m_dMaxSpend[1] = m_RowGroup.GetMaxSpendPurse1Period1();
	m_dMaxSpend[2] = m_RowGroup.GetMaxSpendPurse1Period2();
	m_dMaxSpend[3] = m_RowGroup.GetMaxSpendPurse1Period3();
	m_dMaxSpend[4] = m_RowGroup.GetMaxSpendPurse1Period4();
	m_dMaxSpend[5] = m_RowGroup.GetMaxSpendPurse1Period5();
	m_dMaxSpend[6] = m_RowGroup.GetMaxSpendPurse1Period6();
	/*****/
	m_dMaxSpendP3[1] = m_RowGroup.GetMaxSpendPurse3Period1();
	m_dMaxSpendP3[2] = m_RowGroup.GetMaxSpendPurse3Period2();
	m_dMaxSpendP3[3] = m_RowGroup.GetMaxSpendPurse3Period3();
	m_dMaxSpendP3[4] = m_RowGroup.GetMaxSpendPurse3Period4();
	m_dMaxSpendP3[5] = m_RowGroup.GetMaxSpendPurse3Period5();
	m_dMaxSpendP3[6] = m_RowGroup.GetMaxSpendPurse3Period6();
	/*****/
	m_dOverspend[1] = m_RowGroup.GetMaxOverdraftPurse1Period1();
	m_dOverspend[2] = m_RowGroup.GetMaxOverdraftPurse1Period2();
	m_dOverspend[3] = m_RowGroup.GetMaxOverdraftPurse1Period3();
	m_dOverspend[4] = m_RowGroup.GetMaxOverdraftPurse1Period4();
	m_dOverspend[5] = m_RowGroup.GetMaxOverdraftPurse1Period5();
	m_dOverspend[6] = m_RowGroup.GetMaxOverdraftPurse1Period6();
	/*****/
	m_dOverspendP3[1] = m_RowGroup.GetMaxOverdraftPurse3Period1();
	m_dOverspendP3[2] = m_RowGroup.GetMaxOverdraftPurse3Period2();
	m_dOverspendP3[3] = m_RowGroup.GetMaxOverdraftPurse3Period3();
	m_dOverspendP3[4] = m_RowGroup.GetMaxOverdraftPurse3Period4();
	m_dOverspendP3[5] = m_RowGroup.GetMaxOverdraftPurse3Period5();
	m_dOverspendP3[6] = m_RowGroup.GetMaxOverdraftPurse3Period6();
	/*****/

	CSSPropertyPage::OnInitDialog();
	
	int nMaxSpendType = m_RowGroup.GetMaxSpendType();
	CStandardCombos::FillGroupMaxSpendCombo( m_comboMaxSpendType, nMaxSpendType );
	CStandardCombos::FillGroupMaxSpendOverrideCombo( m_comboMaxSpendOverride, nMaxSpendType );
	
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
		m_staticPeriodStart[nPeriod].SetWindowText(SQLRowSetAuditPeriod.FormatStartTime(nPeriod - 1) );
	}

	return TRUE;
}

//*******************************************************
CEdit* CGroupPropertyPage7::GetEditMaxSpend( int n ){ return GetEdit( m_nControlIDMaxSpend[n] ); }
CEdit* CGroupPropertyPage7::GetEditMaxSpendP3( int n ){ return GetEdit( m_nControlIDMaxSpendP3[n] ); }
CEdit* CGroupPropertyPage7::GetEditOverspend( int n ){ return GetEdit( m_nControlIDOverspend[n] ); }
CEdit* CGroupPropertyPage7::GetEditOverspendP3( int n ){ return GetEdit( m_nControlIDOverspendP3[n] ); }
//*******************************************************

BOOL CGroupPropertyPage7::OnSetActive()
{
	BOOL bResult = CSSPropertyPage::OnSetActive();
	
	if ( TRUE == bResult )
	{
		for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
		{
			for ( int nPurse = 1; nPurse <= 3; nPurse++ )
			{
				m_checkPeriodAllow[nPeriod][nPurse].SetCheck( m_RowGroup.IsPurseAllowed( nPurse, nPeriod - 1 ) );
			}
		}

		OnSelectMaxSpendType();
	}

	return bResult;
}

//*******************************************************

BOOL CGroupPropertyPage7::OnKillActive()
{
	BOOL bResult = FALSE;

	if ( UpdateData() == TRUE )
	{
		int nMaxSpendType = nGRPSPENDTYPE_DAILY_LOCKED;

		switch( m_comboMaxSpendType.GetCurSel() )
		{
		case 0:
			switch( m_comboMaxSpendOverride.GetCurSel() )
			{
			case 1:		nMaxSpendType = nGRPSPENDTYPE_DAILY_RECORD1;	break;
			case 2:		nMaxSpendType = nGRPSPENDTYPE_DAILY_RECORD2;	break;
			case 0:
			default:	nMaxSpendType = nGRPSPENDTYPE_DAILY_LOCKED;		break;
			}
			break;

		case 1:
			switch( m_comboMaxSpendOverride.GetCurSel() )
			{
			case 1:		nMaxSpendType = nGRPSPENDTYPE_PERIOD_RECORD1;	break;
			case 2:		nMaxSpendType = nGRPSPENDTYPE_PERIOD_RECORD2;	break;
			case 0:
			default:	nMaxSpendType = nGRPSPENDTYPE_PERIOD_LOCKED;	break;
			}
			break;
		}
	
		m_RowGroup.SetMaxSpendType ( nMaxSpendType );

		switch( nMaxSpendType )
		{
		case nGRPSPENDTYPE_PERIOD_RECORD1:
		case nGRPSPENDTYPE_PERIOD_RECORD2:
		case nGRPSPENDTYPE_PERIOD_LOCKED:
		
			for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
			{
				for ( int nPurse = 1; nPurse <= 3; nPurse++ )
				{
					m_RowGroup.SetPurseAllowed( nPurse, nPeriod - 1, m_checkPeriodAllow[nPeriod][nPurse].GetCheck() != 0 ); 
				}
			}

			m_RowGroup.SetMaxSpendPurse1Period1 ( m_dMaxSpend[1] );
			m_RowGroup.SetMaxSpendPurse1Period2 ( m_dMaxSpend[2] );
			m_RowGroup.SetMaxSpendPurse1Period3 ( m_dMaxSpend[3] );
			m_RowGroup.SetMaxSpendPurse1Period4 ( m_dMaxSpend[4] );
			m_RowGroup.SetMaxSpendPurse1Period5 ( m_dMaxSpend[5] );
			m_RowGroup.SetMaxSpendPurse1Period6 ( m_dMaxSpend[6] );

			m_RowGroup.SetMaxSpendPurse3Period1( m_dMaxSpendP3[1] );
			m_RowGroup.SetMaxSpendPurse3Period2( m_dMaxSpendP3[2] );
			m_RowGroup.SetMaxSpendPurse3Period3( m_dMaxSpendP3[3] );
			m_RowGroup.SetMaxSpendPurse3Period4( m_dMaxSpendP3[4] );
			m_RowGroup.SetMaxSpendPurse3Period5( m_dMaxSpendP3[5] );
			m_RowGroup.SetMaxSpendPurse3Period6( m_dMaxSpendP3[6] );

			m_RowGroup.SetMaxOverdraftPurse1Period1( m_dOverspend[1] );
			m_RowGroup.SetMaxOverdraftPurse1Period2( m_dOverspend[2] );
			m_RowGroup.SetMaxOverdraftPurse1Period3( m_dOverspend[3] );
			m_RowGroup.SetMaxOverdraftPurse1Period4( m_dOverspend[4] );
			m_RowGroup.SetMaxOverdraftPurse1Period5( m_dOverspend[5] );
			m_RowGroup.SetMaxOverdraftPurse1Period6( m_dOverspend[6] );

			m_RowGroup.SetMaxOverdraftPurse3Period1( m_dOverspendP3[1] );
			m_RowGroup.SetMaxOverdraftPurse3Period2( m_dOverspendP3[2] );
			m_RowGroup.SetMaxOverdraftPurse3Period3( m_dOverspendP3[3] );
			m_RowGroup.SetMaxOverdraftPurse3Period4( m_dOverspendP3[4] );
			m_RowGroup.SetMaxOverdraftPurse3Period5( m_dOverspendP3[5] );
			m_RowGroup.SetMaxOverdraftPurse3Period6( m_dOverspendP3[6] );
			
			break;
			
		case nGRPSPENDTYPE_DAILY_RECORD1:
		case nGRPSPENDTYPE_DAILY_RECORD2:
		case nGRPSPENDTYPE_DAILY_LOCKED:
		default:

			m_RowGroup.SetMaxSpendPurse1Period1 ( m_dMaxSpend[1] );
			m_RowGroup.SetMaxSpendPurse3Period1( m_dMaxSpendP3[1] );
			m_RowGroup.SetMaxOverdraftPurse1Period1( m_dOverspend[1] );
			m_RowGroup.SetMaxOverdraftPurse3Period1( m_dOverspendP3[1] );

			break;
		}

		bResult =  CSSPropertyPage::OnKillActive();
	}

	return bResult;
}

//*******************************************************

void CGroupPropertyPage7::OnSelectMaxSpendType()
{
	bool bPeriod = ( m_comboMaxSpendType.GetCurSel() == 1 );

	bool bPurse2Allow = ( m_RowGroup.GetRefreshType() != nREFRESHTYPE_DISABLED );
	bool bPurse3Allow = System.GetEnablePurse3Flag();
	bool bOverspendAllow = Server.GetAllowOverdraftFlag(); 
	
	// HIDE UNWANTED CONTROLS IF NOT IN PERIOD MODE

	ShowAndEnableWindow( &m_staticPeriod, bPeriod );
	ShowAndEnableWindow( &m_staticStart, bPeriod );
	ShowAndEnableWindow( &m_staticPurse2, bPeriod );

	for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
	{
		ShowAndEnableWindow( &m_editPeriodText[nPeriod], bPeriod );
		ShowAndEnableWindow( &m_staticPeriodStart[nPeriod], bPeriod );

		if ( nPeriod >= 2 )
		{
			ShowAndEnableWindow( GetEditMaxSpend(nPeriod), bPeriod );
			ShowAndEnableWindow( GetEditOverspend(nPeriod), bPeriod );
			ShowAndEnableWindow( GetEditMaxSpendP3(nPeriod), bPeriod );
			ShowAndEnableWindow( GetEditOverspendP3(nPeriod), bPeriod );
		}

		for ( int nPurse = 1; nPurse <= 3; nPurse++ )
		{
			ShowAndEnableWindow( &m_checkPeriodAllow[nPeriod][nPurse], bPeriod );
		}
	}

	// POSITION AND ENABLE CONTROLS ACCORDING TO MODE

	if ( FALSE == bPeriod )
	{
		MoveControl( &m_staticPurse1, 17, 109 );
		MoveControl( &m_staticMaxSpend, 54, 87 );
		MoveControl( &m_staticOverdraft, 118, 87 );
		MoveControl( &m_staticPurse3, 211, 109 );
		MoveControl( &m_staticMaxSpendP3, 248, 87 );
		MoveControl( &m_staticOverdraftP3, 312, 87 );
		MoveControl( GetEditMaxSpend(1), 54, 107 );
		MoveControl( GetEditOverspend(1), 118, 107 );
		MoveControl( GetEditMaxSpendP3(1), 248, 107 );
		MoveControl( GetEditOverspendP3(1), 312, 107 );
		GetEditMaxSpend(1) -> EnableWindow( TRUE );
		GetEditOverspend(1) -> EnableWindow( bOverspendAllow );
		GetEditMaxSpendP3(1) -> EnableWindow( bPurse3Allow );
		GetEditOverspendP3(1) -> EnableWindow( bOverspendAllow & bPurse3Allow );
		Invalidate();
	}
	else
	{
		MoveControl( &m_staticPurse1, 108, 87 );
		MoveControl( &m_staticMaxSpend, 145, 87 );
		MoveControl( &m_staticOverdraft, 209, 87 );
		MoveControl( &m_staticPurse3, 332, 87 );
		MoveControl( &m_staticMaxSpendP3, 369, 87 );
		MoveControl( &m_staticOverdraftP3, 433, 87 );
		MoveControl( GetEditMaxSpend(1), 145, 107 );
		MoveControl( GetEditOverspend(1), 209, 107 );
		MoveControl( GetEditMaxSpendP3(1), 369, 107 );
		MoveControl( GetEditOverspendP3(1), 433, 107 );
		
		for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
		{
			bool bPeriodAllow = SQLRowSetAuditPeriod.IsPeriodSet( nPeriod - 1 );
			
			m_checkPeriodAllow[nPeriod][1].EnableWindow( bPeriodAllow );
			m_checkPeriodAllow[nPeriod][2].EnableWindow( bPeriodAllow & bPurse2Allow );
			m_checkPeriodAllow[nPeriod][3].EnableWindow( bPeriodAllow & bPurse3Allow );
				
			for ( int nPurse = 1; nPurse <= 3; nPurse++ )
			{
				if ( m_checkPeriodAllow[nPeriod][nPurse].IsWindowEnabled() == FALSE )
					m_checkPeriodAllow[nPeriod][nPurse].SetCheck(FALSE);
			}
		}
	
		SetPeriodDisplay();
		RedrawWindow();
	}
}

//*******************************************************

void CGroupPropertyPage7::SetPeriodDisplay()
{
	bool bOverspendAllow = Server.GetAllowOverdraftFlag();
	
	for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
	{
		bool bPurse1Allow = ( m_checkPeriodAllow[nPeriod][1].GetCheck() != 0 );
		bool bPurse3Allow = ( m_checkPeriodAllow[nPeriod][3].GetCheck() != 0 );
	
		GetEditMaxSpend(nPeriod) -> EnableWindow( bPurse1Allow  );
		GetEditOverspend(nPeriod) -> EnableWindow( bPurse1Allow & bOverspendAllow );
		GetEditMaxSpendP3(nPeriod) -> EnableWindow( bPurse3Allow );
		GetEditOverspendP3(nPeriod) -> EnableWindow( bPurse3Allow & bOverspendAllow );
	}
}

//*******************************************************

void CGroupPropertyPage7::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		if ( m_RowGroup.GetRefreshType() == nREFRESHTYPE_PERIOD )
		{
			switch( m_RowGroup.GetMaxSpendType() )
			{
			case nGRPSPENDTYPE_PERIOD_LOCKED:
			case nGRPSPENDTYPE_PERIOD_RECORD1:
			case nGRPSPENDTYPE_PERIOD_RECORD2:
				break;

			default:
				{
					Prompter.Error( "As Purse2 Refresh is set to \"Period Refresh\" the Daily Spend Limits setting for this group has been changed to \"Use Audit Periods\"!", "WARNING" );
					m_RowGroup.SetMaxSpendType( nGRPSPENDTYPE_PERIOD_LOCKED );
				}
				break;
			}
		}
				
		CSSPropertyPage::OnOK();
	}
}

//*******************************************************

