/**********************************************************************/
#include "CommsBatchDlg.h"
#include "NewRecordNumberDlg.h"
#include "DownloadDateDlg.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "PropertySheetOfferGroup.h"
#include "ReportDataTableOfferGroupDlg.h"
#include "SelectMultipleLocationDlg.h"
/**********************************************************************/
#include "ListDataOfferGroupDlg.h"
/**********************************************************************/

CSingleListContentHelpersOfferGroup::CSingleListContentHelpersOfferGroup()
{
}

/**********************************************************************/

int CSingleListContentHelpersOfferGroup::GetArraySize()
{
	return DataManager.OfferGroup.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersOfferGroup::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		COfferGroupsCSVRecord Offer( NODE_OFFER_GROUP );
		DataManager.OfferGroup.GetAt ( nIndex, Offer );

		CCSV csv( '\t' );
		csv.Add( Offer.GetGroupNo() );
		csv.Add( Offer.GetDisplayName() );
			
		strLine = csv.GetLine();
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersOfferGroup::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManager.OfferGroup.GetSize() ) )
	{
		COfferGroupsCSVRecord Offer( NODE_OFFER_GROUP );
		DataManager.OfferGroup.GetAt( nIndex, Offer );
		DataManager.OfferGroupEntry.PurgeGroup( Offer.GetGroupNo() );
		DataManager.OfferGroup.RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersOfferGroup::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 430 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataOfferGroupDlg::CListDataOfferGroupDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersOfferGroup();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "offer group" );
	m_pDisplayHelpers -> SetTopPluralText( "offer groups" );
	m_pDisplayHelpers -> SetBottomSingleText( "group" );
	m_pDisplayHelpers -> SetBottomPluralText( "groups" );

	m_bSearchPrevious = FALSE;
	m_strSearchText = "";
	m_strSearchPrompt = "Find Offer Group by Name";
	m_nSearchColumn = 1;
}

/**********************************************************************/

CListDataOfferGroupDlg::~CListDataOfferGroupDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataOfferGroupDlg::SetTitle()
{
	CString strTitle = "Offer Group Database ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), DealerFlags.GetGlobalPromotionFlag() ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataOfferGroupDlg::SetButtonTexts()
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

void CListDataOfferGroupDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataOfferGroupDlg::HandleButton1()
{
	int nOfferNo = DataManager.OfferGroup.FindFirstFreeNumber();

	if ( nOfferNo < 1 )
	{
		Prompter.Error ( "All available offer groups are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_OFFER_GROUP, this );
	dlg.m_nNumber = nOfferNo;
	if ( dlg.DoModal() != IDOK )
		return;

	int nOfferIdx;
	nOfferNo = dlg.m_nNumber;
	if ( DataManager.OfferGroup.FindTableByNumber ( nOfferNo, nOfferIdx ) )
	{
		Prompter.Error ( "The selected offer group is already in use." );
		return;
	}

	COfferGroupsCSVRecord Offer( NODE_OFFER_GROUP );
	Offer.SetGroupNo( nOfferNo );
	Offer.SetGroupName ( "" );
	DataManager.OfferGroup.InsertAt ( nOfferIdx, Offer );

	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nOfferIdx );

	CPropertySheetOfferGroup propSheet( IDC_SPIN, nOfferIdx, TRUE, FALSE, this );
	propSheet.SetListDataDlg( this );
	
	if ( propSheet.DoModal() == IDOK )
	{
		propSheet.SaveRecord();
	}

	SelectLine( propSheet.GetIndex() );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataOfferGroupDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;
		
	CPropertySheetOfferGroup propSheet( IDC_SPIN, nIndex, TRUE, FALSE, this );
	propSheet.SetListDataDlg( this );
	
	if ( propSheet.DoModal() == IDOK )
	{
		propSheet.SaveRecord();
	}

	SelectLine( propSheet.GetIndex() );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataOfferGroupDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataOfferGroupDlg::HandleButton4() 
{
	if ( DataManager.OfferGroup.GetSize() == 0 )
	{
		Prompter.Error ( "You must create some offer groups\nbefore you can print this report" );
		return;
	}
	else
	{
		CReportDataTableOfferGroupDlg dlg( this );
		dlg.DoModal();
	}
}
		
/**********************************************************************/

void CListDataOfferGroupDlg::HandleButton5() 
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

void CListDataOfferGroupDlg::HandleDownload( CArray<int,int>& arrayLocIdx )
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

bool CListDataOfferGroupDlg::SearchText(int nIndex, CString strSearchText)
{
	bool bResult = FALSE;

	COfferGroupsCSVRecord OG(NODE_OFFER_GROUP);
	DataManager.OfferGroup.GetAt(nIndex, OG);

	CString strText = OG.GetDisplayName();
	strText.MakeUpper();

	if (strText.Find(strSearchText) != -1)
	{
		SelectLine(nIndex);
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/
