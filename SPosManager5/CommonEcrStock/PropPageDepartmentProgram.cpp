/**********************************************************************/
#include "..\CommonEcrStockTray\SysInfo.h"
/**********************************************************************/
#include "PropPageDepartmentProgram.h"
/**********************************************************************/

CPropPageDepartmentProgram::CPropPageDepartmentProgram() : CSSPropertyPage(CPropPageDepartmentProgram::IDD)
{
	//{{AFX_DATA_INIT(CPropPageDepartmentProgram)
	//}}AFX_DATA_INIT
	m_pDeptRecord = NULL;
	m_bShowLoyalty = (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_LOY_V4);
	m_nTickets = 0;
	m_nLoyaltyBonusType = 0;
}

/**********************************************************************/

CPropPageDepartmentProgram::~CPropPageDepartmentProgram()
{
}

/**********************************************************************/

void CPropPageDepartmentProgram::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageDepartmentProgram)

	DDX_Control(pDX, IDC_COMBO_TICKETMODE, m_comboTicketMode);
	DDX_Text(pDX, IDC_EDIT_TICKETS, m_nTickets);

	DDX_Control(pDX, IDC_CHECK_DPQTY, m_checkDPQty);
	DDX_Control(pDX, IDC_CHECK_NEGATIVE, m_checkNegative);
	DDX_Control(pDX, IDC_CHECK_NONADD, m_checkNonAdd);

	DDX_Control(pDX, IDC_STATIC_EFFECTS, m_staticEffects);
	DDX_Control(pDX, IDC_STATIC_LOYALTY, m_staticLoyalty);
	DDX_Control(pDX, IDC_STATIC_TRIP, m_staticTrip);
	DDX_Control(pDX, IDC_RADIO_POINTS, m_radioPoints);
	DDX_Control(pDX, IDC_RADIO_VALUE, m_radioValue);
	DDX_Control(pDX, IDC_RADIO_PERCENT, m_radioPercent);
	DDX_Control(pDX, IDC_CHECK_GENERALBONUS, m_checkGeneralBonus);

	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageDepartmentProgram, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageDepartmentProgram)
	ON_BN_CLICKED(IDC_RADIO_POINTS, OnRadioPoints)
	ON_BN_CLICKED(IDC_RADIO_VALUE, OnRadioValue)
	ON_BN_CLICKED(IDC_RADIO_PERCENT, OnRadioPercent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageDepartmentProgram::OnInitDialog()
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	SubclassEdit(IDC_EDIT_TICKETS, SS_NUM, 2, "%d");
	SubclassEdit(IDC_EDIT_TRIP, SS_NUM_DP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_POINTS, SS_NUM, 6, "%d");
	SubclassEdit(IDC_EDIT_VALUE, SS_NUM_DP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_PERCENT, SS_NUM, 3, "%d");

	m_comboTicketMode.AddString("No");
	m_comboTicketMode.AddString("At finalisation");
	m_comboTicketMode.AddString("At lay-away");

	if (FALSE == m_bShowLoyalty)
	{
		ShowAndEnableWindow(&m_staticLoyalty, FALSE);
		ShowAndEnableWindow(&m_staticTrip, FALSE);
		ShowAndEnableWindow(GetEditTrip(), FALSE);
		ShowAndEnableWindow(&m_radioPoints, FALSE);
		ShowAndEnableWindow(&m_radioValue, FALSE);
		ShowAndEnableWindow(&m_radioPercent, FALSE);
		ShowAndEnableWindow(GetEditPoints(), FALSE);
		ShowAndEnableWindow(GetEditValue(), FALSE);
		ShowAndEnableWindow(GetEditPercent(), FALSE);
		ShowAndEnableWindow(&m_checkGeneralBonus, FALSE);
		MoveControl(&m_staticEffects, 7, 7, 358, 217);
	}

	return TRUE;
}

/**********************************************************************/
CEdit* CPropPageDepartmentProgram::GetEditTickets() { return GetEdit( IDC_EDIT_TICKETS ); }
CEdit* CPropPageDepartmentProgram::GetEditTrip() { return GetEdit( IDC_EDIT_TRIP ); }
CEdit* CPropPageDepartmentProgram::GetEditPoints() { return GetEdit( IDC_EDIT_POINTS ); }
CEdit* CPropPageDepartmentProgram::GetEditValue() { return GetEdit( IDC_EDIT_VALUE ); }
CEdit* CPropPageDepartmentProgram::GetEditPercent() { return GetEdit( IDC_EDIT_PERCENT ); }
/**********************************************************************/

BOOL CPropPageDepartmentProgram::OnKillActive()
{
	bool bResult = (CPropertyPage::OnKillActive() != 0);

	if (TRUE == bResult)
	{
		bResult = SaveRecord();
	}

	return bResult;
}

/**********************************************************************/

BOOL CPropPageDepartmentProgram::OnSetActive()
{
	GetRecordData();
	bool bResult = (CPropertyPage::OnSetActive() != 0);
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageDepartmentProgram::GetRecordData()
{
	m_nTickets = m_pDeptRecord->GetTickets();
}

/**********************************************************************/

void CPropPageDepartmentProgram::SetRecordControls()
{
	int nMode = m_pDeptRecord->GetTicketsMode();

	if ((nMode < 0) || (nMode > 2))
	{
		nMode = 0;
	}

	m_comboTicketMode.SetCurSel(nMode);

	m_checkDPQty.SetCheck(m_pDeptRecord->GetDPQtyFlag());
	m_checkNegative.SetCheck(m_pDeptRecord->GetNegativeFlag());
	m_checkNonAdd.SetCheck(m_pDeptRecord->GetNonAddFlag());

	if (TRUE == m_bShowLoyalty)
	{
		int nType = m_pDeptRecord->GetLoyaltyBonusType();

		if ((nType < 0) || (nType > 2))
		{
			nType = 0;
		}

		switch (nType)
		{
		case 1:
			OnRadioValue();
			break;

		case 2:
			OnRadioPercent();
			break;

		case 0:
		default:
			OnRadioPoints();
		}

		SetEditBoxDouble(*GetEditTrip(), m_pDeptRecord->GetLoyaltyBonusTrip(), 2);
		SetEditBoxInt(*GetEditPoints(), m_pDeptRecord->GetLoyaltyBonusPoints());
		SetEditBoxDouble(*GetEditValue(), m_pDeptRecord->GetLoyaltyBonusValue(), 2);
		SetEditBoxInt(*GetEditPercent(), m_pDeptRecord->GetLoyaltyBonusPercent());
		m_checkGeneralBonus.SetCheck(m_pDeptRecord->GetLoyaltyGeneralSpendBonus());
	}
}

/**********************************************************************/

void CPropPageDepartmentProgram::OnRadioPoints()
{
	UpdateBonusEditBoxes(0);
}

/**********************************************************************/

void CPropPageDepartmentProgram::OnRadioValue()
{
	UpdateBonusEditBoxes(1);
}

/**********************************************************************/

void CPropPageDepartmentProgram::OnRadioPercent()
{
	UpdateBonusEditBoxes(2);
}

/**********************************************************************/

void CPropPageDepartmentProgram::UpdateBonusEditBoxes(int nType)
{
	m_nLoyaltyBonusType = nType;
	m_radioPoints.SetCheck(0 == nType);
	m_radioValue.SetCheck(1 == nType);
	m_radioPercent.SetCheck(2 == nType);
	GetEditPoints()->EnableWindow(0 == nType);
	GetEditValue()->EnableWindow(1 == nType);
	GetEditPercent()->EnableWindow(2 == nType);
}

/**********************************************************************/

bool CPropPageDepartmentProgram::UpdateRecord()
{
	bool bResult = FALSE;
	if (UpdateData(TRUE) == TRUE)
	{
		bResult = SaveRecord();
	}
	return bResult;
}

/**********************************************************************/

bool CPropPageDepartmentProgram::SaveRecord()
{
	m_pDeptRecord->SetTicketsMode(m_comboTicketMode.GetCurSel());
	m_pDeptRecord->SetTickets(m_nTickets);
	m_pDeptRecord->SetDPQtyFlag(m_checkDPQty.GetCheck() != 0);
	m_pDeptRecord->SetNegativeFlag(m_checkNegative.GetCheck() != 0);
	m_pDeptRecord->SetNonAddFlag(m_checkNonAdd.GetCheck() != 0);

	if (TRUE == m_bShowLoyalty)
	{
		int nPercent = GetEditBoxInt(*GetEditPercent());
		if ((nPercent > 100) && (m_nLoyaltyBonusType == 2))
		{
			Prompter.Error("Loyalty bonus percent must be less than or equal to 100");
			GetEditPercent()->SetFocus();
			GetEditPercent()->SetSel(0, -1);
			return FALSE;
		}

		m_pDeptRecord->SetLoyaltyBonusType(m_nLoyaltyBonusType);
		m_pDeptRecord->SetLoyaltyBonusTrip(GetEditBoxDouble(*GetEditTrip()));
		m_pDeptRecord->SetLoyaltyBonusPoints(GetEditBoxInt(*GetEditPoints()));
		m_pDeptRecord->SetLoyaltyBonusValue(GetEditBoxDouble(*GetEditValue()));
		m_pDeptRecord->SetLoyaltyBonusPercent(GetEditBoxInt(*GetEditPercent()));
		m_pDeptRecord->SetLoyaltyGeneralSpendBonus(IsTicked(m_checkGeneralBonus));
	}

	return TRUE;
}

/**********************************************************************/

void CPropPageDepartmentProgram::Refresh()
{
	GetRecordData();
	UpdateData(FALSE);
	SetRecordControls();
}

/**********************************************************************/
