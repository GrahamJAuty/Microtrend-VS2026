/**********************************************************************/
#include "Append.h"
#include "ColumnManager.h"
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "FilterTypes.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockAuditTransfer.h"
#include "StockEditTransferDlg.h"
#include "StockEditTransferRandomDlg.h"
#include "StockImportBasicTransferDlg.h"
#include "StockImportBasicWrapperTransferDlg.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockListTransferDlg.h"
/**********************************************************************/

CStockListTransferDlg::CStockListTransferDlg( int nSpIdx, int nSpIdx2, const char* szReference1, const char* szReference2, CWnd* pParent ) : CStockListDlg ( AUDIT_TRANSFER, AUDIT_EX_TRANSFER, nSpIdx, pParent )
{
	m_nSpIdx2 = nSpIdx2;
	m_strReference1 = szReference1;
	m_strReference2 = szReference2;
		
	int nDbNo = dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() );

	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{	
		if ( dbLocation.GetDbNo( nLocIdx ) == nDbNo )
		{
			if ( DealerFlags.GetPriceSetExcludeByDefaultFlag() == TRUE )
			{
				CString strSource = dbLocation.GetFilePathDataLocalPluInclude( nLocIdx );
				CString strDest = dbLocation.GetFilePathDataLocalPluIncludeTemp( nLocIdx );
				CopyFile ( strSource, strDest, FALSE );
			}
			else
			{
				CString strSource = dbLocation.GetFilePathDataLocalPluExclude( nLocIdx );
				CString strDest = dbLocation.GetFilePathDataLocalPluExcludeTemp( nLocIdx );
				CopyFile ( strSource, strDest, FALSE );
			}
		}
	}
}

/**********************************************************************/
	
void CStockListTransferDlg::SetColumns()
{
	m_ColumnManager.Reset( TableNames::Transfer );
	m_ColumnManager.AddColumn ( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 115, 115 );
	m_ColumnManager.AddColumn ( ColumnNames::Desc, "Description", LVCFMT_LEFT, 135, 170 );
	m_ColumnManager.AddColumn ( ColumnNames::Category, "Category", LVCFMT_LEFT, 80, 140 );
	m_ColumnManager.AddColumn ( ColumnNames::StockUnit, "Unit", LVCFMT_LEFT, 100, 100 );
	m_ColumnManager.AddColumn ( ColumnNames::Apparent, "Apparent From", LVCFMT_RIGHT, 85, 85 );
	m_ColumnManager.AddColumn ( ColumnNames::Apparent2, "Apparent To", LVCFMT_RIGHT, 85, 85 );
	m_ColumnManager.LoadSettings();
}

/**********************************************************************/

void CStockListTransferDlg::EditStockLevel()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nSelection = m_listStock.GetCurSel();

		if ( nSelection >= 0 && nSelection < m_pFilterArray -> GetSize() )
		{
			m_pFilterArray -> SetArrayIdx ( nSelection );	
			CStockEditTransferDlg dlg ( m_listStock, m_pFilterArray, this );
			dlg.DoModal();
			DisplayScreen ( m_pFilterArray -> GetArrayIdx() );
		}
	}
}

/**********************************************************************/

void CStockListTransferDlg::EditStockLevelRandom()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CStockEditTransferRandomDlg dlg( m_pFilterArray, this );
		dlg.DoModal();
		DisplayScreen ( m_pFilterArray -> GetArrayIdx() );
	}
}

/**********************************************************************/

void CStockListTransferDlg::SetFilterArray()
{
	m_pFilterArray = new CStockFilterArray ( FILTERTYPE_TRANSFERS );
	m_pFilterArray -> SetSpIdx ( m_nSpIdx, m_nSpIdx2 );	
	m_pFilterArray ->SetActiveItemOnlyFlag(TRUE);
	m_pFilterArray ->BuildRecordList();
}

/**********************************************************************/

void CStockListTransferDlg::HandleCancel()
{
	int nDbNo = dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() );
	bool bSavedData = FALSE;

	if ( DataManager.Stock.IsModified() == TRUE || m_bImported == TRUE )
	{
		int nChoice = Prompter.YesNoCancel ( "Stock database has been modified - save changes" );
		
		switch ( nChoice )
		{
		case IDYES:
			{
				if ( StockOptions.GetAutoNumXferFlag() == TRUE )
				{
					int nNum = StockOptions.GetAutoNumXfer() + 1;

					m_strReference1.Format( "%s%6.6d",
						StockOptions.GetAutoNumXferPrefix(),
						nNum );

					if ( nNum >= 999999 ) nNum = 0;
					StockOptions.SetAutoNumXfer( nNum );
				}

				bool bDoAudit = TRUE;
				
				CDataManagerInfo info;
				if ( ( DataManager.WriteStockLevels( m_nSpIdx, info ) == FALSE ) || ( DataManager.WriteStockLevels( m_nSpIdx2, info ) == FALSE ) )
				{
					Prompter.WriteError( info );
					bDoAudit = FALSE;
				}

				if ( TRUE == bDoAudit )
				{
					if ( DataManager.WriteTransferDetail( m_nSpIdx, info ) == FALSE ) 
						Prompter.WriteError( info );
				
					if ( DataManager.WriteTransferDetail( m_nSpIdx2, info ) == FALSE ) 
						Prompter.WriteError( info );
				
					AppendAudit();

					for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
					{	
						if ( dbLocation.GetDbNo( nLocIdx ) == nDbNo )
						{
							if ( DealerFlags.GetPriceSetExcludeByDefaultFlag() == TRUE )
							{
								CString strSource = dbLocation.GetFilePathDataLocalPluIncludeTemp( nLocIdx );
								CString strDest = dbLocation.GetFilePathDataLocalPluInclude( nLocIdx );
								CopyFile ( strSource, strDest, FALSE );
							}
							else
							{
								CString strSource = dbLocation.GetFilePathDataLocalPluExcludeTemp( nLocIdx );
								CString strDest = dbLocation.GetFilePathDataLocalPluExclude( nLocIdx );
								CopyFile ( strSource, strDest, FALSE );
							}
						}
					}

					for ( int nIndex = 0; nIndex < m_ImportFilesArray.GetSize(); nIndex++ )
					{
						CString strFile = m_ImportFilesArray.GetAt ( nIndex );
						CFileRemover FileRemover ( strFile );
					}

					bSavedData = TRUE;
				}
			}	
			break;

		case IDNO:
			{
				RemoveAudit();
			}
			break;

		case IDCANCEL:
			return;
		}
	}

	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{		
		if ( dbLocation.GetDbNo( nLocIdx ) == nDbNo )
		{
			CString strFile = dbLocation.GetFilePathDataLocalPluExcludeTemp( nLocIdx );
			CFileRemover FileRemover ( strFile );
		}
	}

	EndDialog ( ( bSavedData ) ? IDOK : IDCANCEL );
}

/**********************************************************************/

void CStockListTransferDlg::HandleImport()
{
	int nSelection = m_listStock.GetCurSel();

	CString strTitle = "";
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		strTitle = "Import Stock Transfers by Barcode Number";
	else
		strTitle = "Import Stock Transfers by Plu Number";

	CString strImportFile = StockOptions.GetImportXferPluName();
	if ( ( strImportFile.GetLength() == 0 ) || ( strImportFile.GetLength() > 25 ) )
		strImportFile = "transfer";

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
	
	CStockImportBasicTransferDlg dlgImport ( m_nSpIdx, m_nSpIdx2, strImportPath, m_strReference1, m_strReference2, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No transfer stock data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing stock transfers" );
	bool bResult = HandleImportInternal ( strImportPath, dlgImport.GetAction() );
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

bool CStockListTransferDlg::HandleImportInternal( const char* szImportPath, int nAction )
{
	CString strImportPath = szImportPath;
	
	CSSFile fileImport;
	if ( fileImport.Open ( strImportPath, "rb" ) == FALSE )
		return FALSE;
			
	CSSFile fileAudit;
	COleDateTime date = COleDateTime::GetCurrentTime();
	CString strTempAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
	if ( fileAudit.Open ( strTempAudit, "ab" ) == FALSE )
		return FALSE;

	CString strBuffer;
	int nLines = 0;

	int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );
	int nSpNo2 = dbStockpoint.GetSpNo( m_nSpIdx2 );

	while ( fileImport.ReadString ( strBuffer ) == TRUE )  
	{
		StatusProgress.SetPos ( fileImport );

		CCSV csv ( strBuffer );
		
		CString strPluNo;
		if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		{
			__int64 nPluNo = DataManager.Barcode.GetPluFromBarcode( _atoi64 ( csv.GetString(0) ) );
			::FormatPluNo ( nPluNo, strPluNo );
		}
		else
			strPluNo = csv.GetString(0);
		
		double dQty = csv.GetDouble( 1 );

		if ( dQty == 0.0 )
			continue;

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
		if ( DataManager.Stock.CheckStockLevelsForTransfer( nStockIdx, StockRecord, nSpNo, nSpNo2 ) != STOCKLEVEL_IMPORT_ACCEPT )
			continue;

		double dStockQtyPerPlu;
		DataManager.Plu.GetPluStockQty ( PluRecord.GetPluNoInt(), dStockQtyPerPlu );
				
		double dSUQtyToTransfer = dQty * dStockQtyPerPlu;

		if ( nAction == SUBTRACT_STOCK_LEVELS )
			dSUQtyToTransfer = -dSUQtyToTransfer;

		DataManager.Stock.UpdateTransferDetail( m_nSpIdx, m_nSpIdx2, nStockIdx, StockRecord, dSUQtyToTransfer );

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_TRANSFER );
		csvOut.Add ( 1 );
		csvOut.Add ( StockRecord.GetStockCode() );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
		csvOut.AddStockAuditQty ( dSUQtyToTransfer );
		fileAudit.WriteLine( csvOut.GetLine() );	
	}
	return TRUE;
}

/**********************************************************************/

void CStockListTransferDlg::HandleImportWrapper()
{
	int nSelection = m_listStock.GetCurSel();
	CString strTitle = "Import Transfers by Wrapper Code";
	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );

	CString strImportFile = StockOptions.GetImportXferWrpName();
	if ( ( strImportFile.GetLength() == 0 ) || ( strImportFile.GetLength() > 25 ) )
		strImportFile = "transfer";

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
	
	CStockImportBasicWrapperTransferDlg dlgImport ( m_nSpIdx, m_nSpIdx2, strImportPath, m_strReference1, m_strReference2, m_SuppRefFinder, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No transfer stock data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing stock transfers" );
	bool bResult = HandleImportWrapperInternal ( strImportPath, dlgImport.GetAction() );
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

bool CStockListTransferDlg::HandleImportWrapperInternal( const char* szImportPath, int nAction )
{
	CString strImportPath = szImportPath;

	CSSFile fileImport;
	if ( fileImport.Open ( strImportPath, "rb" ) == FALSE )
		return FALSE;

	CSSFile fileAudit;
	COleDateTime date = COleDateTime::GetCurrentTime();
	CString strTempAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
	if ( fileAudit.Open ( strTempAudit, "ab" ) == FALSE )
		return FALSE;
	
	CString strBuffer;
	int nLines = 0;

	int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );
	int nSpNo2 = dbStockpoint.GetSpNo( m_nSpIdx2 );

	while ( fileImport.ReadString ( strBuffer ) == TRUE )  
	{
		StatusProgress.SetPos ( fileImport );

		CCSV csvIn ( strBuffer );
		CString strWrapper = csvIn.GetString ( 0 );
		double dDUQty = csvIn.GetDouble( 1 );
		
		int nStockIdx, nStockSuppIdx;
		if ( m_SuppRefFinder.FindItemByWrapper( strWrapper, nStockIdx, nStockSuppIdx ) == FALSE )
			continue;

		CStockCSVRecord StockRecord;
		if ( DataManager.Stock.CheckStockLevelsForTransfer( nStockIdx, StockRecord, nSpNo, nSpNo2 ) != STOCKLEVEL_IMPORT_ACCEPT )
			continue;
		
		double dTotalSUQty = dDUQty * StockRecord.GetYield( nStockSuppIdx );
		
		if ( nAction == SUBTRACT_STOCK_LEVELS )
			dTotalSUQty = -dTotalSUQty;

		DataManager.Stock.UpdateTransferDetail( m_nSpIdx, m_nSpIdx2, nStockIdx, StockRecord, dTotalSUQty );

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_TRANSFER );
		csvOut.Add ( 2 );
		csvOut.Add ( StockRecord.GetStockCode() );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
		csvOut.AddStockAuditQty ( dDUQty );
		csvOut.AddStockAuditQty ( dTotalSUQty );
		fileAudit.WriteLine( csvOut.GetLine() );
	}
	fileImport.Close();

	return TRUE;
}

/**********************************************************************/

void CStockListTransferDlg::HandlePrintSession()
{
	COleDateTime date = COleDateTime::GetCurrentTime();

	CStockAuditTransfer Audit;
	Audit.SetReportType ( AUDIT_TRANSFER );
	
	CStockAuditHeaderInfo info;
	info.m_date = date;
	info.m_strReference1 = m_strReference1;
	info.m_strReference2 = m_strReference2;
	info.m_strSpName1 = dbStockpoint.GetName( m_nSpIdx );
	info.m_strSpName2 = dbStockpoint.GetName( m_nSpIdx2 );
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

void CStockListTransferDlg::PrepareMenu()
{
	m_MenuChanger.KillItem(IDM_EDIT_STOCKLEVEL2);
	m_MenuChanger.KillItem(IDM_COPY_LEVELS);
	m_MenuChanger.KillItem(IDM_IMPORT_CREATESHEET);
	m_MenuChanger.KillParent(IDM_REMOVE_ZEROS);

	if (DealerFlags.GetImportBarcodeFlag() == TRUE)
		m_MenuChanger.SetItemText(IDM_IMPORT_STOCKLEVEL, "By &Barcode Number");

	m_MenuChanger.KillItem(IDM_IMPORT_IMPORTSHEET);
	m_MenuChanger.KillItem(IDM_IMPORT_IMPORTSHEET_FOLDER);

	bool bKeepImportMenu = FALSE;

	if (StockOptions.GetImportXferPluFlag() == FALSE)
		m_MenuChanger.KillItem(IDM_IMPORT_STOCKLEVEL);
	else
		bKeepImportMenu = TRUE;

	if (StockOptions.GetImportXferWrapperFlag() == FALSE)
		m_MenuChanger.KillItem(IDM_IMPORT_WRAPPER);
	else
		bKeepImportMenu = TRUE;

	if (FALSE == bKeepImportMenu)
		m_MenuChanger.KillParent(IDM_IMPORT_STOCKLEVEL);
}

/**********************************************************************/

const char* CStockListTransferDlg::GetSpName()
{
	m_strSpName.Format ( "%s to %s, ", dbStockpoint.GetName( m_nSpIdx ), dbStockpoint.GetName( m_nSpIdx2 ) );
	return m_strSpName;
}

/**********************************************************************/

void CStockListTransferDlg::AppendAudit()
{
	COleDateTime date = COleDateTime::GetCurrentTime();
	CString strAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_NORMAL );
	CString strAudit2 = dbStockpoint.GetFilePathAudit( m_nSpIdx2, date, AUDIT_FAMILY_NORMAL );
	CString strTempAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
	CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
	CString strPrevAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV );
	CString strPrevEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV_EX );
	strAudit2 += "b";		

	CSSFile fileAudit;
	if ( fileAudit.Open ( strAudit, "ab" ) == TRUE )
	{
		CStockAuditBaseReport Audit;
		Audit.SetReportType ( AUDIT_TRANSFER );
		
		CStockAuditHeaderInfo info;
		info.m_date = date;
		info.m_strReference1 = m_strReference1;
		info.m_strReference2 = m_strReference2;
		info.m_strSpName1 = dbStockpoint.GetName( m_nSpIdx );
		info.m_strSpName2 = dbStockpoint.GetName( m_nSpIdx2 );
		fileAudit.WriteLine( Audit.GetHeaderLine( info ) );

		fileAudit.Close();
		::Append ( strTempAudit, strAudit );
	}

	CSSFile fileAudit2;
	if ( fileAudit2.Open ( strAudit2, "ab" ) == TRUE )
	{
		CStockAuditBaseReport Audit;
		Audit.SetReportType ( AUDIT_TRANSFER );
		
		CStockAuditHeaderInfo info;
		info.m_date = date;
		info.m_strReference1 = m_strReference1;
		info.m_strReference2 = m_strReference2;
		info.m_strSpName1 = dbStockpoint.GetName( m_nSpIdx );
		info.m_strSpName2 = dbStockpoint.GetName( m_nSpIdx2 );
		fileAudit2.WriteLine( Audit.GetHeaderLine( info ) );

		fileAudit2.Close();
		::Append ( strTempAudit, strAudit2 );
	}
	
	if ( fileAudit.Open ( strAudit, "ab" ) == TRUE )
	{
		CStockAuditBaseReport Audit;
		Audit.SetReportType ( AUDIT_EX_TRANSFER );
		
		CStockAuditHeaderInfo info;
		info.m_date = date;
		info.m_strReference1 = m_strReference1;
		info.m_strReference2 = m_strReference2;
		fileAudit.WriteLine ( Audit.GetHeaderLine( info ) );

		fileAudit.Close();
		::Append ( strTempEx, strAudit );
	}	
}

/**********************************************************************/
