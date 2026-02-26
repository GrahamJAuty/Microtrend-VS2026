/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "DatabaseCSVArray.h"
#include "NewRecordNumberDlg.h"
#include "PropertySheetPluFilter.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "ListDataPluFilterDlg.h"
/**********************************************************************/

CSingleListContentHelpersPluFilter::CSingleListContentHelpersPluFilter( COfferGroupsCSVArray* pFilterNames, COfferGroupEntriesCSVArray* pFilterItems )
{
	m_pFilterNames = pFilterNames;
	m_pFilterItems = pFilterItems;
}

/**********************************************************************/

int CSingleListContentHelpersPluFilter::GetArraySize()
{
	return m_pFilterNames -> GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersPluFilter::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		COfferGroupsCSVRecord PluFilter( NODE_PLUFILTER_NAME );
		m_pFilterNames -> GetAt ( nIndex, PluFilter );

		CCSV csv( '\t' );
		csv.Add( PluFilter.GetGroupNo() );
		csv.Add( PluFilter.GetDisplayName() );
			
		strLine = csv.GetLine();
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersPluFilter::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_pFilterNames -> GetSize() ) )
	{
		COfferGroupsCSVRecord PluFilter( NODE_PLUFILTER_NAME );
		m_pFilterNames -> GetAt( nIndex, PluFilter );
		m_pFilterItems -> PurgeGroup( PluFilter.GetGroupNo() );
		m_pFilterNames -> RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersPluFilter::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 430 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataPluFilterDlg::CListDataPluFilterDlg( bool bSystem, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_bSystem = bSystem;
	
	if ( TRUE == m_bSystem )
	{
		m_pFilterNames = &DataManagerNonDb.SystemPluFilterNames;
		m_pFilterItems = &DataManagerNonDb.SystemPluFilterItems;
	}
	else
	{
		m_pFilterNames = &DataManager.DbPluFilterNames;
		m_pFilterItems = &DataManager.DbPluFilterItems;
	}

	m_pContentHelpers = new CSingleListContentHelpersPluFilter( m_pFilterNames, m_pFilterItems );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "Plu Filter" );
	m_pDisplayHelpers -> SetTopPluralText( "Plu Filters" );
	m_pDisplayHelpers -> SetBottomSingleText( "filter" );
	m_pDisplayHelpers -> SetBottomPluralText( "filters" );
}

/**********************************************************************/

CListDataPluFilterDlg::~CListDataPluFilterDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataPluFilterDlg::SetTitle()
{
	CString strTitle = "Setup Plu Report Filters ";
	
	if ( FALSE == m_bSystem )
		strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), FALSE );
	else
		strTitle += "(System)";

	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataPluFilterDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "&Print" );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
}

/**********************************************************************/

void CListDataPluFilterDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataPluFilterDlg::HandleButton1()
{
	int nFilterNo = m_pFilterNames -> FindFirstFreeNumber();

	if ( nFilterNo < 1 )
	{
		Prompter.Error ( "All available Plu filters are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_PLUFILTER_NAME, this );
	dlg.m_nNumber = nFilterNo;
	if ( dlg.DoModal() != IDOK )
		return;

	int nFilterIdx;
	nFilterNo = dlg.m_nNumber;
	if ( m_pFilterNames -> FindTableByNumber ( nFilterNo, nFilterIdx ) )
	{
		Prompter.Error ( "The selected Plu filter is already in use." );
		return;
	}

	COfferGroupsCSVRecord PluFilter( NODE_PLUFILTER_NAME );
	PluFilter.SetGroupNo( nFilterNo );
	PluFilter.SetGroupName ( "" );
	m_pFilterNames -> InsertAt ( nFilterIdx, PluFilter );

	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nFilterIdx );

	CPropertySheetPluFilter propSheet( IDC_SPIN, nFilterIdx, m_bSystem, TRUE, m_WarnCancelAfterChange, this );
	propSheet.SetListDataDlg( this );

	if ( propSheet.DoModal() == IDOK )
	{
		propSheet.SaveRecord();
	}

	SelectLine( propSheet.GetIndex() );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataPluFilterDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;
		
	CPropertySheetPluFilter propSheet( IDC_SPIN, nIndex, m_bSystem, TRUE, m_WarnCancelAfterChange, this );
	propSheet.SetListDataDlg( this );
	if ( propSheet.DoModal() == IDOK )
		propSheet.SaveRecord();

	SelectLine( propSheet.GetIndex() );
}

/**********************************************************************/

void CListDataPluFilterDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataPluFilterDlg::HandleButton4() 
{
	if ( m_pFilterNames -> GetSize() == 0 )
	{
		Prompter.Error ( "You must create some Plu filters\nbefore you can print this report" );
		return;
	}

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;

	ReportFile.SetStyle1 ( "Plu Report Filters" );
		
	ReportFile.AddColumn ( "No", TA_LEFT, 120 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Entry", TA_LEFT, 800 );
	
	for ( int nFilterIdx = 0; nFilterIdx < m_pFilterNames -> GetSize(); nFilterIdx++ )
	{
		COfferGroupsCSVRecord PluFilter( NODE_PLUFILTER_NAME );
		m_pFilterNames -> GetAt ( nFilterIdx, PluFilter );
		int nFilterNo = PluFilter.GetGroupNo();
			
		CCSV csvOut ( '\t' );
		csvOut.Add( nFilterNo );
		csvOut.Add( PluFilter.GetDisplayName() );
		ReportFile.WriteLine ( csvOut.GetLine() );

		int nIdx1, nIdx2;
		m_pFilterItems -> GetGroupSubRange( nFilterNo, OFFERENTRY_PLU, nIdx1, nIdx2 );

		for ( int nIdx = nIdx1; nIdx <= nIdx2; nIdx++ )
		{
			COfferGroupEntriesCSVRecord entry;
			m_pFilterItems -> GetAt( nIdx, entry );
			
			int nPluIdx;
			CString strPluNo = BarcodePluNoTable.ConvertPluInt( entry.GetEntryPluNo() );
			
			if ( FALSE == m_bSystem )
			{
				if ( DataManager.Plu.FindPluByNumber( entry.GetEntryPluNo(), nPluIdx ) == TRUE )
				{
					CPluCSVRecord PluRecord;
					DataManager.Plu.GetAt( nPluIdx, PluRecord );

					strPluNo += " ";
					strPluNo += PluRecord.GetReportText();
				}
				else
					strPluNo += " Unknown";
			}

			int nFlags = entry.GetModifierFlags();
			
			if ( ( nFlags & 1 ) == 0 )
				strPluNo += " ****";

			csvOut.RemoveAll();
			csvOut.Add ( "" );
			csvOut.Add ( "" );
			csvOut.Add( strPluNo );
			ReportFile.WriteLine ( csvOut.GetLine() );

			int nMask = 2;
			for ( int n = 1; n <= 9; n++ )
			{
				if ( ( nFlags & nMask ) != 0 )
				{
					CString strModifier;
					
					if ( FALSE == m_bSystem )
					{
						strModifier.Format( "    M%2.2d %s",
							n,
							DataManager.Modifier.GetDisplayName(n) );
					}
					else
					{
						strModifier.Format( "    M%2.2d Modifier %d",
							n, n );
					}

					csvOut.RemoveAll();
					csvOut.Add( "" );
					csvOut.Add( "" );
					csvOut.Add( strModifier );
					ReportFile.WriteLine ( csvOut.GetLine() );
				}

				nMask *= 2;
			}
		}

		ReportFile.WriteLine ( "<LI>" );
	}
	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::PluFilterPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}
		
/**********************************************************************/