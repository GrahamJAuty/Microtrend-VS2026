/**********************************************************************/
#include "PropPageServerGeneral.h"
/**********************************************************************/

CPropPageServerGeneral::CPropPageServerGeneral() : CSSPropertyPage(CPropPageServerGeneral::IDD), m_MacroSelector( m_editMacro, m_comboMacro )
{
	//{{AFX_DATA_INIT(CPropPageServerGeneral)
	//}}AFX_DATA_INIT
	m_nAuthority = 1;
	m_strPassword = "1";
	m_nMacroNo = 0;
	m_nServerDbIdx = 0;
	m_nServerLocIdx = 0;
	m_dHourlyRate = 0.0;
	m_nDailyHours = 8;
	m_nColourScheme = 0;
}

/**********************************************************************/

CPropPageServerGeneral::~CPropPageServerGeneral()
{
}

/**********************************************************************/

void CPropPageServerGeneral::DoDataExchange(CDataExchange* pDX)
{
	CString strMinPassword = "";
	strMinPassword.Format ( "%d", Server::Password.Min );

	CString strMaxPassword = "";
	strMaxPassword.Format ( "%d", Server::Password.Max );

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageServerGeneral)
	DDX_Control(pDX, IDC_EDIT_NAME_RECEIPT, m_editNameReceipt);
	DDX_Control(pDX, IDC_EDIT_NAME_FULL, m_editNameFull);
	DDX_Control(pDX, IDC_CHECK_TRAINING, m_checkTraining);
	DDX_Control(pDX, IDC_EDIT_MACRO, m_editMacro);
	DDX_Control(pDX, IDC_COMBO_MACRO, m_comboMacro);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_COMBO_DRAWER, m_comboDrawer);
	DDX_Control(pDX, IDC_COMBO_VIEW, m_comboView);
	DDX_Control(pDX, IDC_COMBO_OPENTABLE, m_comboOpenTable);
	DDX_Control(pDX, IDC_STATIC_LOCATION, m_staticLocation);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_CHECK_MIDNIGHTSHIFT, m_checkMidnightShift);
	DDX_Control(pDX, IDC_STATIC1, m_static1);
	DDX_Control(pDX, IDC_STATIC2, m_static2);
	DDX_Control(pDX, IDC_STATIC3, m_static3);
	DDX_Control(pDX, IDC_STATIC4, m_static4);
	DDX_Control(pDX, IDC_STATIC5, m_static5);
	DDX_Control(pDX, IDC_STATIC6, m_static6);
	DDX_IntegerString(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDV_IntegerString(pDX, m_strPassword, strMinPassword, strMaxPassword );
	DDX_Text(pDX, IDC_EDIT_AUTH, m_nAuthority);
	DDV_MinMaxInt(pDX, m_nAuthority, Server::Authority.Min, Server::Authority.Max);
	DDX_Text(pDX, IDC_EDIT_HOURLYRATE, m_dHourlyRate);
	DDV_MinMaxDouble(pDX, m_dHourlyRate, Server::HourlyRate.Min, Server::HourlyRate.Max);
	DDX_Text(pDX, IDC_EDIT_DAILYHOURS, m_nDailyHours);
	DDV_MinMaxInt(pDX, m_nDailyHours, Server::DailyHours.Min, Server::DailyHours.Max);
	DDX_Text(pDX, IDC_EDIT_COLOUR, m_nColourScheme);
	DDV_MinMaxInt(pDX, m_nColourScheme, Server::TradingColourScheme.Min, Server::TradingColourScheme.Max);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageServerGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageServerGeneral)
	ON_EN_KILLFOCUS(IDC_EDIT_MACRO, OnKillFocusMacro)
	ON_CBN_SELCHANGE(IDC_COMBO_MACRO, OnSelectMacro)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageServerGeneral::OnInitDialog()
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_editNameReceipt.LimitText(Server::ReceiptName.Max);
	m_editNameFull.LimitText(Server::FullName.Max);

	SubclassEdit(IDC_EDIT_AUTH, SS_NUM, 1, "%d");
	SubclassEdit(IDC_EDIT_HOURLYRATE, SS_NUM_DP, 6, "%.*f", 2);
	SubclassEdit(IDC_EDIT_DAILYHOURS, SS_NUM, 2, "%d");
	SubclassEdit(IDC_EDIT_COLOUR, SS_NUM, 2, "%d");

	if (FALSE == m_bFullEdit)
	{
		m_static1.ShowWindow(SW_HIDE);
		m_static2.ShowWindow(SW_HIDE);
		m_static3.ShowWindow(SW_HIDE);
		m_static4.ShowWindow(SW_HIDE);
		m_static5.ShowWindow(SW_HIDE);
		m_static6.ShowWindow(SW_HIDE);
		ShowAndEnableWindow(&m_checkTraining, FALSE);
		ShowAndEnableWindow(&m_editMacro, FALSE);
		ShowAndEnableWindow(&m_comboMacro, FALSE);
		ShowAndEnableWindow(&m_editPassword, FALSE);
		ShowAndEnableWindow(GetEditAuthority(), FALSE);
		ShowAndEnableWindow(&m_comboDrawer, FALSE);
		ShowAndEnableWindow(&m_comboView, FALSE);
		ShowAndEnableWindow(&m_comboOpenTable, FALSE);
		ShowAndEnableWindow(&m_staticLocation, FALSE);
		ShowAndEnableWindow(&m_comboLocation, FALSE);
		ShowAndEnableWindow(GetEditHourlyRate(), FALSE);
		ShowAndEnableWindow(GetEditDailyHours(), FALSE);
		ShowAndEnableWindow(GetEditColourScheme(), FALSE);
		ShowAndEnableWindow(&m_checkMidnightShift, FALSE);
	}
	else
	{
		m_editMacro.LimitText(4);
		m_MacroSelector.FillMacroCombo(0);
		m_editPassword.LimitText(10);

		m_comboDrawer.AddString("None");
		m_comboDrawer.AddString("One");
		m_comboDrawer.AddString("Two");

		m_comboView.AddString("Left");
		m_comboView.AddString("Right");

		m_comboOpenTable.AddString("Touch");
		m_comboOpenTable.AddString("Select and Open");

		if ((DealerFlags.GetServerHomeLocationFlag() == FALSE) || (FALSE == m_bFullEdit) || (m_nServerLocIdx >= 0))
		{
			ShowAndEnableWindow(&m_staticLocation, FALSE);
			ShowAndEnableWindow(&m_comboLocation, FALSE);
		}
		else
		{
			CWordArray arrayTypes;
			arrayTypes.Add(NODE_SYSTEM);
			arrayTypes.Add(NODE_LOCATION);
			m_LocationSelector.SetForceSystemFlag(TRUE);
			m_LocationSelector.ExcludeWebSales();

			if (m_nServerDbIdx >= 0)
			{
				m_LocationSelector.SetLocDbNoFilter(dbDatabase.GetDbNo(m_nServerDbIdx));
			}

			m_LocationSelector.BuildList(arrayTypes);
			m_LocationSelector.FillLocationCombo(&m_comboLocation, NULL);
		}
	}

	SetRecordControls();
	return TRUE;
}

/**********************************************************************/
CEdit* CPropPageServerGeneral::GetEditAuthority(){ return GetEdit( IDC_EDIT_AUTH ); }
CEdit* CPropPageServerGeneral::GetEditHourlyRate() { return GetEdit(IDC_EDIT_HOURLYRATE); }
CEdit* CPropPageServerGeneral::GetEditDailyHours() { return GetEdit(IDC_EDIT_DAILYHOURS); }
CEdit* CPropPageServerGeneral::GetEditColourScheme() { return GetEdit(IDC_EDIT_COLOUR); }
/**********************************************************************/

BOOL CPropPageServerGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageServerGeneral::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageServerGeneral::GetRecordData()
{
	if (TRUE == m_bFullEdit)
	{
		m_strPassword.Format("%d", m_pServerRecord->GetPassword());
		m_nAuthority = m_pServerRecord->GetAuthority();
		m_dHourlyRate = m_pServerRecord->GetHourlyRate();
		m_nDailyHours = m_pServerRecord->GetDailyHours();
		m_nColourScheme = m_pServerRecord->GetTradingColourScheme();
	}
}

/**********************************************************************/

void CPropPageServerGeneral::SetRecordControls()
{
	m_editNameReceipt.SetWindowText( m_pServerRecord -> GetReceiptEditName() );
	m_editNameFull.SetWindowText( m_pServerRecord -> GetFullName() );

	if ( TRUE == m_bFullEdit )
	{
		m_checkTraining.SetCheck( m_pServerRecord -> GetTrainingModeFlag() );

		m_nMacroNo = m_pServerRecord -> GetLogonMacro();
		m_MacroSelector.UpdateMacroCombo( m_nMacroNo );
		m_MacroSelector.UpdateMacroEditBox();

		int n = m_pServerRecord -> GetDrawer();
		if ( n < 0 || n > 2 ) n = 0;
		m_comboDrawer.SetCurSel(n);

		bool b = m_pServerRecord -> GetFlipViewFlag();
		m_comboView.SetCurSel( ( b == TRUE ) ? 0 : 1 );

		int o = m_pServerRecord -> GetOpenTableMode();
		m_comboOpenTable.SetCurSel( ( 2 == o ) ? 1 : 0 );

		m_checkMidnightShift.SetCheck(m_pServerRecord->GetMidnightShiftFlag());

		if ( ( DealerFlags.GetServerHomeLocationFlag() ) && ( m_nServerLocIdx < 0 ) )
		{
			int nLocIdx = 0;
			int nNwkLocNo = m_pServerRecord->GetHomeNwkLocNo();
			if ( dbLocation.FindLocationByNumber(nNwkLocNo / 1000, nNwkLocNo % 1000, nLocIdx) == TRUE)
			{
				m_LocationSelector.ForceSelection( NODE_LOCATION, nLocIdx);
			}
			else
			{
				m_LocationSelector.ForceSelection( NODE_SYSTEM, 0);
			}
		}

		SetEditBoxInt(*GetEditColourScheme(), m_pServerRecord->GetTradingColourScheme());
	}

	m_editNameReceipt.SetFocus();
	m_editNameReceipt.SetSel ( 0, -1 );
}

/**********************************************************************/

bool CPropPageServerGeneral::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageServerGeneral::SaveRecord()
{
	OnKillFocusMacro();

	m_pServerRecord->SetReceiptName(GetEditBoxText(m_editNameReceipt));
	m_pServerRecord->SetFullName(GetEditBoxText(m_editNameFull));
	
	if (TRUE == m_bFullEdit)
	{
		m_pServerRecord->SetTrainingModeFlag(IsTicked(m_checkTraining));
		m_pServerRecord->SetLogonMacro(m_nMacroNo);
		m_pServerRecord->SetPassword(atoi(m_strPassword));
		m_pServerRecord->SetAuthority(m_nAuthority);
		m_pServerRecord->SetDrawer(m_comboDrawer.GetCurSel());
		m_pServerRecord->SetFlipViewFlag(m_comboView.GetCurSel() == 0);
		m_pServerRecord->SetOpenTableMode((m_comboOpenTable.GetCurSel() == 1) ? 2 : 1);
		m_pServerRecord->SetHomeNwkLocNo(0);
		m_pServerRecord->SetHourlyRate(m_dHourlyRate);
		m_pServerRecord->SetDailyHours(m_nDailyHours);
		m_pServerRecord->SetMidnightShiftFlag(IsTicked(m_checkMidnightShift));
		m_pServerRecord->SetTradingColourScheme(GetEditBoxInt(*GetEditColourScheme()));

		if ((DealerFlags.GetServerHomeLocationFlag()) && (m_nServerLocIdx < 0))
		{
			CLocationSelectorEntity LocSelEntity;
			m_LocationSelector.GetSelectedEntity(LocSelEntity);

			if (LocSelEntity.GetConType() == NODE_LOCATION)
			{
				m_pServerRecord->SetHomeNwkLocNo(dbLocation.GetNetworkLocNo(LocSelEntity.GetEntityIdx()));
			}
		}
	}
}

/**********************************************************************/

void CPropPageServerGeneral::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/

void CPropPageServerGeneral::OnSelectMacro()
{
	m_MacroSelector.SelectMacroFromCombo();
	m_nMacroNo = m_MacroSelector.GetMacroNo();
}

/**********************************************************************/

void CPropPageServerGeneral::OnKillFocusMacro()
{
	m_MacroSelector.SelectMacroFromEditBox();
	m_nMacroNo = m_MacroSelector.GetMacroNo();
}

/**********************************************************************/