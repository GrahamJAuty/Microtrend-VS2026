/**********************************************************************/
 
/**********************************************************************/
#include "GroupCSVArrayPayment.h"
/**********************************************************************/

CGroupCSVArrayPayment::CGroupCSVArrayPayment() : CGroupCSVArrayBase()
{
}

/**********************************************************************/

int CGroupCSVArrayPayment::Open( const char* szFilename, int nMode )
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
		CGroupCSVRecordPayment Group;
		Group.SetGroupNo(1);
		Group.SetRepText( Group.GetDefaultText() );
		InsertAt( 0, Group );
	}

	return nReply;
}

/**********************************************************************/

void CGroupCSVArrayPayment::GetGroupInfo( int nIndex, CBaseGroupInfo& info )
{
	int nResult = 0;
	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		CGroupCSVRecordPayment Group;
		GetAt( nIndex, Group );
		info.m_nGroupNo = Group.GetGroupNo();
		info.m_strReportText = Group.GetReportText();
		info.m_strDefText = Group.GetDefaultText();
	}
}

/**********************************************************************/

bool CGroupCSVArrayPayment::GetListDataLine( int nIndex, CString& strLine )
{
	bool bResult = FALSE;
	m_strListDataLine = "";

	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		CGroupCSVRecordPayment GroupRecord;
		GetAt( nIndex, GroupRecord );

		strLine.Format ( "%d\t%s",
			GroupRecord.GetGroupNo(),
			GroupRecord.GetRepText() );
		
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CGroupCSVArrayPayment::RememberSettings()
{
	m_arraySaved.RemoveAll();
	for ( int n = 0; n < GetSize(); n++ )
		m_arraySaved.Add( GetAt(n) );
}

/**********************************************************************/

void CGroupCSVArrayPayment::RestoreSettings()
{
	RemoveAt( 0, GetSize() );
	for ( int n = 0; n < m_arraySaved.GetSize(); n++ )
	{
		CCSV csv( m_arraySaved.GetAt(n) );
		
		CGroupCSVRecordPayment Group;
		Group.ConvertFromCSV( csv );

		Add( Group );
	}
	m_arraySaved.RemoveAll();
}

/**********************************************************************/

