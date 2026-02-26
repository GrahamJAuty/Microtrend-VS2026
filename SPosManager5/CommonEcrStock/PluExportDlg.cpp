/**********************************************************************/
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
/**********************************************************************/
#include "..\SPosStockManager5\PluFilterDlgStockman.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ExportDate.h"
#include "PluDatabaseHeader.h"
#include "BarcodePluNoTable.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluExportDlg.h"
/**********************************************************************/

CPluExportDlg::CPluExportDlg( CPluFilterArray& FilterArray, int nStartPos, CWnd* pParent)
: CSSDialog(CPluExportDlg::IDD, pParent), m_FilterArray ( FilterArray )
{
	//{{AFX_DATA_INIT(CPluExportDlg)
	m_bChangesOnly = FALSE;
	//}}AFX_DATA_INIT
	__int64 nPluFrom, nPluTo;
	m_FilterArray.GetDefaultRange ( 0, nPluFrom, nPluTo );
	::FormatPluNo( nPluFrom, m_strPluFrom );
	::FormatPluNo( nPluTo, m_strPluTo );
}

/**********************************************************************/

void CPluExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluExportDlg)
	DDX_Control(pDX, IDC_CHECK_CHANGES, m_checkChangesOnly);
	DDX_Control(pDX, IDC_EDIT_DESTINATION, m_editDestination);
	DDX_Check(pDX, IDC_CHECK_CHANGES, m_bChangesOnly);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluExportDlg, CDialog)
	//{{AFX_MSG_MAP(CPluExportDlg)
	ON_WM_MEASUREITEM()
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_STATIC_FROM, OnStaticFrom)
	ON_BN_CLICKED(IDC_STATIC_TO, OnStaticTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluExportDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPluExportDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_RANGE_END ); }
/**********************************************************************/

void CPluExportDlg::ConsolidateString( CReportConsolidationArray<CSortedStringItem>& arraySorted, CString strField )
{
	CSortedStringItem item;
	item.m_strItem = strField;
	arraySorted.Consolidate( item );
}

/**********************************************************************/

void CPluExportDlg::ConsolidateString( CStringArray& arrayStrings, CReportConsolidationArray<CSortedStringItem>& arraySorted, CString strField )
{
	arrayStrings.Add( strField );

	CSortedStringItem item;
	item.m_strItem = strField.MakeUpper();
	arraySorted.Consolidate( item );
}

/**********************************************************************/

void CPluExportDlg::CreateAvailableFieldsArray( CStringArray& arrayFields, CReportConsolidationArray<CSortedStringItem>& arrayAvailableFields )
{
	arrayFields.RemoveAll();
	arrayAvailableFields.RemoveAll();

	CReportConsolidationArray<CSortedStringItem> arrayExcludedFields;

	ConsolidateString( arrayExcludedFields, Plu::PluNo.Label );

	if ( SysInfo.GetMaxBasePluLen() == 0 )
	{
		CSortedStringItem item;
	
		for ( int nMod = 1; nMod <= 9; nMod++ )
		{	
			CPluCSVRecord::GetModifierLabelEnable( nMod, FALSE, item.m_strItem );
			arrayExcludedFields.Consolidate( item );

			CPluCSVRecord::GetModifierLabelEposTextFlag( nMod, FALSE, item.m_strItem );
			arrayExcludedFields.Consolidate( item );

			CPluCSVRecord::GetModifierLabelEposText( nMod, FALSE, item.m_strItem );
			arrayExcludedFields.Consolidate( item );

			CPluCSVRecord::GetModifierLabelKeyTextFlag( nMod, FALSE, item.m_strItem );
			arrayExcludedFields.Consolidate( item );

			CPluCSVRecord::GetModifierLabelKeyText( nMod, FALSE, item.m_strItem );
			arrayExcludedFields.Consolidate( item );

			for ( int nPrice = 1; nPrice <= 10; nPrice++ )
			{
				CPluCSVRecord::GetModifierLabelPrice( nMod, nPrice, FALSE, item.m_strItem );
				arrayExcludedFields.Consolidate( item );
			}
		}

		ConsolidateString( arrayExcludedFields, Plu::KPImageModifier.Label );
	}

#ifdef STOCKMAN_SYSTEM
	ConsolidateString( arrayExcludedFields, Plu::ApparentLevel.Label );
	ConsolidateString( arrayExcludedFields, Plu::MinimumLevel.Label );
	ConsolidateString( arrayExcludedFields, Plu::SupplierRef.Label );
	ConsolidateString( arrayExcludedFields, Plu::SupplierNo.Label );
	ConsolidateString( arrayExcludedFields, Plu::NonStockItem.Label );
	ConsolidateString( arrayExcludedFields, Plu::EcrmanRecipeTable.Label );
#endif

	if (SysInfo.GotLoyaltyOrSmartPayLink() == FALSE)
	{
		ConsolidateString(arrayExcludedFields, Plu::Points.Label);
	}

	if ( SysInfo.GetLoyaltyOrSmartPayType() != LOYALTY_TYPE_SMP_V4)
	{
		ConsolidateString(arrayExcludedFields, Plu::StampWeight.Label);
	}

	ConsolidateString( arrayExcludedFields, "" );

	if ( DealerFlags.Get_LIPA_LJMU_ExportType() == 0 )
	{
		ConsolidateString( arrayExcludedFields, Plu::AgressoGLCode.Label );
		ConsolidateString( arrayExcludedFields, Plu::AgressoCostCentre.Label );
		ConsolidateString( arrayExcludedFields, Plu::AgressoProject.Label );
	}

	ConsolidateString( arrayExcludedFields, Plu::LegacyDiscountType.Label );
	
	//USE PLU DATABASE HEADER TO POPULATE LIST BOX
	CCSV csvHeaderPlu ( PluDatabaseHeader.GetDatabaseHeader() );

	CSortedStringItem item;
	
	for ( int nPos = 0; nPos < csvHeaderPlu.GetSize(); nPos++ )
	{
		int nIdx;
		item.m_strItem = csvHeaderPlu.GetString ( nPos ); 
		if ( arrayExcludedFields.Find( item, nIdx ) == FALSE )
		{
			ConsolidateString( arrayFields, arrayAvailableFields, item.m_strItem );
		}
	}

#ifdef STOCKMAN_SYSTEM

	int nSupplierCount = StockOptions.GetStockMaxSuppliersPerItem();
	CCSV csvHeaderStock ( CStockCSVRecord::GetHeader ( nSupplierCount ) );
	
	for ( int nPos = 0; nPos < csvHeaderStock.GetSize(); nPos++ )
	{
		CString strField = csvHeaderStock.GetString ( nPos );
		bool bAddToList = ( strField != "" );

		if ( StockOptions.GetStockSalesEntryType() == 0 )
		{
			CString strLabel = Stock::DURetail.Label;
			if ( strField == strLabel + "1" ) bAddToList = FALSE;
			else if ( strField == strLabel + "2" ) bAddToList = FALSE;
			else if ( strField == strLabel + "3" ) bAddToList = FALSE;
			else if ( strField == strLabel + "4" ) bAddToList = FALSE;
			else if ( strField == strLabel + "5" ) bAddToList = FALSE;
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
			ConsolidateString( arrayFields, arrayAvailableFields, strField );		
		}
	}

	ConsolidateString( arrayFields, arrayAvailableFields, "PluStockLink" );
	ConsolidateString( arrayFields, arrayAvailableFields, "StockQty" );
	
#endif
}

/**********************************************************************/

BOOL CPluExportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_PLU_RANGE_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_END, SS_NUM, Super.MaxPluLen(), "%s" );

	m_listFields.SubclassDlgItem ( IDC_LIST, this );
	m_listFields.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFields.InsertColumn ( 0, "Fields", LVCFMT_LEFT, 200 );
	m_listFields.SetLockItemZeroFlag( TRUE );

	CStringArray arrayFields;
	CReportConsolidationArray<CSortedStringItem> arrayAvailableFields;
	CreateAvailableFieldsArray( arrayFields, arrayAvailableFields );
	
	//READ LINE OF SELECTED FIELDS IF AVAILABLE
	CString strBuffer;
	CSSFile fileFields;
	CFilenameUpdater FnUp( SysFiles::PluCSVDef );
	if ( fileFields.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE ) 
	{
		fileFields.ReadString ( strBuffer );
		fileFields.Close();
	}
	else
	{
		strBuffer = "";
	}

	AddFieldToList( Plu::PluNo.Label, 1 );

	strBuffer = PluDatabaseHeader.GetUpdatedHeader( strBuffer );

	CCSV csvFields ( strBuffer ); 
	for ( int nPos = 0; nPos < csvFields.GetSize(); nPos++ )
	{
		CString strField = csvFields.GetString ( nPos );
		CString strUpper = strField;
		strUpper.MakeUpper();

		if ( strUpper == "CHANGESONLY" )
		{
			m_checkChangesOnly.SetCheck ( TRUE );
		}
		else
		{
			CSortedStringItem item;
			item.m_strItem = strUpper;

			int nFindPos;
			if ( arrayAvailableFields.Find( item, nFindPos ) == TRUE )
			{
				AddFieldToList( strField, 1 );
				arrayAvailableFields.RemoveAt( nFindPos );
			}
		}
	}

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

	CString strFile = ExportDate.GetLastPluFile();	
	if ( strFile == "" )
	{
		strFile = dbDatabase.GetFolderPathData ( DataManager.GetActiveDbIdx() );
		strFile += "\\plu.csv";
	}
	
	m_editDestination.SetWindowText ( strFile );
	return TRUE;  
}

/**********************************************************************/

void CPluExportDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CPluExportDlg::AddFieldToList( const char* szText, int nTagState )
{
	CSSListTaggedSelectItem item;
	item.m_strText = szText;
	item.m_nData = 0;
	item.SetTagState(nTagState);
	m_listFields.AddItemToList( item );
}

/**********************************************************************/

void CPluExportDlg::OnButtonSave() 
{
	UpdateData( TRUE, FALSE );

	CSSFile fileFields;
	CFilenameUpdater FnUp( SysFiles::PluCSVDef );
	if ( fileFields.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CreateSavedSettingsLine();
		fileFields.WriteLine ( m_strSavedSettingsLine );
	}
}

/**********************************************************************/

void CPluExportDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluExportDlg::OnButtonSet()
{
	char buffer[512];

	CString strTitle = "Export Plu CSV File";
	CFileDialog dlgFile(FALSE, "csv", "pluxfer", OFN_HIDEREADONLY | OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this);
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

void CPluExportDlg::AppendPluHeader ( CCSV& csv )
{
	for (int nPos = 0; nPos < m_listFields.GetItemCount(); nPos++)
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem(nPos, ListItem);

		if (ListItem.GetTagState() == 1)
		{
			csv.Add(ListItem.m_strText);
		}
	}
}

/**********************************************************************/

void CPluExportDlg::CreatePluHeaderLine()
{
	CCSV csv;
	AppendPluHeader ( csv );
	m_strPluHeaderLine = csv.GetLine();
}

/**********************************************************************/

void CPluExportDlg::CreateSavedSettingsLine()
{
	CCSV csv;
	AppendPluHeader ( csv );

	if (m_bChangesOnly == TRUE)
	{
		csv.Add("ChangesOnly");
	}

	m_strSavedSettingsLine = csv.GetLine();
}

/**********************************************************************/

void CPluExportDlg::OnButtonDown()
{
	int nSel = m_listFields.GetCurSel();
	if (nSel > 0 && nSel < m_listFields.GetItemCount() - 1)
	{
		m_listFields.SwapItems(nSel, nSel + 1);
	}
}

/**********************************************************************/

void CPluExportDlg::OnButtonUp()
{
	int nSel = m_listFields.GetCurSel();
	if (nSel > 1 && nSel < m_listFields.GetItemCount())
	{
		m_listFields.SwapItems(nSel, nSel - 1);
	}
}

/**********************************************************************/

void CPluExportDlg::OnButtonAll() 
{
	m_listFields.SelectAll();

	m_bChangesOnly = FALSE;
	__int64 nPluFrom, nPluTo;
	m_FilterArray.GetDefaultRange ( 0,nPluFrom, nPluTo );
	::FormatPluNo( nPluFrom, m_strPluFrom );
	::FormatPluNo( nPluTo, m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluExportDlg::OnButtonNone() 
{
	m_listFields.SelectNone();
}

/**********************************************************************/

void CPluExportDlg::OnStaticFrom() 
{
	CString strStart = "1";
	::AddLeading ( strStart, Super.MaxPluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
}

/**********************************************************************/

void CPluExportDlg::OnStaticTo() 
{
	CString strEnd = "9";
	::AddLeading ( strEnd, Super.MaxPluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CPluExportDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CString strLastFile;
		m_editDestination.GetWindowText( strLastFile );
		ExportDate.SetLastPluFile ( strLastFile );
		CreatePluHeaderLine();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

