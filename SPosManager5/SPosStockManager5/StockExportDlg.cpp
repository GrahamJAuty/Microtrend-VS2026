/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ExportDate.h"
#include "BarcodePluNoTable.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockExportDlg.h"
/**********************************************************************/

CStockExportDlg::CStockExportDlg( CStockFilterArray& FilterArray, int nStartPos, CWnd* pParent)
: CSSDialog(CStockExportDlg::IDD, pParent), m_FilterArray ( FilterArray )
{
	//{{AFX_DATA_INIT(CStockExportDlg)
	//}}AFX_DATA_INIT
	m_FilterArray.GetDefaultRange ( 0, m_strStockFrom, m_strStockTo );
}

/**********************************************************************/

void CStockExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockExportDlg)
	DDX_Control(pDX, IDC_EDIT_DESTINATION, m_editDestination);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_STOCK_RANGE_START, m_strStockFrom);
	DDV_MaxChars(pDX, m_strStockFrom, Stock::StockCode.Max);
	DDX_Text(pDX, IDC_EDIT_STOCK_RANGE_END, m_strStockTo);
	DDV_MaxChars(pDX, m_strStockTo, Stock::StockCode.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockExportDlg, CDialog)
	//{{AFX_MSG_MAP(CStockExportDlg)
	ON_WM_MEASUREITEM()
	ON_EN_KILLFOCUS(IDC_EDIT_STOCK_RANGE_END, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCK_RANGE_START, OnKillFocusStockCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockExportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_listFields.SubclassDlgItem ( IDC_LIST, this );
	m_listFields.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFields.InsertColumn ( 0, "Fields", LVCFMT_LEFT, 200 );
	m_listFields.SetLockItemZeroFlag( TRUE );

	//USE STOCK DATABASE HEADER TO POPULATE LIST BOX
	int nSupplierCount = StockOptions.GetStockMaxSuppliersPerItem();
	CCSV csvHeader ( CStockCSVRecord::GetHeader ( nSupplierCount ) );

	CStringArray arrayFields;
	CReportConsolidationArray<CSortedStringItem> arrayAvailableFields;
	CSortedStringItem item;
	
	for ( int nPos = 0; nPos < csvHeader.GetSize(); nPos++ )
	{
		CString strField = csvHeader.GetString ( nPos );
		bool bAddToList = ( strField != "" ) && ( strField != Stock::StockCode.Label );

		if ( ( TRUE == bAddToList ) && ( StockOptions.GetStockSalesEntryType() == 0 ) )
		{
			CString strLabel = Stock::DURetail.Label;
			if ( strField == strLabel + "1" )		bAddToList = FALSE;
			else if ( strField == strLabel + "2" )	bAddToList = FALSE;
			else if ( strField == strLabel + "3" )	bAddToList = FALSE;
			else if ( strField == strLabel + "4" )	bAddToList = FALSE;
			else if ( strField == strLabel + "5" )	bAddToList = FALSE;
		}

		if ( TRUE == bAddToList )
		{
			CString strLabel = CustomFieldValue::Key.Label;
			if ( strField == strLabel + "1" )		bAddToList = ( DataManagerNonDb.CustomFieldNames.CheckName(1) ) && ( DataManager.CustomFieldValue[1].GetSize() > 0 );
			else if ( strField == strLabel + "2" )	bAddToList = ( DataManagerNonDb.CustomFieldNames.CheckName(2) ) && ( DataManager.CustomFieldValue[2].GetSize() > 0 ); 
			else if ( strField == strLabel + "3" )	bAddToList = ( DataManagerNonDb.CustomFieldNames.CheckName(3) ) && ( DataManager.CustomFieldValue[3].GetSize() > 0 ); 
			else if ( strField == strLabel + "4" )	bAddToList = ( DataManagerNonDb.CustomFieldNames.CheckName(4) ) && ( DataManager.CustomFieldValue[4].GetSize() > 0 ); 
			else if ( strField == strLabel + "5" )	bAddToList = ( DataManagerNonDb.CustomFieldNames.CheckName(5) ) && ( DataManager.CustomFieldValue[5].GetSize() > 0 ); 
		}

		if ( TRUE == bAddToList )
		{
			item.m_strItem = strField;
			arrayFields.Add( item.m_strItem );
			item.m_strItem.MakeUpper();
			arrayAvailableFields.Consolidate( item );
		}
	}
	
	//READ LINE OF SELECTED FIELDS IF AVAILABLE
	CString strBuffer;
	CSSFile fileFields;
	CFilenameUpdater FnUp( SysFiles::StockCSVDef );
	if ( fileFields.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE ) 
	{
		fileFields.ReadString ( strBuffer );
		fileFields.Close();
	}
	else
	{
		strBuffer = "";
	}

	//STOCK CODE IS ALWAYS TICKED
	AddFieldToList( Stock::StockCode.Label, 1 );

	//ADD OTHER TICKED FIELDS TO LIST IN SAVED ORDER
	CCSV csvFields ( strBuffer ); 
	for ( int nPos = 0; nPos < csvFields.GetSize(); nPos++ )
	{
		CString strField = csvFields.GetString ( nPos );
		CString strUpper = strField;
		strUpper.MakeUpper();

		CSortedStringItem item;
		item.m_strItem = strUpper;

		int nFindPos;
		if ( arrayAvailableFields.Find( item, nFindPos ) == TRUE )
		{
			AddFieldToList( strField, 1 );
			arrayAvailableFields.RemoveAt( nFindPos );
		}
	}

	//ADD UNTICKED FIELDS TO LIST IN DEFAULT HEADER ORDER
	for ( int n = 0; n < arrayFields.GetSize(); n++ )
	{
		CString strField = arrayFields.GetAt(n);
		CString strUpper = strField;
		strUpper.MakeUpper();

		CSortedStringItem item;
		item.m_strItem = strUpper;

		int nFindPos;
		if ( arrayAvailableFields.Find( item, nFindPos ) == TRUE )
		{
			AddFieldToList( strField, 0 );
			arrayAvailableFields.RemoveAt( nFindPos );
		}

	}

	CString strFile = ExportDate.GetLastStockFile();	
	if ( strFile == "" )
	{
		strFile = dbDatabase.GetFolderPathData ( DataManager.GetActiveDbIdx() );
		strFile += "\\stock.csv";
	}

	m_editDestination.SetWindowText ( strFile );
	return TRUE;  
}

/**********************************************************************/

void CStockExportDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CStockExportDlg::AddFieldToList( const char* szText, int nTagState )
{
	CSSListTaggedSelectItem item;
	item.m_strText = szText;
	item.m_nData = 0;
	item.SetTagState(nTagState);
	m_listFields.AddItemToList( item );
}

/**********************************************************************/

void CStockExportDlg::OnButtonSave() 
{
	UpdateData( TRUE, FALSE );

	CSSFile fileFields;
	CFilenameUpdater FnUp( SysFiles::StockCSVDef );
	if ( fileFields.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CreateStockHeaderLine();
		fileFields.WriteLine ( m_strStockHeaderLine );
	}
}

/**********************************************************************/

void CStockExportDlg::OnKillFocusStockCode() 
{
	UpdateData( TRUE, FALSE );
	m_strStockFrom = BarcodePluNoTable.ConvertStockCode ( m_strStockFrom );
	m_strStockTo = BarcodePluNoTable.ConvertStockCode ( m_strStockTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CStockExportDlg::OnButtonSet() 
{
	char buffer [ 512 ];
	
	CString strTitle = "Export Stock CSV File";
	CFileDialog dlgFile ( FALSE, "csv", "stkxfer", OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFile.m_ofn.lpstrTitle = strTitle;
	
	CString strLastFile = "";
	m_editDestination.GetWindowText( strLastFile );
	strcpy_s ( buffer, strLastFile );
	dlgFile.m_ofn.lpstrFile = buffer;
	dlgFile.m_ofn.nMaxFile = 512;
	
	if ( dlgFile.DoModal() == IDOK )
		m_editDestination.SetWindowText ( dlgFile.GetPathName() );
}

/**********************************************************************/

void CStockExportDlg::AppendStockHeader ( CCSV& csv )
{
	for ( int nPos = 0; nPos < m_listFields.GetItemCount(); nPos++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( nPos, ListItem );
		
		if ( ListItem.GetTagState() == 1 )
			csv.Add( ListItem.m_strText );
	}
}

/**********************************************************************/

void CStockExportDlg::CreateStockHeaderLine()
{
	CCSV csv;
	AppendStockHeader ( csv );
	m_strStockHeaderLine = csv.GetLine();
}

/**********************************************************************/

void CStockExportDlg::OnButtonAll() 
{
	m_listFields.SelectAll();	
	m_FilterArray.GetDefaultRange ( 0, m_strStockFrom, m_strStockTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CStockExportDlg::OnButtonNone() 
{
	m_listFields.SelectNone();	
	UpdateData ( FALSE );
}

/**********************************************************************/

void CStockExportDlg::OnButtonDown() 
{
	int nSel = m_listFields.GetCurSel();
	if ( nSel > 0 && nSel < m_listFields.GetItemCount() - 1 )
		m_listFields.SwapItems( nSel, nSel + 1 );
}

/**********************************************************************/

void CStockExportDlg::OnButtonUp() 
{
	int nSel = m_listFields.GetCurSel();
	if ( nSel > 1 && nSel < m_listFields.GetItemCount() )
		m_listFields.SwapItems( nSel, nSel - 1 );
}

/**********************************************************************/

void CStockExportDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusStockCode();
		CString strLastFile;
		m_editDestination.GetWindowText( strLastFile );
		ExportDate.SetLastStockFile ( strLastFile );
		CreateStockHeaderLine();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

