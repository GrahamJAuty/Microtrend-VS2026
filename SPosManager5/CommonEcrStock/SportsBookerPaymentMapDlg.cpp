/**********************************************************************/
#include "SportsBookerPaymentMapDlg.h"
/**********************************************************************/

CSportsBookerPaymentMapDlg::CSportsBookerPaymentMapDlg( CWnd* pParent)
	: CSSDialog(CSportsBookerPaymentMapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSportsBookerPaymentMapDlg)
	//}}AFX_DATA_INIT

	for ( int p = 0; p < DataManager.Payment.GetSize(); p++ )
	{
		CPaymentCSVRecord Payment;
		DataManager.Payment.GetAt( p, Payment );
		
		CString strListText, strCellText;
		strListText.Format( "P%2.2d,%s", Payment.GetPaymentNo(), Payment.GetDisplayName() );
		strCellText.Format( "P%2.2d  %s", Payment.GetPaymentNo(), Payment.GetDisplayName() );
		m_PaymentTextBuffer.AddText( strListText, strCellText );
	}

	m_nTabPage = 0;
}

/**********************************************************************/

void CSportsBookerPaymentMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSportsBookerPaymentMapDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TAB_PAYMENTS, m_TabPayments );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSportsBookerPaymentMapDlg, CDialog)
	//{{AFX_MSG_MAP(CSportsBookerPaymentMapDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PAYMENTS, OnTcnSelchangeTabs)
	ON_WM_MEASUREITEM()
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_MESSAGE(WM_GET_CELLTYPE, GetCellType)
END_MESSAGE_MAP()

/**********************************************************************/

void CSportsBookerPaymentMapDlg::GetTabItemCount( int& nStartIdx, int& nItemsOnPage )
{
	switch( m_nTabPage )
	{
	case 1:
		nStartIdx = 10;
		nItemsOnPage = 8;
		break;

	case 0:
	default:
		nStartIdx = 0;
		nItemsOnPage = 10;
		break;
	}
}

/**********************************************************************/

BOOL CSportsBookerPaymentMapDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle = "Setup Sports Booker Payment Map ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), EcrmanOptions.GetGlobalPaymentFlag() ); 
	SetWindowText ( strTitle );

	m_TabPayments.InsertItem( 0, "Payments 1 - 10" );
	m_TabPayments.InsertItem( 1, "Payments 11 - 999" );

	m_listPayments.SubclassDlgItem ( IDC_LIST, this );
	m_listPayments.InsertColumn ( 0, "No", LVCFMT_LEFT, 40 );
	m_listPayments.InsertColumn ( 1, "Sports Booker", LVCFMT_LEFT, 200 );
	m_listPayments.InsertColumn ( 2, "SPOS", LVCFMT_LEFT, 220 );
	m_listPayments.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listPayments.EnableVScroll();

	CreateHelperArrays();
	AddList();

	return TRUE;  
}

/**********************************************************************/

void CSportsBookerPaymentMapDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listPayments.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

BOOL CSportsBookerPaymentMapDlg::PreTranslateMessage(MSG* pMsg) 
{
  if(pMsg->message==WM_KEYDOWN)
  {
	  if(pMsg->wParam==VK_RETURN )
	  {
		  if ( GetFocus() == &m_listPayments )
		  {
			  m_listPayments.HandleReturn(2);	
			  pMsg->wParam=NULL ;
		  }
	  }
  }
  return CDialog::PreTranslateMessage(pMsg);
}

/**********************************************************************/

void CSportsBookerPaymentMapDlg::CreateHelperArrays()
{
	m_arrayCellTypes.RemoveAll();
	m_arrayEditInfo.RemoveAll();

	CellTypeInfo PaymentCell;
	PaymentCell.m_strComboTabs = "5,50";
	PaymentCell.ResetForCombo( m_PaymentTextBuffer );

	for ( int n = 0; n < DataManager.SBPaymentMap.GetMapSize(); n++ )
	{
		CSortedIntByInt item;
		DataManager.SBPaymentMap.GetMapItem( n, item );

		CPaymentMapEditInfo EditInfo;
		EditInfo.m_nSportsBookerPayNo = item.m_nKey;
		EditInfo.m_nCurrentSharpPosNo = item.m_nVal;
		EditInfo.m_bUnknownSharpPosNo = FALSE;
		EditInfo.m_strCurrentSharpPosCellName = "";
		EditInfo.m_strCurrentSharpPosListName = "";
		m_arrayEditInfo.Add( EditInfo );
		
		int nPayIdx;
		if ( DataManager.Payment.FindPaymentByNumber( item.m_nVal, nPayIdx ) == TRUE )
		{
			CellTypeInfoTextItem item;
			m_PaymentTextBuffer.GetItem( nPayIdx, item );	
			m_arrayEditInfo[n].m_bUnknownSharpPosNo = FALSE;
			m_arrayEditInfo[n].m_strCurrentSharpPosCellName = item.m_strCellText;
			m_arrayEditInfo[n].m_strCurrentSharpPosListName = item.m_strListText;
			PaymentCell.m_strCurrent = item.m_strCellText;
			PaymentCell.m_nComboSelect = nPayIdx;
		}
		else
		{
			CString strCellName = "";
			strCellName.Format( "P%2.2d  Payment %d", item.m_nVal, item.m_nVal );

			CString strListName = "";
			strListName.Format( "P%2.2d,Payment %d", item.m_nVal, item.m_nVal );

			m_arrayEditInfo[n].m_bUnknownSharpPosNo = TRUE;
			m_arrayEditInfo[n].m_strCurrentSharpPosCellName = strCellName;
			m_arrayEditInfo[n].m_strCurrentSharpPosListName = strListName;
			PaymentCell.m_nComboSelect = 0;
			PaymentCell.m_strCurrent = strCellName;
		}
		
		if ( TRUE == m_arrayEditInfo[n].m_bUnknownSharpPosNo )
		{
			PaymentCell.m_nMaxInt++;
			m_arrayCellTypes.Add( PaymentCell );
			PaymentCell.m_nMaxInt--;
		}
		else
		{
			m_arrayCellTypes.Add( PaymentCell );
		}
	}
}

/**********************************************************************/

void CSportsBookerPaymentMapDlg::AddList()
{
	m_listPayments.DeleteAllItems();

	int nStartIdx, nItemsOnPage;
	GetTabItemCount( nStartIdx, nItemsOnPage );
	
	for ( int nRow = 0; nRow < nItemsOnPage; nRow++ )
	{
		int nItemIdx = nStartIdx + nRow;

		if ( ( nItemIdx >= 0 ) && ( nItemIdx < m_arrayCellTypes.GetSize() ) )
		{
			CSortedIntByInt item;
			DataManager.SBPaymentMap.GetMapItem( nItemIdx, item );
			CellTypeInfo CellInfo = m_arrayCellTypes.GetAt( nItemIdx );

			CCSV csv;
			csv.Add( item.m_nKey );
			csv.Add( DataManager.SBPaymentMap.GetSportsBookerText( item.m_nKey ) );
			csv.Add( CellInfo.m_strCurrent );

			AddField( csv );
		}
	}
}

/**********************************************************************/

void CSportsBookerPaymentMapDlg::AddField( CCSV& csv )
{
	int nPos = m_listPayments.GetItemCount();

	m_listPayments.InsertItem ( nPos, "" );
	m_listPayments.SetItem ( nPos, 0, LVIF_TEXT, csv.GetString(0), 0, 0, 0, NULL );
	m_listPayments.SetItem ( nPos, 1, LVIF_TEXT, csv.GetString(1), 0, 0, 0, NULL );
	m_listPayments.SetItem ( nPos, 2, LVIF_TEXT, csv.GetString(2), 0, 0, 0, NULL );
	m_listPayments.SetItemData ( nPos, csv.GetInt(0) );
}

/**********************************************************************/

void CSportsBookerPaymentMapDlg::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	SaveTabChanges();

	m_nTabPage = m_TabPayments.GetCurSel();
	if ( ( m_nTabPage < 0 ) || ( m_nTabPage > 1 ) )
		m_nTabPage = 0;

	AddList();
}

/**********************************************************************/

void CSportsBookerPaymentMapDlg::SaveTabChanges()
{
	int nStartIdx, nItemsOnPage;
	GetTabItemCount( nStartIdx, nItemsOnPage );
	
	int nSize = DataManager.SBPaymentMap.GetMapSize();
	if ( ( m_arrayCellTypes.GetSize() != nSize ) || ( m_arrayEditInfo.GetSize() != nSize ) )
		return;
	
	for ( int nRow = 0; nRow < nItemsOnPage; nRow++ )
	{
		int nItemIdx = nStartIdx + nRow;

		if ( ( nItemIdx >= 0 ) && ( nItemIdx < nSize ) )
		{
			int nComboSel = m_arrayCellTypes[nItemIdx].m_nComboSelect;
			if ( TRUE == m_arrayEditInfo[nItemIdx].m_bUnknownSharpPosNo )
				nComboSel--;

			if ( ( nComboSel >= 0 ) && ( nComboSel < DataManager.Payment.GetSize() ) )
			{
				CSortedIntByInt MapItem;
				DataManager.SBPaymentMap.GetMapItem( nItemIdx, MapItem );

				CPaymentCSVRecord Payment;
				DataManager.Payment.GetAt( nComboSel, Payment );
				MapItem.m_nVal = Payment.GetPaymentNo();
				
				DataManager.SBPaymentMap.UpdateMapItem( MapItem );
			}
		}
	}
}

/**********************************************************************/

LRESULT CSportsBookerPaymentMapDlg::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
	m_TabPayments.EnableWindow( FALSE );

	// Get the Combobox window pointer
	CComboBox* pInPlaceCombo = static_cast<CComboBox*> (GetFocus());

	// Get the inplace combbox top left
	CRect obWindowRect;

	pInPlaceCombo->GetWindowRect(&obWindowRect);
	
	CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft()); 
	
	// Get the active list
	// Get the control window rect
	// If the inplace combobox top left is in the rect then
	// The control is the active control
	m_listPayments.GetWindowRect(&obWindowRect);
	
	int nStartIdx, nItemsOnPage;
	GetTabItemCount( nStartIdx, nItemsOnPage );
	int nItemIdx = nStartIdx + wParam;
	
	CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
	pComboList->RemoveAll(); 

	if (obWindowRect.PtInRect(obInPlaceComboTopLeft)) 
	{
		if ( ( nItemIdx >= 0 ) && ( nItemIdx < m_arrayCellTypes.GetSize() ) )
		{
			CCSV csv ( m_arrayCellTypes[nItemIdx].m_strComboListText ); 
	
			if ( m_arrayCellTypes[nItemIdx].m_nType == CELL_TYPE_COMBO_TABBED )
				pComboList -> AddTail( m_arrayCellTypes[nItemIdx].m_strComboTabs );

			if ( m_arrayEditInfo[nItemIdx].m_bUnknownSharpPosNo == TRUE )
				pComboList -> AddTail( m_arrayEditInfo[ nItemIdx ].m_strCurrentSharpPosListName );
			
			for ( int n = 0; n < csv.GetSize(); n++ )
				pComboList -> AddTail( csv.GetString(n) );
		}
	}
	
	return true;
}

/**********************************************************************/

LRESULT CSportsBookerPaymentMapDlg::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam) 
{
	m_TabPayments.EnableWindow( TRUE );
	m_listPayments.AllowListScroll( TRUE );
	m_listPayments.SetEditCell( -1, -1 );

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;
	
	if (wParam == IDC_LIST)
	{
		// Update the item text with the new text
		CString strText = pDispInfo->item.pszText;  
	
		int nRow = pDispInfo -> item.iItem;
		int nCol = pDispInfo -> item.iSubItem;

		int nStartIdx, nItemsOnPage;
		GetTabItemCount( nStartIdx, nItemsOnPage );
		int nItemIdx = nStartIdx + nRow;

		if ( ( nItemIdx >= 0 ) && ( nItemIdx < m_arrayCellTypes.GetSize() ) )
		{
			CellTypeInfo CellInfo = m_arrayCellTypes.GetAt( nItemIdx );

			switch( CellInfo.m_nType )
			{
			case CELL_TYPE_COMBO_TABBED:
				{
					int nOldSel = CellInfo.m_nComboSelect;
					int nNewSel = atoi( strText ) - 1;
					{
						CCSV csv( CellInfo.m_strComboCellText );

						if ( m_arrayEditInfo[nItemIdx].m_bUnknownSharpPosNo == TRUE )
							csv.InsertAt( 0, m_arrayEditInfo[ nItemIdx ].m_strCurrentSharpPosCellName );
						
						int nSel = -1;
						if ( ( nNewSel >= 0 ) && ( nNewSel < csv.GetSize() ) )
							nSel = nNewSel;
						else if ( ( nOldSel >= 0 ) && ( nOldSel < csv.GetSize() ) )
							nSel = nOldSel;

						if ( nSel != -1 )
						{
							CString strCurrent = csv.GetString( nSel );
							m_listPayments.SetItem ( nRow, 2, LVIF_TEXT, strCurrent, 0, 0, 0, NULL );
							
							CellInfo.m_nComboSelect = nSel;
							CellInfo.m_strCurrent = strCurrent;
							m_arrayCellTypes.SetAt( nItemIdx, CellInfo );
						}
					}
				}
				break;
			}
		}
	}

	return 1;
}

/**********************************************************************/

LRESULT CSportsBookerPaymentMapDlg::GetCellType(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);
	
	int nCol = pCellType -> m_nCol;
	int nRow = pCellType -> m_nRow;

	if ( nCol != 2 )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	int nStartIdx, nItemsOnPage;
	GetTabItemCount( nStartIdx, nItemsOnPage );

	if ( ( nRow < 0 ) || ( nRow >= nItemsOnPage ) )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	int nItemIdx = nStartIdx + nRow;

	if ( ( nItemIdx < 0 ) || ( nItemIdx >= m_arrayCellTypes.GetSize() ) )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	pCellType -> m_nType = m_arrayCellTypes[nItemIdx].m_nType;
	pCellType -> m_nSSEditType = m_arrayCellTypes[nItemIdx].m_nSSEditType;
	pCellType -> m_strSSEditFormat = m_arrayCellTypes[nItemIdx].m_strSSEditFormat;
	pCellType -> m_nSSEditWidth = m_arrayCellTypes[nItemIdx].m_nSSEditWidth;
	pCellType -> m_nComboSelect = m_arrayCellTypes[nItemIdx].m_nComboSelect;
	
	return 1;
}

/**********************************************************************/

void CSportsBookerPaymentMapDlg::OnCancel() 
{
	EndDialog( IDOK );
}

/**********************************************************************/

void CSportsBookerPaymentMapDlg::OnOK() 
{
	SaveTabChanges();
	EndDialog( IDOK );
}

/**********************************************************************/
