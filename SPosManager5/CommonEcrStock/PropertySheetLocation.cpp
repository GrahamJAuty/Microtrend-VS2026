/**********************************************************************/
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NetworkTerminalMap.h"
#include "PriceSetItemManager.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropertySheetLocation.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetLocation, CPropertySheet)
/**********************************************************************/

CPropertySheetLocation::CPropertySheetLocation(int nLocIdx, bool bAdd, CWnd* pWndParent)
	: CSSPropertySheet("", pWndParent)
{
	m_nLocIdx = nLocIdx;
	m_bAdd = bAdd;

	m_PriceSetHeaderManager.LoadHeaderForLocationEdit(m_nLocIdx);

	CLocationCSVRecord* pLocation = new CLocationCSVRecord;
	m_LocationCSVArray.Add(pLocation);
	dbLocation.LoadLocationBuffer(m_nLocIdx, m_LocationCSVArray);

	int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;

	int nNetworkIdx;
	int nNetworkNo = dbLocation.GetNetworkNo(m_nLocIdx);
	if (dbNetwork.FindNetworkByNumber(nNetworkNo, nNetworkIdx) == TRUE)
	{
		nConnectionType = dbNetwork.GetConnectionType(nNetworkIdx);
	}

	for (int nTermIdx = 0; nTermIdx < m_LocationCSVArray.GetTerminalCount(0); nTermIdx++)
	{
		CSortedIntItem item;
		item.m_nItem = m_LocationCSVArray.GetTNo(0, nTermIdx);
		m_TNoOldArray.Consolidate(item);
		m_TNoNewArray.Consolidate(item);
	}

	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	AddPage(&m_Page1);

	bool bPlu = (EcrmanOptions.GetPluProdCSVImportType() != 0);
	bool bCustomer = FALSE;

	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		bCustomer = (EcrmanOptions.GetFeaturesCustomerNameImportType() != 0);
	}

	if ((TRUE == bPlu) || (TRUE == bCustomer))
	{
		AddPage(&m_Page5);
	}

	AddPage(&m_Page2);
	AddPage(&m_Page8);

	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		if (EcrmanOptions.GetPluPriceSetsFlag() == TRUE)
		{
			AddPage(&m_Page3);
		}
	}

	if (SysInfo.IsIDraughtExportSystem() == TRUE)
	{
		AddPage(&m_Page4);
	}

#ifdef STOCKMAN_SYSTEM

	if (DealerFlags.GetBritanniaHotelsExportFlag() == TRUE)
	{
		AddPage(&m_Page6);
}

#endif

	if ( DealerFlags.GetLocationNotesFlag() == TRUE )
	{
		m_LocationNotes.SetLocationNotesSource(nLocIdx);
		m_LocationNotes.Read();
		AddPage( &m_Page7 );
	}
	
	m_Page1.SetLocationBuffer ( &m_LocationCSVArray );
	m_Page2.SetLocationBuffer ( &m_LocationCSVArray );
	m_Page3.SetLocationBuffer ( &m_LocationCSVArray );
	m_Page3.SetPriceSetHeaderManager ( &m_PriceSetHeaderManager );
	m_Page4.SetLocationBuffer ( &m_LocationCSVArray );
	m_Page5.SetLocationBuffer ( &m_LocationCSVArray );
	m_Page6.SetLocationBuffer ( &m_LocationCSVArray );
	m_Page7.SetLocationBuffer ( &m_LocationCSVArray );
	m_Page7.SetLocationNotesBuffer( &m_LocationNotes );
	m_Page8.SetLocationBuffer ( &m_LocationCSVArray );
	
	m_Page2.SetLocIdx( m_nLocIdx );
	m_Page2.SetTNoOldArray( &m_TNoOldArray );
	m_Page2.SetTNoNewArray( &m_TNoNewArray );

	m_Page5.SetConnectionType( nConnectionType );
	
	CString strTitle;
	strTitle.Format ( "Edit %s", m_LocationCSVArray.GetSystemName( 0 ) );
	SetTitle ( strTitle );
}

/**********************************************************************/

CPropertySheetLocation::~CPropertySheetLocation()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetLocation, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetLocation)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetLocation::SaveRecord( bool bNewLocation )
{
	if ( EcrmanOptions.GetPluPriceSetsFlag() == TRUE )
	{
		bool bAddedPluFields = FALSE;
		bool bRemovedPluFields = FALSE;
		bool bWasEmpty = FALSE;

		bool bNeedUpdate = FALSE;

		CStringArray arrayWarning;
		int nOldPriceSetCount = dbLocation.GetPluPriceSetCount( m_nLocIdx );
		int nNewPriceSetCount = m_LocationCSVArray.GetPluPriceSetCount(0);
		int nDefPriceSet = m_LocationCSVArray.GetDefaultPriceSet(0);
		
		if ( nNewPriceSetCount < 1 || nNewPriceSetCount > MAX_PRICE_SETS )
		{
			nNewPriceSetCount = 1;
			m_LocationCSVArray.SetPluPriceSetCount(0, 1);
		}

		if ( nDefPriceSet > nNewPriceSetCount ) 
		{
			nDefPriceSet = 1;
			m_LocationCSVArray.SetDefaultPriceSet( 0, 1 );
		}

		//MUST UPDATE IF WE ADD OR REMOVE PRICE SETS
		if ( nNewPriceSetCount != nOldPriceSetCount )
			bNeedUpdate = TRUE;
		
		//MUST WARN IF WE REDUCE PRICE SETS UNLESS OLD PRICE SETS WERE EMPTY
		if ( ( nNewPriceSetCount < nOldPriceSetCount ) && ( bWasEmpty == FALSE ))
		{
			CString strLine;
			strLine.Format ( "Delete Price Sets (%s)", m_LocationCSVArray.GetName(0) );
			arrayWarning.Add ( strLine );
			strLine.Format ( "The number of price sets for this location will be reduced to %1.1d.", nNewPriceSetCount );
			arrayWarning.Add ( strLine );
		}
		
		m_PriceSetHeaderManager.CheckHeaderChanges ( bAddedPluFields, bRemovedPluFields, bWasEmpty );
		
		//MUST UPDATE IF WE ADD OR REMOVE FIELDS
		if ( bAddedPluFields == TRUE || bRemovedPluFields == TRUE )
			bNeedUpdate = TRUE;

		//MUST WARN IF WE REMOVE FIELDS UNLESS OLD PRICE SETS WERE EMPTY
		if ( ( bRemovedPluFields == TRUE ) && ( bWasEmpty == FALSE ) )
		{
			CString strLine = "";
			if ( arrayWarning.GetSize() == 0 )
				strLine.Format ( "Select Price Set Fields (%s)", m_LocationCSVArray.GetName(0) );

			arrayWarning.Add( strLine );
			
			arrayWarning.Add( "The fields that you removed will not be editable in location" );
			arrayWarning.Add( "+specific price sets at this location." );
			arrayWarning.Add( "" );
			arrayWarning.Add( "Any location specific values in these fields will be deleted" );
			arrayWarning.Add( "+from these price sets." );
		}
		
		//MUST WARN IF WE ADD FIELDS UNLESS OLD PRICE SETS WERE EMPTY
		if ( ( bAddedPluFields == TRUE ) && ( bWasEmpty == FALSE ) )
		{	
			CString strLine = "";
			if ( arrayWarning.GetSize() == 0 )
				strLine.Format ( "Select Price Set Fields (%s)", m_LocationCSVArray.GetName(0) );
			
			arrayWarning.Add( strLine );
			
			arrayWarning.Add ( "The fields that you added will be editable in all location" );
			arrayWarning.Add ( "+specific price sets at this location." );
			arrayWarning.Add ( "" );
			arrayWarning.Add ( "You must specify the values for these fields in all the Plu" );
			arrayWarning.Add ( "+items that are linked to these price sets." );
		}
		
		//WARNING
		if ( arrayWarning.GetSize() != 0 )
		{
			if ( Prompter.Warning( arrayWarning ) != IDYES )
			{
				bNeedUpdate = FALSE;
				m_LocationCSVArray.SetPluPriceSetCount( 0, dbLocation.GetPluPriceSetCount( m_nLocIdx ) );
				m_LocationCSVArray.SetDefaultPriceSet( 0, dbLocation.GetDefaultPriceSet( m_nLocIdx ) );
			}
		}

		if ( bNeedUpdate == TRUE )
		{
			CString strFolderPathLocation = m_LocationCSVArray.GetFolderPathData( 0 );
			if ( ::ExistSubdirectory ( strFolderPathLocation ) == FALSE )
				::MakeSubdirectory ( strFolderPathLocation );

			for ( int nSet = MAX_PRICE_SETS; nSet > nNewPriceSetCount; nSet-- )	
			{
				CFileRemover FileRemover;
				FileRemover.RemoveFile ( m_LocationCSVArray.GetFilePathDataLocalPluField ( 0, nSet ) );
				FileRemover.RemoveFile ( m_LocationCSVArray.GetFilePathDataLocalPluFieldTemp ( 0, nSet ) );
			}

			m_PriceSetHeaderManager.UpdateLocationSpecificPriceSetFiles ( m_nLocIdx, nNewPriceSetCount );
			m_PriceSetHeaderManager.CreateLocationSpecificPriceSetFiles ( m_nLocIdx, nNewPriceSetCount );

			//DELETED PRICE SETS SHOULD REVERT TO BEING LOCATION SPECIFIC
			if ( nNewPriceSetCount < 1 ) m_LocationCSVArray.SetDbPriceSet1( 0, 0 );
			if ( nNewPriceSetCount < 2 ) m_LocationCSVArray.SetDbPriceSet2( 0, 0 );
			if ( nNewPriceSetCount < 3 ) m_LocationCSVArray.SetDbPriceSet3( 0, 0 );
		}
	}

	//TERMINALS
	int nOldPos = 0;
	int nNewPos = 0;

	int nOldSize = m_TNoOldArray.GetSize();
	int nNewSize = m_TNoNewArray.GetSize();

	while ( ( nOldPos < nOldSize ) && ( nNewPos < nNewSize ) )
	{
		CSortedIntItem itemOld, itemNew;
		m_TNoOldArray.GetAt( nOldPos, itemOld );
		m_TNoNewArray.GetAt( nNewPos, itemNew );

		if ( itemOld.m_nItem == itemNew.m_nItem )
		{
			nOldPos++;
			nNewPos++;
		}
		else if ( itemOld.m_nItem > itemNew.m_nItem )
		{
			AddTerminal( itemNew.m_nItem );
			nNewPos++;
		}
		else
		{
			RemoveTerminal ( itemOld.m_nItem );
			nOldPos++;
		}
	}

	for ( ; nNewPos < nNewSize; nNewPos++ )
	{
		CSortedIntItem item;
		m_TNoNewArray.GetAt( nNewPos, item );
		AddTerminal( item.m_nItem );
	}
	
	for ( ; nOldPos < nOldSize; nOldPos++ )
	{
		CSortedIntItem item;
		m_TNoOldArray.GetAt( nOldPos, item );
		RemoveTerminal( item.m_nItem );
	}

	if ( TRUE == bNewLocation )
		if ( dbDatabase.GetSize() == 1 )
			m_LocationCSVArray.SetDbNo( 0, dbDatabase.GetDbNo(0) );

	if ( DealerFlags.GetLocationNotesFlag() == TRUE )
		m_LocationNotes.WriteIfChanged();

	dbLocation.SaveLocationBuffer( m_nLocIdx, m_LocationCSVArray );
}

/**********************************************************************/

void CPropertySheetLocation::AddTerminal( int nTNo )
{
	int nTermIdx;
	m_LocationCSVArray.AddTerminal( 0, nTNo, 0, 0, 0, nTermIdx );
	NetworkTerminalMap.AddTerminal( m_LocationCSVArray.GetNetworkNo(0), nTNo, 0 );
}

/**********************************************************************/

void CPropertySheetLocation::RemoveTerminal( int nTNo )
{	
	m_LocationCSVArray.RemoveTerminal( 0, nTNo );
	NetworkTerminalMap.RemoveTerminal( m_LocationCSVArray.GetNetworkNo(0), nTNo );
}

/**********************************************************************/
