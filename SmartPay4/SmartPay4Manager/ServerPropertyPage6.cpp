//$$******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "ServerPropertyPage6.h"
//$$******************************************************************

CServerPropertyPage6::CServerPropertyPage6()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CServerPropertyPage6::IDD)
{
}

//$$******************************************************************

void CServerPropertyPage6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PURSE, m_comboPurse);
	DDX_Control(pDX, IDC_CHECK_REDEEMDISABLEAUTO, m_checkRedeemDisableAuto);
	DDX_Control(pDX, IDC_EDIT_COMMENT, m_editRedeemComment);
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CServerPropertyPage6, CSSAutoShutdownPropertyPage)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CServerPropertyPage6::OnInitDialog()
{
	int nRedeemPurse = Server.GetRedeemPurse();

	CPropertyPage::OnInitDialog();

	m_checkRedeemDisableAuto.SetCheck(Server.GetRedeemDisableAutoFlag());
	m_editRedeemComment.SetWindowText(Server.GetRedeemComment());

	SubclassEdit(IDC_EDIT_REDDEMPOINTSTRIP, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REDEEMVALUE, SS_NUM_SDP, 11, "%.2f");

	SetEditBoxDouble(*GetEditRedeemValue(), Server.GetRedeemValue(), 2);
	SetEditBoxInt(*GetEditRedeemPointsTrip(), Server.GetFileRedeemPointsTrip());

	int nCurSel = 0;
	m_comboPurse.AddString(Account::Purse1.Label);

	if (System.GetEnablePurse3Flag() == TRUE)
	{
		m_comboPurse.AddString(Account::Purse3.Label);

		if (nRedeemPurse == 3)
		{
			nCurSel = 1;
		}
	}
	m_comboPurse.SetCurSel(nCurSel);

	if (m_comboPurse.GetCount() < 2)
	{
		m_comboPurse.EnableWindow(FALSE);
	}

	return TRUE;
}

//$$******************************************************************

CEdit* CServerPropertyPage6::GetEditRedeemPointsTrip()
{
	return GetEdit(IDC_EDIT_REDDEMPOINTSTRIP);
}

//$$******************************************************************

CEdit* CServerPropertyPage6::GetEditRedeemValue()
{
	return GetEdit(IDC_EDIT_REDEEMVALUE);
}

//$$******************************************************************

BOOL CServerPropertyPage6::OnKillActive()
{
	Server.SetFileRedeemPointsTrip(GetEditBoxInt(*GetEditRedeemPointsTrip()));
	Server.SetRedeemValue(GetEditBoxDouble(*GetEditRedeemValue()));
	Server.SetRedeemPurse((m_comboPurse.GetCurSel() == 1) ? 3 : 1);
	Server.SetRedeemDisableAutoFlag(IsTicked(m_checkRedeemDisableAuto));
	Server.SetRedeemComment(GetEditBoxText(m_editRedeemComment));

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************
