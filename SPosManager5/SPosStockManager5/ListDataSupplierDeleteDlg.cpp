/**********************************************************************/
#include "CCSVReportLine.h"
#include "..\CommonEcrStock\CustomExportManager.h"
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "ListDataSupplierDeleteDlg.h"
/**********************************************************************/

CSingleListContentHelpersSupplierDelete::CSingleListContentHelpersSupplierDelete( CWordArray& arraySuppIdx ) : m_arraySuppIdx( arraySuppIdx )
{
}

/**********************************************************************/

int CSingleListContentHelpersSupplierDelete::GetArraySize()
{
	return m_arraySuppIdx.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersSupplierDelete::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		int nSuppIdx = m_arraySuppIdx.GetAt ( nIndex );

		CSupplierCSVRecord SupplierRecord;
		DataManager.Supplier.GetAt ( nSuppIdx, SupplierRecord );

		CCSV csv ( '\t' );
		csv.Add( SupplierRecord.GetKey() );
		csv.Add( SupplierRecord.GetName() );
		
		strLine = csv.GetLine();

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataSupplierDeleteDlg::CListDataSupplierDeleteDlg( CWordArray& arraySuppIdx, CWnd* pParent) : CListDataDlg ( pParent ), m_arraySuppIdx( arraySuppIdx )
{
	m_bMultiSelect = FALSE;
	m_nListSelection = -1;

	m_pContentHelpers = new CSingleListContentHelpersSupplierDelete( m_arraySuppIdx );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataSupplierDeleteDlg::~CListDataSupplierDeleteDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataSupplierDeleteDlg::SetTitle()
{
	SetWindowText ( "Delete Suppliers" );
}

/**********************************************************************/

void CListDataSupplierDeleteDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Delete" );
	m_button2.ShowWindow( SW_HIDE );
	m_button3.ShowWindow( SW_HIDE );
	m_button4.SetWindowText( "&Check" );
	m_button5.ShowWindow( SW_HIDE );
	m_button6.ShowWindow( SW_HIDE );
	m_buttonOK.ShowWindow( SW_HIDE );
	m_buttonCancel.SetWindowText( "Close" );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button4, 368, 147 );
}

/**********************************************************************/

void CListDataSupplierDeleteDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "Supplier Key", LVCFMT_LEFT, 160 );
	m_listData.InsertColumn ( 1, "Name", LVCFMT_LEFT, 320 );
}

/**********************************************************************/

void CListDataSupplierDeleteDlg::HandleButton1() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	int nSuppIdx = m_arraySuppIdx.GetAt( nIndex );
	
	CSupplierCSVRecord Supplier;
	DataManager.Supplier.GetAt( nSuppIdx, Supplier );
	int nSuppNo = Supplier.GetSuppNo();

	CStringArray StringArray;

	CString strTitle;
	strTitle.Format ( "Delete Supplier ( %s )", Supplier.GetName() );
	StringArray.Add ( strTitle );
	StringArray.Add ( "The selected supplier will be deleted." );
	StringArray.Add ( "" );
	StringArray.Add ( "All stock item references to this supplier will be lost." );
	StringArray.Add ( "" );
	StringArray.Add ( "Alternative suppliers may be applied to some stock items." );
	StringArray.Add ( "" );
	StringArray.Add ( "The cost price of some stock items may change." );
	StringArray.Add ( "" );
	StringArray.Add ( "The retail price of some plu items may change." );

	if ( Prompter.Warning( StringArray ) == IDYES )
	{
		//REMOVE FILES RELATING TO THE SUPPLIER
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			CString strPath = dbDatabase.GetFolderPathData ( nDbIdx );
			strPath += "\\";

			if ( ::ExistSubdirectory ( strPath ) )
			{	
				CFileRemover FileRemover;
				FileRemover.RemoveFile ( strPath + DataManager.Supplier.GetAuditFilename( nSuppIdx ) );
				FileRemover.RemoveFile ( strPath + DataManager.Supplier.GetTempFilename ( nSuppIdx ) );
			}
		}

		//REMOVE THE SUPPLIER FROM PLU, STOCK AND ORDER DATABASES
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			CWaitCursor wait;

			CWordArray arrayReadWrite;
			arrayReadWrite.Add( NODE_STOCK );
			arrayReadWrite.Add( NODE_PLU );

			CDataManagerInfo info;
			if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info, FALSE ) == TRUE )
			{
				int nResult = DataManager.Stock.PurgeSupplier ( nSuppNo );	

				if ( nResult != SUPPLIER_PURGE_NOCHANGE )
					DataManager.WriteStock( info, TRUE, TRUE );

				if ( nResult == SUPPLIER_PURGE_CHANGEPLU )
				{
					DataManager.Plu.UpdatePrices();
					DataManager.WritePlu( info );
					CCustomExportManagerPlu CustomExport;
				}

				
				DataManager.CloseDatabase();
			}
		}

		CDataManagerInfo info;

		//REMOVE THE SUPPLIER FROM SUPPLIER SETS
		DataManager.SupplierSet.PurgeSupplier ( nSuppNo );
		DataManager.ForceWriteSupplierSet( info );

		//DELETE THE SUPPLIER
		DataManager.Supplier.DeleteSupplier( nSuppIdx );
		DataManager.ForceWriteSupplier( info );
		
		//ADJUST THE LIST OF SUPPLIER INDICES DOWNWARDS
		m_arraySuppIdx.RemoveAt( nIndex );
		for ( int n = 0; n < m_arraySuppIdx.GetSize(); n++ )
		{
			int nArraySuppIdx = m_arraySuppIdx.GetAt(n);

			if ( nArraySuppIdx > nSuppIdx )
				m_arraySuppIdx.SetAt( n, nArraySuppIdx - 1 );
		}
		
		m_listData.SetItemCountEx( m_arraySuppIdx.GetSize() );
		SelectLine( nIndex );
	}
}

/**********************************************************************/

void CListDataSupplierDeleteDlg::HandleButton4() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	int nSuppIdx = m_arraySuppIdx.GetAt( nIndex );

	CSupplierCSVRecord Supplier;
	DataManager.Supplier.GetAt( nSuppIdx, Supplier );

	CString strTitle;
	strTitle.Format ( "Preferred Supplier Usage (%s)", Supplier.GetName() );

	int nSuppNo = Supplier.GetSuppNo();
	int nTotalLinks = 0;
	int nTotalDatabases = 0;

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		ReportFile.SetStyle1 ( strTitle );
		ReportFile.AddColumn( "Code", TA_LEFT, 380 );
		ReportFile.AddColumn( "Description", TA_LEFT, 300 );
		ReportFile.AddColumn( "Category", TA_LEFT, 300 );
		ReportFile.AddColumn ( "DU Name", TA_LEFT, 250 );
		ReportFile.AddColumn ( "DU Cost", TA_RIGHT, 180 );
		ReportFile.AddColumn ( "DU Yield", TA_RIGHT, 220 );

		StatusProgress.Lock( TRUE, "Creating report" );

		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
			
			if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
				continue;

			bool bGotDatabaseEntry = FALSE;

			int nSize = DataManager.Stock.GetSize();
			for ( int nStockIdx = 0; nStockIdx < nSize; nStockIdx++ )
			{	
				StatusProgress.SetPos( nStockIdx, nSize );

				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt( nStockIdx, StockRecord );

				if ( StockRecord.GetSupplierCount() == 0 )
					continue;

				if ( StockRecord.GetSuppNo(0) != nSuppNo )
					continue;

				if ( bGotDatabaseEntry == FALSE )
				{
					if ( nTotalDatabases != 0 )
						ReportFile.WriteLine( "" );

					ReportFile.WriteLine( dbDatabase.GetName( nDbIdx ) );
					ReportFile.WriteLine( "<LI>" );
					bGotDatabaseEntry = TRUE;
					nTotalDatabases++;
				}

				nTotalLinks++;

				CStockReportLine csv;
				csv.AppendString( StockRecord.GetStockCode() );
				csv.AppendString( StockRecord.GetDescription() );
				csv.AppendString( DataManager.Category.GetText( StockRecord.GetCategory() ) );
				csv.AppendString( StockRecord.GetCase(0) );
				csv.AppendVal( StockRecord.GetCost(0) );
		
				CString strItem;
				strItem.Format ( "%d x %.3f", StockRecord.GetDUItems(0), StockRecord.GetDUItemSize(0) );
				csv.AppendString( strItem );
		
				ReportFile.WriteLine( csv.GetLine() );
			}
		}

		CString strItem = ( nTotalLinks == 1 ) ? "item" : "items";
		CString strDb = ( nTotalDatabases == 1 ) ? "database" : "databases";

		CString strMsg;
		strMsg.Format ( "<..>Supplier %s has preferred status for %d stock %s across %d %s",
			Supplier.GetName(),
			nTotalLinks,
			(const char*) strItem,
			nTotalDatabases,
			(const char*) strDb );

		ReportFile.WriteLine( "" );
		ReportFile.WriteLine( strMsg );
		ReportFile.Close();

		StatusProgress.Unlock();
	
		CReportManager ReportManager;
		CFilenameUpdater FnUp( SysFiles::SupplierDelPrm );
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
}

/**********************************************************************/
