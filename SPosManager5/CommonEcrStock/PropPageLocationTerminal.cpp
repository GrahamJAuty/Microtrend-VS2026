/**********************************************************************/
#include "LocationMasterDlg.h"
#include "LocationMasterMap.h"
#include "SysInfo.h"
#include "TerminalRangeDlg.h"
/**********************************************************************/
#include "PropPageLocationTerminal.h"
/**********************************************************************/

CPropPageLocationTerminal::CPropPageLocationTerminal() : CSSPropertyPage(CPropPageLocationTerminal::IDD)
{
	//{{AFX_DATA_INIT(CPropPageLocationTerminal)
	//}}AFX_DATA_INIT
	m_nLocIdx = 0;
	m_pLocationBuffer = NULL;
	m_nColumnMode = 0;
}

/**********************************************************************/

CPropPageLocationTerminal::~CPropPageLocationTerminal()
{
}

/**********************************************************************/

void CPropPageLocationTerminal::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLocationTerminal)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_STATIC_TERMINALS, m_staticTerminals );
	DDX_Control( pDX, IDC_BUTTON_EDIT, m_buttonEdit );
	DDX_Control( pDX, IDC_BUTTON_ADD, m_buttonAdd );
	DDX_Control( pDX, IDC_BUTTON_REMOVE, m_buttonRemove );
	DDX_Control( pDX, IDC_BUTTON_TOGGLE, m_buttonToggle );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageLocationTerminal, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageLocationTerminal)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON_ADD, OnButtonAdd )
	ON_BN_CLICKED( IDC_BUTTON_REMOVE, OnButtonRemove )
	ON_BN_CLICKED( IDC_BUTTON_EDIT, OnButtonEdit )
	ON_BN_CLICKED( IDC_BUTTON_TOGGLE, OnButtonToggle )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageLocationTerminal::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_listTerminals.SubclassDlgItem ( IDC_LIST, this );
	m_listTerminals.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listTerminals.InsertColumn ( 1, "Terminal", LVCFMT_LEFT, 80 );
	m_listTerminals.InsertColumn ( 2, "Report Name", LVCFMT_LEFT, 150 );
	m_listTerminals.InsertColumn ( 3, "Sales Collection Path", LVCFMT_LEFT, 260 );
	m_listTerminals.SetItemCountEx ( m_pTNoNewArray -> GetSize() );
	m_listTerminals.SetCurSel(0);

	UpdateTerminalCount();

	return TRUE;  
}

/**********************************************************************/

void CPropPageLocationTerminal::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listTerminals.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

void CPropPageLocationTerminal::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButtonEdit();
	*pResult = 0;
}

/**********************************************************************/

void CPropPageLocationTerminal::UpdateTerminalCount()
{
	int nCount = m_pTNoNewArray->GetSize();

	if (0 == nCount)
	{
		m_staticTerminals.SetWindowText("");
	}
	else if (1 == nCount)
	{
		m_staticTerminals.SetWindowText("( 1 terminal )");
	}
	else
	{
		CString str = "";
		str.Format("( %d terminals )", nCount);
		m_staticTerminals.SetWindowText(str);
	}
}

/**********************************************************************/

void CPropPageLocationTerminal::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLine( nIndex );
		CCSV csv ( m_strDisplayLine, '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

bool CPropPageLocationTerminal::PrepareLine( int nIndex )
{
	if ( nIndex < 0 || nIndex >= m_pTNoNewArray -> GetSize() )
		return FALSE;

	CSortedIntItem item;
	m_pTNoNewArray -> GetAt ( nIndex, item );

	if ( 0 == m_nColumnMode )
	{
		m_strDisplayLine.Format ( "TNo %d\t%s\t%s", 
			item.m_nItem,
			LocationMasterMap.GetTerminalName( m_nLocIdx, item.m_nItem ),
			LocationMasterMap.GetCashRSPFolder( m_nLocIdx, item.m_nItem ) );
	}
	else
	{
		m_strDisplayLine.Format ( "TNo %d\t%s\t%s", 
			item.m_nItem,
			LocationMasterMap.GetMasterName( m_nLocIdx, item.m_nItem ),
			LocationMasterMap.GetDownloadFolder( m_nLocIdx, item.m_nItem, FALSE ) );
	}

	return TRUE;
}
			
/**********************************************************************/

BOOL CPropPageLocationTerminal::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageLocationTerminal::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageLocationTerminal::OnButtonAdd()
{
	HandleAddTerminals();
}

/**********************************************************************/

void CPropPageLocationTerminal::HandleAddTerminals()
{
	CTerminalRangeDlg dlg(FALSE, this);
	dlg.m_nFrom = 1;
	dlg.m_nTo = 1;

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	if (dlg.m_nFrom > dlg.m_nTo)
	{
		return;
	}

	if ((dlg.m_nFrom < 1) || (dlg.m_nTo > 999))
	{
		return;
	}

	int nNewSize = m_pTNoNewArray->GetSize();

	for (int nTNo = dlg.m_nFrom; nTNo <= dlg.m_nTo; nTNo++)
	{
		CSortedIntItem item;
		item.m_nItem = nTNo;

		int nPos;
		if (m_pTNoNewArray->Find(item, nPos) == FALSE)
		{
			nNewSize++;
		}
	}

	if (nNewSize > MAX_TERMINALS)
	{
		CString str = "";
		str.Format("You can only add a maximum of %d terminals to each location.",
			MAX_TERMINALS);

		Prompter.Error(str);
		return;
	}

	for (int nTNo = dlg.m_nFrom; nTNo <= dlg.m_nTo; nTNo++)
	{
		CSortedIntItem item;
		item.m_nItem = nTNo;

		int nPos = 0;
		if (m_pTNoNewArray->Find(item, nPos) == FALSE)
		{
			m_pTNoNewArray->Consolidate(item);
		}
	}

	m_listTerminals.SetItemCountEx(m_pTNoNewArray->GetSize());

	CSortedIntItem item;
	item.m_nItem = dlg.m_nFrom;

	int nPos = 0;
	if (m_pTNoNewArray->Find(item, nPos) == TRUE)
	{
		m_listTerminals.SetCurSel(nPos);
	}

	m_listTerminals.Invalidate();
	UpdateTerminalCount();
}

/**********************************************************************/

void CPropPageLocationTerminal::OnButtonRemove()
{
	int nSel = m_listTerminals.GetCurSel();
	if ( ( nSel < 0 ) || ( nSel >= m_pTNoNewArray -> GetSize() ) )
		return;

	CSortedIntItem item;
	m_pTNoNewArray -> GetAt( nSel, item );
	int nTNo = item.m_nItem;

	CTerminalRangeDlg dlg( TRUE, this );
	dlg.m_nFrom = nTNo;
	dlg.m_nTo = nTNo;

	if ( dlg.DoModal() != IDOK )
		return;

	if ( dlg.m_nFrom > dlg.m_nTo )
		return;

	if ( ( dlg.m_nFrom < 1 ) || ( dlg.m_nTo > 999 ) )
		return;

	int nTNoFrom = dlg.m_nFrom;
	int nTNoTo = dlg.m_nTo;

	CSortedIntItem itemFrom;
	itemFrom.m_nItem = nTNoFrom;

	int nPos;
	m_pTNoNewArray -> Find( itemFrom, nPos );

	while( nPos < m_pTNoNewArray -> GetSize() )
	{
		CSortedIntItem item;
		m_pTNoNewArray -> GetAt(nPos, item);
		int nTNo = item.m_nItem; 
		
		if ( ( nTNo < nTNoFrom ) || ( nTNo > nTNoTo ) )
			break;

		CLocationMasterInfo infoMaster;
		infoMaster.SetLocNo( dbLocation.GetLocNo( m_nLocIdx ) );
		infoMaster.SetTerminalNo( nTNo );
		LocationMasterMap.RemoveMaster( infoMaster );

		m_pTNoNewArray -> RemoveAt( nPos );
	}

	m_listTerminals.SetItemCountEx( m_pTNoNewArray -> GetSize() );
	m_listTerminals.SetCurSel( nPos );
	m_listTerminals.Invalidate();
	UpdateTerminalCount();
}

/**********************************************************************/

void CPropPageLocationTerminal::OnButtonEdit()
{
	int nSel = m_listTerminals.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_pTNoNewArray -> GetSize() ) )
	{
		CSortedIntItem item;
		m_pTNoNewArray -> GetAt( nSel, item );
		CLocationMasterDlg dlgMaster( m_nLocIdx, item.m_nItem, FALSE, this );
		dlgMaster.DoModal();
		m_listTerminals.Invalidate();
	}
}

/**********************************************************************/

void CPropPageLocationTerminal::OnButtonToggle()
{
	m_nColumnMode = ( m_nColumnMode == 0 ) ? 1 : 0;

	{
		LVCOLUMN col;
		col.mask = LVCF_TEXT;
        char szColText[64];
        strcpy_s(szColText, (0 == m_nColumnMode) ? "Report Name" : "Master Name");
        col.pszText = szColText;
        m_listTerminals.SetColumn(1, &col);
	}

	{
		LVCOLUMN col;
		col.mask = LVCF_TEXT;
		char szColText[64];
		strcpy_s(szColText, (0 == m_nColumnMode) ? "Sales Collection Path" : "Database Update Path");
		col.pszText = szColText;
		m_listTerminals.SetColumn( 2, &col );
	}

	m_listTerminals.Invalidate();
}

/**********************************************************************/

bool CPropPageLocationTerminal::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageLocationTerminal::Refresh()
{
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPropPageLocationTerminal::GetRecordData()
{
}

/**********************************************************************/

void CPropPageLocationTerminal::SaveRecord()
{
}

/**********************************************************************/
