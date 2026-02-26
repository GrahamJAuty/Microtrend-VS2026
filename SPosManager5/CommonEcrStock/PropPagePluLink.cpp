/**********************************************************************/
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
#include "..\SPosStockManager5\PluFilterDlgStockman.h"
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "FilterTypes.h"
#include "PluMiniBrowseDlg.h"
#include "PriceSetItemManager.h"
#include "ReportHelpers.h"
#include "ReportPluCustomList.h"
#include "ReportPluCustomList.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePluLink.h"
/**********************************************************************/

CPropPagePluLink::CPropPagePluLink() : CSSPropertyPage(CPropPagePluLink::IDD), 
	m_FilterArray ( FILTERTYPE_BROWSE_PLU ) 
{
	//{{AFX_DATA_INIT(CPropPagePluLink)
	//}}AFX_DATA_INIT
	m_pPluRecord = NULL;
	m_pPluChangesBuffer = NULL;
	m_bReadOnly = FALSE;
	m_bCanEnableSellTieldItems = TRUE;
	
	for ( int n = 0; n <= MODIFIER_COUNT; n++ )
	{
		if (n == 0)
		{
			m_ModifierTextBuffer.AddText("", "");
		}
		else
		{
			CString strCell = "";
			strCell.Format( "M%2.2d %s",
				n,
				DataManager.Modifier.GetDisplayName(n) );

			CString strList = "";
			strList.Format( "M%2.2d,%s",
				n,
				DataManager.Modifier.GetDisplayName(n) );

			m_ModifierTextBuffer.AddText( strList, strCell );
		}
	}
}

/**********************************************************************/

CPropPagePluLink::~CPropPagePluLink()
{
}

/**********************************************************************/

void CPropPagePluLink::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePluLink)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_LINKTYPE, m_comboLinkType);
	DDX_Control(pDX, IDC_CHECK_SELLTIEDITEMS, m_checkSellTiedItems);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_buttonRemove);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_BUTTON_SCAN, m_buttonScan);
	DDX_Control(pDX, IDC_EDIT_PLU, m_editPluNo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePluLink, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePluLink)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_CBN_SELCHANGE(IDC_COMBO_LINKTYPE, OnSelectLinkType)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU, OnKillFocusPlu)
	ON_MESSAGE (WM_APP, OnAddPluMessage)
	ON_MESSAGE (WM_APP + 1, OnAddScannedPlu)
	ON_MESSAGE (WM_APP + 2, OnSetDefaultFocus)
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_MESSAGE(WM_GET_CELLTYPE, GetCellType)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePluLink::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_editPluNo.LimitText( Super.MaxPluLen() );
	m_editPluNo.SetMessageTarget( this, WM_APP + 1 );

	SetRecordControls();

	m_listLinkPluNo.SubclassDlgItem ( IDC_LIST, this );
	m_listLinkPluNo.InsertColumn ( 0, "", LVCFMT_LEFT, 30 );
	m_listLinkPluNo.InsertColumn ( 1, "Plu No", LVCFMT_LEFT, 130 );
	m_listLinkPluNo.InsertColumn ( 2, "Description", LVCFMT_LEFT, 340 );
	m_listLinkPluNo.InsertColumn ( 3, "Modifier", LVCFMT_LEFT, 240 );
	m_listLinkPluNo.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listLinkPluNo.EnableVScroll();

	m_comboLinkType.AddString( "None" );
	m_comboLinkType.AddString( "Parent Total" );
	m_comboLinkType.AddString( "Individual" );

	Refresh();
	
	return TRUE;  
}

/**********************************************************************/

void CPropPagePluLink::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listLinkPluNo.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

BOOL CPropPagePluLink::PreTranslateMessage(MSG* pMsg) 
{
  if(pMsg->message==WM_KEYDOWN)
  {
	  if(pMsg->wParam==VK_RETURN )
	  {
		  if ( GetFocus() == &m_listLinkPluNo )
		  {
			  m_listLinkPluNo.HandleReturn(3);	
			  pMsg->wParam=NULL ;
		  }
	  }
  }
  return CDialog::PreTranslateMessage(pMsg);
}

/**********************************************************************/

void CPropPagePluLink::AddList()
{
	m_arrayCellTypes.RemoveAll();
	m_listLinkPluNo.DeleteAllItems();

	for (int n = 0; n < m_arrayLinkPluNo.GetSize(); n++)
	{
		__int64 nPluNo = m_arrayLinkPluNo.GetAt(n);

		if (AddPluNoToList(nPluNo) == FALSE)
		{
			m_arrayLinkPluNo.RemoveAt(n--);
		}
	}
}

/**********************************************************************/

bool CPropPagePluLink::AddPluNoToList( __int64 nPluNo )
{
	CSQLPluNoInfo infoPluNo;
	infoPluNo.m_nEposPluNo = nPluNo;
	::ProcessPluNo( infoPluNo );

	if (FALSE == infoPluNo.m_bValid)
	{
		return FALSE;
	}

	CString strPluNo = "";
	::FormatPluNo( infoPluNo.m_nBasePluNo, strPluNo );

	int nPluIdx = 0;
	CString strDescription = "Unknown";
	if ( DataManager.Plu.FindPluByNumber( infoPluNo.m_nBasePluNo, nPluIdx ) == TRUE )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		strDescription = ReportHelpers.GetReportText( PluRecord );
	}

	CellTypeInfoTextItem TextItem;
	m_ModifierTextBuffer.GetItem( infoPluNo.m_nModifier, TextItem );

	CCSV csv;
	csv.Add( CustomPluList::PluNo.Type );
	csv.Add( strPluNo );
	csv.Add( strDescription );
	csv.Add( TextItem.m_strCellText );
	AddField( csv );
	
	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo( m_ModifierTextBuffer );
	info.m_nComboSelect = infoPluNo.m_nModifier;
	m_arrayCellTypes.Add( info );

	return TRUE;
}

/**********************************************************************/

void CPropPagePluLink::AddField( CCSV& csv )
{
	int nPos = m_listLinkPluNo.GetItemCount();

	CString strNum = "";
	strNum.Format( "%d", nPos + 1 );

	m_listLinkPluNo.InsertItem ( nPos, "" );
	m_listLinkPluNo.SetItem ( nPos, 0, LVIF_TEXT, strNum, 0, 0, 0, NULL );
	m_listLinkPluNo.SetItem ( nPos, 1, LVIF_TEXT, csv.GetString(1), 0, 0, 0, NULL );
	m_listLinkPluNo.SetItem ( nPos, 2, LVIF_TEXT, csv.GetString(2), 0, 0, 0, NULL );
	m_listLinkPluNo.SetItem ( nPos, 3, LVIF_TEXT, csv.GetString(3), 0, 0, 0, NULL );
	m_listLinkPluNo.SetItemData ( nPos, csv.GetInt(0) );
}

/**********************************************************************/

long CPropPagePluLink::OnSetDefaultFocus( WPARAM wIndex, LPARAM lParam )
{
	m_editPluNo.SetFocus();
	m_editPluNo.SetSel(0,-1);
	return 0L;
}

/**********************************************************************/

BOOL CPropPagePluLink::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePluLink::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	RefreshInternal();
	SetRecordControls();
	PostMessage( WM_APP + 2, 0, 0 );
	return bResult;
}

/**********************************************************************/

void CPropPagePluLink::Refresh()
{
	RefreshInternal();
	SetRecordControls();
}

/**********************************************************************/

void CPropPagePluLink::RefreshInternal()
{
	m_pPluRecord->GetLinkTableArray(m_arrayLinkPluNo);
	AddList();

	int nType = m_pPluRecord->GetLinkType();
	if ((nType >= 0) && (nType <= 2))
	{
		m_comboLinkType.SetCurSel(nType);
	}
	else
	{
		m_comboLinkType.SetCurSel(0);
	}

	m_checkSellTiedItems.SetCheck(m_pPluRecord->GetLinkSellTiedItemFlag());

	OnSelectLinkType();
}

/**********************************************************************/

void CPropPagePluLink::OnSelectLinkType()
{
	if (m_comboLinkType.GetCurSel() == 1)
	{
		ShowAndEnableWindow(&m_checkSellTiedItems, TRUE, m_bCanEnableSellTieldItems);
	}
	else
	{
		ShowAndEnableWindow(&m_checkSellTiedItems, FALSE);
	}
}

/**********************************************************************/

bool CPropPagePluLink::UpdateRecord()
{
	SaveRecord();
	return TRUE;
}

/**********************************************************************/

void CPropPagePluLink::SetRecordControls()
{
	m_bCanEnableSellTieldItems = TRUE;

	if ( ( PriceSetItemManager.IsPluMaster() == FALSE ) || ( m_bReadOnly == TRUE ) )
	{
		int nPluStatus = PriceSetItemManager.GetPluStatus();

		if ( ( nPluStatus != LOCALPLU_LOCAL ) || ( m_bReadOnly == TRUE ) )
		{
			m_bCanEnableSellTieldItems = FALSE;

			m_comboLinkType.EnableWindow( FALSE );
			m_checkSellTiedItems.EnableWindow(FALSE);
			m_editPluNo.EnableWindow( FALSE );
			m_buttonBrowse.EnableWindow( FALSE );
			m_buttonAdd.EnableWindow( FALSE );
			m_buttonRemove.EnableWindow( FALSE );
			m_buttonScan.EnableWindow( FALSE );
		}
		else
		{
			bool bType = PriceSetItemManager.CheckField( Plu::LinkType.Label );
			bool bTable = PriceSetItemManager.CheckField( Plu::LinkTable.Label );
			m_bCanEnableSellTieldItems = PriceSetItemManager.CheckField(Plu::LinkSellTiedItem.Label);
			
			m_comboLinkType.EnableWindow( bType );
			m_editPluNo.EnableWindow( bTable );
			m_buttonBrowse.EnableWindow( bTable );
			m_buttonAdd.EnableWindow( bTable );
			m_buttonRemove.EnableWindow( bTable );
			m_buttonScan.EnableWindow( bTable );

			if (FALSE == m_bCanEnableSellTieldItems)
			{
				m_checkSellTiedItems.EnableWindow(FALSE);
			}
			else
			{
				m_checkSellTiedItems.EnableWindow(m_comboLinkType.GetCurSel() == 1);
			}
		}
	}
}

/**********************************************************************/

void CPropPagePluLink::SaveRecord()
{
	if (m_bReadOnly == TRUE)
	{
		return;
	}

	UpdateLinkPluNoArray();
	m_pPluRecord -> SetLinkType( m_comboLinkType.GetCurSel() );
	m_pPluRecord -> SetLinkTableArray( m_arrayLinkPluNo );

	if (m_comboLinkType.GetCurSel() == 1)
	{
		m_pPluRecord->SetLinkSellTiedItemFlag(IsTicked(m_checkSellTiedItems));
	}
	else
	{
		m_pPluRecord->SetLinkSellTiedItemFlag(FALSE);
	}
}

/**********************************************************************/

void CPropPagePluLink::OnButtonAdd() 
{
	CPluMiniBrowseDlg dlgFilter( NODE_PLU, m_FilterArray, this );
	dlgFilter.DoModal();
}

/**********************************************************************/

long CPropPagePluLink::OnAddPluMessage( WPARAM wIndex, LPARAM lParam )
{ 
	switch ( wIndex )
	{
	case ADDPLU_MESSAGE:
		{
			CPluMiniBrowseDlg* pDlg = (CPluMiniBrowseDlg*) lParam;
				
			int nPluIdx = pDlg -> GetPluIdx();

			if ( nPluIdx >= 0 && nPluIdx < DataManager.Plu.GetSize() )
			{
				if ( m_arrayLinkPluNo.GetSize() >= MAX_LINK_PLU_ITEMS )
				{
					CString strMsg;
					strMsg.Format( "You can only add a maximum of\n%d items to a Plu link table", MAX_LINK_PLU_ITEMS );
					Prompter.Error( strMsg );
				}
				else
				{
					__int64 nPluNo = DataManager.Plu.GetPluNoInt( nPluIdx );

					__int64 nEPOSPluNo = nPluNo;
					if (SysInfo.IsModifiableSSPluNo(nEPOSPluNo) == TRUE)
					{
						nEPOSPluNo *= 10;
					}

					if ( AddPluNoToList( nEPOSPluNo ) == TRUE )
					{
						m_arrayLinkPluNo.Add( nEPOSPluNo );
						
						int nPos = m_listLinkPluNo.GetItemCount() - 1;
						m_listLinkPluNo.SetSelectionMark( nPos );
						m_listLinkPluNo.SetItemState( nPos, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
						m_listLinkPluNo.EnsureVisible( nPos, FALSE );
					}
				}
			}
		}
		break;
	}

	return 0L;
}

/**********************************************************************/

long CPropPagePluLink::OnAddScannedPlu( WPARAM wIndex, LPARAM lParam )
{
	AddScannedPlu();
	return 0L;
}

/**********************************************************************/

void CPropPagePluLink::OnButtonScan()
{
	AddScannedPlu();
}

/**********************************************************************/

void CPropPagePluLink::AddScannedPlu()
{
	CString strPluNo;
	m_editPluNo.GetWindowText( strPluNo );
	strPluNo = BarcodePluNoTable.ConvertPlu( strPluNo );
	m_editPluNo.SetWindowText( strPluNo );

	__int64 nPluNo = _atoi64( strPluNo );

	if ( nPluNo == 0 )
	{
		m_editPluNo.SetWindowText( "" );
		Prompter.Error( "Please enter a Plu number" );
	}
	else if ( m_arrayLinkPluNo.GetSize() >= MAX_LINK_PLU_ITEMS )
	{
		CString strMsg;
		strMsg.Format( "You can only add a maximum of\n%d items to a Plu link table", MAX_LINK_PLU_ITEMS );
		Prompter.Error( strMsg );
	}
	else
	{
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
		{		
			__int64 nEPOSPluNo = nPluNo;
			if ( SysInfo.IsModifiableSSPluNo( nEPOSPluNo ) == TRUE )
				nEPOSPluNo *= 10;

			if ( AddPluNoToList( nEPOSPluNo ) == TRUE )
			{
				m_arrayLinkPluNo.Add( nEPOSPluNo );
				
				int nPos = m_listLinkPluNo.GetItemCount() - 1;
				m_listLinkPluNo.SetSelectionMark( nPos );
				m_listLinkPluNo.SetItemState( nPos, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
				m_listLinkPluNo.EnsureVisible( nPos, FALSE );
			}
		}
		else
		{
			CString strError;
			strError.Format( "Plu number %s does not exist in your database.", (const char*) strPluNo );
			Prompter.Error( strError );
		}
	}

	m_editPluNo.SetSel( 0, -1 );
	m_editPluNo.SetFocus();
}

/**********************************************************************/

void CPropPagePluLink::OnButtonRemove() 
{
	int nSel = m_listLinkPluNo.GetSelectionMark();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayLinkPluNo.GetSize() ) )
	{
		UpdateLinkPluNoArray();
		m_arrayLinkPluNo.RemoveAt( nSel );
		AddList();

		if ( nSel < m_arrayLinkPluNo.GetSize() )
		{
			m_listLinkPluNo.SetSelectionMark( nSel );
			m_listLinkPluNo.SetItemState( nSel, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
			m_listLinkPluNo.EnsureVisible( nSel, FALSE );
		}
		else if ( nSel > 0 )
		{
			m_listLinkPluNo.SetSelectionMark( nSel - 1 );
			m_listLinkPluNo.SetItemState( nSel - 1, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
			m_listLinkPluNo.EnsureVisible( nSel - 1, FALSE );
		}
	}
}

/**********************************************************************/

void CPropPagePluLink::OnButtonBrowse() 
{
	UpdateData( TRUE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_FilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK == nDlgResult )
	{
		if ( nPluIdx >= 0 && nPluIdx < DataManager.Plu.GetSize() )
			m_editPluNo.SetWindowText ( DataManager.Plu.GetPluNoString ( nPluIdx ) );
	}
}

/**********************************************************************/

void CPropPagePluLink::OnKillFocusPlu() 
{
	CString strPluNo;
	m_editPluNo.GetWindowText( strPluNo );
	strPluNo = BarcodePluNoTable.ConvertPlu( strPluNo );
	m_editPluNo.SetWindowText( strPluNo );
}

/**********************************************************************/

LRESULT CPropPagePluLink::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
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
	m_listLinkPluNo.GetWindowRect(&obWindowRect);
	
	int iRowIndex = wParam;
	
	CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
	pComboList->RemoveAll(); 

	if (obWindowRect.PtInRect(obInPlaceComboTopLeft)) 
	{
		if ( iRowIndex >= 0 && iRowIndex < m_arrayCellTypes.GetSize() )
		{
			CCSV csv ( m_arrayCellTypes[iRowIndex].m_strComboListText ); 

			if ( m_arrayCellTypes[iRowIndex].m_nType == CELL_TYPE_COMBO_TABBED )
				pComboList -> AddTail( m_arrayCellTypes[iRowIndex].m_strComboTabs );
	
			for ( int n = 0; n < csv.GetSize(); n++ )
				pComboList -> AddTail( csv.GetString(n) );
		}
	}
	
	return true;
}

/**********************************************************************/

LRESULT CPropPagePluLink::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam) 
{
	m_listLinkPluNo.AllowListScroll( TRUE );
	m_listLinkPluNo.SetEditCell( -1, -1 );

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;
	
	if (wParam == IDC_LIST)
	{
		// Update the item text with the new text
		CString strText = pDispInfo->item.pszText;  
	
		int nRow = pDispInfo -> item.iItem;
		int nCol = pDispInfo -> item.iSubItem;

		if ( nRow >= 0 && nRow < m_arrayCellTypes.GetSize() )
		{
			CellTypeInfo info = m_arrayCellTypes.GetAt( nRow );

			switch( info.m_nType )
			{
			case CELL_TYPE_COMBO_TABBED:
				{
					int nOldSel = m_arrayCellTypes[nRow].m_nComboSelect;
					int nNewSel = atoi( strText ) - 1;
					{
						CCSV csv( info.m_strComboCellText );
						if ( ( nNewSel >= 0 ) && ( nNewSel < csv.GetSize() ) )
						{
							m_arrayCellTypes[nRow].m_nComboSelect = nNewSel;
							m_listLinkPluNo.SetItem ( nRow, 3, LVIF_TEXT, csv.GetString( nNewSel ), 0, 0, 0, NULL );
						}
						else if ( ( nOldSel >= 0 ) && ( nOldSel < csv.GetSize() ) )
						{
							m_arrayCellTypes[nRow].m_nComboSelect = nOldSel;
							m_listLinkPluNo.SetItem ( nRow, 3, LVIF_TEXT, csv.GetString( nOldSel ), 0, 0, 0, NULL );
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

LRESULT CPropPagePluLink::GetCellType(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);
	
	int nCol = pCellType -> m_nCol;
	int nRow = pCellType -> m_nRow;

	if ( nCol != 3 )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	if ( nRow < 0 || nRow >= m_arrayCellTypes.GetSize() )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	if ( ( m_bReadOnly == TRUE ) || ( PriceSetItemManager.IsPluMaster() == FALSE ) )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	__int64 nPluNo = m_arrayLinkPluNo.GetAt(nRow);

	CSQLPluNoInfo infoPluNo;
	infoPluNo.m_nEposPluNo = nPluNo;
	::ProcessPluNo( infoPluNo );

	if ( FALSE == infoPluNo.m_bModifiable )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	pCellType -> m_nType = m_arrayCellTypes[nRow].m_nType;
	pCellType -> m_nSSEditType = m_arrayCellTypes[nRow].m_nSSEditType;
	pCellType -> m_strSSEditFormat = m_arrayCellTypes[nRow].m_strSSEditFormat;
	pCellType -> m_nSSEditWidth = m_arrayCellTypes[nRow].m_nSSEditWidth;
	pCellType ->m_nComboSelect = m_arrayCellTypes[nRow].m_nComboSelect;
	
	return 1;
}

/**********************************************************************/

void CPropPagePluLink::UpdateLinkPluNoArray()
{
	int nSize = m_arrayLinkPluNo.GetSize();
	
	if ( m_arrayCellTypes.GetSize() == nSize )
	{
		for ( int n = 0; n < m_arrayLinkPluNo.GetSize(); n++ )
		{
			__int64 nPluNo = m_arrayLinkPluNo.GetAt(n);

			CSQLPluNoInfo infoPluNo;
			infoPluNo.m_nEposPluNo = nPluNo;
			::ProcessPluNo( infoPluNo );

			__int64 nEPOSPluNo = infoPluNo.m_nBasePluNo;

			if ( TRUE == infoPluNo.m_bModifiable )
			{
				nEPOSPluNo *= 10;

				CellTypeInfo info = m_arrayCellTypes[n];

				int nModifier = info.m_nComboSelect;
				if ( ( nModifier >= 1 ) && ( nModifier <= 9 ) )
					nEPOSPluNo += nModifier;
			}

			m_arrayLinkPluNo.SetAt( n, nEPOSPluNo );
		}
	}
}

/**********************************************************************/

BOOL CPropPagePluLink::OnApply()
{
	CWnd* pWnd = GetFocus();

	if ( pWnd == &m_buttonScan )
	{
		AddScannedPlu();
		return FALSE;
	}
	else
	{
		OnOK();
		return TRUE;
	}
}

/**********************************************************************/

void CPropPagePluLink::OnOK()
{
	CPropertyPage::OnOK();
}

/**********************************************************************/

void CPropPagePluLink::OnCancel()
{
	CPropertyPage::OnCancel();
}

/**********************************************************************/

