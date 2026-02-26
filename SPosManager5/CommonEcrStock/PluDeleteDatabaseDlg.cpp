/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "CustomExportManager.h"
#include "DatabaseCSVArray.h"
#include "FilterTypes.h"
#include "KeyboardHelpers.h"
#include "MessageLogger.h"
#include "PluDeletePrintDlg.h"
#include "PluDeleteRangeSetDlg.h"
#include "PluInactiveCloseDlg.h"
#include "NodeTypes.h"
#include "PluNumberDlg.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockDeleteCloseDlg.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "PluDeleteDatabaseDlg.h"
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

CPluDeleteDatabaseDlg::CPluDeleteDatabaseDlg( bool bDeleteItems, CWnd* pParent )
	: CDialog(CPluDeleteDatabaseDlg::IDD, pParent), 
	m_FilterArray ( FILTERTYPE_PLU_DELETION ),
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CPluDeleteDatabaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bDoneDelete = FALSE;
	m_bDeleteItems = bDeleteItems;

	m_nCacheIndex = -1;

	m_strDatabaseName = "";
	int nPluDbIdx = DataManager.GetActiveDbIdx();

	if ( nPluDbIdx >= 0 && nPluDbIdx < dbDatabase.GetSize() )
		m_strDatabaseName = dbDatabase.GetName( nPluDbIdx );
	
	m_nDeletionCount = 0;
	m_FilterArray.QuickBuildUnlinked( FALSE == bDeleteItems );
	
	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_bInitDialog = FALSE;
	
	if ( TRUE == m_bDeleteItems )
		m_strTitle = "Delete Unlinked Plu Records";
	else
		m_strTitle = "Make Unlinked Items Inactive";

	m_bUnusedMode = FALSE;
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
}	
	
/**********************************************************************/
#endif
/**********************************************************************/

CPluDeleteDatabaseDlg::CPluDeleteDatabaseDlg( bool bDeleteItems, CByteArray& FlagArray, CWnd* pParent )
	: CDialog(CPluDeleteDatabaseDlg::IDD, pParent), 
	m_FilterArray ( FILTERTYPE_PLU_DELETION ),
	m_CacheCSV ( '\t' )
{
	m_bDoneDelete = FALSE;
	m_bDeleteItems = bDeleteItems;

	m_nCacheIndex = -1;

	m_strDatabaseName = "";
	int nPluDbIdx = DataManager.GetActiveDbIdx();

	if ( nPluDbIdx >= 0 && nPluDbIdx < dbDatabase.GetSize() )
		m_strDatabaseName = dbDatabase.GetName( nPluDbIdx );

	m_nDeletionCount = 0;
	m_FilterArray.QuickBuild( FlagArray, (FALSE == bDeleteItems) );
	
	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_bInitDialog = FALSE;

	if ( TRUE == m_bDeleteItems )
		m_strTitle = "Delete Unsold Plu Records";
	else
		m_strTitle = "Make Unsold Items Inactive";

	m_bUnusedMode = TRUE;
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
}	
	
/**********************************************************************/

CPluDeleteDatabaseDlg::~CPluDeleteDatabaseDlg()
{
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluDeleteDatabaseDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluDeleteDatabaseDlg, CDialog)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CPluDeleteDatabaseDlg)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_COMMAND(IDM_STOCK_DELETE_RANGE, OnPluDeleteRange)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_COMMAND(IDM_STOCK_DELETE_ALL, OnPluDeleteAll)
	ON_COMMAND(IDM_STOCK_DELETE_NONE, OnPluDeleteNone)
	ON_COMMAND(IDM_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM_COLUMN_DEFAULT, OnColumnDefault)
	ON_COMMAND(IDM_EXIT, OnCancel)
	ON_COMMAND(IDM_FILE_EXIT, OnCancel)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	//}}AFX_MSG_MAP
	ON_COMMAND(IDM_PRINT_SESSION, OnPrint)
	ON_MESSAGE( WM_USER_BUTTON, OnUserButton )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluDeleteDatabaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );

	CString strDelete = m_bDeleteItems ? "Delete" : "Inactive";

	m_listPlu.SubclassDlgItem ( IDC_LIST, this );

	m_ColumnManager.Reset( TableNames::DeletePlu );
	m_ColumnManager.AddColumn ( ColumnNames::PluNo, "Plu Number", LVCFMT_LEFT, 120, 120 );
	m_ColumnManager.AddColumn ( ColumnNames::Desc, "Description", LVCFMT_LEFT, 120, 170 );

#ifdef STOCKMAN_SYSTEM
	m_ColumnManager.AddColumn ( ColumnNames::StockCode,"Stock Code", LVCFMT_LEFT, 115, 115 );
#endif
					
	m_ColumnManager.AddColumn ( ColumnNames::Dept, "Dept", LVCFMT_LEFT, 40, 40 );					
	m_ColumnManager.AddColumn ( ColumnNames::Price1, "Price1", LVCFMT_RIGHT, 55, 70 );
	m_ColumnManager.AddColumn ( ColumnNames::Delete, strDelete, LVCFMT_LEFT, 60, 60 );
	m_ColumnManager.LoadSettings();
	
	m_listPlu.SetItemCountEx ( m_FilterArray.GetSize() );

	CMenuChanger MenuChanger;
	MenuChanger.EatMenuTopLevel( GetMenu() );

	if ( TRUE == m_bDeleteItems )
		MenuChanger.SetItemText( IDM_PRINT_SESSION, "Deletion List" );
	else
		MenuChanger.SetItemText( IDM_PRINT_SESSION, "Inactive Items List" );
	
	MenuChanger.BuildMenu ( GetMenu() );
	DrawMenuBar();

	
	m_bInitDialog = TRUE;
	m_ColumnManager.RedrawList( &m_listPlu, this );
	CenterWindow();
	DisplayScreen ( 0 );
	return TRUE;
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listPlu.MoveWindow ( 0,0,cx,cy );
	}
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::DisplayScreen ( int nIndex )
{
	CString strMsg;
	int nSize = m_FilterArray.GetSize();

	CString strFiltered = ( m_FilterArray.GetFilteredFlag() == TRUE ) ? ", Filtered" : "";

	strMsg.Format ( "%s - (%s, selected %d of %d)",
		(const char*) m_strTitle,
		(const char*) m_strDatabaseName, 
		m_nDeletionCount,
		m_FilterArray.GetSize() );

	SetWindowText ( strMsg );

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listPlu.SetCurSel ( nIndex );
	else
		m_listPlu.SetCurSel ( nSize - 1 );

	m_listPlu.Invalidate();
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ( nIndex != m_nCacheIndex  )
		{
			m_CacheCSV.ParseLine( m_FilterArray.GetDisplayLine ( nIndex ) );
			m_nCacheIndex = nIndex;
		}

		lstrcpy ( pDispInfo->item.pszText, m_CacheCSV.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ToggleFlag();	
	*pResult = 0;
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnDefaultButton() 
{
	ToggleFlag();	
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CPluDeleteDatabaseDlg::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	int nIndex = ( int ) wIndex;
	int nType = m_ColumnManager.GetListColumnType( nIndex );

	if ( COLUMN_PLUTYPE == nType )
		if ( m_FilterArray.GetSize() > 0 )
			OnFindPluNumber();
	
	return 0L;
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnFindPluNumber()
{
	const char* szTitle = "Find Plu Number";
	CPluNumberDlg dlg ( szTitle, FALSE, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		int nIndex;

		__int64 nPluNo = _atoi64( BarcodePluNoTable.ConvertPlu ( dlg.m_strPluNumber ) );
		bool bFound = m_FilterArray.FindPluItemByNumber ( nPluNo, nIndex );

		if ( bFound == FALSE )
		{
			CString strPluNo;
			::FormatPluNo( nPluNo, strPluNo );

			CString strMsg;
			strMsg.Format ( "Cannot find Plu number %s", (const char*) strPluNo );
			Prompter.Error ( strMsg );
		}		
		m_listPlu.SetCurSel ( nIndex );
	}
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnContextMenu( CWnd* pWnd, CPoint point )
{
	HWND hWnd = HWND( m_listPlu );

	HDHITTESTINFO hitTest;
	hitTest.pt.x = point.x;
	hitTest.pt.y = point.y;

	::ScreenToClient( hWnd, &hitTest.pt );

	CHeaderCtrl* headerCtrl = m_listPlu.GetHeaderCtrl();
	int nIndex = headerCtrl -> SendMessage( HDM_HITTEST, 0, (LPARAM) &hitTest );

	int nType = m_ColumnManager.GetListColumnType( nIndex );

	switch( nType )
	{
	case COLUMN_PLUNO:		SortListInternal( PLUFILTER_SORT_FORWARD_PLUNO );		break;
	case COLUMN_DESC:		SortListInternal( PLUFILTER_SORT_FORWARD_TEXT );		break;
	case COLUMN_DEPT:		SortListInternal( PLUFILTER_SORT_FORWARD_DEPT );		break;
	case COLUMN_STOCKCODE:	SortListInternal( PLUFILTER_SORT_FORWARD_STOCKCODE );	break;
	}
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::ToggleFlag()
{
	int nIndex = m_listPlu.GetCurSel();

	if ( nIndex >= 0 && nIndex < m_FilterArray.GetSize() )
	{
		if ( m_FilterArray.GetDeletionFlagAt( nIndex ) == TRUE )
		{
			m_FilterArray.SetDeletionFlagAt( nIndex, FALSE );
			m_nDeletionCount--;
		}
		else
		{
			m_FilterArray.SetDeletionFlagAt( nIndex, TRUE );
			m_nDeletionCount++;
		}

		DisplayScreen( nIndex );
	}
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnPluDeleteRange() 
{
	int nSelection = m_listPlu.GetCurSel();

	CPluDeleteRangeSetDlg dlg ( this );

	if ( nSelection >= 0 )
	{
		int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nSelection );
		CString strPluNo = DataManager.Plu.GetPluNoString ( nPluIdx );
		dlg.m_strPluFrom = strPluNo; 
		dlg.m_strPluTo = strPluNo;
	}
	else
	{
		dlg.m_strPluFrom = "";
		dlg.m_strPluTo = "";
	}
		
	dlg.m_nDepartmentFilter = DEPARTMENTSET_ALL;

	if ( dlg.DoModal() == IDOK )
	{
		__int64 nPluFrom = _atoi64( dlg.m_strPluFrom );
		__int64 nPluTo = _atoi64( dlg.m_strPluTo );

		int nStartIdx, nRange;
		m_FilterArray.GetArrayIdxRange( nPluFrom, nPluTo, nStartIdx, nRange );

		StatusProgress.Lock( TRUE, "Updating item list" );
		
		int nCount = 0;
		for ( int nOffset = 0; nOffset < nRange; nOffset++ )
		{
			StatusProgress.SetPos ( nOffset, nRange ); 
			
			CPluCSVRecord PluRecord;
			int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nStartIdx + nOffset );
			DataManager.Plu.GetAt ( nPluIdx, PluRecord );

			if ( ( PluRecord.GetPluNoInt() < nPluFrom ) || ( PluRecord.GetPluNoInt() > nPluTo ) )
				continue;
		
			CDepartmentSetCSVRecord DepartmentSet;
			DepartmentSet.CreateDepartmentSet ( dlg.m_nDepartmentFilter );
	
			if ( DepartmentSet.GetDepartmentStatus ( PluRecord.GetBaseDeptNo() ) == FALSE )
				continue;

			if ( 0 == dlg.m_nDelete ) //0 = DELETE
			{
				if ( m_FilterArray.GetDeletionFlagAt( nStartIdx + nOffset ) == FALSE )
				{
					m_FilterArray.SetDeletionFlagAt( nStartIdx + nOffset, TRUE );
					m_nDeletionCount++;
				}
			}
			else
			{
				if ( m_FilterArray.GetDeletionFlagAt( nStartIdx + nOffset ) == TRUE )
				{
					m_FilterArray.SetDeletionFlagAt( nStartIdx + nOffset, FALSE );
					m_nDeletionCount--;
				}
			}

			nCount++;
		}
		StatusProgress.Unlock();
		
		DisplayScreen( nSelection );
		
		CString strBuf;

		switch ( nCount )
		{
		case 0:
			Prompter.Info ( "No records were matched" );
			break;

		case 1:
			Prompter.Info ( "1 record was matched" );
			break;

		default:
			strBuf.Format ( "%d records were matched", nCount );
			Prompter.Info ( strBuf );
			break;
		}
	}
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnPluDeleteAll() 
{
	CString strPrompt = "Are you sure you wish to ";
	
	if ( TRUE == m_bDeleteItems )
		strPrompt += "select all these items for deletion.";
	else
		strPrompt += "flag all these items as inactive.";

	if ( Prompter.YesNo( strPrompt ) == IDYES )
		SelectAll ( 1 );
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnPluDeleteNone() 
{
	CString strPrompt = "Are you sure you wish to retain all these items";
	
	if ( TRUE == m_bDeleteItems )
		strPrompt += ".";
	else
		strPrompt += " as active.";
	
	if ( Prompter.YesNo ( strPrompt ) == IDYES )
		SelectAll ( 0 );
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::SelectAll ( int nFlag )
{
	int nSelection = m_listPlu.GetCurSel();
	int nCount = m_FilterArray.GetSize();

	StatusProgress.Lock( TRUE, "Updating item list" );
	
	for ( int nIdx = 0; nIdx < nCount; nIdx++ )
		m_FilterArray.SetDeletionFlagAt( nIdx, ( 1 == nFlag ) );

	m_nDeletionCount = ( 1 == nFlag ) ? nCount : 0;

	StatusProgress.Unlock();

	DisplayScreen( nSelection );	
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listPlu, this );
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listPlu, this );
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listPlu, this );
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnPrint()
{
	bool bLines = TRUE;
	bool bMarked = TRUE;
	bool bPrice = TRUE;
	bool bStockCode = TRUE;
	bool bEmpty = TRUE;
	
	CSSFile fileOptions;
	CFilenameUpdater FnUp( SysFiles::PluDeleteDef );
	if ( fileOptions.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		CString strBuffer;
		fileOptions.ReadString( strBuffer );
		CCSV csv( strBuffer );

		if ( csv.GetInt(0) == 1 )
		{
			bLines = csv.GetBool(1);
			bMarked = csv.GetBool(2);
			bPrice = csv.GetBool(3);
			bStockCode = csv.GetBool(4);
			bEmpty = csv.GetBool(5);
		}
	}

	while( TRUE )
	{
		CPluDeletePrintDlg dlg( m_bDeleteItems, this );

		dlg.m_bLines = bLines;
		dlg.m_bMarked = bMarked;
		dlg.m_bPrice = bPrice;
		dlg.m_bStockCode = bStockCode;
		dlg.m_bEmpty = bEmpty;
	
		if ( dlg.DoModal() != IDOK )
			return;

		bLines = dlg.m_bLines;
		bMarked = dlg.m_bMarked;
		bPrice = dlg.m_bPrice;
		bStockCode = dlg.m_bStockCode;
		bEmpty = dlg.m_bEmpty;

		bool bShowStockCode = ( bStockCode != 0 );

#ifndef STOCKMAN_SYSTEM
		bShowStockCode = FALSE;
#endif

		CReportFile ReportFile;
		if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		{
			Prompter.Error( "Unable to create report" );
			continue;
		}
	
		ReportFile.SetStyle1 ( m_bDeleteItems ? "Plu Deletion List" : "Plu Items To Flag As Inactive" );
		
		CCSV csvColumns;
		AddReportColumn( ReportFile, csvColumns, "Plu", TA_LEFT, 380 );
		AddReportColumn( ReportFile, csvColumns, "Description", TA_LEFT, 300 );
		AddReportColumn( ReportFile, csvColumns, "Dpt.No", TA_LEFT, 200 );

		if ( bShowStockCode != 0 )
			AddReportColumn( ReportFile, csvColumns, "Stock Code", TA_LEFT, 380 );
		
		if ( bPrice != 0 )
			AddReportColumn( ReportFile, csvColumns, "Retail", TA_RIGHT, 200 );

		if ( bEmpty != 0 )
			AddReportColumn( ReportFile, csvColumns, "", TA_LEFT, 450 );

		AddReportColumn( ReportFile, csvColumns, m_bDeleteItems ? "Delete" : "Inactive", TA_LEFT, 200 );
				
		for ( int nIdx = 0; nIdx < m_FilterArray.GetSize(); nIdx++ )
		{
			int nPluIdx = m_FilterArray.GetPluItemIndexAt( nIdx );
		
			if ( bMarked == 0 )
				if ( m_FilterArray.GetDeletionFlagAt( nIdx ) == FALSE )
					continue;

			if ( ( nPluIdx >= 0 ) && ( nPluIdx < DataManager.Plu.GetSize() ) )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt ( nPluIdx, PluRecord );
				
				CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );

				CReportHelpers ReportHelpers;
				csv.Add ( ReportHelpers.GetDisplayPluNo ( PluRecord.GetPluNoInt() ) );
				csv.Add ( ReportHelpers.GetReportText ( PluRecord ) );		
				csv.Add ( PluRecord.GetBaseDeptNo() );

#ifdef STOCKMAN_SYSTEM
				if ( bShowStockCode != 0 )
				{
					CString strStockCode = "";
					DataManager.Plu.GetPluStockCode( PluRecord.GetPluNoInt(), strStockCode, TRUE );
					csv.Add ( ReportHelpers.GetDisplayStockCode ( strStockCode ) );
				}
#endif

				if ( bPrice != 0 )
					csv.Add ( PluRecord.GetPrice(0), SysInfo.GetDPValue() );
				
				if ( bEmpty != 0 )
					csv.Add( "" );

				if ( m_FilterArray.GetDeletionFlagAt( nIdx ) == TRUE )
					csv.Add ( "Yes" );

				ReportFile.WriteLine ( csv.GetLine() );

				if ( bLines != 0 )
					ReportFile.WriteLine( "<LI>" );
			}
		}
		ReportFile.Close();
	
		CReportManager ReportManager;
		CFilenameUpdater FnUp ( SysFiles::PluDeletePrm );
		CString strKey = csvColumns.GetLine();
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, this );
	}
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::AddReportColumn( CReportFile& ReportFile, CCSV& csv, const char* szType, UINT nAlign, int nWidth )
{
	ReportFile.AddColumn( szType, nAlign, nWidth );
	csv.Add( szType );
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::OnCancel()
{
	CString strNoAction = "";
	strNoAction += "No records were ";
	strNoAction += m_bDeleteItems ? "deleted." : "changed.";

	CDWordArray dw;
	dw.Add( m_nDeletionCount );

	if ( m_nDeletionCount == 0 )
	{
		Prompter.Info ( strNoAction );
		MessageLogger.LogSystem( 5, 3, FALSE );
		EndDialog ( IDOK );
		return;
	}

	if ( TRUE == m_bDeleteItems )
	{
		CStockDeleteCloseDlg dlg( NODE_PLU, m_nDeletionCount, this );
		dlg.DoModal();

		if ( STOCKDELETE_CANCEL == dlg.m_nAction )
		{
			Prompter.Info ( strNoAction );
			MessageLogger.LogSystem( 5, 4, FALSE, dw );
			EndDialog ( IDOK );
			return;
		}

		if ( STOCKDELETE_CONFIRM != dlg.m_nAction )
			return;
	}
	else
	{
		CPluInactiveCloseDlg dlg( m_nDeletionCount, this );
		dlg.DoModal();

		if ( PLUINACTIVE_CANCEL == dlg.m_nAction )
		{
			Prompter.Info ( strNoAction );
			MessageLogger.LogSystem( 5, 4, FALSE, dw );
			EndDialog ( IDOK );
			return;
		}

		if ( PLUINACTIVE_CONFIRM != dlg.m_nAction )
			return;
	}

#ifdef STOCKMAN_SYSTEM
	DataManager.StockPlu.Invalidate();
#endif

	m_FilterArray.SetSortType( PLUFILTER_SORT_FORWARD_PLUNO );
	m_FilterArray.QuickSort();

	int nProgress = 0;
	int nCount = m_FilterArray.GetSize();

	StatusProgress.Lock( TRUE, "Updating Plu Database" );			
	for ( int nFilterIdx = nCount - 1; nFilterIdx >= 0; nFilterIdx-- )
	{
		StatusProgress.SetPos ( nProgress++, nCount );

		if ( m_FilterArray.GetDeletionFlagAt( nFilterIdx ) == TRUE )
		{
			int nPluIdx = m_FilterArray.GetPluItemIndexAt( nFilterIdx );

			if ( TRUE == m_bDeleteItems )
			{
				DataManager.ResourceWrapperPlu.FlagDeletedPlu( DataManager.Plu.GetPluNoInt( nPluIdx ) );
				DataManager.ListKeyImageWrapperPlu.FlagDeletedPlu( DataManager.Plu.GetPluNoInt( nPluIdx ) );
				DataManager.Plu.RemoveAt ( nPluIdx );
			}
			else
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt( nPluIdx, PluRecord );
				PluRecord.SetActiveItemFlag(FALSE);
				DataManager.Plu.SetAt( nPluIdx, PluRecord );
			}
		}
	}
	StatusProgress.Unlock();
	
	CDataManagerInfo info;
	if ( DataManager.WritePlu( info ) == FALSE )
	{
		MessageLogger.LogSystem( 5, 6, FALSE, dw );
		Prompter.WriteError( info );
	}
	else
	{
		if ( TRUE == m_bDeleteItems )
		{
			DataManager.ResourceWrapperPlu.SaveDatabaseChanges();
			DataManager.ListKeyImageWrapperPlu.SaveDatabaseChanges();
		}

		CCustomExportManagerPlu CustomExport;

		m_bDoneDelete = TRUE;

		MessageLogger.LogSystem( 5, 5, FALSE, dw );
	}
	EndDialog ( IDOK );
}

/**********************************************************************/

void CPluDeleteDatabaseDlg::SortListInternal( int nType )
{
	__int64 nPluNo = 0;
	
	{
		int nIndex = m_listPlu.GetCurSel();
		if ( ( nIndex >= 0 ) && ( nIndex < m_FilterArray.GetSize() ) )
			nPluNo = DataManager.Plu.GetPluNoInt( m_FilterArray.GetPluItemIndexAt( nIndex ) );
	}

	m_FilterArray.ChangeSortOrder( nType, FALSE );
	m_listPlu.SetItemCountEx ( m_FilterArray.GetSize() );

	if ( nPluNo == 0 )
		DisplayScreen(0);
	else
	{
		int nSel;
		m_FilterArray.FindPluItemByNumber( nPluNo, nSel );
		DisplayScreen( nSel );
	}
}

/**********************************************************************/
