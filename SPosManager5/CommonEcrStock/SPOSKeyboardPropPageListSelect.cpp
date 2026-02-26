/**********************************************************************/
#include "SPOSKeyboardPropPageListSelect.h"
/**********************************************************************/

CSPOSKeyboardPropPageListSelect::CSPOSKeyboardPropPageListSelect() : CPropertyPage(CSPOSKeyboardPropPageListSelect::IDD)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardPropPageListSelect)
	//}}AFX_DATA_INIT
	m_nListType = LISTTYPE_ITEM_STANDARD;
}

/**********************************************************************/

CSPOSKeyboardPropPageListSelect::~CSPOSKeyboardPropPageListSelect()
{
}

/**********************************************************************/

void CSPOSKeyboardPropPageListSelect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPropPageListSelect)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropPageListSelect, CPropertyPage)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropPageListSelect)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED( IDC_BUTTON_ALL, OnButtonAll )
	ON_BN_CLICKED( IDC_BUTTON_NONE, OnButtonNone )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPropPageListSelect::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_listSelected.SubclassDlgItem ( IDC_LIST, this );
	m_listSelected.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listSelected.InsertColumn ( 0, "", LVCFMT_LEFT, 275 );
	m_listSelected.SetLockItemZeroFlag( FALSE );

	int nPos = 0;
	for ( int nListIdx = 0; nListIdx < SPOSKeyboardManager.GetListManager() -> GetListCount(); nListIdx++ )
	{
		CSPOSKeyboardListRecord ListRecord;
		SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );

		if ( ListRecord.GetListType() == m_nListType )
		{
			CString strName;
			strName.Format( "L%5.5d   ",
				ListRecord.GetListNo() );

			strName += ListRecord.GetName();

			CSSListTaggedSelectItem item;
			item.m_strText = strName;
			item.m_nData = 0;
			item.SetTagState(ListRecord.GetAllowChangeFlag() ? 1 : 0 );
			m_listSelected.AddItemToList( item );
		}
	}

	return TRUE;  
}

/**********************************************************************/

void CSPOSKeyboardPropPageListSelect::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listSelected.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageListSelect::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageListSelect::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardPropPageListSelect::SaveRecord()
{
	if ( m_arrayListNo.GetSize() == m_listSelected.GetItemCount() )
	{
		for ( int n = 0; n < m_arrayListNo.GetSize(); n++ )
		{
			int nListIdx;
			__int64 nListNo = m_arrayListNo[n];
			if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == TRUE )
			{
				CSPOSKeyboardListRecord ListRecord;
				SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
				ListRecord.SetAllowChangeFlag( m_listSelected.GetItemTagState(n) == 1 );
				SPOSKeyboardManager.GetListManager() -> SetListRecord( nListIdx, ListRecord );
			}
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageListSelect::OnButtonAll()
{
	m_listSelected.SelectAll();
}

/**********************************************************************/

void CSPOSKeyboardPropPageListSelect::OnButtonNone()
{
	m_listSelected.SelectNone();
}

/**********************************************************************/
