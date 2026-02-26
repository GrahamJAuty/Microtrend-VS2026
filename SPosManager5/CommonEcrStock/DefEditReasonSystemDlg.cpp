/**********************************************************************/
#include "DefEditReasonSystemDlg.h"
/**********************************************************************/

CDefEditReasonSystemDlg::CDefEditReasonSystemDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent /*=NULL*/)
	: CSSDialog(CDefEditReasonSystemDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	//{{AFX_DATA_INIT(CDefEditReasonSystemDlg)
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
}

/**********************************************************************/

void CDefEditReasonSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditReasonSystemDlg)
	DDX_Control(pDX, IDC_EDIT_TEXT, m_editText);
	DDX_Control(pDX, IDC_CHECK_MANUAL, m_checkManual);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditReasonSystemDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditReasonSystemDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditReasonSystemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_editText.LimitText( CascadingReason::ReasonText.Max );
	GetData();
	return TRUE;  
}

/**********************************************************************/

void CDefEditReasonSystemDlg::GetData()
{
	CCascadingReasonCSVRecord Reason;
	DataManagerNonDb.CCReasonSystem.GetAt(m_nIndex, Reason);

	CString strTitle;
	strTitle.Format("R%2.2d Reason Text %d", Reason.GetReasonNo(), Reason.GetReasonNo());
	SetWindowText(strTitle);

	m_editText.SetWindowText(Reason.GetLocalReasonText());
	m_checkManual.SetCheck(Reason.GetLocalManualEntryFlag());
}

/**********************************************************************/

void CDefEditReasonSystemDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();

	*pResult = 0;
}

/**********************************************************************/

void CDefEditReasonSystemDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditReasonSystemDlg::SpinPrevious()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex > 0 )
		{
			SaveData();
			m_nIndex--;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine ( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditReasonSystemDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < DataManagerNonDb.CCReasonSystem.GetSize() - 1 )
		{
			SaveData();
			m_nIndex++;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditReasonSystemDlg::SaveData()
{
	CCascadingReasonCSVRecord Reason;
	DataManagerNonDb.CCReasonSystem.GetAt(m_nIndex, Reason);

	Reason.SetLocalReasonText(GetEditBoxText(m_editText));
	Reason.SetLocalManualEntryFlag(IsTicked(m_checkManual));

	DataManagerNonDb.CCReasonSystem.SetAt(m_nIndex, Reason);

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditReasonSystemDlg::OnOK() 
{
	SaveData();
	EndDialog ( IDOK );
}

/**********************************************************************/
