//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "ServerPropertyPage6.h"
//$$******************************************************************

CServerPropertyPage6::CServerPropertyPage6()
	: CSSPropertyPage(CServerPropertyPage6::IDD)
{
	m_nRedeemPointsTrip = 0;
	m_dRedeemPointsValue = 0.0;
}

//$$******************************************************************

void CServerPropertyPage6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REDEEMPOINTSTRIP, m_nRedeemPointsTrip);
	DDX_Text(pDX, IDC_EDIT_REDEEMPOINTSVALUE, m_dRedeemPointsValue);
	DDX_Control(pDX, IDC_EDIT_COMMENT, m_editComment);
	DDX_Control(pDX, IDC_CHECK_DISABLEREDEEM, m_checkDisableAutoRedeem);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CServerPropertyPage6, CPropertyPage)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CServerPropertyPage6::OnInitDialog()
{
	m_nRedeemPointsTrip				= Server.GetRedeemPointsTrip();
	m_dRedeemPointsValue			= Server.GetRedeemPointsValue();
	
	CPropertyPage::OnInitDialog();

	SubclassEdit ( IDC_EDIT_REDEEMPOINTSTRIP,	SS_NUM_S,	 8, "%ld" );
	SubclassEdit ( IDC_EDIT_REDEEMPOINTSVALUE,	SS_NUM_SDP, 11, "%.2f" );

	m_editComment.SetWindowText( Server.GetRedeemComment() );
	m_checkDisableAutoRedeem.SetCheck( Server.GetRedeemDisableAutoFlag() );
	
	return TRUE;
}

//$$******************************************************************
CEdit* CServerPropertyPage6::GetEditRedeemPointsValue(){ return GetEdit( IDC_EDIT_REDEEMPOINTSVALUE ); }
//$$******************************************************************

BOOL CServerPropertyPage6::OnKillActive()
{
	if ( UpdateData() == TRUE )
	{
		CString strComment;
		m_editComment.GetWindowText( strComment );

		Server.SetRedeemPointsTrip		( m_nRedeemPointsTrip );
		Server.SetRedeemPointsValue		( m_dRedeemPointsValue );
		Server.SetRedeemComment			( strComment );
		Server.SetRedeemDisableAutoFlag	( m_checkDisableAutoRedeem.GetCheck() != 0 );
	}

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************

