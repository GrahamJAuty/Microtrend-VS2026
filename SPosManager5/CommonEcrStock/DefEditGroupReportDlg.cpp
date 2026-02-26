/**********************************************************************/
#include "GroupCSVArrayReport.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DefEditGroupReportDlg.h"
/**********************************************************************/

CDefEditGroupReportDlg::CDefEditGroupReportDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent)
	: CSSDialog(CDefEditGroupReportDlg::IDD, pParent),
	m_GroupSelectorConsol( m_editGroupConsol, m_comboGroupConsol, NODE_DEPT_GROUP_CONSOL ),
	m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	m_pListDataDlg = NULL;

	//{{AFX_DATA_INIT(CDefEditGroupReportDlg)
	m_strRepText = _T("");
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
}

/**********************************************************************/

void CDefEditGroupReportDlg::DoDataExchange(CDataExchange* pDX)
{
	int nMaxLen = 32;
	if ( nMaxLen < ReportGroup::RepText.Max ) nMaxLen = ReportGroup::RepText.Max;
	
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditGroupReportDlg)
	DDX_Control(pDX, IDC_EDIT_REPTEXT, m_editRepText);
	DDX_Control(pDX, IDC_EDIT_GROUP_CONSOL, m_editGroupConsol);
	DDX_Control(pDX, IDC_COMBO_GROUP_CONSOL, m_comboGroupConsol);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_REPTEXT, m_strRepText);
	DDV_MaxChars(pDX, m_strRepText, nMaxLen);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditGroupReportDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditGroupReportDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP_CONSOL, OnSelectGroupConsol)
	ON_EN_KILLFOCUS(IDC_EDIT_GROUP_CONSOL, OnKillFocusGroupNoConsol)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditGroupReportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetData();
	return TRUE;  
}

/**********************************************************************/

void CDefEditGroupReportDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/**********************************************************************/

void CDefEditGroupReportDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditGroupReportDlg::SpinPrevious()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex <= 1 )
			return;

		SaveData();
		m_nIndex--;
		GetData();

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine( m_nIndex );
	}
}

/**********************************************************************/

void CDefEditGroupReportDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < DataManager.ReportGroup.GetSize() - 1 )
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

void CDefEditGroupReportDlg::GetData()
{
	CString strTitle;

	CGroupCSVRecordReport GroupRecord;
	DataManager.ReportGroup.GetAt ( m_nIndex, GroupRecord );

	m_editRepText.SetWindowText ( GroupRecord.GetRepText() );
	strTitle.Format ( "G%2.2d %s", GroupRecord.GetGroupNo(), GroupRecord.GetDefaultText() );
	SetWindowText ( strTitle );

	m_GroupSelectorConsol.FillGroupCombo ( GroupRecord.GetParentGroupNo() );

	m_editRepText.SetSel ( 0, -1 );
	m_editRepText.SetFocus();
}

/**********************************************************************/

void CDefEditGroupReportDlg::SaveData()
{
	CGroupCSVRecordReport GroupRecord;
	DataManager.ReportGroup.GetAt ( m_nIndex, GroupRecord );
	GroupRecord.SetRepText ( m_strRepText );
	GroupRecord.SetParentGroupNo( m_GroupSelectorConsol.GetGroupFilter() );
	DataManager.ReportGroup.SetAt ( m_nIndex, GroupRecord );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditGroupReportDlg::OnSelectGroupConsol() 
{
	m_GroupSelectorConsol.SelectGroupFromCombo();
}

/**********************************************************************/

void CDefEditGroupReportDlg::OnKillFocusGroupNoConsol() 
{
	m_GroupSelectorConsol.SelectGroupFromEditBox();
}

/**********************************************************************/

void CDefEditGroupReportDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
