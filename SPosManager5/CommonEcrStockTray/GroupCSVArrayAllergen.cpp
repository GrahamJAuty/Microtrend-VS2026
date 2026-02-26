/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManagerNonDb.h"
/**********************************************************************/
#include "GroupCSVArrayAllergen.h"
/**********************************************************************/

CGroupCSVArrayAllergen::CGroupCSVArrayAllergen() : CGroupCSVArrayBase()
{
	m_strPrefix = "A";
}

/**********************************************************************/

int CGroupCSVArrayAllergen::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	if ( GetSize() == 0 )
	{
		DirectAddAllergen ( 1, "Celery" );
		DirectAddAllergen ( 2, "Gluten" );
		DirectAddAllergen ( 3, "Crustacean" );
		DirectAddAllergen ( 4, "Eggs" );
		DirectAddAllergen ( 5, "Fish" );
		DirectAddAllergen ( 6, "Lupin" );
		DirectAddAllergen ( 7, "Milk" );
		DirectAddAllergen ( 8, "Molluscs" );
		DirectAddAllergen ( 9, "Mustard" );
		DirectAddAllergen ( 10, "Nuts" );
		DirectAddAllergen ( 11, "Peanuts" );
		DirectAddAllergen ( 12, "Sesame Seeds" );
		DirectAddAllergen ( 13, "Soya" );
		DirectAddAllergen ( 14, "Sulphites" );
	}

	return nReply;
}

/**********************************************************************/

void CGroupCSVArrayAllergen::DirectAddAllergen( int nGroupNo, const char* szName )
{
	CGroupCSVRecordAllergen Group;
	Group.SetGroupNo( nGroupNo );
	Group.SetRepText( szName );
	Add( Group );
}

/**********************************************************************/

void CGroupCSVArrayAllergen::GetGroupInfo( int nIndex, CBaseGroupInfo& info )
{
	int nResult = 0;
	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		CGroupCSVRecordAllergen Group;
		GetAt( nIndex, Group );
		info.m_nGroupNo = Group.GetGroupNo();
		info.m_strReportText = Group.GetReportText();
		info.m_strDefText = Group.GetDefaultText();
	}
}

/**********************************************************************/

bool CGroupCSVArrayAllergen::GetListDataLine( int nIndex, CString& strLine )
{
	bool bResult = FALSE;
	m_strListDataLine = "";

	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		CGroupCSVRecordAllergen GroupRecord;
		GetAt( nIndex, GroupRecord );

		strLine.Format ( "%d\t%s",
			GroupRecord.GetGroupNo(),
			GroupRecord.GetRepText() );
		
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CGroupCSVArrayAllergen::RememberSettings()
{
	m_arraySaved.RemoveAll();
	for ( int n = 0; n < GetSize(); n++ )
		m_arraySaved.Add( GetAt(n) );
}

/**********************************************************************/

void CGroupCSVArrayAllergen::RestoreSettings()
{
	RemoveAt( 0, GetSize() );
	for ( int n = 0; n < m_arraySaved.GetSize(); n++ )
	{
		CCSV csv( m_arraySaved.GetAt(n) );
		
		CGroupCSVRecordAllergen Group;
		Group.ConvertFromCSV( csv );

		Add( Group );
	}
	m_arraySaved.RemoveAll();
}

/**********************************************************************/

void CGroupCSVArrayAllergen::AddAllergenZero()
{
	bool bNeedToAdd = FALSE;
	
	if ( GetSize() == 0 )
	{
		bNeedToAdd = TRUE;
	}
	else
	{
		CGroupCSVRecordAllergen Allergen;
		GetAt( 0, Allergen );
		bNeedToAdd = ( Allergen.GetGroupNo() != 0 );
	}

	if ( TRUE == bNeedToAdd )
	{
		CGroupCSVRecordAllergen Allergen;
		Allergen.SetGroupNo(0);
		Allergen.SetRepText( "None" );
		InsertAt(0, Allergen );
	}
}

/**********************************************************************/

void CGroupCSVArrayAllergen::RemoveAllergenZero()
{
	bool bNeedToRemove = FALSE;

	if ( GetSize() != 0 )
	{
		CGroupCSVRecordAllergen Allergen;
		GetAt( 0, Allergen );
		bNeedToRemove = ( Allergen.GetGroupNo() == 0 );
	}

	if ( TRUE == bNeedToRemove )
		RemoveAt(0);
}

/**********************************************************************/

void CGroupCSVArrayAllergen::WriteSMAllergenFile()
{
	CString strFilename;
	strFilename.Format( "%s\\smfile03.dat", 
		dbDatabase.GetFolderPathData( 0 ) );

	CSSFile fileSM;
	if ( fileSM.Open( strFilename, "wb" ) == TRUE )
	{
		fileSM.WriteLine( "No,Text" );

		for ( int n = 0; n < GetSize(); n++ )
		{
			CBaseGroupInfo infoGroup;
			GetGroupInfo( n, infoGroup );

			CCSV csv;
			csv.Add( infoGroup.m_nGroupNo );
			csv.Add( infoGroup.m_strReportText );
			
			fileSM.WriteLine( csv.GetLine() );
		}
	}
}

/**********************************************************************/
