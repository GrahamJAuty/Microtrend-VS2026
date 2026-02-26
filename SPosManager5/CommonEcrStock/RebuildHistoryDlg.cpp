/**********************************************************************/
 
/**********************************************************************/
#include "CashRSPImporter.h"
#include "ClearHistoryEposSales.h"
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
//#include "DealerFlags.h"
#include "EposReports.h"
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "ReportTypes.h"
#include "RepSpawn.h"
#include "NetworkCSVArray.h"
#include "StatusProgress.h"
#include "..\SPosStockManager5\StockAuditExceptions.h"
#include "..\SPosStockManager5\StockAuditExceptionsAdjustmentPlu.h"
#include "..\SPosStockManager5\StockAuditExceptionsSales.h"
/**********************************************************************/
#include "RebuildHistoryDlg.h"
/**********************************************************************/

CRebuildHistoryDlg::CRebuildHistoryDlg(CWnd* pParent)
	: CDialog(CRebuildHistoryDlg::IDD, pParent),
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CRebuildHistoryDlg)
	//}}AFX_DATA_INIT
	dbLocation.BuildSortIdxTables();
	dbDatabase.BuildSortIdxTables();
}

/**********************************************************************/

void CRebuildHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRebuildHistoryDlg)
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_CHECK_REMOVE_DUPLICATES, m_checkRemoveDuplicates);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CRebuildHistoryDlg, CDialog)
	//{{AFX_MSG_MAP(CRebuildHistoryDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_REBUILD, OnButtonRebuild)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CRebuildHistoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.SetMode( DATE_SELECTOR_MODE_STANDARD );
	m_DateSelectorRange.AddPastReportRanges();
	m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );
	
	m_listLocations.SubclassDlgItem ( IDC_LIST, this );
	m_listLocations.InsertColumn ( 0, "", LVCFMT_LEFT, 50 );
	m_listLocations.InsertColumn ( 1, "Location", LVCFMT_LEFT, 170 );
	m_listLocations.InsertColumn ( 2, "Terminals", LVCFMT_RIGHT, 70 );
	m_listLocations.InsertColumn ( 3, "Total TLog Size", LVCFMT_RIGHT, 110 );
	m_listLocations.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );
	
	m_ImageList.Create ( IDB_ICONS_TREE, 31, 0, COLORREF(0xFFFFFF) );
	m_listLocations.SetImageList ( &m_ImageList, LVSIL_SMALL );

	AddList();

	if ( m_listLocations.GetItemCount() > 0 )
	{
		m_listLocations.SetSelectionMark( 0 );
		m_listLocations.SetItemState( 0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
		m_listLocations.EnsureVisible( 0, FALSE );
		m_listLocations.SetFocus();
	}

	MessageLogger.LogSystem( 9, 1, TRUE );
	return FALSE;  
}

/**********************************************************************/

void CRebuildHistoryDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CRebuildHistoryDlg::GetReportDates()
{
	COleDateTime dateFrom,dateTo;
	m_DatePickerFrom.GetTime( dateFrom );
	m_DatePickerTo.GetTime( dateTo );
	m_OleDateFrom = dateFrom;
	m_OleDateTo = dateTo;
}

/**********************************************************************/

void CRebuildHistoryDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CRebuildHistoryDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CRebuildHistoryDlg::AddList()
{
	for ( int nNetworkIdx = 0; nNetworkIdx < dbNetwork.GetSize(); nNetworkIdx++ )
	{
		int nStartLocIdx, nEndLocIdx;
		dbLocation.GetNetworkLocIdxRange( nNetworkIdx, nStartLocIdx, nEndLocIdx );

		for ( int nLocIdx = nStartLocIdx; nLocIdx <= nEndLocIdx; nLocIdx++ )
		{
			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber( dbLocation.GetDbNo( nLocIdx ), nDbIdx ) == TRUE )
			{
				CString strCount;
				int nCount = dbLocation.GetTerminalCount( nLocIdx );
				strCount.Format( "%d", dbLocation.GetTerminalCount( nLocIdx ) );

				int nFileLength = 0;
				for ( int nTerminalIdx = 0; nTerminalIdx < nCount; nTerminalIdx++ )
				{
					int nTerminalNo = dbLocation.GetTNo( nLocIdx, nTerminalIdx );

					CString strFilenameLegacy, strFilenameTrading, strFilenameTraining;
					::GetTermSaleFilename( CASHRSP_OBSOLETE_TERMSAVE, nTerminalNo, strFilenameLegacy );
					::GetTermSaleFilename( CASHRSP_SALES_TRADING, nTerminalNo, strFilenameTrading );

					CString strNetworkPath = dbNetwork.GetFolderPathData( nNetworkIdx );
					strNetworkPath += "\\";

					CSSFile fileLegacy;
					if ( fileLegacy.Open( strNetworkPath + strFilenameLegacy, "rb" ) == TRUE )
					{
						nFileLength += fileLegacy.GetLength();
						fileLegacy.Close();
					}
					
					CSSFile fileTrading;
					if ( fileTrading.Open( strNetworkPath + strFilenameTrading, "rb" ) == TRUE )
					{
						nFileLength += fileTrading.GetLength();
						fileTrading.Close();
					}
				}
					
				int nKbLength = 0;

				if ( nFileLength > 1024 ) 
					nKbLength = nFileLength / 1024;
				else if ( nFileLength > 0 )
					nKbLength = 1;
				else
					nKbLength = 0;
					
				CRebuildHistoryInfo info;
				info.m_nNetworkIdx = nNetworkIdx;
				info.m_nLocIdx = nLocIdx;
				info.m_nKbLength = nKbLength;

				m_arrayFileInfo.Add( info );

				CString strKb;
				strKb.Format( "%d kb", nKbLength );

				int nIconType = NODE_ICON_LOCATION;

				int nPos = m_listLocations.GetItemCount();
				m_listLocations.InsertItem ( nPos, "", nIconType );
				m_listLocations.SetItem ( nPos, 1, LVIF_TEXT, dbLocation.GetName ( nLocIdx ), 0, 0, 0, NULL );
				m_listLocations.SetItem ( nPos, 2, LVIF_TEXT, strCount, 0, 0, 0, NULL );
				m_listLocations.SetItem ( nPos, 3, LVIF_TEXT, strKb, 0, 0, 0, NULL );
				m_listLocations.SetCheck ( nPos, TRUE );
			}
		}
	}
}

/**********************************************************************/

void CRebuildHistoryDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowTransactionLog( FALSE );
	*pResult = 0;
}

/**********************************************************************/

void CRebuildHistoryDlg::OnButtonAll()
{
	for ( int n = 0; n < m_listLocations.GetItemCount(); n++ )
		m_listLocations.SetCheck( n, TRUE );

	RefreshSelection();
}

/**********************************************************************/

void CRebuildHistoryDlg::OnButtonNone()
{
	for ( int n = 0; n < m_listLocations.GetItemCount(); n++ )
		m_listLocations.SetCheck( n, FALSE );

	RefreshSelection();
}

/**********************************************************************/

void CRebuildHistoryDlg::OnButtonView() 
{
	ShowTransactionLog( TRUE );
	RefreshSelection();
}

/**********************************************************************/

void CRebuildHistoryDlg::RefreshSelection() 
{
	int nSel = m_listLocations.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayFileInfo.GetSize() ) )
	{
		m_listLocations.SetSelectionMark( nSel );
		m_listLocations.SetItemState( nSel, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
		m_listLocations.EnsureVisible( nSel, FALSE );
		m_listLocations.SetFocus();
	}
}

/**********************************************************************/

void CRebuildHistoryDlg::ShowTransactionLog( bool bAllTicked )
{
	StatusProgress.SetFixedForegroundWindow( this );
	ShowTransactionLogInternal( bAllTicked );
	StatusProgress.ClearFixedForegroundWindow();
}

/**********************************************************************/

void CRebuildHistoryDlg::ShowTransactionLogInternal( bool bAllTicked )
{
	CEposSelectArray SelectArray;
	CArray<int,int> arraySelected;
	
	if ( FALSE == bAllTicked )
	{
		int nSel = m_listLocations.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayFileInfo.GetSize() ) )
			arraySelected.Add( nSel );			
	}
	else
	{
		for ( int nSel = 0; ( nSel < m_arrayFileInfo.GetSize() ) && ( nSel < m_listLocations.GetItemCount() ); nSel++ )
			if ( m_listLocations.GetCheck(nSel) != 0 )
				arraySelected.Add( nSel );
	}

	for ( int n = 0; n < arraySelected.GetSize(); n++ )
	{
		int nSel = arraySelected.GetAt(n);
		int nLocIdx = m_arrayFileInfo[nSel].m_nLocIdx;
		SelectArray.SelectLocation( nLocIdx );
	}

	if ( SelectArray.IsEmpty() == TRUE )
	{
		Prompter.Error( "You must select at least one location for this report" );
	}
	else
	{
		CString strTitle = "Selected Locations";

		if ( arraySelected.GetSize() == 1 )
		{
			int nSel = arraySelected.GetAt(0);
			int nLocIdx = m_arrayFileInfo[nSel].m_nLocIdx;
			strTitle.Format ( "%s", dbLocation.GetName( nLocIdx ) );
		}

		/*
		int nDbIdx;
		int nDbNo = dbLocation.GetDbNo( nLocIdx );
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
		}
		else
			DataManager.CloseDatabase();
		*/
	
		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		
		CSessionCSVRecord ReportSession;

		GetReportDates();

		CEposReportTransactionSorted Report( SelectArray, REPORT_TYPE_TRANSACTION_REBUILD );
		Report.SetDates( m_OleDateFrom, m_OleDateTo, ReportSession );
		Report.SetConLevelTitle( strTitle );
		Report.SetSessionTitle( "All Transactions" );
		Report.m_ReportFilters.DeptFilter.SetDeptFilterNo( DEPARTMENTSET_ALL );
		//Report.SetNoReadDatabaseFlag();
		
		bool bResult = Report.CreateReport();

		if ( Report.GetCashRSPVersionCheckerFailFlag() == TRUE )
		{
			Report.ShowCashRSPVersionCheckerFailMessage();
		}
		else if ( ( FALSE == bResult ) )
			Prompter.Error( "Unable to create report." );
		else 
		{
			CReportManager ReportManager;
			if ( ReportManager.DisplayReport ( Super.ReportFilename(), Report.GetParamsFilename(), "", this ) == TRUE )
			{
				for ( int n = 0; n < arraySelected.GetSize(); n++ )
				{
					int nSel = arraySelected.GetAt(n);
					int nNetworkIdx = m_arrayFileInfo[nSel].m_nNetworkIdx;
					int nLocIdx = m_arrayFileInfo[nSel].m_nLocIdx;
					int nKbLength = m_arrayFileInfo[nSel].m_nKbLength;

					CStringArray str;
					str.Add( dbLocation.GetName( nLocIdx ) );

					CDWordArray dw;
					dw.Add( dbNetwork.GetNetworkNo ( nNetworkIdx ) );
					dw.Add( dbLocation.GetLocNo ( nLocIdx ) );
					dw.Add( nKbLength );

					MessageLogger.LogSystem( 9, 2, FALSE, dw, str );
				}
			}
		}
	}
}

/**********************************************************************/

void CRebuildHistoryDlg::OnButtonRebuild() 
{
	StatusProgress.SetFixedForegroundWindow( this );
	HandleRebuild();
	StatusProgress.ClearFixedForegroundWindow();
}

/**********************************************************************/

void CRebuildHistoryDlg::HandleRebuild()
{

#ifdef STOCKMAN_SYSTEM

	if ( DealerFlags.GetRebuildStockSalesHistoryFlag() == TRUE )
	{
		RebuildStockSalesHistory();
		return;
	}

#endif

	CEposSelectArray SelectArray;
	CArray<int,int> arraySelected;

	for ( int nSel = 0; ( nSel < m_arrayFileInfo.GetSize() ) && ( nSel < m_listLocations.GetItemCount() ); nSel++ )
		if ( m_listLocations.GetCheck(nSel) != 0 )
			arraySelected.Add( nSel );

	for ( int n = 0; n < arraySelected.GetSize(); n++ )
	{
		int nSel = arraySelected.GetAt(n);
		int nLocIdx = m_arrayFileInfo[nSel].m_nLocIdx;
		SelectArray.SelectLocation( nLocIdx );
	}

	if ( SelectArray.IsEmpty() == TRUE )
	{
		Prompter.Error( "You must select at least one location to rebuild sales history." );
	}
	else
	{
		CStringArray array;
		array.Add( "Rebuild Plu Sales History for Selected Locations" );
		array.Add( "The existing sales history for these locations will be deleted." );
		array.Add( "" );
		array.Add( "The sales history will be rebuilt from the transaction logs." );

		if ( Prompter.Warning( array, FALSE ) != IDYES )
			return;

		//AVOID USING NODE_DATABASE / NODE_SYSTEM SO THAT STOCK SALES STAY IN PLACE
		CLocationSelectorEntity LocSelEntity;
		LocSelEntity.SetLocationNode(0);

		GetReportDates();
		CClearHistoryEposSales Clear ( m_OleDateFrom, m_OleDateTo, LocSelEntity, SelectArray, TRUE, TRUE, TRUE, TRUE );
		
		CashRSPImporter.ClearDuplicatedTransactionCount();
		CashRSPImporter.ClearCashRSPVersionCheckerFailFlag();
		CashRSPImporter.SetRebuildRemoveDuplicatesFlag( m_checkRemoveDuplicates.GetCheck() != 0 );
		
		for ( int n = 0; ( n < arraySelected.GetSize() ) && ( CashRSPImporter.GetCashRSPVersionCheckerFailFlag() == FALSE ); n++ )
		{	
			CString strDateFrom;
			strDateFrom.Format( "%4.4d%2.2d%2.2d", m_OleDateFrom.GetYear(), m_OleDateFrom.GetMonth(), m_OleDateFrom.GetDay() );

			CString strDateTo;
			strDateTo.Format( "%4.4d%2.2d%2.2d", m_OleDateTo.GetYear(), m_OleDateTo.GetMonth(), m_OleDateTo.GetDay() );

			int nSel = arraySelected.GetAt(n);
			int nLocIdx = m_arrayFileInfo[nSel].m_nLocIdx;
			CashRSPImporter.RebuildLocationSales( nLocIdx, strDateFrom, strDateTo );
		}

		if ( CashRSPImporter.GetCashRSPVersionCheckerFailFlag() == TRUE )
		{
			CashRSPImporter.ShowCashRSPVersionCheckerFailMessage();
			CashRSPImporter.ClearCashRSPVersionCheckerFailFlag();
		}
		else
		{
			for ( int n = 0; n < m_arrayFileInfo.GetSize(); n++ )
			{
				CStringArray str;
				str.Add( dbLocation.GetName( m_arrayFileInfo[n].m_nLocIdx ) );

				CDWordArray dw;
				dw.Add( dbNetwork.GetNetworkNo ( m_arrayFileInfo[n].m_nNetworkIdx ) );
				dw.Add( dbLocation.GetLocNo ( m_arrayFileInfo[n].m_nLocIdx ) );
				dw.Add( m_arrayFileInfo[n].m_nKbLength );

				MessageLogger.LogSystem( 9, 4, FALSE, dw, str );
			}

			CString strDuplicates = "";
			if ( m_checkRemoveDuplicates.GetCheck() != 0 )
			{
				int nDTs = CashRSPImporter.GetDuplicatedTransactionCount();
				switch( nDTs )
				{
				case 0:
					strDuplicates = "\n\n(no duplicated transactions were found)";
					break;

				case 1:
					strDuplicates = "\n\n(1 duplicated transaction was removed)";
					break;

				default:
					strDuplicates.Format( "\n\n(%d duplicated transactions were removed)", nDTs );
					break;
				}
			}

			CString strMessage = "";
			strMessage += "The sales history has been rebuilt as requested.";
			strMessage += strDuplicates;
			Prompter.Info ( strMessage  );
		}
	}
}

/**********************************************************************/

void CRebuildHistoryDlg::RebuildStockSalesHistory()
{
#ifdef STOCKMAN_SYSTEM

	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
	{
		CString strPath = "";
		strPath += dbStockpoint.GetFolderPathHistory( nSpIdx );
		strPath += "\\";
		
		CStringArray arrayFiles;

		CFileFind finder;
		BOOL bWorking = finder.FindFile ( strPath + "st*.002" );
		while ( bWorking == TRUE )
		{
			bWorking = finder.FindNextFile();

			if ( finder.IsDots() == TRUE )
				continue;

			if ( finder.IsDirectory() == TRUE )
				continue;

			CString strFilename = finder.GetFileName();
			if ( strFilename.GetLength() != 12 )
				continue;

			CString strMonth = strFilename.Mid(2,6);
			if ( ::TestNumeric( strMonth ) == FALSE )
				continue;

			CString strYear = strMonth.Left(2);
			if ( ( strYear == "19" ) || ( strYear == "20" ) )
				continue;

			arrayFiles.Add( strFilename );
		}

		for ( int n = 0; n < arrayFiles.GetSize(); n++ )
		{
			CString strOldFilename = arrayFiles.GetAt(n);
			
			CString strNewFilename = "St20";
			strNewFilename += strOldFilename.Mid(6,2);
			strNewFilename += strOldFilename.Mid(2,2);
			strNewFilename += ".002";

			int nNewDate = atoi( strOldFilename.Mid(4,2) );

			CString strRenamedFile = strOldFilename + ".XXX";
			if ( rename( strPath + strOldFilename, strPath + strRenamedFile ) != 0 )
				continue;

			CSSFile fileOld;
			if ( fileOld.Open( strPath + strRenamedFile, "rb" ) == FALSE )
				continue;

			CSSFile fileNew;
			if ( fileNew.Open( strPath + strNewFilename, "ab" ) == FALSE )
				continue;

			CString strBuffer;
			while( fileOld.ReadString( strBuffer ) == TRUE )
			{
				CCSV csv( strBuffer );
				csv.SetAt( 0, nNewDate );
				fileNew.WriteLine( csv.GetLine() );
			}

			fileOld.Close();
			fileNew.Close();

			CFileRemover FileRemover( strPath + strRenamedFile );

		}
	}

	GetReportDates();
		
	CString strDateFrom;
	strDateFrom.Format( "%4.4d%2.2d%2.2d", m_OleDateFrom.GetYear(), m_OleDateFrom.GetMonth(), m_OleDateFrom.GetDay() );

	CString strDateTo;
	strDateTo.Format( "%4.4d%2.2d%2.2d", m_OleDateTo.GetYear(), m_OleDateTo.GetMonth(), m_OleDateTo.GetDay() );

	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
	{
		CString strPath = "";
		strPath += dbStockpoint.GetFolderPathAudit( nSpIdx );
		strPath += "\\";
		
		CFileFind finder;
		BOOL bWorking = finder.FindFile ( strPath + "*.001" );
		while ( bWorking == TRUE )
		{
			bWorking = finder.FindNextFile();

			if ( finder.IsDots() == TRUE )
				continue;

			if ( finder.IsDirectory() == TRUE )
				continue;

			CString strFilename = finder.GetFileName();
			if ( strFilename.GetLength() != 12 )
				continue;

			CString strDate = strFilename.Left(8);
			if ( ::TestNumeric( strDate ) == FALSE )
				continue;

			if ( ( strDate < strDateFrom ) || ( strDate > strDateTo ) )
				continue;

			COleDateTime dateCheck = COleDateTime(
				atoi( strDate.Left(4) ),
				atoi( strDate.Mid(4,2) ),
				atoi( strDate.Right(2) ),
				0, 0, 0 );

			if ( dateCheck.m_status != COleDateTime::valid )
				continue;

			CSSFile fileSource;
			if ( fileSource.Open( strPath + strFilename, "rb" ) == FALSE )
				continue;

			CString strDestPath;
			strDestPath = dbStockpoint.GetFolderPathSalesXXX( nSpIdx );
			::CreateSubdirectory( strDestPath );
			strDestPath += "\\SALESXXX_";
			strDestPath += strFilename;

			CSSFile fileDest;
			if ( fileDest.Open( strDestPath, "ab" ) == FALSE )
				continue;

			CStockAuditExceptionsSales AuditSales;
			AuditSales.SetAuditMode( AUDIT_MODE_NORMAL );
			AuditSales.SetReportType( AUDIT_EX_SALES );

			CStockAuditExceptionsAdjustmentPlu AuditWastage;
			AuditWastage.SetAuditMode( AUDIT_MODE_NORMAL );
			AuditWastage.SetReportType( AUDIT_EX_ADJUSTMENT_PLU );

			CStockAuditExceptions AuditDelivery( AUDIT_EX_DELIVERY );
			AuditDelivery.SetAuditMode( AUDIT_MODE_NORMAL );
			
			AuditSales.SetE1SalesFlag( TRUE );
			AuditSales.SetManualSalesFlag( TRUE );
			AuditSales.SetImportSalesFlag( TRUE );
			bool bSales = FALSE;
			bool bWastage = FALSE;
			bool bDelivery = FALSE;
			bool bWrittenHeader = FALSE;

			CString strMisc20 = "(MISC20) ";

			CString strBuffer;
			while ( fileSource.ReadString( strBuffer ) == TRUE )
			{
				CCSV csv( strBuffer );
				if ( strBuffer.Left(1) == "H" )
				{
					bSales = FALSE;
					bWastage = FALSE;
					bDelivery = FALSE;
					bWrittenHeader = FALSE;

					if ( AuditSales.ProcessHeader( strBuffer ) == TRUE )
						bSales = TRUE;
					else if ( AuditWastage.ProcessHeader( strBuffer ) == TRUE )
						bWastage = TRUE;
					else if ( AuditDelivery.ProcessHeader( strBuffer ) == TRUE )
						bDelivery = TRUE;

					continue;
				}

				if ( strBuffer.Left(1) == "D" )
				{
					if ( TRUE == bSales )
					{
						if ( AuditSales.ProcessLine( strBuffer ) == TRUE )
						{
							if ( FALSE == bWrittenHeader )
							{
								CString strLineDate = AuditSales.GetLineSaleDate();
								CString strLineTime = AuditSales.GetLineSaleTime();
								::MakeYearFirstDate( strLineDate );

								CCSV csvOut;
								csvOut.Add( "H" );
								csvOut.Add( "1" );
								csvOut.Add( AuditSales.GetHeaderDate() );
								csvOut.Add( strLineDate );
								csvOut.Add( strLineTime );
								csvOut.Add( strMisc20 + AuditSales.GetLineLoc() );
								fileDest.WriteLine( csvOut.GetLine() );
								bWrittenHeader = TRUE;
							}

							CCSVStockAudit csvOut;
							csvOut.Add( "D" );
							csvOut.Add( "1" );
							csvOut.Add( "1" );
							csvOut.Add( AuditSales.GetLinePluNo() );
							csvOut.Add( "0" );
							csvOut.AddStockAuditQty( AuditSales.GetLineQty() );
							csvOut.AddStockAuditVal( AuditSales.GetLineVal() );
							fileDest.WriteLine( csvOut.GetLine() );
						}
					}
					else if ( TRUE == bWastage )
					{
						if ( AuditWastage.ProcessLine( strBuffer ) == TRUE )
						{
							CString strReason = AuditWastage.GetLineReason();
							strReason.MakeUpper();

							if ( strReason == "EPOS WASTAGE" )
							{
								if ( FALSE == bWrittenHeader )
								{
									CString strLineDate = AuditWastage.GetLineSaleDate();
									CString strLineTime = AuditWastage.GetLineSaleTime();
									::MakeYearFirstDate( strLineDate );

									CCSV csvOut;
									csvOut.Add( "H" );
									csvOut.Add( "1" );
									csvOut.Add( AuditWastage.GetHeaderDate() );
									csvOut.Add( strLineDate );
									csvOut.Add( strLineTime );
									csvOut.Add( strMisc20 + "Unknown" );
									fileDest.WriteLine( csvOut.GetLine() );
									bWrittenHeader = TRUE;
								}

								CCSVStockAudit csvOut;
								csvOut.Add( "D" );
								csvOut.Add( "1" );
								csvOut.Add( "2" );
								csvOut.Add( AuditWastage.GetLinePluNo() );
								csvOut.Add( "0" );
								csvOut.AddStockAuditQty( AuditWastage.GetLineQty() );
								csvOut.AddStockAuditVal( AuditWastage.GetLineVal() );
								fileDest.WriteLine( csvOut.GetLine() );
							}
						}
					}
					else if ( TRUE == bDelivery )
					{
						if ( AuditDelivery.ProcessLine( strBuffer ) == TRUE )
						{
							if ( FALSE == bWrittenHeader )
							{
								CString strLineDate = AuditDelivery.GetLineSaleDate();
								CString strLineTime = AuditDelivery.GetLineSaleTime();
								::MakeYearFirstDate( strLineDate );

								CCSV csvOut;
								csvOut.Add( "H" );
								csvOut.Add( "1" );
								csvOut.Add( AuditDelivery.GetHeaderDate() );
								csvOut.Add( strLineDate );
								csvOut.Add( strLineTime );
								csvOut.Add( strMisc20 + "Unknown" );
								fileDest.WriteLine( csvOut.GetLine() );
								bWrittenHeader = TRUE;
							}

							CCSVStockAudit csvOut;
							csvOut.Add( "D" );
							csvOut.Add( "1" );
							csvOut.Add( "3" );
							csvOut.Add( AuditDelivery.GetLinePluNo() );
							csvOut.Add( "0" );
							csvOut.AddStockAuditQty( AuditWastage.GetLineQty() );
							csvOut.Add( "0" );
							fileDest.WriteLine( csvOut.GetLine() );
						}
					}
				}
			}
		}
	}

	Prompter.Info( "Fixed Stock Sales History & Exceptions\n\nEPOS daily files unchanged." );

#endif
}

/**********************************************************************/

void CRebuildHistoryDlg::OnCancel() 
{	
	EndDialog( IDCANCEL );
}

/**********************************************************************/

void CRebuildHistoryDlg::OnOK() 
{	
	EndDialog( IDCANCEL );
}

/**********************************************************************/
