/**********************************************************************/
#include "Append.h"
#include "ColumnManager.h"
#include "FileRemover.h"
#include "FilterTypes.h"
#include "NodeTypes.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockAuditOpeningClosing.h"
#include "StockEditOpeningDlg.h"
#include "StockEditOpeningRandomDlg.h"
#include "StockImportFilterPluOpenCloseDlg.h"
#include "StockImportFilterPluOpenCloseSheetDlg.h"
#include "StockImportFilterWrapperDlg.h"
#include "StockImportSheetDlg.h"
#include "StockImportSheetHelpers.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockListOpeningDlg.h"
/**********************************************************************/

void CStockListOpeningDlg::SetColumns()
{
	m_ColumnManager.Reset( TableNames::Opening );
	m_ColumnManager.AddColumn ( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 115, 115 );
	m_ColumnManager.AddColumn ( ColumnNames::Desc, "Description", LVCFMT_LEFT, 135, 170 );
	m_ColumnManager.AddColumn ( ColumnNames::Category, "Category", LVCFMT_LEFT, 80, 140 );
	m_ColumnManager.AddColumn ( ColumnNames::StockUnit, "Unit", LVCFMT_LEFT, 100, 100 );
	m_ColumnManager.AddColumn ( ColumnNames::Opening, "Opening", LVCFMT_RIGHT, 75, 75 );
	m_ColumnManager.AddColumn ( ColumnNames::Apparent, "Apparent", LVCFMT_RIGHT, 75, 75 );
	m_ColumnManager.LoadSettings();
}

/**********************************************************************/

void CStockListOpeningDlg::EditStockLevel()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nSelection = m_listStock.GetCurSel();

		if ( nSelection >= 0 && nSelection < m_pFilterArray -> GetSize() )
		{		
			m_pFilterArray -> SetArrayIdx ( nSelection );
			CStockEditOpeningDlg dlg ( m_listStock, m_pFilterArray, FALSE, this );
			dlg.DoModal();
			DisplayScreen ( m_pFilterArray -> GetArrayIdx() );
		}
	}
}

/**********************************************************************/

void CStockListOpeningDlg::EditStockLevelRandom()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CStockEditOpeningRandomDlg dlg( m_pFilterArray, FALSE, 0, this );
		dlg.DoModal();
		DisplayScreen ( m_pFilterArray -> GetArrayIdx() );
	}
}

/**********************************************************************/

void CStockListOpeningDlg::DisplayScreen ( int nIndex )
{
	int nSize = m_pFilterArray -> GetSize();
	
	CString strFiltered = "";
	if ( m_pFilterArray -> GetFilteredFlag() == TRUE )
		strFiltered = ", Filtered";

	CString strStocktake = "";
	if ( m_pFilterArray -> GetStocktakeExcludeCount() != 0 )
		strStocktake.Format ( ", %d in stocktake", m_pFilterArray -> GetStocktakeExcludeCount() );

	CString strInactive = "";
	if ( m_pFilterArray -> GetInactiveExcludeCount() != 0 )
		strInactive.Format ( ", %d inactive", m_pFilterArray -> GetInactiveExcludeCount() );

	CString strText;
	strText.Format ( "%s (%s%d of %d records%s%s%s)",
		GetTitle(),
		GetSpName(),
		nSize,
		DataManager.Stock.GetSize(),
		(const char*) strStocktake,
		(const char*) strInactive,
		(const char*) strFiltered );
	
	SetWindowText ( strText );

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listStock.SetCurSel ( nIndex );
	else
		m_listStock.SetCurSel ( nSize - 1 );

	m_listStock.Invalidate();
}

/**********************************************************************/

void CStockListOpeningDlg::SetFilterArray()
{
	m_pFilterArray = new CStockFilterArray ( FILTERTYPE_OPENING );
	m_pFilterArray ->SetSpIdx ( m_nSpIdx, 0 );
	m_pFilterArray ->SetStocktakeFilter( 0 );
	m_pFilterArray ->SetActiveItemOnlyFlag(TRUE);
	m_pFilterArray ->BuildRecordList();
}

/**********************************************************************/

void CStockListOpeningDlg::HandleImport()
{
	int nSelection = m_listStock.GetCurSel();

	CString strTitle = "";
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		strTitle = "Import Opening Stock by Barcode Number";
	else
		strTitle = "Import Opening Stock by Plu Number";

	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );

	CString strImportFile = StockOptions.GetImportOpeningPluName();
	if ( ( strImportFile.GetLength() == 0 ) || ( strImportFile.GetLength() > 25 ) )
		strImportFile = "opening";

	CFileDialog dlgFile ( TRUE, "csv", strImportFile, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFile.m_ofn.lpstrTitle = strTitle;
	dlgFile.m_ofn.lpstrInitialDir = strPath;
	
	if ( dlgFile.DoModal() == IDCANCEL )
		return;

	CString strImportPath = dlgFile.GetPathName();
	
	for ( int nIndex = 0; nIndex < m_ImportFilesArray.GetSize(); nIndex++ )
	{
		if ( strImportPath == m_ImportFilesArray.GetAt ( nIndex ) )
		{
			Prompter.Error ( "You have already imported the selected file." );
			return;
		}
	}

	CStockImportFilterArray arrayImport;
	CStockImportFilterPluOpenCloseDlg dlgImport( m_nSpIdx, FALSE, strImportPath, arrayImport, this );
	dlgImport.SetStocktakeNo(0);
	dlgImport.ProcessImportData();
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No opening stock data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing opening stock" );
	bool bResult = HandleImportInternal ( arrayImport, dlgImport.GetAction() );
	StatusProgress.Unlock();

	if ( bResult == TRUE )
	{
		COleDateTime date = COleDateTime::GetCurrentTime();
		CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
		CString strPrevEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV_EX );
		::Append( strPrevEx, strTempEx );

		m_ImportFilesArray.Add ( strImportPath );
		DisplayScreen ( nSelection );
		m_bImported = TRUE;
	}
}

/**********************************************************************/

bool CStockListOpeningDlg::HandleImportInternal( CStockImportFilterArray& arrayImport, int nAction )
{
	CSSFile fileAudit;
	COleDateTime date = COleDateTime::GetCurrentTime();
	CString strTempAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
	if ( fileAudit.Open ( strTempAudit, "ab" ) == FALSE )
		return FALSE;

	int nArraySize = arrayImport.GetSize();
	for ( int n = 0; n < nArraySize; n++ )
	{
		StatusProgress.SetPos( n, nArraySize );

		CStockImportFilterItem item;
		arrayImport.GetItem( n, item );

		if ( FALSE == item.m_bSelected )
			continue;

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( item.m_nStockIdx, StockRecord );
		int nSpNo = m_pFilterArray -> GetSpNo();
		
		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals ( item.m_nStockIdx, StockRecord, nSpNo, StockLevels );

		if ( StockLevels.GetStocktakeNo() != 0 )
			continue;

		double dOldOpeningQty = StockLevels.GetOpeningQty();
		double dNewOpeningQty = 0.0;

		switch ( nAction )
		{
		case SUBTRACT_STOCK_LEVELS:
			dNewOpeningQty = dOldOpeningQty - item.m_dImportSUQty;
			break;
			
		case OVERWRITE_STOCK_LEVELS:
			dNewOpeningQty = item.m_dImportSUQty;
			break;
			
		case ADD_STOCK_LEVELS:
		default:
			nAction = ADD_STOCK_LEVELS;
			dNewOpeningQty = dOldOpeningQty + item.m_dImportSUQty;
			break;
		}
		
		StockLevels.SetOpeningQty( dNewOpeningQty );
		DataManager.Stock.SetStockpointTotals ( item.m_nStockIdx, StockRecord, nSpNo, StockLevels );

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_OPENING );
		csvOut.Add ( 2 );
		csvOut.Add ( StockRecord.GetStockCode() );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
		csvOut.Add ( nAction );
		csvOut.AddStockAuditQty( 0.0 );
		csvOut.AddStockAuditQty ( item.m_dImportSUQty );
		csvOut.AddStockAuditQty ( dNewOpeningQty );
		fileAudit.WriteLine( csvOut.GetLine() );	
	}

	return TRUE;
}

/**********************************************************************/

void CStockListOpeningDlg::HandleImportWrapper()
{
	int nSelection = m_listStock.GetCurSel();
	CString strTitle = "Import Opening Stock by Wrapper Code";
	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );

	CString strImportFile = StockOptions.GetImportOpeningWrpName();
	if ( ( strImportFile.GetLength() == 0 ) || ( strImportFile.GetLength() > 25 ) )
		strImportFile = "opening";

	CFileDialog dlgFile ( TRUE, "csv", strImportFile, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFile.m_ofn.lpstrTitle = strTitle;
	dlgFile.m_ofn.lpstrInitialDir = strPath;
	
	if ( dlgFile.DoModal() == IDCANCEL )
		return;

	CString strImportPath = dlgFile.GetPathName();
	
	for ( int nIndex = 0; nIndex < m_ImportFilesArray.GetSize(); nIndex++ )
	{
		if ( strImportPath == m_ImportFilesArray.GetAt ( nIndex ) )
		{
			Prompter.Error ( "You have already imported the selected file." );
			return;
		}
	}
	
	CStockImportFilterArray arrayImport;
	CStockImportFilterWrapperDlg dlgImport( m_nSpIdx, FALSE, strImportPath, m_SuppRefFinder, arrayImport, this );
	dlgImport.ProcessImportData();

	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No opening stock data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing opening stock" );
	bool bResult = HandleImportWrapperInternal ( arrayImport, dlgImport.GetAction() );
	StatusProgress.Unlock();

	if ( bResult == TRUE )
	{
		COleDateTime date = COleDateTime::GetCurrentTime();
		CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
		CString strPrevEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV_EX );
		::Append( strPrevEx, strTempEx );

		m_ImportFilesArray.Add ( strImportPath );
		DisplayScreen ( nSelection );
		m_bImported = TRUE;
	}
}

/**********************************************************************/

bool CStockListOpeningDlg::HandleImportWrapperInternal( CStockImportFilterArray& arrayImport, int nAction )
{
	CSSFile fileAudit;
	COleDateTime date = COleDateTime::GetCurrentTime();
	CString strTempAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
	if ( fileAudit.Open ( strTempAudit, "ab" ) == FALSE )
		return FALSE;

	int nArraySize = arrayImport.GetSize();
	for ( int n = 0; n < nArraySize; n++ )
	{
		StatusProgress.SetPos( n, nArraySize );

		CStockImportFilterItem item;
		arrayImport.GetItem( n, item );

		if ( FALSE == item.m_bSelected )
			continue;
		
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( item.m_nStockIdx, StockRecord );
		
		CStockLevelsCSVRecord StockLevels;
		int nSpNo = m_pFilterArray -> GetSpNo();
		DataManager.Stock.GetStockpointTotals ( item.m_nStockIdx, StockRecord, nSpNo, StockLevels );

		if ( StockLevels.GetStocktakeNo() != 0 )
			continue;
	
		double dOldOpeningQty = StockLevels.GetOpeningQty();
		double dNewOpeningQty = 0.0;

		switch ( nAction )
		{
		case SUBTRACT_STOCK_LEVELS:
			dNewOpeningQty = dOldOpeningQty - item.m_dImportSUQty;
			break;
			
		case OVERWRITE_STOCK_LEVELS:
			dNewOpeningQty = item.m_dImportSUQty;
			break;
			
		case ADD_STOCK_LEVELS:
		default:
			nAction = ADD_STOCK_LEVELS;
			dNewOpeningQty = dOldOpeningQty + item.m_dImportSUQty;
			break;
		}
		
		StockLevels.SetOpeningQty( dNewOpeningQty );
		DataManager.Stock.SetStockpointTotals ( item.m_nStockIdx, StockRecord, nSpNo, StockLevels );

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_OPENING );
		csvOut.Add ( 2 );
		csvOut.Add ( StockRecord.GetStockCode() );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
		csvOut.Add ( nAction );
		csvOut.AddStockAuditQty ( item.m_dImportDUQty );
		csvOut.AddStockAuditQty ( item.m_dImportSUQty );
		csvOut.AddStockAuditQty ( dNewOpeningQty );
		fileAudit.WriteLine( csvOut.GetLine() );	
	}

	return TRUE;
}

/**********************************************************************/

void CStockListOpeningDlg::HandleImportSheetFile()
{
	int nSelection = m_listStock.GetCurSel();

	CStockImportSheetHelpers ImportSheetHelpers( m_nSpIdx, 0 );
	CString strImportFile = ImportSheetHelpers.GetImportSheetFilename();

	CFileDialog dlgFile ( TRUE, "csv", strImportFile, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFile.m_ofn.lpstrTitle = "Import Opening Stock";
	dlgFile.m_ofn.lpstrInitialDir = dbStockpoint.GetFolderPath( m_nSpIdx );
	
	if ( dlgFile.DoModal() == IDCANCEL )
		return;

	CString strImportPath = dlgFile.GetPathName();
	
	for ( int nIndex = 0; nIndex < m_ImportFilesArray.GetSize(); nIndex++ )
	{
		if ( strImportPath == m_ImportFilesArray.GetAt ( nIndex ) )
		{
			Prompter.Error ( "You have already imported the selected file." );
			return;
		}
	}
	
	CStockImportFilterArray arrayImport;
	CStockImportFilterPluOpenCloseSheetDlg dlgImport( m_nSpIdx, FALSE, FALSE, strImportPath, arrayImport, this );
	dlgImport.SetStocktakeNo( 0 );
	dlgImport.ProcessImportData();

	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No opening stock data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing opening stock" );
	bool bResult = HandleImportInternal ( arrayImport, dlgImport.GetAction() );
	StatusProgress.Unlock();

	if ( bResult == TRUE )
	{
		COleDateTime date = COleDateTime::GetCurrentTime();
		CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
		CString strPrevEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV_EX );
		::Append( strPrevEx, strTempEx );

		m_ImportFilesArray.Add ( strImportPath );
		DisplayScreen ( nSelection );
		m_bImported = TRUE;
	}
}

/**********************************************************************/

void CStockListOpeningDlg::HandleImportSheetFolder()
{
	CStockImportSheetHelpers ImportSheetHelpers(m_nSpIdx, 0);
	if (ImportSheetHelpers.ConsolidateImportSheets() == FALSE)
	{
		return;
	}

	int nSelection = m_listStock.GetCurSel();

	CString strImportPath = ImportSheetHelpers.GetImportSheetConsolidatedFilename();

	CStockImportFilterArray arrayImport;
	CStockImportFilterPluOpenCloseSheetDlg dlgImport(m_nSpIdx, FALSE, TRUE, strImportPath, arrayImport, this);
	dlgImport.SetStocktakeNo(0);
	dlgImport.ProcessImportData();

	if (dlgImport.GotData() == FALSE)
	{
		Prompter.Error("No opening stock data was found");
		return;
	}

	if (dlgImport.DoModal() == IDCANCEL)
	{
		return;
	}

	//REMOVE PREVIOUS SESSION AUDIT
	COleDateTime date = COleDateTime::GetCurrentTime();
	CString strTempAudit = dbStockpoint.GetFilePathAudit(m_nSpIdx, date, AUDIT_FAMILY_TEMP);

	{
		CFileRemover FileRemover;
		FileRemover.RemoveFile(strTempAudit);
	}

	CSSFile fileAudit;
	fileAudit.Open(strTempAudit, "wb");

	//RESET OPENING STOCK LEVELS TO ZERO
	int nSpNo = m_pFilterArray->GetSpNo();
	for (int nStockIdx = 0; nStockIdx < DataManager.Stock.GetSize(); nStockIdx++)
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt(nStockIdx, StockRecord);

		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals(nStockIdx, StockRecord, nSpNo, StockLevels);

		if (StockLevels.GetStocktakeNo() == 0 )
		{
			StockLevels.SetOpeningQty(0.0);
			DataManager.Stock.SetStockpointTotals(nStockIdx, StockRecord, nSpNo, StockLevels);

			CCSVStockAudit csvOut;
			csvOut.Add("D");
			csvOut.Add(AUDIT_OPENING);
			csvOut.Add(2);
			csvOut.Add(StockRecord.GetStockCode());
			csvOut.Add(StockRecord.GetDescription());
			csvOut.Add(DataManager.Category.GetText(StockRecord.GetCategory()));
			csvOut.Add(DataManager.Stock.GetSubUnits(StockRecord));
			csvOut.Add(OVERWRITE_STOCK_LEVELS);
			csvOut.AddStockAuditQty(0.0);
			csvOut.AddStockAuditQty(0.0);
			csvOut.AddStockAuditQty(0.0);
			fileAudit.WriteLine(csvOut.GetLine());
		}
	}

	fileAudit.Close();

	StatusProgress.Lock(TRUE, "Importing opening stock");
	bool bResult = HandleImportInternal(arrayImport, dlgImport.GetAction());
	StatusProgress.Unlock();

	if (bResult == TRUE)
	{
		COleDateTime date = COleDateTime::GetCurrentTime();
		CString strTempEx = dbStockpoint.GetFilePathAudit(m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX);
		CString strPrevEx = dbStockpoint.GetFilePathAudit(m_nSpIdx, date, AUDIT_FAMILY_PREV_EX);
		::Append(strPrevEx, strTempEx);

		DisplayScreen(nSelection);
		m_bImported = TRUE;
	}
}

/**********************************************************************/

void CStockListOpeningDlg::HandlePrintSession()
{
	COleDateTime date = COleDateTime::GetCurrentTime();

	CStockAuditOpeningClosing Audit;
	Audit.SetReportType ( AUDIT_OPENING );
	
	CStockAuditHeaderInfo info;
	info.m_date = date;
	Audit.SetPreviewHeader( Audit.GetHeaderLine( info ) );

	Audit.SetConLevel ( NODE_STOCKPOINT );			
	Audit.SetConLevelText ( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo ( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode ( AUDIT_MODE_TEMP );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CStockListOpeningDlg::PrepareMenu()
{
	m_MenuChanger.KillItem(IDM_EDIT_STOCKLEVEL2);
	m_MenuChanger.KillItem(IDM_COPY_LEVELS);
	m_MenuChanger.KillParent(IDM_REMOVE_ZEROS);

	if (DealerFlags.GetImportBarcodeFlag() == TRUE)
	{
		m_MenuChanger.SetItemText(IDM_IMPORT_STOCKLEVEL, "By &Barcode Number");
	}

	bool bKeepImportMenu = FALSE;

	if (StockOptions.GetImportOpeningPluFlag() == FALSE)
	{
		m_MenuChanger.KillItem(IDM_IMPORT_STOCKLEVEL);
	}
	else
	{
		bKeepImportMenu = TRUE;
	}

	if (StockOptions.GetImportOpeningWrapperFlag() == FALSE)
	{
		m_MenuChanger.KillItem(IDM_IMPORT_WRAPPER);
	}
	else
	{
		bKeepImportMenu = TRUE;
	}

	if (StockOptions.GetImportOpeningSheetFlag() == FALSE)
	{
		m_MenuChanger.KillItem(IDM_IMPORT_IMPORTSHEET);
		m_MenuChanger.KillItem(IDM_IMPORT_IMPORTSHEET_FOLDER);
		m_MenuChanger.KillItem(IDM_IMPORT_CREATESHEET);
	}
	else
	{
		bKeepImportMenu = TRUE;
	}

	if (FALSE == bKeepImportMenu)
	{
		m_MenuChanger.KillParent(IDM_IMPORT_STOCKLEVEL);
	}
}

/**********************************************************************/

void CStockListOpeningDlg::AppendAudit()
{
	COleDateTime date = COleDateTime::GetCurrentTime();
	CString strAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_NORMAL );
	CString strTempAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
	CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
	CString strPrevAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV );
	CString strPrevEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV_EX );
			
	CSSFile fileAudit;
	if ( fileAudit.Open ( strAudit, "ab" ) == TRUE )
	{
		CStockAuditBaseReport Audit;
		Audit.SetReportType ( AUDIT_OPENING );
		
		CStockAuditHeaderInfo info;
		info.m_date = date;		
		fileAudit.WriteLine ( Audit.GetHeaderLine( info ) );

		fileAudit.Close();
		::Append ( strTempAudit, strAudit );
	}
	
	if ( fileAudit.Open ( strAudit, "ab" ) == TRUE )
	{
		CStockAuditBaseReport Audit;
		Audit.SetReportType ( AUDIT_EX_OPENING );
		
		CStockAuditHeaderInfo info;
		info.m_date = date;	
		fileAudit.WriteLine ( Audit.GetHeaderLine( info ) );

		fileAudit.Close();
		::Append ( strTempEx, strAudit );
	}	
}

/**********************************************************************/

void CStockListOpeningDlg::HandleCreateImportSheet()
{
	CStockImportSheetDlg ImportSheetDlg(m_nSpIdx, 0, this);
	if (ImportSheetDlg.DoModal() == IDOK)
	{
		CStockImportSheetHelpers ImportSheetHelpers(m_nSpIdx,0);
		CString strFields = ImportSheetDlg.GetSavedSettingsLine();
		ImportSheetHelpers.CreateSheet(m_pFilterArray, strFields, ImportSheetDlg.m_nCopies);
	}
}

/**********************************************************************/
