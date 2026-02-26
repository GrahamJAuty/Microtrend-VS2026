/**********************************************************************/
#include "CommsBatchDlg.h"
#include "DefEditDlg.h"
#include "DownloadDateDlg.h"
#include "NetworkCSVArray.h"
#include "NewRecordNumberDlg.h"
#include "Password.h"
#include "PropertySheetMixMatch.h"
#include "ReportDataTableMixMatchDlg.h"
#include "SelectMultipleLocationDlg.h"
/**********************************************************************/
#include "ListDataMixMatchDlg.h"
/**********************************************************************/

CSingleListContentHelpersMixMatch::CSingleListContentHelpersMixMatch() 
{
}

/**********************************************************************/

int CSingleListContentHelpersMixMatch::GetArraySize()
{
	return DataManager.MixMatch.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersMixMatch::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CMixMatchCSVRecord MixMatch;
		DataManager.MixMatch.GetAt ( nIndex, MixMatch );

		CCSV csv( '\t' );
		csv.Add( MixMatch.GetMixMatchNo() );
		csv.Add( MixMatch.GetDisplayName() );
		strLine = csv.GetLine();

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersMixMatch::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManager.MixMatch.GetSize() ) )
	{
		DataManager.MixMatch.RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersMixMatch::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 430 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataMixMatchDlg::CListDataMixMatchDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersMixMatch();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "mix match table" );
	m_pDisplayHelpers -> SetTopPluralText( "mix match tables" );
	m_pDisplayHelpers -> SetBottomSingleText( "table" );
	m_pDisplayHelpers -> SetBottomPluralText( "tables" );

	m_bSearchPrevious = FALSE;
	m_strSearchText = "";
	m_strSearchPrompt = "Find Mix Match Offer by Name";
	m_nSearchColumn = 1;
}

/**********************************************************************/

CListDataMixMatchDlg::~CListDataMixMatchDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataMixMatchDlg::SetTitle()
{
	CString strTitle = "Mix Match Database ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), DealerFlags.GetGlobalPromotionFlag() ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataMixMatchDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "&Print" );
	m_button5.SetWindowText ( "Do&wnload" );
	m_button6.ShowWindow ( SW_HIDE );

	if ( PasswordArray.GetEnable( PasswordTicks::CommsDownloads ) == FALSE )
	{
		m_button5.EnableWindow( FALSE );
		m_button5.ShowWindow( SW_HIDE );
	}

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataMixMatchDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

bool CListDataMixMatchDlg::SearchText(int nIndex, CString strSearchText)
{
	bool bResult = FALSE;

	CMixMatchCSVRecord MMRecord;
	DataManager.MixMatch.GetAt(nIndex, MMRecord);

	CString strText = MMRecord.GetDisplayName();
	strText.MakeUpper();

	if (strText.Find(strSearchText) != -1)
	{
		SelectLine(nIndex);
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CListDataMixMatchDlg::HandleButton1()
{
	int nOfferNo = DataManager.MixMatch.FindFirstFreeNumber();

	if ( nOfferNo < 1 )
	{
		Prompter.Error ( "All available MixMatch tables are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_MIXMATCH, this );
	dlg.m_nNumber = nOfferNo;
	if ( dlg.DoModal() != IDOK )
		return;

	int nOfferIdx;
	nOfferNo = dlg.m_nNumber;
	if ( DataManager.MixMatch.FindTableByNumber ( nOfferNo, nOfferIdx ) )
	{
		Prompter.Error ( "The selected table number is already in use." );
		return;
	}

	CMixMatchCSVRecord Offer;
	Offer.SetMixMatchNo( nOfferNo );
	Offer.SetDescription ( "" );
	DataManager.MixMatch.InsertAt ( nOfferIdx, Offer );

	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nOfferIdx );

	CPropertySheetMixMatch propSheet( IDC_SPIN, TRUE, nOfferIdx, this );
	propSheet.SetListDataDlg( this );
	
	if ( propSheet.DoModal() == IDOK )
	{
		propSheet.SaveRecord();
	}
	
	SelectLine ( propSheet.GetIndex() );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataMixMatchDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if (nIndex == -1)
	{
		return;
	}

	CPropertySheetMixMatch propSheet( IDC_SPIN, TRUE, nIndex, this );
	propSheet.SetListDataDlg( this );
	
	if ( propSheet.DoModal() == IDOK )
	{
		propSheet.SaveRecord();
	}
	
	SelectLine ( propSheet.GetIndex() );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataMixMatchDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataMixMatchDlg::HandleButton4() 
{
	if ( DataManager.MixMatch.GetSize() == 0 )
	{
		Prompter.Error ( "You must create some mix match tables\nbefore you can print this report" );
		return;
	}
	else
	{
		CReportDataTableMixMatchDlg dlg( this );
		dlg.DoModal();
	}
}
		
/**********************************************************************/

void CListDataMixMatchDlg::HandleButton5() 
{
	int	nDbIdx = DataManager.GetActiveDbIdx();

	bool bBlocked = FALSE;
	CArray<int,int> arrayLocIdx;
	dbNetwork.GetDatabaseCommsLocArray( nDbIdx, arrayLocIdx, bBlocked );

	if ( arrayLocIdx.GetSize() == 0 )
	{
		if ( FALSE == bBlocked )
			Prompter.Error ( "There are no locations available for\ndownload on this database." );
		else
			Prompter.Error ( "You are not authorised to download files\nfrom this database." );

		return;
	}

	HandleDownload( arrayLocIdx );
}

/**********************************************************************/

void CListDataMixMatchDlg::HandleDownload( CArray<int,int>& arrayLocIdx )
{
	if ( CSelectMultipleLocationDlg::FilterLocListForDownload( arrayLocIdx, "Download Mix Match", this ) == TRUE )
	{
		CEposTaskDatabase TaskDatabase;

		if ( EcrmanOptions.GetFeaturesTimePlanDownloadFlag() == FALSE )
			TaskDatabase.CreateSimpleTask( DN_MIX, arrayLocIdx );
		else
		{
			CWordArray arrayJobs;
			arrayJobs.Add( DN_MIX );
			arrayJobs.Add( DN_TPLAN );
			TaskDatabase.CreateSimpleTask( arrayJobs, arrayLocIdx );
		}

		CString strDate = "";
		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );
		
			CString strTitle;
			TaskHeader.SetName( "Download Mix Match" );
			strTitle.Format ( "Epos Import Date (%s)", TaskHeader.GetName() );

			CDownloadDateDlg dlgDate( strTitle, this );
			if ( dlgDate.RunIfNeeded() == FALSE )
				return;

			if ( dlgDate.m_bImmediate == TRUE )
				TaskHeader.SetImportType( IMPORT_IMMEDIATE );
			else
			{
				TaskHeader.SetImportType( IMPORT_SPECIFIED );
				strDate = dlgDate.m_strDate;
			}

			TaskDatabase.SetEditableTaskHeader( TaskHeader );
		}

		CKeyboardMap KeyboardMap;
		CCommsBatchDlg dlgComms ( TaskDatabase, strDate, TRUE, KeyboardMap, -1, 0, 0, this );
		dlgComms.DoModal();
		m_bWarnCancelAfterDownload = TRUE;
	}
}

/**********************************************************************/
