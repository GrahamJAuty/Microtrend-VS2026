//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//$$******************************************************************
#include "GroupPropertyPage6.h"
//$$******************************************************************

CGroupPropertyPage6::CGroupPropertyPage6(CSQLRowGroupFull& RowGroup)
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CGroupPropertyPage6::IDD)
	, m_RowGroup(RowGroup)
{
	m_dRedeemValue = 0.0;
	m_nRedeemTrip = 0;
}

//$$******************************************************************

CGroupPropertyPage6::~CGroupPropertyPage6()
{
}

//$$******************************************************************

void CGroupPropertyPage6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_GROUPREDEEM, m_checkUseGroupRedeem);
	DDX_Control(pDX, IDC_CHECK_DISABLEREDEEM, m_checkDisableRedeem);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT1, m_comboAccount1Purse);
	DDX_Control(pDX, IDC_BUTTON_DEFAULT1, m_buttonDefault1);
	DDX_Text(pDX, IDC_EDIT_REDDEMPOINTSTRIP, m_nRedeemTrip);
	DDX_Text(pDX, IDC_EDIT_REDEEMVALUE, m_dRedeemValue);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CGroupPropertyPage6, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_GROUPREDEEM, OnToggleGroupRedeem)
	ON_BN_CLICKED(IDC_CHECK_DISABLEREDEEM, OnToggleDisableRedeem)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT1, OnButtonDefault1)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CGroupPropertyPage6::OnInitDialog()
{
	m_nRedeemTrip = m_RowGroup.GetRedeemTrip();
	m_dRedeemValue = m_RowGroup.GetRedeemValue();
	int nRedeemPurse = m_RowGroup.GetRedeemPurse();

	CSSPropertyPage::OnInitDialog();

	m_checkUseGroupRedeem.SetCheck(m_RowGroup.GetFileRedeemFlag());
	m_checkDisableRedeem.SetCheck(m_RowGroup.GetRedeemDisable());

	SubclassEdit(IDC_EDIT_REDDEMPOINTSTRIP, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REDEEMVALUE, SS_NUM_SDP, 11, "%.2f");

	int nCurSel = 0;
	m_comboAccount1Purse.AddString(Account::Purse1.Label);
	m_comboAccount1Purse.AddString(Account::Purse2.Label);

	if (nRedeemPurse == 2)
	{
		nCurSel = 1;
	}

	if (System.GetEnablePurse3Flag() == TRUE)
	{
		m_comboAccount1Purse.AddString(Account::Purse3.Label);

		if (nRedeemPurse == 3)
		{
			nCurSel = 2;
		}
	}
	m_comboAccount1Purse.SetCurSel(nCurSel);

	if (m_comboAccount1Purse.GetCount() < 2)
	{
		m_comboAccount1Purse.EnableWindow(FALSE);
	}
	SetDisplay();

	return TRUE;
}

//$$******************************************************************
CEdit* CGroupPropertyPage6::GetEditRedeemPointsTrip(){ return GetEdit( IDC_EDIT_REDDEMPOINTSTRIP ); }
CEdit* CGroupPropertyPage6::GetEditRedeemValue(){ return GetEdit( IDC_EDIT_REDEEMVALUE ); }
//$$******************************************************************

BOOL CGroupPropertyPage6::OnKillActive()
{
	if ( UpdateData() == TRUE )
	{
		int nRedeemPurse = 1;									// assume purse 1

		CString strPurse;
		m_comboAccount1Purse.GetLBText ( m_comboAccount1Purse.GetCurSel(), strPurse );
			 
		if (strPurse == "Purse2")
		{
			nRedeemPurse = 2;
		}
		else if (strPurse == "Purse3")
		{
			nRedeemPurse = 3;
		}

		m_RowGroup.SetFileRedeemFlag( m_checkUseGroupRedeem.GetCheck() != 0 );
		m_RowGroup.SetRedeemTrip( m_nRedeemTrip );
		m_RowGroup.SetRedeemValue( m_dRedeemValue );
		m_RowGroup.SetRedeemPurse( nRedeemPurse );
		m_RowGroup.SetRedeemDisable( m_checkDisableRedeem.GetCheck() != 0 );
	}

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************
void CGroupPropertyPage6::OnToggleGroupRedeem()		{ SetDisplay();	}
void CGroupPropertyPage6::OnToggleDisableRedeem()	{ SetDisplay(); }
//$$******************************************************************

void CGroupPropertyPage6::SetDisplay()
{
	if ( UpdateData() == TRUE )
	{
		BOOL bAllowed = ( m_checkUseGroupRedeem.GetCheck() != 0 );
		m_checkDisableRedeem.EnableWindow(bAllowed);

		if (m_checkDisableRedeem.GetCheck() != 0)
		{
			bAllowed = FALSE;
		}

		GetEditRedeemPointsTrip()->EnableWindow(bAllowed);
		GetEditRedeemValue()->EnableWindow(bAllowed);
		m_comboAccount1Purse.EnableWindow(bAllowed);
		m_buttonDefault1.EnableWindow(bAllowed);
	}
}

//$$******************************************************************

void CGroupPropertyPage6::OnButtonDefault1()
{
	if (UpdateData() == TRUE)
	{
		Server.Read(Filenames.GetServerDataFilename());

		m_nRedeemTrip = Server.GetFileRedeemPointsTrip();
		m_dRedeemValue = Server.GetRedeemValue();

		int nRedeemPurse = Server.GetRedeemPurse();
		int nCurSel = 0;
		if (nRedeemPurse == 2)
		{
			nCurSel = 1;
		}
		else if (System.GetEnablePurse3Flag() == TRUE && nRedeemPurse == 3)
		{
			nCurSel = 2;
		}

		m_comboAccount1Purse.SetCurSel(nCurSel);

		UpdateData(FALSE);
	}
}

//$$******************************************************************
