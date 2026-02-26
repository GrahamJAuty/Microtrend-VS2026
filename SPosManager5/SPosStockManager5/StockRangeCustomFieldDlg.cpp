#include "BarcodePluNoTable.h"
#include "MessageLogger.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "StockRangeCustomFieldDlg.h"
/**********************************************************************/
#define PROGRAM_FIELD_CATEGORY 100
#define PROGRAM_FIELD_CUSTOM_FIELD1 101
#define PROGRAM_FIELD_CUSTOM_FIELD2 102
#define PROGRAM_FIELD_CUSTOM_FIELD3 103
#define PROGRAM_FIELD_CUSTOM_FIELD4 104
#define PROGRAM_FIELD_CUSTOM_FIELD5 105
#define PROGRAM_FIELD_STOCKUNIT 106
#define PROGRAM_FIELD_BARCODE_LABELS 107
#define PROGRAM_FIELD_DISABLE_CHANGE 108
#define PROGRAM_FIELD_DISABLE_ORDER 109
/**********************************************************************/

CStockRangeCustomFieldDlg::CStockRangeCustomFieldDlg( CStockFilterArray& FilterArray, CWnd* pParent )
	: CSSDialog(CStockRangeCustomFieldDlg::IDD, pParent), m_FilterArray ( FilterArray )
{
	//{{AFX_DATA_INIT(CStockRangeCustomFieldDlg)
	m_strStockCodeFrom = _T("");
	m_strStockCodeTo = _T("");
	//}}AFX_DATA_INIT
	m_bModifiedStock = FALSE;

	CDataManagerInfo info;
	DataManagerNonDb.OpenStockUnit( DB_READONLY, info );

	//CATEGORY
	DataManager.GetSortedCategoryList( m_arrayCategoryNo );

	//CUSTOM FIELDS
	for ( int nFieldNo = 1; nFieldNo <= 5; nFieldNo++ )
	{
		if ( DataManagerNonDb.CustomFieldNames.CheckName( nFieldNo ) == TRUE )
		{
			DataManager.SetActiveCustomFieldNo( nFieldNo );
			
			if ( DataManager.GetActiveCustomField() -> GetSize() > 0 )
			{
				m_arrayCustomFieldValues[nFieldNo].Add(0);
				for ( int v = 0; v < DataManager.GetActiveCustomField() -> GetSize(); v++ )
					m_arrayCustomFieldValues[nFieldNo].Add( DataManager.GetActiveCustomField() -> GetNumber(v) );
			}
		}
	}

	//STOCK UNIT
	
}

/**********************************************************************/

CStockRangeCustomFieldDlg::~CStockRangeCustomFieldDlg()
{
	CDataManagerInfo info;
	DataManagerNonDb.CloseStockUnit( info );
}

/**********************************************************************/

void CStockRangeCustomFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockRangeCustomFieldDlg)
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_STOCKCODE_FROM, m_strStockCodeFrom);
	DDV_MaxChars(pDX, m_strStockCodeFrom, Stock::StockCode.Max);
	DDX_Text(pDX, IDC_EDIT_STOCKCODE_TO, m_strStockCodeTo);
	DDV_MaxChars(pDX, m_strStockCodeTo, Stock::StockCode.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockRangeCustomFieldDlg, CDialog)
	//{{AFX_MSG_MAP(CStockRangeCustomFieldDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE_FROM, OnKillFocusStockCode)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE_TO, OnKillFocusStockCode)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_MESSAGE(WM_GET_CELLTYPE, GetCellType)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockRangeCustomFieldDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	FillCategoryCombo();	

	m_listSettings.SubclassDlgItem ( IDC_LIST, this );
	m_listSettings.InsertColumn ( 1, "Field", LVCFMT_LEFT, 180 );
	m_listSettings.InsertColumn ( 2, "Value", LVCFMT_LEFT, 260 );
	m_listSettings.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listSettings.EnableVScroll( TRUE );
	
	AddList();

	return TRUE;  
}

/**********************************************************************/

void CStockRangeCustomFieldDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listSettings.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

BOOL CStockRangeCustomFieldDlg::PreTranslateMessage(MSG* pMsg) 
{
  if(pMsg->message==WM_KEYDOWN)
  {
	  if(pMsg->wParam==VK_RETURN )
	  {
		  if ( GetFocus() == &m_listSettings )
		  {
			  m_listSettings.HandleReturn(1);	
			  pMsg->wParam=NULL ;
		  }
	  }
  }
  return CDialog::PreTranslateMessage(pMsg);
}

/**********************************************************************/

void CStockRangeCustomFieldDlg::FillCategoryCombo()
{
	m_CategorySelector.SelectCategorySet ( m_FilterArray.GetCategoryFilter() );
	m_CategorySelector.IncludeAll();
	m_CategorySelector.IncludeCategorySets();
	m_CategorySelector.FillCategoryCombo ( &m_comboCategory );
}

/**********************************************************************/

void CStockRangeCustomFieldDlg::OnKillFocusStockCode() 
{
	UpdateData( TRUE, FALSE );
	m_strStockCodeFrom = BarcodePluNoTable.ConvertStockCode( m_strStockCodeFrom );
	m_strStockCodeTo = BarcodePluNoTable.ConvertStockCode( m_strStockCodeTo );
	UpdateData( FALSE );
}

/**********************************************************************/

void CStockRangeCustomFieldDlg::AddList()
{
	m_listSettings.DeleteAllItems();

	AddCategoryField();

	for ( int nFieldNo = 1; nFieldNo <= 5; nFieldNo++ )
		if ( DataManagerNonDb.CustomFieldNames.CheckName( nFieldNo ) == TRUE )
			AddCustomField( nFieldNo );

	if ( DataManagerNonDb.StockUnit.GetSize() != 0 )
		AddStockUnitField();
	
	if ( ( EcrmanOptions.GetLabelsProductAutoBatchFlag() ) && ( Sysset.IsBarmanSystem() ) )
		AddYesNoField( PROGRAM_FIELD_BARCODE_LABELS, "Barcode Labels" );
	
	AddYesNoField( PROGRAM_FIELD_DISABLE_CHANGE, "Disable Change by Plu Range" );
	AddYesNoField( PROGRAM_FIELD_DISABLE_ORDER, "Disable Automatic Ordering" );

}

/**********************************************************************/

void CStockRangeCustomFieldDlg::AddField( CCSV& csv )
{
	int nPos = m_listSettings.GetItemCount();
	m_listSettings.InsertItem ( nPos, "" );
	m_listSettings.SetItem ( nPos, 0, LVIF_TEXT, csv.GetString(1), 0, 0, 0, NULL );
	m_listSettings.SetItem ( nPos, 1, LVIF_TEXT, csv.GetString(2), 0, 0, 0, NULL );
	m_listSettings.SetItemData ( nPos, csv.GetInt(0) );
	m_listSettings.SetCheck ( nPos, FALSE );
}

/**********************************************************************/

void CStockRangeCustomFieldDlg::AddCategoryField()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader( TRUE );

	for ( int n = 0; n < m_arrayCategoryNo.GetSize(); n++ )
	{
		int nCatNo = m_arrayCategoryNo.GetAt(n);
		
		CString strName = DataManager.Category.GetText(nCatNo);
		if ( "" == strName )
			strName = DataManager.Category.GetDefaultText(nCatNo);

		CString strCellText, strListText;
		strCellText.Format( "C%3.3d  %s", nCatNo, (const char*) strName );
		strListText.Format( "C%3.3d,%s", nCatNo, (const char*) strName );
		arrayTexts.AddText( strListText, strCellText );
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo( arrayTexts );
	m_arrayCellTypes.Add( info );

	CCSV csv;
	csv.Add( PROGRAM_FIELD_CATEGORY );
	csv.Add( "Category" );
	csv.Add( info.m_strCurrent );
	AddField( csv );	
}

/**********************************************************************/

void CStockRangeCustomFieldDlg::AddStockUnitField()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader( TRUE );

	for ( int n = 0; n < DataManagerNonDb.StockUnit.GetSize(); n++ )
		arrayTexts.AddTabbedText( DataManagerNonDb.StockUnit.GetText(n) );

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo( arrayTexts );
	m_arrayCellTypes.Add( info );

	CCSV csv;
	csv.Add( PROGRAM_FIELD_STOCKUNIT );
	csv.Add( "Stock Unit" );
	csv.Add( info.m_strCurrent );
	AddField( csv );	
}

/**********************************************************************/

void CStockRangeCustomFieldDlg::AddYesNoField( int nType, const char* szText)
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddYesNoTexts();

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo( arrayTexts );
	m_arrayCellTypes.Add( info );

	CCSV csv;
	csv.Add( nType );
	csv.Add( szText );
	csv.Add( info.m_strCurrent );
	AddField( csv );	
}

/**********************************************************************/

void CStockRangeCustomFieldDlg::AddCustomField( int nFieldNo )
{
	DataManager.SetActiveCustomFieldNo( nFieldNo );
			
	if ( DataManager.GetActiveCustomField() -> GetSize() > 0 )
	{
		CellTypeInfoTextBuffer arrayTexts;
		arrayTexts.AddNoChangeHeader( TRUE );
		arrayTexts.AddTabbedText( "N/A" );

		for ( int v = 0; v < DataManager.GetActiveCustomField() -> GetSize(); v++ )
		{
			arrayTexts.AddTabbedText( DataManager.GetActiveCustomField() -> GetName(v) );
		}
		
		CellTypeInfo info;
		info.m_strComboTabs = "5,50";
		info.ResetForCombo( arrayTexts );
		m_arrayCellTypes.Add( info );

		CCSV csv;
		csv.Add( PROGRAM_FIELD_CUSTOM_FIELD1 + nFieldNo - 1 );
		csv.Add( DataManagerNonDb.CustomFieldNames.GetName ( nFieldNo ) );
		csv.Add( info.m_strCurrent );
		AddField( csv );
	}
}

/**********************************************************************/

LRESULT CStockRangeCustomFieldDlg::GetCellType(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);
	
	int nCol = pCellType -> m_nCol;
	int nRow = pCellType -> m_nRow;

	if ( nCol != 1 )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	if ( nRow < 0 || nRow >= m_arrayCellTypes.GetSize() )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	pCellType -> m_nType = m_arrayCellTypes[nRow].m_nType;
	pCellType -> m_nSSEditType = m_arrayCellTypes[nRow].m_nSSEditType;
	pCellType -> m_strSSEditFormat = m_arrayCellTypes[nRow].m_strSSEditFormat;
	pCellType -> m_nSSEditWidth = m_arrayCellTypes[nRow].m_nSSEditWidth;
	pCellType -> m_nComboSelect = m_arrayCellTypes[nRow].m_nComboSelect;
	
	return 1;
}

/**********************************************************************/

LRESULT CStockRangeCustomFieldDlg::PopulateComboList(WPARAM wParam, LPARAM lParam)
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
	m_listSettings.GetWindowRect(&obWindowRect);
	
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

LRESULT CStockRangeCustomFieldDlg::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam) 
{
	m_listSettings.AllowListScroll( TRUE );
	m_listSettings.SetEditCell( -1, -1 );

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
			case CELL_TYPE_COMBO_NORMAL:
			case CELL_TYPE_COMBO_TABBED:
				{
					int nOldSel = m_arrayCellTypes[nRow].m_nComboSelect;
					int nNewSel = atoi( strText ) - 1;
					{
						CCSV csv( info.m_strComboCellText );
						if ( ( nNewSel >= 0 ) && ( nNewSel < csv.GetSize() ) )
						{
							m_arrayCellTypes[nRow].m_nComboSelect = nNewSel;
							m_listSettings.SetItem ( nRow, 1, LVIF_TEXT, csv.GetString( nNewSel ), 0, 0, 0, NULL );
						}
						else if ( ( nOldSel >= 0 ) && ( nOldSel < csv.GetSize() ) )
						{
							m_arrayCellTypes[nRow].m_nComboSelect = nOldSel;
							m_listSettings.SetItem ( nRow, 1, LVIF_TEXT, csv.GetString( nOldSel ), 0, 0, 0, NULL );
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

void CStockRangeCustomFieldDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusStockCode();

		StatusProgress.Lock( TRUE, "Updating Stock Database" );
		int nCount = UpdateProgram();
		StatusProgress.Unlock();

		switch ( nCount )
		{
		case 0:
			Prompter.Info ( "No records were changed" );
			break;

		case 1:
			Prompter.Info ( "1 record was changed" );
			break;

		default:
			{
				CString strBuf;
				strBuf.Format ( "%d records were changed", nCount );
				Prompter.Info ( strBuf );
			}
			break;
		}

		MessageLogger.LogSystem( 3, 3, FALSE, nCount );

		EndDialog ( IDOK );
	}
}

/**********************************************************************/

int CStockRangeCustomFieldDlg::UpdateProgram()
{
	CCategorySetCSVRecord CategorySet;
	CategorySet.CreateCategorySet ( m_CategorySelector.GetSelection() );
		
	int nStartIdx, nRange;
	m_FilterArray.GetArrayIdxRange( m_strStockCodeFrom, m_strStockCodeTo, nStartIdx, nRange );

	int nCount = 0;
	
	for ( int nOffset = 0; nOffset < nRange; nOffset++ )
	{
		StatusProgress.SetPos ( nOffset, nRange ); 

		CStockCSVRecord StockRecord;
		int nStockIdx = m_FilterArray.GetStockItemIndexAt ( nStartIdx + nOffset );
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );

		CString strStockCode = StockRecord.GetStockCode();
		if ( ( strStockCode < m_strStockCodeFrom ) || ( strStockCode > m_strStockCodeTo ) )
			continue;

		if ( CategorySet.GetCategoryStatus ( StockRecord.GetCategory() ) == FALSE )
			continue;

		for ( int n = 0; n < m_listSettings.GetItemCount(); n++ )
		{
			if ( n >= m_arrayCellTypes.GetSize() )
				break;

			CellTypeInfo info = m_arrayCellTypes[n];

			switch ( info.m_nType )
			{
			case CELL_TYPE_COMBO_NORMAL:
			case CELL_TYPE_COMBO_TABBED:
				break;

			default:
				continue;
			}

			if ( ( info.m_nComboSelect < 1 ) || ( info.m_nComboSelect > info.m_nMaxInt ) )
				continue;

			int nCellInt = info.m_nComboSelect - 1;
			int nType = m_listSettings.GetItemData( n );
			int nFieldNo = 0;
			
			switch( nType )
			{
			case PROGRAM_FIELD_CUSTOM_FIELD1:	nFieldNo = 1;	break;
			case PROGRAM_FIELD_CUSTOM_FIELD2:	nFieldNo = 2;	break;
			case PROGRAM_FIELD_CUSTOM_FIELD3:	nFieldNo = 3;	break;
			case PROGRAM_FIELD_CUSTOM_FIELD4:	nFieldNo = 4;	break;
			case PROGRAM_FIELD_CUSTOM_FIELD5:	nFieldNo = 5;	break;

			case PROGRAM_FIELD_CATEGORY:
				if ( nCellInt >= 0 && nCellInt < m_arrayCategoryNo.GetSize() )
					StockRecord.SetCategory( m_arrayCategoryNo.GetAt( nCellInt ) );
				break;

			case PROGRAM_FIELD_STOCKUNIT:
				if ( nCellInt >= 0 && nCellInt < DataManagerNonDb.StockUnit.GetSize() )
					StockRecord.SetStockUnitName( DataManagerNonDb.StockUnit.GetText( nCellInt ) );
				break;

			case PROGRAM_FIELD_BARCODE_LABELS:
				StockRecord.SetAutoLabelFlag( 0 == nCellInt );
				break;

			case PROGRAM_FIELD_DISABLE_CHANGE:
				StockRecord.SetDisablePluRangeFlag( 0 == nCellInt );
				break;

			case PROGRAM_FIELD_DISABLE_ORDER:
				StockRecord.SetDisableAutoOrderFlag( 0 == nCellInt );
				break;
			}

			if ( nFieldNo != 0 )
			{
				if ( 0 == nCellInt )
					StockRecord.SetCustomFieldValueNo( nFieldNo, 0);
				else if ( ( nCellInt >= 1 ) && ( nCellInt <= m_arrayCustomFieldValues[ nFieldNo ].GetSize() ) )
					StockRecord.SetCustomFieldValueNo( nFieldNo, m_arrayCustomFieldValues[ nFieldNo ].GetAt( nCellInt ) );
			}
		}

		DataManager.Stock.SetAt ( nStockIdx, StockRecord );
		m_bModifiedStock = TRUE;
		nCount++;
	}
	
	return nCount;
}

/**********************************************************************/
