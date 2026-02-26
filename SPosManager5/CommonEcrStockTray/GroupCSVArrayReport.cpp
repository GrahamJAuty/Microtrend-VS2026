/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "DealerFlags.h"
/**********************************************************************/
#include "GroupCSVArrayReport.h"
/**********************************************************************/

CGroupCSVArrayReport::CGroupCSVArrayReport() : CGroupCSVArrayBase()
{
}

/**********************************************************************/

int CGroupCSVArrayReport::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	return nReply;
}

/**********************************************************************/

void CGroupCSVArrayReport::GetGroupInfo( int nIndex, CBaseGroupInfo& info )
{
	int nResult = 0;
	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		CGroupCSVRecordReport Group;
		GetAt( nIndex, Group );
		info.m_nGroupNo = Group.GetGroupNo();
		info.m_strReportText = Group.GetReportText();
		info.m_strDefText = Group.GetDefaultText();
	}
}

/**********************************************************************/

bool CGroupCSVArrayReport::GetListDataLine( int nIndex, CString& strLine )
{
	bool bResult = FALSE;
	m_strListDataLine = "";

	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		CGroupCSVRecordReport GroupRecord;
		GetAt( nIndex, GroupRecord );

		strLine.Format ( "%d\t%s\t%s",
			GroupRecord.GetGroupNo(),
			GroupRecord.GetRepText(),
			(const char*) DataManager.ConsolGroup.GetGroupDisplayText( GroupRecord.GetParentGroupNo() ) );
		
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CGroupCSVArrayReport::RememberSettings()
{
	m_arraySaved.RemoveAll();
	for ( int n = 0; n < GetSize(); n++ )
		m_arraySaved.Add( GetAt(n) );
}

/**********************************************************************/

void CGroupCSVArrayReport::RestoreSettings()
{
	RemoveAt( 0, GetSize() );
	for ( int n = 0; n < m_arraySaved.GetSize(); n++ )
	{
		CCSV csv( m_arraySaved.GetAt(n) );
		
		CGroupCSVRecordReport Group;
		Group.ConvertFromCSV( csv );

		Add( Group );
	}
	m_arraySaved.RemoveAll();
}

/**********************************************************************/
