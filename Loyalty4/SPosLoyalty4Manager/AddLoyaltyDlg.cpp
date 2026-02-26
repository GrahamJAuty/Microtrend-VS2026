//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//*******************************************************************
#include "GroupComboHelpers.h"
#include "AddLoyaltyDlg.h"
//*******************************************************************

CAddLoyaltyDlg::CAddLoyaltyDlg(int nGroupNo, CWnd* pParent)
	: CSSDialog(CAddLoyaltyDlg::IDD, pParent),
	m_Comments(Filenames.GetAuditCommentsFilename())
{
	m_dSpendToDate = 0.0;
	m_dCurrentPurse = 0.0;
	m_nCurrentPoints = 0;
	m_dNewPurseSpend = 0.0;
	m_dNewCashSpend = 0.0;
	m_dNewCashReward = 0.0;
	m_nNewPointsReward = 0;
	m_bAllowPurseSpend = FALSE;
	m_bAllowCashSpend = FALSE;
	m_nNoShowReason = 1;
	m_strLabelPurseSpend = "Spend ";

	Server.Read(Filenames.GetServerDataFilename());
	m_dLoyaltyTripValue = Server.GetRewardTrip();
	m_nLoyaltyType = Server.GetRewardType();
	m_nPointsRewardPerTrip = Server.GetRewardPoints();
	m_dCashRewardPerTrip = Server.GetRewardValue();

	CSQLRowGroup RowGroup;
	RowGroup.SetGroupNo(nGroupNo);

	CSQLRepositoryGroup repoGroup;
	if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if ( RowGroup.GetRewardFlag() == TRUE )
		{
			m_dLoyaltyTripValue = RowGroup.GetRewardTrip();
			m_nLoyaltyType = RowGroup.GetRewardType();
			m_nPointsRewardPerTrip = RowGroup.GetRewardPoints();
			m_dCashRewardPerTrip = RowGroup.GetRewardBonus();
		}
	}

	if (Server.GetRewardPurse1SpendFlag() == TRUE)
	{
		m_strLabelPurseSpend += "Purse1";
		m_bAllowPurseSpend = TRUE;
	}

	if (Server.GetRewardPurse2SpendFlag() == TRUE)
	{
		if (TRUE == m_bAllowPurseSpend)
		{
			m_strLabelPurseSpend += " + ";
		}

		m_strLabelPurseSpend += "Purse2";
		m_bAllowPurseSpend = TRUE;
	}

	m_bAllowCashSpend = Server.GetRewardCashSpendFlag();

	m_nNoShowReason = 0;
	if (FALSE == (m_bAllowPurseSpend || m_bAllowCashSpend))
	{
		m_nNoShowReason = 1;
	}
	else if (::CompareDoubles(m_dLoyaltyTripValue, 0.0, 3) != 1)
	{
		m_nNoShowReason = 2;
	}
}

//*******************************************************************

void CAddLoyaltyDlg::DisplayNoShowReason()
{
	CString strError = "Unable to enter manual spend bonus.";

	switch( m_nNoShowReason )
	{
	case 1:
		strError += "\n\nBonus is disabled for all spend types.";
		break;

	case 2:
		strError += "\n\nBonus trip has not been configured.";
		break;
	}

	Prompter.Error( strError, "Manual Spend Bonus" );
}

//*******************************************************************

void CAddLoyaltyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SPENDTD, m_editSpendTD);
	DDX_Control(pDX, IDC_STATIC_CURRENT, m_staticCurrent);
	DDX_Control(pDX, IDC_EDIT_CURRENT, m_editCurrent);
	DDX_Control(pDX, IDC_EDIT_TRIP, m_editTrip);
	DDX_Control(pDX, IDC_STATIC_PURSESPEND, m_staticPurseSpend);
	DDX_Control(pDX, IDC_COMBO_COMMENT, m_comboComments);
	DDX_Text(pDX, IDC_EDIT_PURSESPEND, m_dNewPurseSpend);
	DDX_Text(pDX, IDC_EDIT_CASHSPEND, m_dNewCashSpend);
	DDX_Control(pDX, IDC_EDIT_REWARD, m_editReward);
	DDX_CBString(pDX, IDC_COMBO_COMMENT, m_strComment);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CAddLoyaltyDlg, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT_PURSESPEND, OnKillFocusSpend)
	ON_EN_KILLFOCUS(IDC_EDIT_CASHSPEND, OnKillFocusSpend)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnClickStaticComment)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CAddLoyaltyDlg::OnInitDialog()
{
	CSSDialog::OnInitDialog();

	SubclassEdit ( IDC_EDIT_PURSESPEND, SS_NUM_SDP, 11, "%.2f" ); 
	SubclassEdit ( IDC_EDIT_CASHSPEND, SS_NUM_SDP, 11, "%.2f" ); 

	{
		CString str;
		str.Format( "%.2f", m_dSpendToDate );
		m_editSpendTD.SetWindowText( str );
	}

	{
		CString strLabel, strValue, strTrip;
		
		if ( m_nLoyaltyType == 0 )
		{
			strLabel = "Points balance";
			strValue.Format( "%d", m_nCurrentPoints );
			strTrip.Format ( "£%.2f => %d pts", m_dLoyaltyTripValue, m_nPointsRewardPerTrip );
			m_editReward.SetWindowText("0");
		}
		else
		{
			strLabel = "Purse balance";
			strValue.Format( "%.2f", m_dCurrentPurse );
			strTrip.Format ( "£%.2f => £%.2f", m_dLoyaltyTripValue, m_dCashRewardPerTrip );
			m_editReward.SetWindowText("0.00");
		}

		m_staticCurrent.SetWindowText( strLabel );
		m_editCurrent.SetWindowText( strValue );
		m_editTrip.SetWindowText( strTrip );
	}

	m_staticPurseSpend.SetWindowText( m_strLabelPurseSpend );
	GetEditPurseSpend() -> EnableWindow( m_bAllowPurseSpend );
	GetEditCashSpend() -> EnableWindow( m_bAllowCashSpend );

	AddComments();

	return TRUE;
}

//*******************************************************************
CEdit* CAddLoyaltyDlg::GetEditPurseSpend(){ return GetEdit( IDC_EDIT_PURSESPEND ); }
CEdit* CAddLoyaltyDlg::GetEditCashSpend(){ return GetEdit( IDC_EDIT_CASHSPEND ); }
//*******************************************************************

void CAddLoyaltyDlg::AddComments()
{
	for (int i = 0; i < m_Comments.GetSize(); i++)
	{
		m_comboComments.AddString(m_Comments.GetAt(i));
	}
}

//*******************************************************************

void CAddLoyaltyDlg::OnKillFocusSpend()
{
	if ( UpdateData() == TRUE )
	{
		if ( m_nLoyaltyType == 0 )
		{
			m_nNewPointsReward = CalculateLoyaltyPoints();
			
			CString str;
			str.Format( "%d", m_nNewPointsReward );
			m_editReward.SetWindowText( str );
		}
		else
		{
			m_dNewCashReward = CalculateLoyaltyValue();
			
			CString str;
			str.Format( "%.2f", m_dNewCashReward );
			m_editReward.SetWindowText( str );
		}

		UpdateData(FALSE);
	}
}

//*******************************************************************

int CAddLoyaltyDlg::CalculateLoyaltyPoints()
{
	int nPoints = 0;

	double dTotalSpend = m_dNewPurseSpend + m_dNewCashSpend;

	int nFactor = (int)( dTotalSpend / m_dLoyaltyTripValue ); 
	nPoints += m_nPointsRewardPerTrip * nFactor;

	return nPoints;
}

//*******************************************************************

double CAddLoyaltyDlg::CalculateLoyaltyValue()
{
	double dValue = 0.0;

	double dTotalSpend = m_dNewPurseSpend + m_dNewCashSpend;

	int nFactor = (int)( dTotalSpend / m_dLoyaltyTripValue ); 
	dValue += m_dCashRewardPerTrip * (double)nFactor;

	return dValue;
}

//*******************************************************************

void CAddLoyaltyDlg::OnClickStaticComment()
{
	if ( UpdateData() == TRUE )
	{
		if ( m_strComment != "" )
		{
			m_Comments.Delete ( m_strComment );			// delete comment
			m_comboComments.ResetContent();				// new list
			AddComments();

			m_strComment = "";
			UpdateData(FALSE);
		}
	}
}

//*******************************************************************

void CAddLoyaltyDlg::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		m_Comments.Save ( m_strComment );				// save comment if required
		EndDialog( IDOK );
	}
}

//*******************************************************************
