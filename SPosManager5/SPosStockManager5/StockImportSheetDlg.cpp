/**********************************************************************/
#include "StockImportSheetDlg.h"
/**********************************************************************/

CStockImportSheetDlg::CStockImportSheetDlg( int nSpIdx, int nStocktakeNo, CWnd* pParent)
: CSSDialog(CStockImportSheetDlg::IDD, pParent), m_ImportSheetHelpers( nSpIdx, nStocktakeNo )
{
	//{{AFX_DATA_INIT(CStockImportSheetDlg)
	//}}AFX_DATA_INIT
	m_nSpIdx = nSpIdx;
	m_nCopies = 1;
}

/**********************************************************************/

void CStockImportSheetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockImportSheetDlg)
	DDX_Control(pDX, IDC_RADIO_COPY1, m_radioCopy1);
	DDX_Control(pDX, IDC_RADIO_COPY2, m_radioCopy2);
	DDX_Control(pDX, IDC_BUTTON_SET, m_buttonSet);
	DDX_Control(pDX, IDC_COMBO_COPIES, m_comboCopies);
	DDX_Control(pDX, IDC_EDIT_DESTINATION, m_editDestination);
	DDX_Control(pDX, IDC_COMBO_SORTORDER, m_comboSortOrder);
	DDX_Control(pDX, IDC_CHECK_ALPHASORT, m_checkAlphaSort);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockImportSheetDlg, CDialog)
	//{{AFX_MSG_MAP(CStockImportSheetDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_RADIO_COPY1, OnRadioCopy1)
	ON_BN_CLICKED(IDC_RADIO_COPY2, OnRadioCopy2)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockImportSheetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_listFields.SubclassDlgItem ( IDC_LIST, this );
	m_listFields.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFields.InsertColumn ( 0, "Fields", LVCFMT_LEFT, 400 );
	m_listFields.SetLockItemZeroFlag( TRUE );

	//READ LINE OF SELECTED FIELDS IF AVAILABLE
	CString strBuffer = "";
	{
		CSSFile fileFields;
		CFilenameUpdater FnUp(SysFiles::StockImportSheetDef);
		if (fileFields.Open(FnUp.GetFilenameToUse(), "rb") == TRUE)
		{
			fileFields.ReadString(strBuffer);
			fileFields.Close();
		}
		else
		{
			strBuffer = "";
		}
	}

	int nSortOrder = SH_STKLVL_NONE;
	int nCopies = 1;
	bool bAlphaSort = FALSE;

	{
		BOOL bTagged[ MAX_STOCK_IMPORTSHEET_FIELD + 1 ];
		for ( int n = 0; n <= MAX_STOCK_IMPORTSHEET_FIELD; n++ )
		{
			bTagged[n] = ( n == 0 );
		}

		CArray<int,int> arrayFields;
		arrayFields.Add(0);

		CCSV csvFields ( strBuffer ); 
		for ( int nPos = 0; nPos < csvFields.GetSize(); nPos++ )
		{
			int nField = csvFields.GetInt(nPos);
			if ( ( nField >= 1 ) && ( nField <= MAX_STOCK_IMPORTSHEET_FIELD ) )
			{
				if ( bTagged[nField] == FALSE )
				{
					bTagged[nField] = TRUE;
					arrayFields.Add( nField );
				}
			}
			else if ( ( nField >= SH_STKLVL_NONE ) && ( nField <= 200 ))
			{
				nSortOrder = nField;
			}
			else if ((nField >= 201) && (nField <= 210))
			{
				nCopies = nField - 200;
			}
			else if (301 == nField)
			{
				bAlphaSort = TRUE;
			}
		}

		int nTaggedCount = arrayFields.GetSize();

		{
			CArray<int,int> arrayFieldOrder;
			arrayFieldOrder.Add( STOCK_IMPORTSHEET_DESCRIPTION );
			arrayFieldOrder.Add( STOCK_IMPORTSHEET_CATEGORY );
			arrayFieldOrder.Add( STOCK_IMPORTSHEET_SUPPLIER );
			arrayFieldOrder.Add( STOCK_IMPORTSHEET_SHEETCODE );
			arrayFieldOrder.Add( STOCK_IMPORTSHEET_SUNAME );
			arrayFieldOrder.Add( STOCK_IMPORTSHEET_SUQTY );
			arrayFieldOrder.Add( STOCK_IMPORTSHEET_DUNAME );
			arrayFieldOrder.Add( STOCK_IMPORTSHEET_DUQTY );
			
			for ( int n = 0; n < arrayFieldOrder.GetSize(); n++ )
			{
				int nField = arrayFieldOrder.GetAt(n);

				if ( bTagged[nField] == FALSE )
				{
					arrayFields.Add(nField);
				}
			}
		}

		for ( int n = 0; n < arrayFields.GetSize(); n++ )
		{
			AddFieldToList( arrayFields.GetAt(n), n < nTaggedCount );
		}
	}

	//TICK THE OPTIONS THAT WERE FOUND IN THE FILE	
	CReportConsolidationArray<CSortedIntItem> arrayHeader;

	CCSV csvFields ( strBuffer ); 
	for ( int nPos = 0; nPos < csvFields.GetSize(); nPos++ )
	{
		CString strField = csvFields.GetString ( nPos );
		
		if ( ::TestNumeric( strField ) == TRUE )
		{
			CSortedIntItem item;
			item.m_nItem = atoi( strField );
			arrayHeader.Consolidate( item );
		}
	}

	//STOCKCODE IS ALREADY SELECTED
	int nNextTickPos = 1;
	for ( int n = 1; n < m_listFields.GetItemCount(); n++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( n, ListItem );
		
		CSortedIntItem item;
		item.m_nItem = ListItem.m_nData;
		
		int nPos;
		if ( arrayHeader.Find( item, nPos ) == TRUE )
		{
			m_listFields.SetItemTagState( n, 1 );
			m_listFields.SwapItems( n, nNextTickPos );
			m_listFields.Update( nNextTickPos );
			nNextTickPos++;
		}
	}

	m_listFields.SetItemTagState( 0, 1 );
	m_editDestination.SetWindowText ( m_ImportSheetHelpers.GetImportSheetFilename() );

	FillSortOrderCombo( nSortOrder );

	for (int n = 2; n <= 10; n++)
	{
		CString str = "";
		str.Format("%d", n);
		m_comboCopies.AddString(str);
	}

	if (nCopies > 1)
	{
		m_comboCopies.SetCurSel(nCopies - 2);
		OnRadioCopy1();
	}
	else
	{
		m_comboCopies.SetCurSel(0);
		OnRadioCopy2();
	}

	m_checkAlphaSort.SetCheck(bAlphaSort);

	return TRUE;  
}

/**********************************************************************/

void CStockImportSheetDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CStockImportSheetDlg::AddFieldToList( int nItemData, bool bTagged )
{
	CSSListTaggedSelectItem item;
	item.m_strText = m_ImportSheetHelpers.GetFieldName( nItemData );
	item.m_nData = nItemData;
	item.SetTagState( bTagged ? 1 : 0 );
	m_listFields.AddItemToList( item );
}

/**********************************************************************/

void CStockImportSheetDlg::FillSortOrderCombo( int nSortOrder )
{
	CArray<int,int> arraySortOrders;
	arraySortOrders.Add( SH_STKLVL_NONE );
	arraySortOrders.Add( SH_STKLVL_SUPP );
	arraySortOrders.Add( SH_STKLVL_SUPP_CAT );
	arraySortOrders.Add( SH_STKLVL_CAT );
	arraySortOrders.Add( SH_STKLVL_CAT_SUPP );
	
	m_comboSortOrder.ResetContent();

	for ( int n = 0; n < arraySortOrders.GetSize(); n++ )
	{
		CString strLine = "";

		int nSortType = arraySortOrders.GetAt(n);

		switch( nSortType )
		{
		case SH_STKLVL_NONE:		strLine = "Stock Code";						break;
		case SH_STKLVL_SUPP:		strLine = "Supplier";						break;
		case SH_STKLVL_SUPP_CAT:	strLine = "Supplier Category";				break;
		case SH_STKLVL_CAT:			strLine = "Category";						break;
		case SH_STKLVL_CAT_SUPP:	strLine = "Category Supplier";				break;
		}

		int nPos = m_comboSortOrder.AddString( strLine );
		m_comboSortOrder.SetItemData( nPos, nSortType );
	}

	m_comboSortOrder.SelectUsingItemData( nSortOrder, 0 );
}

/**********************************************************************/

void CStockImportSheetDlg::OnRadioCopy1()
{
	m_radioCopy1.SetCheck(TRUE);
	m_radioCopy2.SetCheck(FALSE);
	m_comboCopies.EnableWindow(TRUE);
	m_buttonSet.EnableWindow(FALSE);
}

/**********************************************************************/

void CStockImportSheetDlg::OnRadioCopy2()
{
	m_radioCopy1.SetCheck(FALSE);
	m_radioCopy2.SetCheck(TRUE);
	m_comboCopies.EnableWindow(FALSE);
	m_buttonSet.EnableWindow(TRUE);
}

/**********************************************************************/

void CStockImportSheetDlg::OnButtonSave() 
{
	UpdateData( TRUE, FALSE );

	CSSFile fileFields;
	CFilenameUpdater FnUp( SysFiles::StockImportSheetDef );
	if ( fileFields.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CreateSavedSettingsLine();
		fileFields.WriteLine ( m_strSavedSettingsLine );
	}
}

/**********************************************************************/

void CStockImportSheetDlg::OnButtonSet()
{
	char buffer[512];

	CString strTitle = "Stock ImportSheet Filename";
	CFileDialog dlgFile(FALSE, "csv", "importsheet", OFN_HIDEREADONLY | OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this);
	dlgFile.m_ofn.lpstrTitle = strTitle;

	CString strLastFile = "";
	m_editDestination.GetWindowText(strLastFile);
	strcpy_s(buffer, strLastFile);
	dlgFile.m_ofn.lpstrFile = buffer;
	dlgFile.m_ofn.nMaxFile = 512;

	if (dlgFile.DoModal() == IDOK)
	{
		m_editDestination.SetWindowText(dlgFile.GetPathName());
	}
}

/**********************************************************************/

bool CStockImportSheetDlg::CreateSavedSettingsLine()
{
	bool bGotQtyField = FALSE;

	CCSV csv;
	for ( int nPos = 0; nPos < m_listFields.GetItemCount(); nPos++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( nPos, ListItem );
		
		if ( ListItem.GetTagState() == 1 )
		{
			csv.Add( ListItem.m_nData );

			switch( ListItem.m_nData )
			{
			case STOCK_IMPORTSHEET_SUQTY:
			case STOCK_IMPORTSHEET_DUQTY:
				bGotQtyField = TRUE;
				break;
			}
		}
	}

	int nSortOrder = m_comboSortOrder.GetItemDataOfSelection();
	
	switch( nSortOrder )
	{
	case SH_STKLVL_SUPP:		
	case SH_STKLVL_SUPP_CAT:	
	case SH_STKLVL_CAT:			
	case SH_STKLVL_CAT_SUPP:	
		break;	

	default:
		nSortOrder = SH_STKLVL_NONE;
		break;
	}

	csv.Add( nSortOrder );

	if (m_radioCopy1.GetCheck() == TRUE)
	{
		csv.Add(202 + m_comboCopies.GetCurSel());
	}
	else
	{
		csv.Add(201);
	}

	if (IsTicked(m_checkAlphaSort))
	{
		csv.Add(301);
	}

	m_strSavedSettingsLine = csv.GetLine();
	return bGotQtyField;
}

/**********************************************************************/

void CStockImportSheetDlg::OnButtonDown() 
{
	int nSel = m_listFields.GetCurSel();
	if ( nSel > 0 && nSel < m_listFields.GetItemCount() - 1 )
	{
		m_listFields.SwapItems( nSel, nSel + 1 );
	}
}

/**********************************************************************/

void CStockImportSheetDlg::OnButtonUp() 
{
	int nSel = m_listFields.GetCurSel();
	if ( nSel > 1 && nSel < m_listFields.GetItemCount() )
	{
		m_listFields.SwapItems( nSel, nSel - 1 );
	}
}

/**********************************************************************/

void CStockImportSheetDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( CreateSavedSettingsLine() == FALSE )
		{
			Prompter.Error( "The ImportSheet must include a quantity field" );
		}
		else
		{
			CString strLastFile = "";
			m_editDestination.GetWindowText(strLastFile);

			{
				CSSFile file;
				CFilenameUpdater FnUp(SysFiles::LastImportSheetFile, m_nSpIdx);
				if (file.Open(FnUp.GetFilenameToUse(), "wb") == TRUE)
				{
					file.WriteLine(strLastFile);
					file.Close();
				}
			}

			if (m_radioCopy1.GetCheck() == TRUE)
			{
				m_nCopies = m_comboCopies.GetCurSel() + 2;
			}
			else
			{
				m_nCopies = 1;
			}

			EndDialog(IDOK);
		}
	}
}

/**********************************************************************/

