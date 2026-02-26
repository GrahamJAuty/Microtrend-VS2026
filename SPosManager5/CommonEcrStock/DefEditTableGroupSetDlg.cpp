/**********************************************************************/
#include "TableGroupCSVArray.h"
/**********************************************************************/
#include "DefEditTableGroupSetDlg.h"
/**********************************************************************/

CDefEditTableGroupSetDlg::CDefEditTableGroupSetDlg( int nIndex, int nLevel, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent /*=NULL*/)
	: CSSDialog(CDefEditTableGroupSetDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	//{{AFX_DATA_INIT(CDefEditTableGroupSetDlg)
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_nLevel = nLevel;
}

/**********************************************************************/

void CDefEditTableGroupSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditTableGroupSetDlg)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_FILTER, m_checkFilter);
	DDX_Control(pDX, IDC_CHECK_UNKNOWN, m_checkUnknown);
	DDX_Control(pDX, IDC_CHECK_NONTABLE, m_checkNonTable);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditTableGroupSetDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditTableGroupSetDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditTableGroupSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_editName.LimitText( SPosTableGroupSet::GroupSetName.Max );

	m_listGroups.SubclassDlgItem ( IDC_LIST, this );
	m_listGroups.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listGroups.InsertColumn ( 0, "Groups", LVCFMT_LEFT, 250 );
	m_listGroups.SetLockItemZeroFlag( FALSE );

	for ( int nIndex = 0; nIndex < MAX_TABLEGROUP; nIndex++ )
	{		
		CTableGroupCSVRecord TableGroup;
		DataManagerNonDb.TableGroups.GetAt( nIndex, TableGroup );
		
		CSSListTaggedSelectItem item;	
		item.m_strText = TableGroup.GetGroupName(m_nLevel);
		item.m_nData = 0;
		item.SetTagState(0);

		m_listGroups.AddItemToList( item );
	}
	
	m_listGroups.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	m_listGroups.SetSelectionMark(0);	
	
	GetData();
	return TRUE;  
}

/**********************************************************************/

void CDefEditTableGroupSetDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listGroups.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CDefEditTableGroupSetDlg::GetData()
{
	CTableGroupSetCSVRecord TableGroupSet;
	DataManagerNonDb.TableGroupSets.GetAt( m_nIndex, TableGroupSet );

	CString strName = TableGroupSet.GetGroupSetName(m_nLevel);

	CString strTitle;
	strTitle.Format( "Edit Table Group Set (%s)",
		(const char*) strName );
	SetWindowText( strTitle );

	m_editName.SetWindowText( strName );
	m_checkFilter.SetCheck( TableGroupSet.GetShowAsFilterFlag(m_nLevel) );
	m_checkUnknown.SetCheck( TableGroupSet.GetIncludeUnknownFlag(m_nLevel) );
	m_checkNonTable.SetCheck( TableGroupSet.GetIncludeNonTableFlag(m_nLevel) );

	int nMask = 1;
	int nGroups = TableGroupSet.GetGroupMask(m_nLevel);
	for ( int nIndex = 0; nIndex < MAX_TABLEGROUP; nIndex++ )
	{		
		m_listGroups.SetItemTagState( nIndex, ( ( nMask & nGroups ) == 0 ) ? 0 : 1);
		nMask *= 2;
	}
	m_listGroups.RedrawWindow();
}

/**********************************************************************/

void CDefEditTableGroupSetDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();

	*pResult = 0;
}

/**********************************************************************/

void CDefEditTableGroupSetDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditTableGroupSetDlg::SpinPrevious()
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

void CDefEditTableGroupSetDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < DataManagerNonDb.TableGroupSets.GetSize() - 1 )
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

void CDefEditTableGroupSetDlg::SaveData()
{
	CTableGroupSetCSVRecord TableGroupSet;
	DataManagerNonDb.TableGroupSets.GetAt( m_nIndex, TableGroupSet );
	
	TableGroupSet.SetGroupSetName( m_nLevel, GetEditBoxText( m_editName ) );
	TableGroupSet.SetShowAsFilterFlag( m_nLevel, IsTicked( m_checkFilter ) );
	TableGroupSet.SetIncludeUnknownFlag( m_nLevel, IsTicked( m_checkUnknown ) );
	TableGroupSet.SetIncludeNonTableFlag( m_nLevel, IsTicked( m_checkNonTable ) );
	
	int nGroups = 0;
	int nMask = 1;
	for ( int nIndex = 0; nIndex < MAX_TABLEGROUP; nIndex++ )
	{
		if ( m_listGroups.GetItemTagState(nIndex) != 0 )
		{
			nGroups += nMask;
		}
		nMask *= 2;
	}

	TableGroupSet.SetGroupMask( m_nLevel, nGroups );

	DataManagerNonDb.TableGroupSets.SetAt( m_nIndex, TableGroupSet );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditTableGroupSetDlg::OnOK() 
{
	SaveData();
	EndDialog ( IDOK );
}

/**********************************************************************/
