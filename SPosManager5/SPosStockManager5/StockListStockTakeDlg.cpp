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
#include "StockImportFilterArray.h"
#include "StockImportFilterPluOpenCloseDlg.h"
#include "StockImportFilterPluOpenCloseSheetDlg.h"
#include "StockImportFilterWrapperDlg.h"
#include "StockImportSheetDlg.h"
#include "StockImportSheetHelpers.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockListStocktakeDlg.h"
/**********************************************************************/

CStockListStocktakeDlg::CStockListStocktakeDlg(int nSpIdx, CStocktakeCSVRecord& Stocktake, CWnd* pParent ) 
: CStockListDlg ( AUDIT_STOCKTAKE, AUDIT_EX_CLOSING, nSpIdx, pParent), m_Stocktake( Stocktake ) 
{
	m_nUpdatedItemCount = 0;
	m_bStockWriteError = FALSE;
}

/**********************************************************************/

void CStockListStocktakeDlg::SetColumns()
{
	m_ColumnManager.Reset( TableNames::Stocktake );
	m_ColumnManager.AddColumn ( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 115, 115 );
	m_ColumnManager.AddColumn ( ColumnNames::Desc, "Description", LVCFMT_LEFT, 135, 170 );
	m_ColumnManager.AddColumn ( ColumnNames::Category, "Category", LVCFMT_LEFT, 80, 140 );
	m_ColumnManager.AddColumn ( ColumnNames::StockUnit, "Unit", LVCFMT_LEFT, 0, 100 );
	m_ColumnManager.AddColumn ( ColumnNames::Apparent, "Apparent", LVCFMT_RIGHT, 75, 75 );
	m_ColumnManager.AddColumn ( ColumnNames::Closing, "Closing", LVCFMT_RIGHT, 75, 75 );
	m_ColumnManager.LoadSettings();	
}

/**********************************************************************/

const char* CStockListStocktakeDlg::GetTitle()
{
	m_strTitle.Format ( "Stocktake %3.3d for %s (%s, %s)",
		m_Stocktake.GetStocktakeNo(),
		dbStockpoint.GetName( m_nSpIdx ),
		m_Stocktake.GetComment(),
		m_Stocktake.GetFormattedDate() );

	return m_strTitle;
}

/**********************************************************************/

void CStockListStocktakeDlg::DisplayScreen(int nIndex)
{
	SetWindowText(GetTitle());

	int nSize = m_pFilterArray->GetSize();
	if ((nIndex >= 0) && (nIndex < nSize))
	{
		m_listStock.SetCurSel(nIndex);
	}
	else
	{
		m_listStock.SetCurSel(nSize - 1);
	}

	m_listStock.Invalidate();
}

/**********************************************************************/

void CStockListStocktakeDlg::EditStockLevel()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nSelection = m_listStock.GetCurSel();

		if ( nSelection >= 0 && nSelection < m_pFilterArray -> GetSize() )
		{
			m_pFilterArray -> SetArrayIdx ( nSelection );	
			CStockEditOpeningDlg dlg ( m_listStock, m_pFilterArray, TRUE, this );
			dlg.DoModal();
			DisplayScreen ( m_pFilterArray -> GetArrayIdx() );
		}
	}
}

/**********************************************************************/

void CStockListStocktakeDlg::EditStockLevelRandom()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CStockEditOpeningRandomDlg dlg( m_pFilterArray, TRUE, m_Stocktake.GetStocktakeNo(), this );
		dlg.DoModal();
		DisplayScreen ( m_pFilterArray -> GetArrayIdx() );
	}
}

/**********************************************************************/

void CStockListStocktakeDlg::SetFilterArray()
{
	m_pFilterArray = new CStockFilterArray ( FILTERTYPE_STOCKTAKE );
	m_pFilterArray -> SetSpIdx ( m_nSpIdx, 0 );
	m_pFilterArray -> SetStocktakeFilter( m_Stocktake.GetStocktakeNo() );	
	m_pFilterArray -> BuildRecordList();
	m_nUpdatedItemCount = m_pFilterArray -> GetSize();
}

/**********************************************************************/

void CStockListStocktakeDlg::HandleImport()
{
	int nSelection = m_listStock.GetCurSel();

	CString strTitle;
	strTitle.Format ( "Import Closing Stock for %s", m_Stocktake.GetFormattedDate() );

	CString strImportFile = StockOptions.GetImportClosingPluName();
	if ((strImportFile.GetLength() == 0) || (strImportFile.GetLength() > 25))
	{
		strImportFile = "closing";
	}

	CFileDialog dlgFile ( TRUE, "csv", strImportFile, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFile.m_ofn.lpstrTitle = strTitle;
	dlgFile.m_ofn.lpstrInitialDir = dbStockpoint.GetFolderPath( m_nSpIdx );
	
	if (dlgFile.DoModal() == IDCANCEL)
	{
		return;
	}

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
	CStockImportFilterPluOpenCloseDlg dlgImport( m_nSpIdx, TRUE, strImportPath, arrayImport, this );
	dlgImport.SetStocktakeNo( m_Stocktake.GetStocktakeNo() );
	dlgImport.ProcessImportData();

	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No closing stock data was found" );
		return;
	}

	if (dlgImport.DoModal() == IDCANCEL)
	{
		return;
	}

	StatusProgress.Lock( TRUE, "Importing closing stock" );
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

bool CStockListStocktakeDlg::HandleImportInternal( CStockImportFilterArray& arrayImport, int nAction )
{	
	CSSFile fileAudit;
	COleDateTime date = COleDateTime::GetCurrentTime();
	CString strTempAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
	if (fileAudit.Open(strTempAudit, "ab") == FALSE)
	{
		return FALSE;
	}

	int nArraySize = arrayImport.GetSize();
	for ( int n = 0; n < nArraySize; n++ )
	{
		StatusProgress.SetPos( n, nArraySize );

		CStockImportFilterItem item;
		arrayImport.GetItem( n, item );

		if (FALSE == item.m_bSelected)
		{
			continue;
		}

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( item.m_nStockIdx, StockRecord );
		int nSpNo = m_pFilterArray -> GetSpNo();

		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals ( item.m_nStockIdx, StockRecord, nSpNo, StockLevels );

		if ( StockLevels.GetStocktakeNo() != m_Stocktake.GetStocktakeNo() )
			continue;

		double dOldClosingQty = StockLevels.GetStocktakeClosingQty();
		double dNewClosingQty = 0.0;

		switch ( nAction )
		{
		case SUBTRACT_STOCK_LEVELS:
			dNewClosingQty = dOldClosingQty - item.m_dImportSUQty;
			break;
			
		case OVERWRITE_STOCK_LEVELS:
			dNewClosingQty = item.m_dImportSUQty;
			break;
			
		case ADD_STOCK_LEVELS:
		default:
			nAction = ADD_STOCK_LEVELS;
			dNewClosingQty = dOldClosingQty + item.m_dImportSUQty;
			break;
		}
		
		StockLevels.SetStocktakeClosingQty( dNewClosingQty );
		DataManager.Stock.SetStockpointTotals ( item.m_nStockIdx, StockRecord, nSpNo, StockLevels );
	
		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_CLOSING );
		csvOut.Add ( 2 );
		csvOut.Add ( StockRecord.GetStockCode() );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
		csvOut.Add ( nAction );
		csvOut.AddStockAuditQty ( item.m_dImportDUQty );
		csvOut.AddStockAuditQty ( item.m_dImportSUQty );
		csvOut.AddStockAuditQty ( dNewClosingQty );
		fileAudit.WriteLine( csvOut.GetLine() );	
	}
	
	return TRUE;
}

/**********************************************************************/

void CStockListStocktakeDlg::HandleImportWrapper()
{
	int nSelection = m_listStock.GetCurSel();

	CString strTitle;
	strTitle.Format ( "Import Closing Stock for %s", m_Stocktake.GetFormattedDate() );
	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );

	CString strImportFile = StockOptions.GetImportClosingWrpName();
	if ((strImportFile.GetLength() == 0) || (strImportFile.GetLength() > 25))
	{
		strImportFile = "closing";
	}

	CFileDialog dlgFile ( TRUE, "csv", strImportFile, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFile.m_ofn.lpstrTitle = strTitle;
	dlgFile.m_ofn.lpstrInitialDir = strPath;
	
	if (dlgFile.DoModal() == IDCANCEL)
	{
		return;
	}

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
	CStockImportFilterWrapperDlg dlgImport( m_nSpIdx, TRUE, strImportPath, m_SuppRefFinder, arrayImport, this );
	dlgImport.SetStocktakeNo( m_Stocktake.GetStocktakeNo() );
	dlgImport.ProcessImportData();

	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No closing stock data was found" );
		return;
	}

	if (dlgImport.DoModal() == IDCANCEL)
	{
		return;
	}

	StatusProgress.Lock( TRUE, "Importing closing stock" );
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

bool CStockListStocktakeDlg::HandleImportWrapperInternal( CStockImportFilterArray& arrayImport, int nAction )
{
	CSSFile fileAudit;
	COleDateTime date = COleDateTime::GetCurrentTime();
	CString strTempAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
	if (fileAudit.Open(strTempAudit, "ab") == FALSE)
	{
		return FALSE;
	}

	int nArraySize = arrayImport.GetSize();
	for ( int n = 0; n < nArraySize; n++ )
	{
		StatusProgress.SetPos( n, nArraySize );

		CStockImportFilterItem item;
		arrayImport.GetItem( n, item );

		if (FALSE == item.m_bSelected)
		{
			continue;
		}

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( item.m_nStockIdx, StockRecord );
		
		CStockLevelsCSVRecord StockLevels;
		int nSpNo = m_pFilterArray -> GetSpNo();
		DataManager.Stock.GetStockpointTotals ( item.m_nStockIdx, StockRecord, nSpNo, StockLevels );

		if (StockLevels.GetStocktakeNo() != m_Stocktake.GetStocktakeNo())
		{
			continue;
		}

		double dOldClosingQty = StockLevels.GetStocktakeClosingQty();
		double dNewClosingQty = 0.0;

		switch ( nAction )
		{
		case SUBTRACT_STOCK_LEVELS:
			dNewClosingQty = dOldClosingQty - item.m_dImportSUQty;
			break;
			
		case OVERWRITE_STOCK_LEVELS:
			dNewClosingQty = item.m_dImportSUQty;
			break;
			
		case ADD_STOCK_LEVELS:
		default:
			nAction = ADD_STOCK_LEVELS;
			dNewClosingQty = dOldClosingQty + item.m_dImportSUQty;
			break;
		}
		
		StockLevels.SetStocktakeClosingQty( dNewClosingQty );
		DataManager.Stock.SetStockpointTotals ( item.m_nStockIdx, StockRecord, nSpNo, StockLevels );

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_CLOSING );
		csvOut.Add ( 2 );
		csvOut.Add ( StockRecord.GetStockCode() );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
		csvOut.Add ( nAction );
		csvOut.AddStockAuditQty ( item.m_dImportDUQty );
		csvOut.AddStockAuditQty ( item.m_dImportSUQty );
		csvOut.AddStockAuditQty ( dNewClosingQty );
		fileAudit.WriteLine( csvOut.GetLine() );	
	}

	return TRUE;
}

/**********************************************************************/

void CStockListStocktakeDlg::HandleImportSheetFile()
{
	int nSelection = m_listStock.GetCurSel();

	CString strTitle;
	strTitle.Format ( "Import Closing Stock for %s", m_Stocktake.GetFormattedDate() );

	CStockImportSheetHelpers ImportSheetHelpers( m_nSpIdx, m_Stocktake.GetStocktakeNo() );
	CString strImportFile = ImportSheetHelpers.GetImportSheetFilename();

	CFileDialog dlgFile ( TRUE, "csv", strImportFile, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFile.m_ofn.lpstrTitle = strTitle;
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
	CStockImportFilterPluOpenCloseSheetDlg dlgImport( m_nSpIdx, TRUE, FALSE, strImportPath, arrayImport, this );
	dlgImport.SetStocktakeNo( m_Stocktake.GetStocktakeNo() );
	dlgImport.ProcessImportData();

	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No closing stock data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing closing stock" );
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

void CStockListStocktakeDlg::HandleImportSheetFolder()
{
	CStockImportSheetHelpers ImportSheetHelpers(m_nSpIdx,m_Stocktake.GetStocktakeNo());
	if (ImportSheetHelpers.ConsolidateImportSheets() == FALSE)
	{
		return;
	}

	int nSelection = m_listStock.GetCurSel();

	CString strImportPath = ImportSheetHelpers.GetImportSheetConsolidatedFilename();

	CStockImportFilterArray arrayImport;
	CStockImportFilterPluOpenCloseSheetDlg dlgImport(m_nSpIdx, TRUE, TRUE, strImportPath, arrayImport, this);
	dlgImport.SetStocktakeNo(m_Stocktake.GetStocktakeNo());
	dlgImport.ProcessImportData();

	if (dlgImport.GotData() == FALSE)
	{
		Prompter.Error("No closing stock data was found");
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

	//RESET CLOSING STOCK LEVELS TO ZERO
	int nSpNo = m_pFilterArray->GetSpNo();
	for (int nStockIdx = 0; nStockIdx < DataManager.Stock.GetSize(); nStockIdx++)
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt(nStockIdx, StockRecord);

		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals(nStockIdx, StockRecord, nSpNo, StockLevels);

		if (StockLevels.GetStocktakeNo() == m_Stocktake.GetStocktakeNo())
		{
			StockLevels.SetStocktakeClosingQty(0.0);
			DataManager.Stock.SetStockpointTotals(nStockIdx, StockRecord, nSpNo, StockLevels);

			CCSVStockAudit csvOut;
			csvOut.Add("D");
			csvOut.Add(AUDIT_CLOSING);
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

	StatusProgress.Lock(TRUE, "Importing closing stock");
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

void CStockListStocktakeDlg::HandlePrintSession()
{
	COleDateTime date = COleDateTime::GetCurrentTime();

	CStockAuditOpeningClosing Audit;
	Audit.SetReportType ( AUDIT_CLOSING );
	
	CStockAuditHeaderInfo info;
	info.m_date = date;
	info.m_strReference1 = m_Stocktake.GetComment();
	info.m_strReference2 = "";
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

void CStockListStocktakeDlg::AppendAudit()
{
	COleDateTime date = COleDateTime::GetCurrentTime();
	CString strAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_NORMAL );
	CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
			
	CSSFile fileAudit;
	if ( fileAudit.Open ( strAudit, "ab" ) == TRUE )
	{
		CStockAuditBaseReport Audit;
		Audit.SetReportType ( AUDIT_EX_CLOSING );
		
		CStockAuditHeaderInfo info;
		info.m_date = date;
		info.m_strReference1 = m_Stocktake.GetComment();
		info.m_strReference2 = "";
		fileAudit.WriteLine ( Audit.GetHeaderLine( info ) );

		fileAudit.Close();
		::Append ( strTempEx, strAudit );
	}	
}

/**********************************************************************/

void CStockListStocktakeDlg::PrepareMenu()
{
	m_MenuChanger.KillItem(IDM_EDIT_STOCKLEVEL2);
	m_MenuChanger.KillItem(IDM_COPY_LEVELS);
	m_MenuChanger.KillParent(IDM_REMOVE_ZEROS);

	if (DealerFlags.GetImportBarcodeFlag() == TRUE)
	{
		m_MenuChanger.SetItemText(IDM_IMPORT_STOCKLEVEL, "By &Barcode Number");
	}

	bool bKeepImportMenu = FALSE;

	if (StockOptions.GetImportClosingPluFlag() == FALSE)
	{
		m_MenuChanger.KillItem(IDM_IMPORT_STOCKLEVEL);
	}
	else
	{
		bKeepImportMenu = TRUE;
	}

	if (StockOptions.GetImportClosingWrapperFlag() == FALSE)
	{
		m_MenuChanger.KillItem(IDM_IMPORT_WRAPPER);
	}
	else
	{
		bKeepImportMenu = TRUE;
	}

	if (StockOptions.GetImportClosingSheetFlag() == FALSE)
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

void CStockListStocktakeDlg::HandleCreateImportSheet()
{
	CStockImportSheetDlg ImportSheetDlg(m_nSpIdx, m_Stocktake.GetStocktakeNo(), this);
	if (ImportSheetDlg.DoModal() == IDOK)
	{
		CStockImportSheetHelpers ImportSheetHelpers(m_nSpIdx, m_Stocktake.GetStocktakeNo());
		CString strFields = ImportSheetDlg.GetSavedSettingsLine();
		ImportSheetHelpers.CreateSheet(m_pFilterArray, strFields, ImportSheetDlg.m_nCopies);
	}
}

/**********************************************************************/

