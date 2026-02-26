/**********************************************************************/
#include "FilterTypes.h"
#include "Password.h"
#include "Repspawn.h"
#include "SelectMultipleStockpointDlg.h"
#include "StatusProgress.h"
#include "StockEditReorderDlg.h"
#include "StockEditReorderRandomDlg.h"
#include "StockInfoMinimumDlg.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockListReorderDlg.h"
/**********************************************************************/

CStockListReorderDlg::CStockListReorderDlg( int nSpIdx, CWnd* pParent ) : CStockListDlg ( 0, 0, nSpIdx, pParent )
{
	m_bAllowReorder = ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES );
}

/**********************************************************************/

const char* CStockListReorderDlg::GetTitle()
{
	if ( TRUE == m_bAllowReorder )
		return "Min / Reorder / Max Stock Levels";
	else
		return "Min / Max Stock Levels";
}

/**********************************************************************/

void CStockListReorderDlg::SetColumns()
{
	m_ColumnManager.Reset( TableNames::Reorder );
	m_ColumnManager.AddColumn ( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 115, 115 );
	m_ColumnManager.AddColumn ( ColumnNames::Desc, "Description", LVCFMT_LEFT, 135, 170 );
	m_ColumnManager.AddColumn ( ColumnNames::Category, "Category", LVCFMT_LEFT, 80, 140 );
	m_ColumnManager.AddColumn ( ColumnNames::StockUnit, "Unit", LVCFMT_LEFT, 100, 100 );
	m_ColumnManager.AddColumn ( ColumnNames::Minimum, "Minimum", LVCFMT_RIGHT, 75, 75 );
	
	if ( TRUE == m_bAllowReorder )
		m_ColumnManager.AddColumn ( ColumnNames::Reorder, "Reorder", LVCFMT_RIGHT, 75, 75 );

	m_ColumnManager.AddColumn ( ColumnNames::Maximum, "Maximum", LVCFMT_RIGHT, 75, 75 );
	
	m_ColumnManager.LoadSettings();
}

/**********************************************************************/

void CStockListReorderDlg::EditStockLevel()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nSelection = m_listStock.GetCurSel();

		if ( nSelection >= 0 && nSelection < m_pFilterArray -> GetSize() )
		{		
			m_pFilterArray -> SetArrayIdx ( nSelection );
			CStockEditReorderDlg dlg ( m_listStock, m_pFilterArray, this );
			dlg.DoModal();
			DisplayScreen ( m_pFilterArray -> GetArrayIdx() );
		}
	}
}

/**********************************************************************/

void CStockListReorderDlg::EditStockLevelRandom()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CStockEditReorderRandomDlg dlg( m_pFilterArray, this );
		dlg.DoModal();
		DisplayScreen ( m_pFilterArray -> GetArrayIdx() );
	}
}

/**********************************************************************/

void CStockListReorderDlg::SetFilterArray()
{
	m_pFilterArray = new CStockFilterArray ( FILTERTYPE_REORDER );
	m_pFilterArray ->SetSpIdx ( m_nSpIdx, 0 );
	m_pFilterArray ->SetActiveItemOnlyFlag(TRUE);
	m_pFilterArray ->BuildRecordList();
}

/**********************************************************************/

void CStockListReorderDlg::HandleCopyLevels()
{
	if ( m_pFilterArray -> GetSize() == 0 )
	{
		Prompter.Error ( "You must select some stock records\nbefore you can use this option" );
		return;
	}

	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange ( DataManager.GetActiveDbIdx(), nStartIdx, nEndIdx );

	CArray<int,int> SpIdxArray;
	for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
		if ( nSpIdx != m_nSpIdx )
			SpIdxArray.Add ( nSpIdx );
	
	if ( SpIdxArray.GetSize() == 0 )
		return;
	
	CString strAction = "";
	strAction += "Copy Min / ";

	if ( TRUE == m_bAllowReorder )
		strAction += "Reorder / ";

	strAction += "Max Levels";

	dbStockpoint.SortSpListByName( SpIdxArray );

	CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arraySpInfo;
	for ( int n = 0; n < SpIdxArray.GetSize(); n++ )
	{
		CSelectMultipleEntityInfo infoNode;
		infoNode.m_nSpIdx = SpIdxArray.GetAt(n);
		infoNode.m_bSelected = FALSE;
		arraySpInfo.Add( infoNode );
	}

	CSelectMultipleStockpointDlg dlg ( strAction, arraySpInfo, this );	
	if ( dlg.DoModal() != IDOK )
		return;

	{
		for ( int n = arraySpInfo.GetSize() - 1; n >= 0; n-- )
			if ( FALSE == arraySpInfo[n].m_bSelected )
				arraySpInfo.RemoveAt(n);
	}

	if ( arraySpInfo.GetSize() == 0 )
		return;

	CReportFile ReportFile;	
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
	{
		Prompter.Error( "Unable to create report file" );
		return;
	}
	
	ReportFile.SetStyle1 ( strAction );
	ReportFile.AddColumn ( "Date", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Destination", TA_LEFT, 600 );
	ReportFile.AddColumn ( "Result", TA_LEFT, 600 );

	{
		CString strLine = "";
		strLine += "<..>";
		strLine += strAction;
		strLine += " from ";
		strLine += dbStockpoint.GetName( m_nSpIdx );
		ReportFile.WriteLine( strLine );
		ReportFile.WriteLine( "<LI>" );
	}

	int nCount = ( m_pFilterArray -> GetSize() ) * arraySpInfo.GetSize();
	int nProgress = 0;

	int nSpNoSource = m_pFilterArray -> GetSpNo();

	{
		CString strProgressAction = "";
		strProgressAction += "Copying ";
		strProgressAction += strAction.Right( strAction.GetLength() - 5 );
		StatusProgress.Lock( TRUE, strProgressAction );
	}

	for ( int nIndex = 0; nIndex < arraySpInfo.GetSize(); nIndex++ )
	{
		StatusProgress.SetPos( nProgress, nCount );

		CSelectMultipleEntityInfo infoNode = arraySpInfo.GetAt( nIndex );
		
		int nSpIdx = infoNode.m_nSpIdx;

		COleDateTime oleTimeNow = COleDateTime::GetCurrentTime();

		CString strDate;
		strDate.Format ( "%2.2d/%2.2d/%4.4d",
			oleTimeNow.GetDay(),
			oleTimeNow.GetMonth(),
			oleTimeNow.GetYear() );

		CString strTime;
		strTime.Format ( "%2.2d:%2.2d:%2.2d",
			oleTimeNow.GetHour(),
			oleTimeNow.GetMinute(),
			oleTimeNow.GetSecond() );

		CCSV csv( '\t' );
		csv.Add( strDate );
		csv.Add( strTime );
		csv.Add( dbStockpoint.GetName( nSpIdx ) );
		
		CDataManagerInfo info;
		if ( DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
		{
			csv.Add( "FAILED : Stock levels file is locked" );
			ReportFile.WriteLine( csv.GetLine() );
			ReportFile.WriteLine( "<LI>" );

			nProgress += m_pFilterArray -> GetSize();
		}
		else
		{
			for ( int nFilterIdx = 0; nFilterIdx < m_pFilterArray -> GetSize(); nFilterIdx++ )
			{
				nProgress++;
				StatusProgress.SetPos ( nProgress, nCount );

				CStockCSVRecord StockRecord;
				int nStockIdx = m_pFilterArray -> GetStockItemIndexAt ( nFilterIdx );
				DataManager.Stock.GetAt( nStockIdx, StockRecord );
		
				CStockLevelsCSVRecord StockLevels;
				DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, nSpNoSource, StockLevels );
				double dMinimumQty = StockLevels.GetMinimumLevelQty();
				double dReorderQty = StockLevels.GetReorderLevelQty();
				double dMaximumQty = StockLevels.GetMaximumLevelQty();

				int nSpNoDest = dbStockpoint.GetSpNo( nSpIdx );

				DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, nSpNoDest, StockLevels );

				if ( StockLevels.GetActiveFlag() == FALSE )
					continue;
				
				if ( TRUE == m_bAllowReorder )
					StockLevels.SetReorderLevelQty ( dReorderQty );
				
				StockLevels.SetMinimumLevelQty ( dMinimumQty );
				StockLevels.SetMaximumLevelQty ( dMaximumQty );
				DataManager.Stock.SetStockpointTotals ( nStockIdx, StockRecord, nSpNoDest, StockLevels );
			}

			if ( DataManager.WriteStockLevels( nSpIdx, info ) == FALSE )
				csv.Add( "FAILED : Unable to write stock levels file" );
			else
				csv.Add( "OK" );

			ReportFile.WriteLine( csv.GetLine() );
			ReportFile.WriteLine( "<LI>" );

			DataManager.CloseStockLevels( nSpIdx, info );
		}
	}
	StatusProgress.Unlock();

	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::ReorderCopyPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}

/**********************************************************************/

void CStockListReorderDlg::PrepareMenu()
{
	m_MenuChanger.KillItem(IDM_EDIT_STOCKLEVEL2);

	if (TRUE == m_bAllowReorder)
		m_MenuChanger.SetItemText(IDM_PRINT_SESSION, "Print Min / Reorder / Max &Levels");
	else
		m_MenuChanger.SetItemText(IDM_PRINT_SESSION, "Print Min / Max &Levels");

	m_MenuChanger.KillParent(IDM_REMOVE_ZEROS);

	m_MenuChanger.KillItem(IDM_IMPORT_WRAPPER);
	m_MenuChanger.KillItem(IDM_IMPORT_IMPORTSHEET);
	m_MenuChanger.KillItem(IDM_IMPORT_IMPORTSHEET_FOLDER);
	m_MenuChanger.KillItem(IDM_IMPORT_CREATESHEET);

	bool bImportPlu = StockOptions.GetImportReorderPluFlag();
	bool bImportWrapper = StockOptions.GetImportReorderWrapperFlag();

	if (bImportPlu && bImportWrapper)
	{
		if (DealerFlags.GetImportBarcodeFlag() == TRUE)
			m_MenuChanger.SetItemText(IDM_IMPORT_STOCKLEVEL, "By &Barcode Number or Wrapper");
		else
			m_MenuChanger.SetItemText(IDM_IMPORT_STOCKLEVEL, "By &Plu Number or Wrapper");
	}
	else if (bImportPlu)
	{
		if (DealerFlags.GetImportBarcodeFlag() == TRUE)
			m_MenuChanger.SetItemText(IDM_IMPORT_STOCKLEVEL, "By &Barcode Number");
		else
			m_MenuChanger.SetItemText(IDM_IMPORT_STOCKLEVEL, "By &Plu Number");
	}
	else if (bImportWrapper)
	{
		m_MenuChanger.SetItemText(IDM_IMPORT_STOCKLEVEL, "By &Wrapper");
	}
	else
	{
		m_MenuChanger.KillParent(IDM_IMPORT_STOCKLEVEL);
	}

	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange(DataManager.GetActiveDbIdx(), nStartIdx, nEndIdx);

	if (nEndIdx < nStartIdx + 1)
		m_MenuChanger.KillItem(IDM_COPY_LEVELS);

	if (PasswordArray.GetLocationAccessType() == LOCATION_ACCESS_LOC)
		m_MenuChanger.KillItem(IDM_COPY_LEVELS);
}

/**********************************************************************/

void CStockListReorderDlg::HandleImport()
{
	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );

	CString strImportFile;
	
	if ( StockOptions.GetImportReorderPluFlag() == FALSE )
		strImportFile = StockOptions.GetImportReorderWrpName();
	else
		strImportFile = StockOptions.GetImportReorderPluName();

	if ( ( strImportFile.GetLength() == 0 ) || ( strImportFile.GetLength() > 25 ) )
		strImportFile = "minimum";

	CString strTitle = "";
	CStockInfoMinimumDlg::GetTitle( m_bAllowReorder, strTitle );

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

	CReorderImportHelper ImportHelpers;
	
	{
		CSSFile fileCheck;
		if ( fileCheck.Open( strImportPath, "rb" ) == FALSE )
		{
			Prompter.Error( "Unable to open import file" );
			return;
		}

		CString strBuffer = "";
		fileCheck.ReadString( strBuffer );
		fileCheck.Close();

		if ( ImportHelpers.CheckHeader( strBuffer ) == FALSE )
		{
			ImportHelpers.ShowHeaderError();
			return;
		}
	}

	CStockInfoMinimumDlg dlgImport ( m_nSpIdx, ImportHelpers, m_SuppRefFinder, this );
	
	int nSelection = m_listStock.GetCurSel();

	dlgImport.ProcessImportData( strImportPath );

	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No data was found in the import file" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Processing Import File" );
	
	bool bResult = FALSE;
	if ( ImportHelpers.IsWrapperImport() == TRUE )
		bResult = ImportWrapperInternal( strImportPath, ImportHelpers );
	else
		bResult = ImportPluInternal( strImportPath, ImportHelpers );
	
	StatusProgress.Unlock();
	
	if ( bResult == TRUE )
	{
		m_ImportFilesArray.Add ( strImportPath );
		DisplayScreen ( nSelection );
		m_bImported = TRUE;
	}
}

/**********************************************************************/

bool CStockListReorderDlg::ImportPluInternal( const char* szImportPath, CReorderImportHelper& ImportHelper )
{
	CSSFile fileImport;
	CString strImportPath = szImportPath;
	if ( fileImport.Open ( strImportPath, "rb" ) == FALSE )
		return FALSE;
	
	//SKIP HEADER LINE
	CString strBuffer;
	fileImport.ReadString( strBuffer );

	while ( fileImport.ReadString ( strBuffer ) == TRUE )  
	{
		StatusProgress.SetPos ( fileImport );

		CCSV csv ( strBuffer );
		
		CString strPluNo = "";
		if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		{
			__int64 nPluNo = DataManager.Barcode.GetPluFromBarcode( _atoi64 ( csv.GetString(0) ) );
			::FormatPluNo( nPluNo, strPluNo );
		}
		else
			strPluNo = csv.GetString( 0 );
		
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber ( _atoi64(strPluNo), nPluIdx ) == FALSE )
			continue;

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );
		if ( DataManager.Plu.IsRecipe ( PluRecord.GetPluNoInt() ) )
			continue;
		
		CString strStockCode;
		DataManager.Plu.GetPluStockCode ( PluRecord.GetPluNoInt(), strStockCode, FALSE );
		if ( strStockCode == "" )
			continue;
		
		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
			continue;

		CStockCSVRecord StockRecord;
		CStockLevelsCSVRecord StockLevels;
		int nSpNo = m_pFilterArray -> GetSpNo();
		if ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, nSpNo, -1 ) != STOCKLEVEL_IMPORT_ACCEPT )
			continue;
		
		double dStockQtyPerPlu;
		DataManager.Plu.GetPluStockQty ( PluRecord.GetPluNoInt(), dStockQtyPerPlu );

		if ( ImportHelper.GotMinField() == TRUE )
		{
			double dMin = ImportHelper.GetMinQty( csv );
			StockLevels.SetMinimumLevelQty( dMin * dStockQtyPerPlu );
		}

		if ( ImportHelper.GotReorderField() == TRUE )
		{
			double dReorder = ImportHelper.GetReorderQty( csv );
			StockLevels.SetReorderLevelQty( dReorder * dStockQtyPerPlu );
		}

		if ( ImportHelper.GotMaxField() == TRUE )
		{
			double dMax = ImportHelper.GetMaxQty( csv );
			StockLevels.SetMaximumLevelQty( dMax * dStockQtyPerPlu );
		}

		DataManager.Stock.SetStockpointTotals ( nStockIdx, StockRecord, nSpNo, StockLevels );
	}

	fileImport.Close();
	
	return TRUE;
}

/**********************************************************************/

bool CStockListReorderDlg::ImportWrapperInternal( const char* szImportPath, CReorderImportHelper& ImportHelper )
{
	CSSFile fileImport;
	CString strImportPath = szImportPath;
	if ( fileImport.Open ( strImportPath, "rb" ) == FALSE )
		return FALSE;

	//SKIP HEADER LINE
	CString strBuffer;
	fileImport.ReadString( strBuffer );
	
	while ( fileImport.ReadString ( strBuffer ) == TRUE )  
	{
		StatusProgress.SetPos ( fileImport );

		CCSV csv ( strBuffer );
		CString strWrapper = csv.GetString ( 0 );
		
		int nStockIdx, nStockSuppIdx;
		if ( m_SuppRefFinder.FindItemByWrapper( strWrapper, nStockIdx, nStockSuppIdx ) == FALSE )
			continue;
		
		CStockCSVRecord StockRecord;
		CStockLevelsCSVRecord StockLevels;
		int nSpNo = m_pFilterArray -> GetSpNo();
		if ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, nSpNo, -1 ) != STOCKLEVEL_IMPORT_ACCEPT )
			continue;

		double dYield = StockRecord.GetYield( nStockSuppIdx );
		
		if ( ImportHelper.GotMinField() == TRUE )
		{
			double dMin = ImportHelper.GetMinQty( csv );
			StockLevels.SetMinimumLevelQty( dMin * dYield );
		}

		if ( ImportHelper.GotReorderField() == TRUE )
		{
			double dReorder = ImportHelper.GetReorderQty( csv );
			StockLevels.SetReorderLevelQty( dReorder * dYield );
		}

		if ( ImportHelper.GotMaxField() == TRUE )
		{
			double dMax = ImportHelper.GetMaxQty( csv );
			StockLevels.SetMaximumLevelQty( dMax * dYield );
		}

		DataManager.Stock.SetStockpointTotals ( nStockIdx, StockRecord, nSpNo, StockLevels );
	}

	fileImport.Close();
	
	return TRUE;
}

/**********************************************************************/

void CStockListReorderDlg::HandlePrintSession()
{
	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;

	{
		CString strTitle = "";
		strTitle += "Stock Item Min / ";

		if ( TRUE == m_bAllowReorder )
			strTitle += "Reorder / ";

		strTitle += "Max Levels (";
		strTitle += dbStockpoint.GetName( m_nSpIdx );
		strTitle += ")";

		ReportFile.SetStyle1 ( strTitle );
	}
		
	ReportFile.AddColumn( "Code", TA_LEFT, 380 );
	ReportFile.AddColumn( "Description", TA_LEFT, 300 );
	ReportFile.AddColumn( "Category", TA_LEFT, 300 );
	ReportFile.AddColumn( "Stock Unit", TA_LEFT, 300 );
	ReportFile.AddColumn( "Minimum", TA_RIGHT, 300 );

	if ( TRUE == m_bAllowReorder )
		ReportFile.AddColumn( "Reorder", TA_RIGHT, 300 );

	ReportFile.AddColumn( "Maximum", TA_RIGHT, 300 );

	for ( int n = 0; n < m_pFilterArray -> GetSize(); n++ )
	{
		CStockCSVRecord StockRecord;
		int nStockIdx = m_pFilterArray -> GetStockItemIndexAt( n );
		DataManager.Stock.GetAt( nStockIdx, StockRecord );

		CStockLevelsCSVRecord StockLevels;
		int nSpNo = m_pFilterArray -> GetSpNo();
		DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, nSpNo, StockLevels );

		CCSV csv ( '\t' );
		csv.Add ( StockRecord.GetStockCode() );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csv.Add ( DataManager.Stock.GetStockSubUnitName( StockRecord ) );
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevels.GetMinimumLevelQty() ) );
		
		if ( TRUE == m_bAllowReorder )
			csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevels.GetReorderLevelQty() ) );

		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevels.GetMaximumLevelQty() ) );
		ReportFile.WriteLine( csv.GetLine() );
	}

	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::ReorderLevelPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}

/**********************************************************************/
