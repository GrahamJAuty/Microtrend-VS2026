//*******************************************************
#include "StandardCombos.h"
//*******************************************************
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//*******************************************************
#include "SetupGroupPeriods2Dlg.h"
//*******************************************************

CSetupGroupPeriods2Dlg::CSetupGroupPeriods2Dlg(CSQLRowGroupFull& RowGroup, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetupGroupPeriods2Dlg::IDD, pParent)
	, m_RowGroup(RowGroup)
{
	m_strTitle.Format(" Setup Group %d Purse2 Period Refresh Settings", m_RowGroup.GetGroupNo());

	SQLRowSetAuditPeriod.LoadPeriods();
	m_strPeriodText1 = SQLRowSetAuditPeriod.GetPeriodName(0);
	m_strPeriodText2 = SQLRowSetAuditPeriod.GetPeriodName(1);
	m_strPeriodText3 = SQLRowSetAuditPeriod.GetPeriodName(2);
	m_strPeriodText4 = SQLRowSetAuditPeriod.GetPeriodName(3);
	m_strPeriodText5 = SQLRowSetAuditPeriod.GetPeriodName(4);
	m_strPeriodText6 = SQLRowSetAuditPeriod.GetPeriodName(5);
	
	m_strStartPeriod1 = SQLRowSetAuditPeriod.FormatStartTime(0);
	m_strStartPeriod2 = SQLRowSetAuditPeriod.FormatStartTime(1);
	m_strStartPeriod3 = SQLRowSetAuditPeriod.FormatStartTime(2);
	m_strStartPeriod4 = SQLRowSetAuditPeriod.FormatStartTime(3);
	m_strStartPeriod5 = SQLRowSetAuditPeriod.FormatStartTime(4);
	m_strStartPeriod6 = SQLRowSetAuditPeriod.FormatStartTime(5);

	m_bPeriod1Purse2 = m_RowGroup.IsPurseAllowed(2, 0);
	m_bPeriod2Purse2 = m_RowGroup.IsPurseAllowed(2, 1);
	m_bPeriod3Purse2 = m_RowGroup.IsPurseAllowed(2, 2);
	m_bPeriod4Purse2 = m_RowGroup.IsPurseAllowed(2, 3);
	m_bPeriod5Purse2 = m_RowGroup.IsPurseAllowed(2, 4);
	m_bPeriod6Purse2 = m_RowGroup.IsPurseAllowed(2, 5);

	m_dRefreshPeriod1 = m_RowGroup.GetRefreshValueByPeriod(0);
	m_dRefreshPeriod2 = m_RowGroup.GetRefreshValueByPeriod(1);
	m_dRefreshPeriod3 = m_RowGroup.GetRefreshValueByPeriod(2);
	m_dRefreshPeriod4 = m_RowGroup.GetRefreshValueByPeriod(3);
	m_dRefreshPeriod5 = m_RowGroup.GetRefreshValueByPeriod(4);
	m_dRefreshPeriod6 = m_RowGroup.GetRefreshValueByPeriod(5);

	m_nPeriod1Purse2 = m_RowGroup.GetRefreshTypeByPeriod(0);
	m_nPeriod2Purse2 = m_RowGroup.GetRefreshTypeByPeriod(1);
	m_nPeriod3Purse2 = m_RowGroup.GetRefreshTypeByPeriod(2);
	m_nPeriod4Purse2 = m_RowGroup.GetRefreshTypeByPeriod(3);
	m_nPeriod5Purse2 = m_RowGroup.GetRefreshTypeByPeriod(4);
	m_nPeriod6Purse2 = m_RowGroup.GetRefreshTypeByPeriod(5);
}

//*******************************************************

void CSetupGroupPeriods2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PERIODTEXT1, m_strPeriodText1);
	DDX_Text(pDX, IDC_EDIT_PERIODTEXT2, m_strPeriodText2);
	DDX_Text(pDX, IDC_EDIT_PERIODTEXT3, m_strPeriodText3);
	DDX_Text(pDX, IDC_EDIT_PERIODTEXT4, m_strPeriodText4);
	DDX_Text(pDX, IDC_EDIT_PERIODTEXT5, m_strPeriodText5);
	DDX_Text(pDX, IDC_EDIT_PERIODTEXT6, m_strPeriodText6);

	DDX_Text(pDX, IDC_STATIC_STARTPERIOD1, m_strStartPeriod1);
	DDX_Text(pDX, IDC_STATIC_STARTPERIOD2, m_strStartPeriod2);
	DDX_Text(pDX, IDC_STATIC_STARTPERIOD3, m_strStartPeriod3);
	DDX_Text(pDX, IDC_STATIC_STARTPERIOD4, m_strStartPeriod4);
	DDX_Text(pDX, IDC_STATIC_STARTPERIOD5, m_strStartPeriod5);
	DDX_Text(pDX, IDC_STATIC_STARTPERIOD6, m_strStartPeriod6);

	DDX_Control(pDX, IDC_CHECK_P1_PURSE2, m_checkPeriod1Purse2);
	DDX_Control(pDX, IDC_CHECK_P2_PURSE2, m_checkPeriod2Purse2);
	DDX_Control(pDX, IDC_CHECK_P3_PURSE2, m_checkPeriod3Purse2);
	DDX_Control(pDX, IDC_CHECK_P4_PURSE2, m_checkPeriod4Purse2);
	DDX_Control(pDX, IDC_CHECK_P5_PURSE2, m_checkPeriod5Purse2);
	DDX_Control(pDX, IDC_CHECK_P6_PURSE2, m_checkPeriod6Purse2);

	DDX_Check(pDX, IDC_CHECK_P1_PURSE2, m_bPeriod1Purse2);
	DDX_Check(pDX, IDC_CHECK_P2_PURSE2, m_bPeriod2Purse2);
	DDX_Check(pDX, IDC_CHECK_P3_PURSE2, m_bPeriod3Purse2);
	DDX_Check(pDX, IDC_CHECK_P4_PURSE2, m_bPeriod4Purse2);
	DDX_Check(pDX, IDC_CHECK_P5_PURSE2, m_bPeriod5Purse2);
	DDX_Check(pDX, IDC_CHECK_P6_PURSE2, m_bPeriod6Purse2);

	DDX_Text(pDX, IDC_EDIT_REFRESH1, m_dRefreshPeriod1);
	DDX_Text(pDX, IDC_EDIT_REFRESH2, m_dRefreshPeriod2);
	DDX_Text(pDX, IDC_EDIT_REFRESH3, m_dRefreshPeriod3);
	DDX_Text(pDX, IDC_EDIT_REFRESH4, m_dRefreshPeriod4);
	DDX_Text(pDX, IDC_EDIT_REFRESH5, m_dRefreshPeriod5);
	DDX_Text(pDX, IDC_EDIT_REFRESH6, m_dRefreshPeriod6);
	
	DDX_Control(pDX, IDC_COMBO_PURSE2_P1, m_comboPeriod1Purse2);
	DDX_Control(pDX, IDC_COMBO_PURSE2_P2, m_comboPeriod2Purse2);
	DDX_Control(pDX, IDC_COMBO_PURSE2_P3, m_comboPeriod3Purse2);
	DDX_Control(pDX, IDC_COMBO_PURSE2_P4, m_comboPeriod4Purse2);
	DDX_Control(pDX, IDC_COMBO_PURSE2_P5, m_comboPeriod5Purse2);
	DDX_Control(pDX, IDC_COMBO_PURSE2_P6, m_comboPeriod6Purse2);

	DDX_CBIndex(pDX, IDC_COMBO_PURSE2_P1, m_nPeriod1Purse2);
	DDX_CBIndex(pDX, IDC_COMBO_PURSE2_P2, m_nPeriod2Purse2);
	DDX_CBIndex(pDX, IDC_COMBO_PURSE2_P3, m_nPeriod3Purse2);
	DDX_CBIndex(pDX, IDC_COMBO_PURSE2_P4, m_nPeriod4Purse2);
	DDX_CBIndex(pDX, IDC_COMBO_PURSE2_P5, m_nPeriod5Purse2);
	DDX_CBIndex(pDX, IDC_COMBO_PURSE2_P6, m_nPeriod6Purse2);
}

//*******************************************************

BEGIN_MESSAGE_MAP(CSetupGroupPeriods2Dlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_CHECK_P1_PURSE2, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_P2_PURSE2, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_P3_PURSE2, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_P4_PURSE2, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_P5_PURSE2, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_P6_PURSE2, SetDisplay)
END_MESSAGE_MAP()

//*******************************************************

BOOL CSetupGroupPeriods2Dlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText(m_strTitle);

	SubclassEdit(IDC_EDIT_REFRESH1, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REFRESH2, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REFRESH3, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REFRESH4, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REFRESH5, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REFRESH6, SS_NUM_SDP, 11, "%.2f");

	m_checkPeriod1Purse2.EnableWindow(SQLRowSetAuditPeriod.IsPeriodSet(0));
	m_checkPeriod2Purse2.EnableWindow(SQLRowSetAuditPeriod.IsPeriodSet(1));
	m_checkPeriod3Purse2.EnableWindow(SQLRowSetAuditPeriod.IsPeriodSet(2));
	m_checkPeriod4Purse2.EnableWindow(SQLRowSetAuditPeriod.IsPeriodSet(3));
	m_checkPeriod5Purse2.EnableWindow(SQLRowSetAuditPeriod.IsPeriodSet(4));
	m_checkPeriod6Purse2.EnableWindow(SQLRowSetAuditPeriod.IsPeriodSet(5));

	CStandardCombos::FillAutoRefreshCombo(m_comboPeriod1Purse2, m_nPeriod1Purse2);
	CStandardCombos::FillAutoRefreshCombo(m_comboPeriod2Purse2, m_nPeriod2Purse2);
	CStandardCombos::FillAutoRefreshCombo(m_comboPeriod3Purse2, m_nPeriod3Purse2);
	CStandardCombos::FillAutoRefreshCombo(m_comboPeriod4Purse2, m_nPeriod4Purse2);
	CStandardCombos::FillAutoRefreshCombo(m_comboPeriod5Purse2, m_nPeriod5Purse2);
	CStandardCombos::FillAutoRefreshCombo(m_comboPeriod6Purse2, m_nPeriod6Purse2);

	SetDisplay();

	return TRUE;
}

//*******************************************************

void CSetupGroupPeriods2Dlg::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		CString strMsg = CheckPeriodRefresh();
		if ( strMsg != "" )
		{
			Prompter.Error( strMsg, "Period Refresh Purse2 Settings Error" );
			return;
		}

		m_RowGroup.SetPurseAllowed(2, 0, ( m_bPeriod1Purse2 != 0 ) );
		m_RowGroup.SetPurseAllowed(2, 1, ( m_bPeriod2Purse2 != 0 ) );
		m_RowGroup.SetPurseAllowed(2, 2, ( m_bPeriod3Purse2 != 0 ) );
		m_RowGroup.SetPurseAllowed(2, 3, ( m_bPeriod4Purse2 != 0 ) );
		m_RowGroup.SetPurseAllowed(2, 4, ( m_bPeriod5Purse2 != 0 ) );
		m_RowGroup.SetPurseAllowed(2, 5, ( m_bPeriod6Purse2 != 0 ) );

		m_RowGroup.SetRefreshValueByPeriod( 0, m_dRefreshPeriod1 );
		m_RowGroup.SetRefreshValueByPeriod( 1, m_dRefreshPeriod2 );
		m_RowGroup.SetRefreshValueByPeriod( 2, m_dRefreshPeriod3 );
		m_RowGroup.SetRefreshValueByPeriod( 3, m_dRefreshPeriod4 );
		m_RowGroup.SetRefreshValueByPeriod( 4, m_dRefreshPeriod5 );
		m_RowGroup.SetRefreshValueByPeriod( 5, m_dRefreshPeriod6 );

		m_RowGroup.SetRefreshTypeByPeriod( 0, m_nPeriod1Purse2 );
		m_RowGroup.SetRefreshTypeByPeriod( 1, m_nPeriod2Purse2 );
		m_RowGroup.SetRefreshTypeByPeriod( 2, m_nPeriod3Purse2 );
		m_RowGroup.SetRefreshTypeByPeriod( 3, m_nPeriod4Purse2 );
		m_RowGroup.SetRefreshTypeByPeriod( 4, m_nPeriod5Purse2 );
		m_RowGroup.SetRefreshTypeByPeriod( 5, m_nPeriod6Purse2 );

		CSSAutoShutdownDialog::OnOK();
	}
}

//*******************************************************

void CSetupGroupPeriods2Dlg::SetDisplay()
{
	UpdateData(TRUE);

	m_comboPeriod1Purse2.EnableWindow(m_bPeriod1Purse2);
	GetEditRefresh1()->EnableWindow(m_bPeriod1Purse2);

	m_comboPeriod2Purse2.EnableWindow(m_bPeriod2Purse2);
	GetEditRefresh2()->EnableWindow(m_bPeriod2Purse2);

	m_comboPeriod3Purse2.EnableWindow(m_bPeriod3Purse2);
	GetEditRefresh3()->EnableWindow(m_bPeriod3Purse2);

	m_comboPeriod4Purse2.EnableWindow(m_bPeriod4Purse2);
	GetEditRefresh4()->EnableWindow(m_bPeriod4Purse2);

	m_comboPeriod5Purse2.EnableWindow(m_bPeriod5Purse2);
	GetEditRefresh5()->EnableWindow(m_bPeriod5Purse2);

	m_comboPeriod6Purse2.EnableWindow(m_bPeriod6Purse2);
	GetEditRefresh6()->EnableWindow(m_bPeriod6Purse2);
}

//*******************************************************

CString CSetupGroupPeriods2Dlg::CheckPeriodRefresh()
{
	CString strMsg = "";
	//if ( m_pDbGroup->GetRefreshType() == nREFRESHTYPE_PERIOD )
	{
		int nCount = 0;
		int nCheckCount = 0;

		for ( int n = 1 ; n <= 6 ; n++ )
		{
			BOOL bCheck;
			double dValue;

			switch ( n )
			{
			case 1:	bCheck = m_bPeriod1Purse2;
					dValue = m_dRefreshPeriod1;
					break;

			case 2:	bCheck = m_bPeriod2Purse2;
					dValue = m_dRefreshPeriod2;
					break;

			case 3:	bCheck = m_bPeriod3Purse2;
					dValue = m_dRefreshPeriod3;
					break;

			case 4:	bCheck = m_bPeriod4Purse2;
					dValue = m_dRefreshPeriod4;
					break;

			case 5:	bCheck = m_bPeriod5Purse2;
					dValue = m_dRefreshPeriod5;
					break;

			case 6:	bCheck = m_bPeriod6Purse2;
					dValue = m_dRefreshPeriod6;
					break;
			}

			if ( bCheck == TRUE )
			{
				++nCheckCount;
				/*
				if ( dValue == 0 )
				{
					if ( strMsg != "" )		strMsg += "\n\n";
					CString strMsg2;		strMsg2.Format ( "%d ) Period %d Refresh Value is set to 0!", ++nCount, n );
					strMsg += strMsg2;
				}
				*/
			}
		}

		if ( nCheckCount == 0 )
		{
			if (strMsg != "")
			{
				strMsg += "\n\n";
			}

			CString strMsg2= "";
			strMsg2.Format ( "%d ) No periods for Purse2 usage have been set!", ++nCount );
			strMsg += strMsg2;
		}
	}

	return strMsg;
}

//*******************************************************
