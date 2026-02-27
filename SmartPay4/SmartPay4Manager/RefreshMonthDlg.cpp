// RefreshMonthDlg.cpp : implementation file
//

#include "RefreshMonthDlg.h"


// CRefreshMonthDlg dialog

CRefreshMonthDlg::CRefreshMonthDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CRefreshMonthDlg::IDD, pParent)
	, m_nDayNo(1)
{

}

CRefreshMonthDlg::~CRefreshMonthDlg()
{
}

void CRefreshMonthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MONTHDAY, m_nDayNo);
	DDV_MinMaxInt(pDX, m_nDayNo, 1, 31);
}


BEGIN_MESSAGE_MAP(CRefreshMonthDlg, CSSAutoShutdownDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DAYNO, OnDeltaposSpinDayno)
END_MESSAGE_MAP()


// CRefreshMonthDlg message handlers

void CRefreshMonthDlg::OnDeltaposSpinDayno(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if ( UpdateData() == TRUE )
	{
		if ( pNMUpDown->iDelta > 0 )			// previous / Down
		{	
			if ( m_nDayNo > 1 )
				--m_nDayNo;	
		}
		else									// next / Up
		{
			if ( m_nDayNo < 31 )
				++m_nDayNo;
		}	

		UpdateData(FALSE);	
	}
	
	*pResult = 0;
}
