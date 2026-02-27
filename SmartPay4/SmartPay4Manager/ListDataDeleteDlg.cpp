/**********************************************************************/
#include "ListDataDeleteDlg.h"
/**********************************************************************/

CListDataDeleteDlg::CListDataDeleteDlg( CSingleListContentHelpers* pContentHelpers, CSingleListDisplayHelpers* pDisplayHelpers, CWnd* pParent) : 
	CSSAutoShutdownDialog(AutoShutdownHelper, CListDataDeleteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListDataDeleteDlg)
	//}}AFX_DATA_INIT
	m_bEnableColour = FALSE;
	m_pContentHelpers = pContentHelpers;
	m_pDisplayHelpers = pDisplayHelpers;
}

/**********************************************************************/

void CListDataDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListDataDeleteDlg)
	DDX_Control( pDX, IDC_STATIC_PROMPT, m_staticPrompt );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CListDataDeleteDlg, CSSAutoShutdownDialog)
	//{{AFX_MSG_MAP(CListDataDeleteDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillFocusList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetFocusList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CListDataDeleteDlg::CSSAutoShutdownPostInitDialog()
{
	{
		CString strTitle = "Delete ";
		
		if ( m_pDisplayHelpers -> GetSelectedArraySize() > 1 )
			strTitle += m_pDisplayHelpers -> GetTopPluralText();
		else
			strTitle += m_pDisplayHelpers -> GetTopSingleText();
		
		SetWindowText( strTitle );
	}
	
	{
		CString strPrompt = "Are you sure you wish to delete ";

		if ( m_pDisplayHelpers -> GetSelectedArraySize() > 1 )
		{
			strPrompt += "these ";
			strPrompt += m_pDisplayHelpers -> GetBottomPluralText();
		}
		else
		{
			strPrompt += "this ";
			strPrompt += m_pDisplayHelpers -> GetBottomSingleText();
		}

		strPrompt += " ?";
		m_staticPrompt.SetWindowText( strPrompt );
	}

	m_pContentHelpers -> SetColumns( m_listData, this );
	m_pDisplayHelpers -> SetDeletionList( &m_listData );
	m_pDisplayHelpers -> PrepareDeletionList( m_bEnableColour );
	return FALSE;
}

/**********************************************************************/

int CListDataDeleteDlg::GetArraySize()
{
	return m_pDisplayHelpers -> GetSelectedArraySize();
}

/**********************************************************************/

void CListDataDeleteDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
      lpMeasureItemStruct->itemHeight += 10;
}

/**********************************************************************/

void CListDataDeleteDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nListIndex = pDispInfo-> item.iItem;

		if ( ( nListIndex >= 0 ) && ( nListIndex < GetArraySize() ) )
		{
			CString strDisplayLine;
			m_pContentHelpers -> PrepareDeletionLine( m_pDisplayHelpers -> GetSelectedLine( nListIndex ), strDisplayLine );

			CCSV csv ( strDisplayLine, '\t' );
			lstrcpy ( pDispInfo -> item.pszText, csv.GetString ( pDispInfo -> item.iSubItem ) );

			if ( TRUE == m_bEnableColour )
				m_listData.SetColour( nListIndex, m_pContentHelpers -> GetListLineColour() );
		}
	}
	*pResult = 0;
}

/**********************************************************************/

void CListDataDeleteDlg::OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( IDCANCEL );
	*pResult = 0;
}

/**********************************************************************/

void CListDataDeleteDlg::OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	SetDefID ( IDCANCEL );
	*pResult = 0;
}

/**********************************************************************/

void CListDataDeleteDlg::OnOK() 
{
	EndDialog ( IDOK );
}

/**********************************************************************/

void CListDataDeleteDlg::OnCancel() 
{
	EndDialog ( IDCANCEL );
}

/**********************************************************************/

