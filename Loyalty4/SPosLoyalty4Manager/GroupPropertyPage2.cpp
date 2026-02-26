//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "GroupPropertyPage2.h"
//$$******************************************************************

CGroupPropertyPage2::CGroupPropertyPage2(CSQLRowGroup& RowGroup)
	: CSSPropertyPage(CGroupPropertyPage2::IDD), m_RowGroup(RowGroup)
{
	m_nLoyaltyType = 0;
}

//$$******************************************************************

CGroupPropertyPage2::~CGroupPropertyPage2()
{
}

//$$******************************************************************

void CGroupPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_LOYALTYGROUP, m_checkLoyaltyGroup);
	DDX_Control(pDX, IDC_CHECK_LOYALTYDISABLE, m_checkDisableLoyalty);
	DDX_Control(pDX, IDC_RADIO_LOYALTYPOINTS, m_radioLoyaltyPoints);
	DDX_Control(pDX, IDC_RADIO_LOYALTYVALUE, m_radioLoyaltyValue);
	DDX_Control(pDX, IDC_CHECK_GROUPREDEEM, m_checkGroupRedeem);
	DDX_Control(pDX, IDC_CHECK_DISABLEREDEEM, m_checkDisableRedeem);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CGroupPropertyPage2, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_LOYALTYGROUP, OnToggleLoyaltyGroup)
	ON_BN_CLICKED(IDC_CHECK_LOYALTYDISABLE, OnToggleLoyaltyDisable)
	ON_BN_CLICKED(IDC_RADIO_LOYALTYPOINTS, OnRadioLoyaltyPoints)
	ON_BN_CLICKED(IDC_RADIO_LOYALTYVALUE, OnRadioLoyaltyValue)
	ON_BN_CLICKED(IDC_CHECK_GROUPREDEEM, OnToggleGroupRedeem)
	ON_BN_CLICKED(IDC_CHECK_DISABLEREDEEM, OnToggleDisableRedeem)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_SPEND, OnButtonDefaultSpend)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_REDEEM, OnButtonDefaultRedeem)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CGroupPropertyPage2::OnInitDialog()
{
	m_nLoyaltyType = m_RowGroup.GetRewardType();

	CSSPropertyPage::OnInitDialog();

	SubclassEdit(IDC_EDIT_LOYALTYTRIPCARD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_LOYALTYVALUE, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_LOYALTYPOINTS, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_MULTIPLIERTRIGGER, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_UPPERSPENDLIMIT, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REDEEMPOINTSTRIP, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REDEEMPOINTSVALUE, SS_NUM_SDP, 11, "%.2f");

	m_checkLoyaltyGroup.SetCheck(m_RowGroup.GetRewardFlag());
	m_checkDisableLoyalty.SetCheck(m_RowGroup.GetRewardDisable());

	SetEditBoxDouble(*GetEditLoyaltyTripCard(), m_RowGroup.GetRewardTrip(), 2);
	SetEditBoxInt(*GetEditLoyaltyPoints(), m_RowGroup.GetRewardPoints());
	SetEditBoxDouble(*GetEditLoyaltyValue(), m_RowGroup.GetRewardBonus(), 2);
	SetEditBoxDouble(*GetEditMultiplierTrigger(), m_RowGroup.GetRewardTrigger(), 2);
	SetEditBoxDouble(*GetEditUpperSpendLimit(), m_RowGroup.GetRewardLimit(), 2);

	if ((Server.GetRewardPurse1SpendFlag() == FALSE) && (Server.GetRewardPurse2SpendFlag() == FALSE) && (Server.GetRewardCashSpendFlag() == FALSE))
	{
		GetEditMultiplierTrigger()->EnableWindow(FALSE);
	}

	OnToggleLoyaltyGroup();

	/*****/

	m_checkGroupRedeem.SetCheck(m_RowGroup.GetRedeemFlag());
	m_checkDisableRedeem.SetCheck(m_RowGroup.GetRedeemDisable());

	SetEditBoxInt(*GetEditRedeemPointsTrip(), m_RowGroup.GetRedeemTrip());
	SetEditBoxDouble(*GetEditRedeemPointsValue(), m_RowGroup.GetRedeemValue(), 2);

	SetRedeemDisplay();
	GetEditRedeemPointsValue()->SetSel(0, -1);
	GetEditRedeemPointsValue()->SetFocus();

	return TRUE;
}

//$$******************************************************************
CEdit* CGroupPropertyPage2::GetEditLoyaltyTripCard() { return GetEdit(IDC_EDIT_LOYALTYTRIPCARD); }
CEdit* CGroupPropertyPage2::GetEditLoyaltyPoints() { return GetEdit(IDC_EDIT_LOYALTYPOINTS); }
CEdit* CGroupPropertyPage2::GetEditLoyaltyValue() { return GetEdit(IDC_EDIT_LOYALTYVALUE); }
CEdit* CGroupPropertyPage2::GetEditMultiplierTrigger() { return GetEdit(IDC_EDIT_MULTIPLIERTRIGGER); }
CEdit* CGroupPropertyPage2::GetEditUpperSpendLimit() { return GetEdit(IDC_EDIT_UPPERSPENDLIMIT); }
CEdit* CGroupPropertyPage2::GetEditRedeemPointsTrip() { return GetEdit(IDC_EDIT_REDEEMPOINTSTRIP); }
CEdit* CGroupPropertyPage2::GetEditRedeemPointsValue() { return GetEdit(IDC_EDIT_REDEEMPOINTSVALUE); }
//$$******************************************************************

BOOL CGroupPropertyPage2::OnKillActive()
{
	m_RowGroup.SetRewardFlag(IsTicked(m_checkLoyaltyGroup));
	m_RowGroup.SetRewardDisable(IsTicked(m_checkDisableLoyalty));
	m_RowGroup.SetRewardType(m_nLoyaltyType);
	m_RowGroup.SetRewardTrip(GetEditBoxDouble(*GetEditLoyaltyTripCard()));
	m_RowGroup.SetRewardPoints(GetEditBoxInt(*GetEditLoyaltyPoints()));
	m_RowGroup.SetRewardBonus(GetEditBoxDouble(*GetEditLoyaltyValue()));
	m_RowGroup.SetRewardTrigger(GetEditBoxDouble(*GetEditMultiplierTrigger()));
	m_RowGroup.SetRewardLimit(GetEditBoxDouble(*GetEditUpperSpendLimit()));
	/*****/
	m_RowGroup.SetRedeemFlag(IsTicked(m_checkGroupRedeem));
	m_RowGroup.SetRedeemDisable(IsTicked(m_checkDisableRedeem));
	m_RowGroup.SetRedeemTrip(GetEditBoxInt(*GetEditRedeemPointsTrip()));
	m_RowGroup.SetRedeemValue(GetEditBoxDouble(*GetEditRedeemPointsValue()));
	m_RowGroup.SetRedeemType(0);

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************

void CGroupPropertyPage2::OnToggleLoyaltyDisable()	
{ 
	OnToggleLoyaltyGroup(); 
}

//$$******************************************************************

void CGroupPropertyPage2::OnToggleLoyaltyGroup()
{
	BOOL bAllowed = IsTicked(m_checkLoyaltyGroup);
	m_checkDisableLoyalty.EnableWindow(bAllowed);

	if (bAllowed == TRUE)
	{
		bAllowed = (IsTicked(m_checkDisableLoyalty) == FALSE);
	}

	GetEditLoyaltyTripCard()->EnableWindow(bAllowed);
	m_radioLoyaltyPoints.EnableWindow(bAllowed);
	m_radioLoyaltyValue.EnableWindow(bAllowed);
	GetEditLoyaltyPoints()->EnableWindow(bAllowed);
	GetEditLoyaltyValue()->EnableWindow(bAllowed);
	GetEditUpperSpendLimit()->EnableWindow(bAllowed);
	GetEditMultiplierTrigger()->EnableWindow(bAllowed);

	if (bAllowed == TRUE)
	{
		SetLoyaltyDisplay();
		GetEditLoyaltyTripCard()->SetFocus();
	}
}

//$$******************************************************************

void CGroupPropertyPage2::OnToggleGroupRedeem()
{
	SetRedeemDisplay();
}

//$$******************************************************************

void CGroupPropertyPage2::OnToggleDisableRedeem()
{
	SetRedeemDisplay();
}

//$$******************************************************************

void CGroupPropertyPage2::SetRedeemDisplay()
{
	BOOL bAllowed = IsTicked(m_checkGroupRedeem);
	m_checkDisableRedeem.EnableWindow(bAllowed);

	if (IsTicked(m_checkDisableRedeem))
	{
		bAllowed = FALSE;
	}

	GetEditRedeemPointsValue()->EnableWindow(bAllowed);
	GetEditRedeemPointsTrip()->EnableWindow(bAllowed);
}

//$$******************************************************************

void CGroupPropertyPage2::OnRadioLoyaltyPoints()	
{ 
	m_nLoyaltyType = 0;
	SetLoyaltyDisplay(); 
}

//$$******************************************************************

void CGroupPropertyPage2::OnRadioLoyaltyValue()	
{ 
	m_nLoyaltyType = 1;
	SetLoyaltyDisplay(); 
}

//$$******************************************************************

void CGroupPropertyPage2::SetLoyaltyDisplay()
{
	if ((m_nLoyaltyType < 0) || (m_nLoyaltyType > 1))
	{
		m_nLoyaltyType = 0;
	}

	m_radioLoyaltyPoints.SetCheck(0 == m_nLoyaltyType);
	m_radioLoyaltyValue.SetCheck(1 == m_nLoyaltyType);

	GetEditLoyaltyPoints()->EnableWindow(0 == m_nLoyaltyType);
	GetEditLoyaltyValue()->EnableWindow(1 == m_nLoyaltyType);
	
	switch (m_nLoyaltyType)
	{
	case 0:
		GetEditLoyaltyPoints()->SetFocus();
		break;

	case 1:
		GetEditLoyaltyValue()->SetFocus();
		break;
	}
}

//*******************************************************************

void CGroupPropertyPage2::OnButtonDefaultSpend()
{
	Server.Read ( Filenames.GetServerDataFilename() );

	SetEditBoxDouble(*GetEditLoyaltyTripCard(), Server.GetRewardTrip(), 2);

	switch (Server.GetRewardType())
	{
	case 1:
		OnRadioLoyaltyValue();
		break;

	case 0:
	default:
		OnRadioLoyaltyPoints();
		break;
	}

	SetEditBoxInt(*GetEditLoyaltyPoints(), Server.GetRewardPoints());
	SetEditBoxDouble(*GetEditLoyaltyValue(), Server.GetRewardValue(),2);
	SetEditBoxDouble(*GetEditMultiplierTrigger(), Server.GetRewardMultiplierTrigger(),2);
	SetEditBoxDouble(*GetEditUpperSpendLimit(), Server.GetRewardUpperSpendLimit(), 2);

	OnToggleLoyaltyGroup();
}

//$$******************************************************************

void CGroupPropertyPage2::OnButtonDefaultRedeem()
{
	Server.Read(Filenames.GetServerDataFilename());

	SetEditBoxInt(*GetEditRedeemPointsTrip(), Server.GetRedeemPointsTrip());
	SetEditBoxDouble(*GetEditRedeemPointsValue(), Server.GetRedeemPointsValue(), 2);
}

//$$******************************************************************