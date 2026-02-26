/**********************************************************************/
#include "EntityIDCreator.h"
#include "FileRemover.h"
#include "ListTimeSliceDlg.h"
#include "TimeSliceCSVArray.h"
#include "TimeSliceMapAddDlg.h"
#include "TimeSliceMapCSVArray.h"
/**********************************************************************/
#include "ListDataTimeSliceMapDlg.h"
/**********************************************************************/

CSingleListContentHelpersTimeSliceMap::CSingleListContentHelpersTimeSliceMap( int& nChangedMaps ) : m_nChangedMaps( nChangedMaps )
{
}

/**********************************************************************/

int CSingleListContentHelpersTimeSliceMap::GetArraySize()
{
	return DataManagerNonDb.TimeSliceList.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersTimeSliceMap::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CTimeSliceMapCSVRecord ReportRecord;
		DataManagerNonDb.TimeSliceList.GetAt( nIndex, ReportRecord );

		CCSV csvOut ( '\t' );

		if ( ReportRecord.GetReportType() == SH_CUSTOM )
			csvOut.Add( "Custom" );
		else
			csvOut.Add( "Standard" );
				
		csvOut.Add( ReportRecord.GetListName() );
		strLine = csvOut.GetLine();

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersTimeSliceMap::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManagerNonDb.TimeSliceList.GetSize() ) )
	{
		int nFileNo = DataManagerNonDb.TimeSliceList.GetTimeSliceFileNo( nIndex );

		CFilenameUpdater FnUp( SysFiles::TimeSliceDetail, -1, nFileNo );
		CFileRemover FileRemover( FnUp.GetFilenameToUse() );

		MarkChange( nFileNo );

		DataManagerNonDb.TimeSliceList.RemoveAt ( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersTimeSliceMap::MarkChange( int nFileNo )
{
	switch( nFileNo )
	{
	case 1:		m_nChangedMaps |= 1;	break;
	case 2:		m_nChangedMaps |= 2;	break;
	case 3:		m_nChangedMaps |= 4;	break;
	case 4:		m_nChangedMaps |= 8;	break;
	case 5:		m_nChangedMaps |= 16;	break;
	case 6:		m_nChangedMaps |= 32;	break;
	case 7:		m_nChangedMaps |= 64;	break;
	case 8:		m_nChangedMaps |= 128;	break;
	case 9:		m_nChangedMaps |= 256;	break;
	case 10:	m_nChangedMaps |= 512;	break;
	}
}

/**********************************************************************/

void CSingleListContentHelpersTimeSliceMap::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Type", LVCFMT_LEFT, 100 );
	list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 380 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataTimeSliceMapDlg::CListDataTimeSliceMapDlg( int nInitialSel, int& nChangedMaps, CWnd* pParent) : CListDataDlg ( pParent ), m_nChangedMaps( nChangedMaps )
{
	m_nInitialSel = nInitialSel;
	
	m_pContentHelpers = new CSingleListContentHelpersTimeSliceMap( nChangedMaps );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "time slice map" );
	m_pDisplayHelpers -> SetTopPluralText( "time slice maps" );
	m_pDisplayHelpers -> SetBottomSingleText( "map" );
	m_pDisplayHelpers -> SetBottomPluralText( "maps" );
}

/**********************************************************************/

CListDataTimeSliceMapDlg::~CListDataTimeSliceMapDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataTimeSliceMapDlg::SetTitle()
{
	SetWindowText ( "Setup Time Slice Maps" );
}

/**********************************************************************/

void CListDataTimeSliceMapDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "Move &Up" );
	m_button5.SetWindowText ( "Move D&own" );
	m_button6.SetWindowText ( "&Copy" );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 42 );
	MoveControl( &m_button3, 368, 77 );
	MoveControl( &m_button4, 368, 122 );
	MoveControl( &m_button5, 368, 157 );
	MoveControl( &m_button6, 368, 202 );
}

/**********************************************************************/

void CListDataTimeSliceMapDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataTimeSliceMapDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if ( m_nInitialSel >= 100 )
		SelectLine( m_nInitialSel - 100 );
	else if ( m_nInitialSel != 0 )
	{
		for ( int nIndex = 0; nIndex < DataManagerNonDb.TimeSliceList.GetSize(); nIndex++ )
		{
			if ( DataManagerNonDb.TimeSliceList.GetReportType( nIndex ) == m_nInitialSel )
			{
				SelectLine( nIndex );
				break;
			}
		}
	}
}

/**********************************************************************/

void CListDataTimeSliceMapDlg::MarkChange( int n )
{
	CSingleListContentHelpersTimeSliceMap* pHelpers = (CSingleListContentHelpersTimeSliceMap*) m_pContentHelpers;
	pHelpers -> MarkChange( n );
}

/**********************************************************************/

void CListDataTimeSliceMapDlg::AddOrFindReport( int nInsertPos, int nReportType )
{
	for ( int nPos = 0; nPos < DataManagerNonDb.TimeSliceList.GetSize(); nPos++ )
	{
		int nArrayReportType = DataManagerNonDb.TimeSliceList.GetReportType( nPos );
		if ( nArrayReportType == nReportType )
		{
			SelectLine( nPos );
			return;
		}
	}

	CTimeSliceMapCSVRecord NewRecord;
	NewRecord.SetName( "" );
	NewRecord.SetReportType( nReportType );
	DataManagerNonDb.TimeSliceList.InsertAt ( nInsertPos, NewRecord );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nInsertPos );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataTimeSliceMapDlg::HandleButton1() 
{
	int nInsertPos = GetSafeSelection() + 1;
	if ( nInsertPos == 0 ) nInsertPos = DataManagerNonDb.TimeSliceList.GetSize();

	CTimeSliceMapAddDlg dlg ( this );
	
	if ( dlg.DoModal() == IDOK )
	{
		switch ( dlg.m_nReportType )
		{
		case SH_TIMESLICE_SECOND:
		case SH_TIMESLICE_MINUTE:
		case SH_TIMESLICE_5MIN:
		case SH_TIMESLICE_10MIN:
		case SH_TIMESLICE_15MIN:
		case SH_TIMESLICE_30MIN:
		case SH_TIMESLICE_60MIN:
			AddOrFindReport( nInsertPos, dlg.m_nReportType );
			break;

		case 999:
			{
				for ( int nPos = 0; nPos < DataManagerNonDb.TimeSliceList.GetSize(); nPos++ )
				{
					if ( DataManagerNonDb.TimeSliceList.GetReportType( nPos ) == 0 )
						continue;

					DataManagerNonDb.TimeSliceList.RemoveAt ( nPos );
					m_listData.SetItemCountEx ( GetArraySize() );
					nPos--;
				}

				AddOrFindReport( 0, SH_TIMESLICE_SECOND );
				AddOrFindReport( 1, SH_TIMESLICE_MINUTE );
				AddOrFindReport( 2, SH_TIMESLICE_5MIN );
				AddOrFindReport( 3, SH_TIMESLICE_10MIN );
				AddOrFindReport( 4, SH_TIMESLICE_15MIN );
				AddOrFindReport( 5, SH_TIMESLICE_30MIN );
				AddOrFindReport( 6, SH_TIMESLICE_60MIN );

				m_WarnCancelAfterChange.SetFlag();
			}
			break;

		case SH_CUSTOM:
		default:
			{
				int nFileNo = DataManagerNonDb.TimeSliceList.GetNextTimeSliceFileNo();

				if ( nFileNo == 0 )
					Prompter.Error ( "You have already created the maximum\nnumber of custom time slice maps." );
				else
				{
					MarkChange( nFileNo );

					CTimeSliceMapCSVRecord NewRecord;
					NewRecord.SetName( "Custom Time Slice Map" );
					NewRecord.SetReportType( SH_CUSTOM );
					NewRecord.SetTimeSliceFileNo( nFileNo );

					CString strID = "";
					CEntityIDCreator<CTimeSliceMapCSVArray> EntityIDCreator;
					if ( EntityIDCreator.CreateEntityID( DataManagerNonDb.TimeSliceList, strID ) == TRUE )
					{
						NewRecord.SetEntityID( strID );
					
						DataManagerNonDb.TimeSliceList.InsertAt( nInsertPos, NewRecord );
						m_listData.SetItemCountEx ( GetArraySize() );
						SelectLine ( nInsertPos );

						CFilenameUpdater FnUp( SysFiles::TimeSliceDetail, -1, nFileNo );
						CFileRemover FileRemover( FnUp.GetFilenameToUse() );

						HandleButton2();

						m_WarnCancelAfterChange.SetFlag();
					}
				}
			}
			break;
		}
	}	
}

/**********************************************************************/

void CListDataTimeSliceMapDlg::HandleButton2() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 )
		return;
	
	CTimeSliceMapCSVRecord ReportRecord;
	DataManagerNonDb.TimeSliceList.GetAt ( nSelection, ReportRecord );
	
	if ( ReportRecord.GetReportType() != 0 )
	{
		Prompter.Error ( "You cannot edit the standard time slot maps." );
		return;
	}
	
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenTimeSliceDetail( ReportRecord.GetTimeSliceFileNo(), DB_READWRITE, info ) == FALSE )
		Prompter.ShareError( info );
	else
	{
		CListTimeSliceDlg dlg( ReportRecord.GetListName(), this );

		if ( dlg.DoModal() == IDOK )
		{
			MarkChange( ReportRecord.GetTimeSliceFileNo() );

			ReportRecord.SetName( dlg.m_strName );
			DataManagerNonDb.TimeSliceList.SetAt ( nSelection, ReportRecord );
	
			if ( DataManagerNonDb.WriteTimeSliceDetail( info ) == FALSE )
				Prompter.WriteError( info );

			m_WarnCancelAfterChange.SetFlag();
		}
		DataManagerNonDb.CloseTimeSliceDetail( info );
	}
	
	SelectLine ( nSelection );
}
		
/**********************************************************************/
	
void CListDataTimeSliceMapDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataTimeSliceMapDlg::HandleButton4()  
{
	int nSelection = GetSafeSelection ( 1, GetArraySize() - 1 );
	if ( nSelection == -1 )
		return;
	
	CTimeSliceMapCSVRecord TempRecord1; 
	CTimeSliceMapCSVRecord TempRecord2; 
	DataManagerNonDb.TimeSliceList.GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.TimeSliceList.GetAt ( nSelection - 1, TempRecord2 );
	DataManagerNonDb.TimeSliceList.SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.TimeSliceList.SetAt ( nSelection - 1, TempRecord1 );
	SelectLine ( nSelection - 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataTimeSliceMapDlg::HandleButton5() 
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 2 );
	if ( nSelection == -1 ) 
		return;

	CTimeSliceMapCSVRecord TempRecord1; 
	CTimeSliceMapCSVRecord TempRecord2; 
	DataManagerNonDb.TimeSliceList.GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.TimeSliceList.GetAt ( nSelection + 1, TempRecord2 );
	DataManagerNonDb.TimeSliceList.SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.TimeSliceList.SetAt ( nSelection + 1, TempRecord1 );
	SelectLine ( nSelection + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataTimeSliceMapDlg::HandleButton6()
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 1 );
	if ( nSelection == -1 ) 
		return;

	CTimeSliceMapCSVRecord TempRecord1, TempRecord2; 
	DataManagerNonDb.TimeSliceList.GetAt ( nSelection, TempRecord1 );

	switch( TempRecord1.CreateCopy( TempRecord2 ) )
	{
	case 0:
		{
			CString strID = "";
			CEntityIDCreator<CTimeSliceMapCSVArray> EntityIDCreator;
			if ( EntityIDCreator.CreateEntityID( DataManagerNonDb.TimeSliceList, strID ) == TRUE )		
			{
				TempRecord2.SetEntityID( strID );
				DataManagerNonDb.TimeSliceList.Add( TempRecord2 );
				MarkChange( TempRecord2.GetTimeSliceFileNo() );
				m_listData.SetItemCountEx ( GetArraySize() );
				SelectLine( GetArraySize() - 1 );

				m_WarnCancelAfterChange.SetFlag();
			}
		}
		break;

	case 1:
		Prompter.Error( "The standard time slice maps cannot be copied." );
		break;

	case 2:
		Prompter.Error ( "You have already created the maximum\nnumber of custom time slice maps." );
		break;

	case 3:
		Prompter.Error ( "Unable to create a copy of the selected time slice map file." );
		break;
	}
}

/**********************************************************************/


