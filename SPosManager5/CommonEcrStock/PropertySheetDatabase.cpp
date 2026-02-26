/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
#include "PriceSetItemManager.h"
 
#include "PropertySheetDatabase.h"
#include "SysInfo.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetDatabase, CPropertySheet)
/**********************************************************************/

CPropertySheetDatabase::CPropertySheetDatabase( int nDbIdx, bool bAdd, CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	m_nDbIdx = nDbIdx;
	m_bAdd = bAdd;

	CDatabaseCSVRecord* pDatabase = new CDatabaseCSVRecord;
	m_DatabaseCSVArray.Add( pDatabase );
	dbDatabase.LoadDatabaseBuffer( m_nDbIdx, m_DatabaseCSVArray );

	CAddressInfo infoAddress;
	dbDatabase.GetAddressInfo( m_nDbIdx, infoAddress );
	m_DatabaseCSVArray.SetLegacyAddress1( 0, infoAddress.m_strAddress1 );
	m_DatabaseCSVArray.SetLegacyAddress2( 0, infoAddress.m_strAddress2 );
	m_DatabaseCSVArray.SetLegacyAddress3( 0, infoAddress.m_strAddress3 );
	m_DatabaseCSVArray.SetLegacyPostcode( 0, infoAddress.m_strPostcode );
	m_DatabaseCSVArray.SetLegacyPhone( 0, infoAddress.m_strPhone );
	m_DatabaseCSVArray.SetLegacyFax( 0, infoAddress.m_strFax );
	m_DatabaseCSVArray.SetLegacyEmail( 0, infoAddress.m_strEmail );

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
	AddPage( &m_Page1 );

	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		if ( EcrmanOptions.GetPluPriceSetsFlag() == TRUE )
			AddPage( &m_Page2 );
	}
	
	m_Page1.SetDatabaseBuffer ( &m_DatabaseCSVArray );
	m_Page2.SetDatabaseBuffer ( &m_DatabaseCSVArray );
	m_Page2.SetPriceSetHeaderManager ( &m_PriceSetHeaderManager );
	m_Page2.SetDbIdx( m_nDbIdx );
	
	CString strTitle;
	strTitle.Format ( "Edit %s", m_DatabaseCSVArray.GetSystemName( 0 ) );
	SetTitle ( strTitle );
}

/**********************************************************************/

CPropertySheetDatabase::~CPropertySheetDatabase()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetDatabase, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetDatabase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetDatabase::SaveRecord()
{
	dbDatabase.SaveDatabaseBuffer( m_nDbIdx, m_DatabaseCSVArray );

	if ( EcrmanOptions.GetPluPriceSetsFlag() == TRUE )
	{
		int nSet = m_Page2.GetPriceSet();
			
		bool bAddedPluFields, bRemovedPluFields, bWasEmpty;
		m_PriceSetHeaderManager.CheckHeaderChanges( bAddedPluFields, bRemovedPluFields, bWasEmpty );

		CStringArray arrayWarning;

		CString strTitle;
		strTitle.Format ( "Select Price Set Fields (%s, Set %d)", dbDatabase.GetName(m_nDbIdx), nSet );
		
		bool bNeedUpdate = FALSE;

		//MUST UPDATE IF WE ADD OR REMOVE FIELDS
		if ( bAddedPluFields == TRUE || bRemovedPluFields == TRUE )
			bNeedUpdate = TRUE;

		//MUST WARN IF WE REMOVE FIELDS UNLESS OLD PRICE SETS WERE EMPTY
		if ( ( bRemovedPluFields == TRUE ) && ( bWasEmpty == FALSE ) )
		{
			arrayWarning.Add( strTitle );
			arrayWarning.Add( "The fields that you removed will not be editable in this price set." );
			arrayWarning.Add( "" );
			arrayWarning.Add( "Any price set specific values in these fields will be deleted." );
		}
		
		//MUST WARN IF WE ADD FIELDS UNLESS OLD PRICE SETS WERE EMPTY
		if ( ( bAddedPluFields == TRUE ) && ( bWasEmpty == FALSE ) )
		{	
			if ( arrayWarning.GetSize() == 0 )
				arrayWarning.Add( strTitle );
			else
				arrayWarning.Add( "" );
			
			arrayWarning.Add ( "The fields that you added will be editable in this price set." );
			arrayWarning.Add ( "" );
			arrayWarning.Add ( "You must specify values for these fields in all the Plu items" );
			arrayWarning.Add ( "+that are linked to this price set." );
		}
		
		//WARNING
		if ( arrayWarning.GetSize() != 0 )
		{
			if ( Prompter.Warning( arrayWarning ) != IDYES )
				bNeedUpdate = FALSE;
		}

		if ( bNeedUpdate == TRUE )
		{
			m_PriceSetHeaderManager.UpdateDatabasePriceSetFile( m_nDbIdx, nSet );
			m_PriceSetHeaderManager.CreateDatabasePriceSetFile( m_nDbIdx, nSet );
		}
	}
}

/**********************************************************************/
