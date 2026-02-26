/**********************************************************************/
#include "ColumnListDlg.h"
/**********************************************************************/

CColumnListDlg::CColumnListDlg( const char* szType, CArray<CColumnStatusInfo,CColumnStatusInfo>& arrayPos, CWnd* pParent)
: CSSDialog(CColumnListDlg::IDD, pParent), m_arrayPos( arrayPos )
{
	//{{AFX_DATA_INIT(CColumnListDlg)
	//}}AFX_DATA_INIT
	m_strType = szType;
	m_nProtectPos = 0;
}

/**********************************************************************/

void CColumnListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColumnListDlg)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CColumnListDlg, CDialog)
	//{{AFX_MSG_MAP(CColumnListDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CColumnListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle;
	strTitle.Format( "Select Column Order (%s)", (const char*) m_strType );
	SetWindowText( strTitle );

	m_listColumn.SubclassDlgItem ( IDC_LIST, this );
	m_listColumn.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listColumn.InsertColumn( 0, "", LVCFMT_LEFT, 260 );
	m_listColumn.SetLockItemZeroFlag( FALSE );

	m_nProtectPos = -1;
	for ( int n = 0; n < m_arrayPos.GetSize(); n++ )
	{
		CColumnStatusInfo infoStatus = m_arrayPos.GetAt(n);

		CSSListTaggedSelectItem item;
		item.m_strText = infoStatus.m_strHeaderText;
		item.m_nData = 0;
		item.SetTagState(0);
		
		if ( COLUMN_STATUS_LOCKED == infoStatus.m_nStatus )
		{
			item.SetTagState(1);
			item.m_bLocked = TRUE;
			m_nProtectPos = n;
		}
		else
		{
			item.SetTagState( COLUMN_STATUS_SHOWN == infoStatus.m_nStatus ? 1 : 0 );
			item.m_bLocked = FALSE;
		}

		m_listColumn.AddItemToList( item );
	}

	return TRUE;  
}

/**********************************************************************/

void CColumnListDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listColumn.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CColumnListDlg::OnButtonDown() 
{
	int nSel = m_listColumn.GetCurSel();
	if ( ( nSel < m_nProtectPos + 1 ) || ( nSel > m_listColumn.GetItemCount() - 2 ) )
		return;
	
	m_listColumn.SwapItems( nSel, nSel + 1 );
	m_listColumn.SetCurSel( nSel + 1 );

	CColumnStatusInfo info = m_arrayPos.GetAt( nSel );
	m_arrayPos.RemoveAt( nSel );
	m_arrayPos.InsertAt( nSel + 1, info );
}

/**********************************************************************/

void CColumnListDlg::OnButtonUp() 
{
	int nSel = m_listColumn.GetCurSel();
	if ( ( nSel < m_nProtectPos + 2 ) || ( nSel > m_listColumn.GetItemCount() - 1 ) )
		return;

	m_listColumn.SwapItems( nSel, nSel - 1 );
	m_listColumn.SetCurSel( nSel - 1 );

	CColumnStatusInfo info = m_arrayPos.GetAt( nSel );
	m_arrayPos.RemoveAt( nSel );
	m_arrayPos.InsertAt( nSel - 1, info );
}

/**********************************************************************/

void CColumnListDlg::OnOK() 
{
	for ( int n = 0; n < m_arrayPos.GetSize(); n++ )
	{
		if ( m_arrayPos[n].m_nStatus != COLUMN_STATUS_LOCKED )
		{
			if ( m_listColumn.GetItemTagState(n) != 0 )
				m_arrayPos[n].m_nStatus = COLUMN_STATUS_SHOWN;
			else
				m_arrayPos[n].m_nStatus = COLUMN_STATUS_HIDDEN;
		}
	}

	EndDialog( IDOK );
}

/**********************************************************************/
