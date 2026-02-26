//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "RedeemPointsDlg.h"
//$$******************************************************************

CRedeemPointsDlg ::CRedeemPointsDlg (CWnd* pParent )
	: CSSDialog(CRedeemPointsDlg ::IDD, pParent)
	, m_Comments(Filenames.GetAuditCommentsFilename())
{
	m_strComment = "";
	m_nRedeemPoints = 0;
	m_nMaxRedeemPoints = 0;
	m_dRedeemValue = 0.0;
	m_nTripFactor = 0;
	m_dTripValue = 0.0;
	m_bDisableRedeem = FALSE;
}

//$$******************************************************************

void CRedeemPointsDlg::DisplayNoShowReason( int nReason )
{
	CString strError = "Unable to process manual redeem.";

	switch( nReason )
	{
	case 1:
		strError += "\n\nNo cash redemption for family promotions.";
		break;

	case 2:
		strError += "\n\nRedemption trip has not been configured.";
		break;

	case 3:
		strError += "\n\nInsufficient points to redeem.";
		break;

	case 4:
		strError += "\n\nRedemption disabled for this group.";
		break;
	}

	Prompter.Error( strError, "Manual Redeem" );
}

//$$******************************************************************

CRedeemPointsDlg ::~CRedeemPointsDlg ()
{
}

//$$******************************************************************

void CRedeemPointsDlg ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TRIP, m_editTrip);
	DDX_Control(pDX, IDC_EDIT_AVAILABLE, m_editAvailable);
	DDX_Text(pDX, IDC_EDIT_POINTS, m_nRedeemPoints);
	DDX_Text(pDX, IDC_EDIT_POINTSVALUE, m_dRedeemValue);
	DDX_Control(pDX, IDC_COMBO_COMMENT, m_comboComments);
	DDX_CBString(pDX, IDC_COMBO_COMMENT, m_strComment);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CRedeemPointsDlg , CDialog)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnClickStaticComment)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_REDEEMPOINTS, OnSpinRedeemPoints)
	ON_EN_KILLFOCUS(IDC_EDIT_POINTS, OnKillFocusEditPoints)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CRedeemPointsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_nMaxRedeemPoints = m_nRedeemPoints;

	SubclassEdit(IDC_EDIT_POINTS, SS_NUM, 8, "%ld");
	SubclassEdit(IDC_EDIT_POINTSVALUE, SS_NUM_DP, 11, "%.2f");			// Purse1 or Purse3

	CString strTrip = "";
	strTrip.Format("%d pts => £%.2f", m_nTripFactor, m_dTripValue);
	m_editTrip.SetWindowText(strTrip);

	CString strAvailable = "";
	strAvailable.Format("%d", m_nMaxRedeemPoints);
	m_editAvailable.SetWindowText(strAvailable);

	AddComments();

	return TRUE;
}

//$$******************************************************************
CEdit* CRedeemPointsDlg::GetEditPoints() { return GetEdit(IDC_EDIT_POINTS); }
//$$******************************************************************

void CRedeemPointsDlg::AddComments()
{
	for (int i = 0; i < m_Comments.GetSize(); i++)
	{
		m_comboComments.AddString(m_Comments.GetAt(i));
	}
}

//$$******************************************************************

void CRedeemPointsDlg::OnClickStaticComment()
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

//$$******************************************************************

void CRedeemPointsDlg::OnSpinRedeemPoints(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if ( UpdateData() == TRUE )
	{
		if ( pNMUpDown->iDelta > 0 )			// previous / Down
		{	
			if (m_nRedeemPoints > 0)
			{
				m_nRedeemPoints -= m_nTripFactor;
			}

			int nOffset = ( m_nRedeemPoints % m_nTripFactor );
			
			if (nOffset != 0)
			{
				m_nRedeemPoints += (m_nTripFactor - nOffset);
			}
		}
		else									// next / Up
		{
			if (m_nRedeemPoints < m_nMaxRedeemPoints)
			{
				m_nRedeemPoints += m_nTripFactor;
			}

			int nOffset = ( m_nRedeemPoints % m_nTripFactor );
			
			if (nOffset != 0)
			{
				m_nRedeemPoints -= nOffset;
			}
		}	

		int nFactor = ( m_nRedeemPoints / m_nTripFactor ); 
		m_dRedeemValue = ( m_dTripValue * nFactor );

		UpdateData(FALSE);	
	}
	*pResult = 0;
}

//$$******************************************************************

void CRedeemPointsDlg::OnKillFocusEditPoints()
{
	UpdateData(TRUE);
	int nFactor = ( m_nRedeemPoints / m_nTripFactor ); 
	m_dRedeemValue = ( m_dTripValue * nFactor );
	UpdateData(FALSE);
}

//$$******************************************************************

void CRedeemPointsDlg::OnOK()
{
	OnKillFocusEditPoints();
	
	if ( ( ( m_nRedeemPoints % m_nTripFactor ) != 0 ) || ( m_nRedeemPoints > m_nMaxRedeemPoints ) )
	{
		CString strMsg = "";
		strMsg .Format( "You must select a number of points that is:\n\n(1) An exact multiple of the trip factor (%d)\n\n(2) Not more than %d points", m_nTripFactor, m_nMaxRedeemPoints );
		Prompter.Error( strMsg, "Entry Error" );

		CEdit* pWnd = GetEditPoints();
		pWnd -> SetFocus();
		pWnd -> SetSel(0,-1);
		return;
	}

	if ( UpdateData() == TRUE )
	{
		m_Comments.Save ( m_strComment );				// save comment if required
		EndDialog(IDOK);
	}
}

//$$******************************************************************
