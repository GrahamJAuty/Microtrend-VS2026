/**********************************************************************/
 
/**********************************************************************/
#include "SelectPresetActionDlg.h"
/**********************************************************************/

CSelectPresetActionDlg::CSelectPresetActionDlg( CArray<int,int>& arrayActions, int nOldEntityNo, int nNewEntityNo, CWnd* pParent /*=NULL*/)
	: CSSDialog(CSelectPresetActionDlg::IDD, pParent), m_arrayActions( arrayActions )
{
	//{{AFX_DATA_INIT(CSelectPresetActionDlg)
	//}}AFX_DATA_INIT
	m_nOldEntityNo = nOldEntityNo;
	m_nNewEntityNo = nNewEntityNo;
}

/**********************************************************************/

void CSelectPresetActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectPresetActionDlg)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSelectPresetActionDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectPresetActionDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSelectPresetActionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_listActions.SubclassDlgItem ( IDC_LIST, this );
	m_listActions.InsertColumn ( 0, "", LVCFMT_LEFT, 200 );
	m_listActions.ModifyStyle( LVS_OWNERDRAWFIXED, 0, 0 );
	m_listActions.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	FillListBox();

	m_listActions.SetCurSel(0);
	m_listActions.SetFocus();
	m_listActions.Invalidate();

	SetWindowText( "Presets" );
	
	return FALSE;  
}

/**********************************************************************/

void CSelectPresetActionDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )	
	{
		m_listActions.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CSelectPresetActionDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();
	*pResult = 0;
}

/**********************************************************************/

void CSelectPresetActionDlg::FillListBox()
{
	m_listActions.DeleteAllItems();

	CStringArray arrayItems;
	for ( int n = 0; n < m_arrayActions.GetSize(); n++ )
	{
		switch( m_arrayActions.GetAt(n) )
		{
		case 0:	
			{
				CString strItem;
				strItem.Format( "Create Preset #%d", m_nNewEntityNo );
				arrayItems.Add( strItem );
			}	
			break;

		case 1:	
			{
				CString strItem;
				strItem.Format( "Modify Preset #%d", m_nOldEntityNo );
				arrayItems.Add( strItem );
			}
			break;

		case 2:
			arrayItems.Add( "Open Preset List " );	
			break;
		}
	}
	
	for ( int n = 0; n < arrayItems.GetSize(); n++ )
	{
		m_listActions.InsertItem ( n, "" );
		m_listActions.SetItem ( n, 0, LVIF_TEXT, arrayItems.GetAt(n), 0, 0, 0, NULL );
	}
}

/**********************************************************************/

void CSelectPresetActionDlg::OnOK() 
{
	int nSel = m_listActions.GetCurSel();

	if ( ( nSel < 0 ) || ( nSel >= m_arrayActions.GetSize() ) )
		nSel = 0;

	m_nAction = m_arrayActions.GetAt(nSel);

	EndDialog ( IDOK );
}

/**********************************************************************/

void CSelectPresetActionDlg::OnCancel() 
{
	EndDialog ( IDCANCEL );
}

/**********************************************************************/
