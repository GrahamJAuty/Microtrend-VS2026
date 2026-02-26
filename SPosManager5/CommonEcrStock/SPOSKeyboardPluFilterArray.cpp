/**********************************************************************/
#include "SPOSKeyboardPluFilterArray.h"
/**********************************************************************/

CSPOSKeyboardPluFilterArray::CSPOSKeyboardPluFilterArray() 
{ 
	m_strDescription = "";
	m_bShowModifiers = TRUE;
	m_bAllFlag = TRUE;
	m_IndexArray.RemoveAll();
} 

/**********************************************************************/

int CSPOSKeyboardPluFilterArray::GetSize()
{
	if ( m_bAllFlag == FALSE )
		return m_IndexArray.GetSize();
	else
		return SPOSKeyboardManager.GetPluDatabase() -> GetSize();
}

/**********************************************************************/

void CSPOSKeyboardPluFilterArray::SetDescriptionFilter ( const char* szDescription )
{
	m_strDescription = szDescription;
}

/**********************************************************************/

int CSPOSKeyboardPluFilterArray::GetRecordIndexAt( int nArrayIdx )
{
	if ( nArrayIdx >= GetSize() )
		return -1;

	if ( m_bAllFlag == TRUE )
		return nArrayIdx;
	else
		return m_IndexArray.GetAt ( nArrayIdx );
}

/**********************************************************************/

const char* CSPOSKeyboardPluFilterArray::GetDisplayLine ( int nArrayIdx )
{
	m_strRecord ="";
	int nPluIdx;
	
	if ( m_bAllFlag == FALSE )
	{
		if ( nArrayIdx >= m_IndexArray.GetSize() )
			return "";

		nPluIdx = m_IndexArray.GetAt( nArrayIdx );
	}
	else
		nPluIdx = nArrayIdx;

	GetLine( nPluIdx );
	return m_strRecord;
}

/**********************************************************************/

bool CSPOSKeyboardPluFilterArray::ComparePluDescription( const char* szText, int nArrayIdx )
{
	CString strSearchText = szText;
	strSearchText.MakeUpper();

	CSPOSKeyboardImportPluCSVRecord Record;
	int nPluIdx = GetRecordIndexAt ( nArrayIdx );
	SPOSKeyboardManager.GetPluDatabase() -> GetAt ( nPluIdx, Record );
		
	CString strDescription = Record.GetEcrText();		
	strDescription.MakeUpper();
		
	if ( strDescription.Find ( strSearchText ) != - 1 )  
		return TRUE;
	
	strDescription = Record.GetRepText();		
	strDescription.MakeUpper();	
	if ( strDescription.Find ( strSearchText ) != - 1 )  
		return TRUE;
	
	return FALSE;
}

/**********************************************************************/

bool CSPOSKeyboardPluFilterArray::FindPluItemByNumber ( __int64 nNumber, int& nArrayIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nArrayIdx = ( nStart + nEnd ) / 2;
		int nPluIdx = GetRecordIndexAt ( nArrayIdx );

		CSPOSKeyboardImportPluCSVRecord PluRecord;
		SPOSKeyboardManager.GetPluDatabase() -> GetAt ( nPluIdx, PluRecord );
		__int64 nPluNo = PluRecord.GetPluNumber();

		if ( nPluNo < nNumber )
			nStart = nArrayIdx + 1;

		else if ( nPluNo > nNumber )
			nEnd = nArrayIdx - 1;

		else
			return TRUE;
	}
		
	nArrayIdx = nStart;
	return FALSE;
}

/**********************************************************************/

bool CSPOSKeyboardPluFilterArray::FindPluItemByIndex ( int nPluIdx, int& nArrayIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nArrayIdx = ( nStart + nEnd ) / 2;
		int nThisPluIdx = GetRecordIndexAt ( nArrayIdx );

		if ( nThisPluIdx < nPluIdx )
			nStart = nArrayIdx + 1;

		if ( nThisPluIdx > nPluIdx )
			nEnd = nArrayIdx - 1;

		if ( nThisPluIdx == nPluIdx )
			return TRUE;
	}
		
	nArrayIdx = nStart;
	return FALSE;
}

/**********************************************************************/

bool CSPOSKeyboardPluFilterArray::FindPluItemByDescription ( const char* szText, int nStartPos, int nSearchType, bool bMatchCase, int& nArrayIdx )
{		
	int nCount = ( GetSize() - nStartPos ) + 1;
	bool bResult = FALSE;

	for ( int nOffset = 0; nOffset < nCount; nOffset++ )
	{
		if ( ComparePluDescription ( szText, nStartPos + nOffset ) == TRUE )
		{	 
			nArrayIdx = nStartPos + nOffset;
			bResult = TRUE;
			break;
		}
	}
	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardPluFilterArray::BuildRecordList()
{
	CWaitCursor wait;
	if ( BuildFirst() == TRUE )
		while ( BuildNext() == TRUE );
}

/**********************************************************************/

bool CSPOSKeyboardPluFilterArray::BuildFirst()
{
	m_nBuildIndex = 0;
	m_bAllFlag = ( ( m_strDescription == "" ) && ( m_bShowModifiers == TRUE ) );
	if ( m_bAllFlag == TRUE )
		return TRUE;

	m_IndexArray.RemoveAll();
	return ( SPOSKeyboardManager.GetPluDatabase() -> GetSize() != 0 );
}

/**********************************************************************/
			
bool CSPOSKeyboardPluFilterArray::BuildNext()
{
	if ( m_nBuildIndex >= SPOSKeyboardManager.GetPluDatabase() -> GetSize() )
		return FALSE;

	CSPOSKeyboardImportPluCSVRecord PluRecord;
	SPOSKeyboardManager.GetPluDatabase() -> GetAt ( m_nBuildIndex, PluRecord );

	if ( m_bShowModifiers == FALSE )
	{
		__int64 nPluNo = PluRecord.GetPluNumber();

		CString strPluNo;
		strPluNo.Format( "%I64d", nPluNo );
		int nPluNoLen = strPluNo.GetLength();

		int nMinDirectPluLen = 1;
		if ( SPOSKeyboardManager.GetLayoutOptions() -> GetMaxBasePluLen() != 0 )
			nMinDirectPluLen = SPOSKeyboardManager.GetLayoutOptions() -> GetMaxBasePluLen() + 2;

		if ( ( ( nPluNo % 10 ) != 0 ) && ( nPluNoLen < nMinDirectPluLen ) )
		{
			m_nBuildIndex++;
			return TRUE;
		}
	}

	if ( m_strDescription != "" )
	{
		bool bFoundString = FALSE;
		CString strSearchText = m_strDescription;
		strSearchText.MakeUpper();
		
		CString strDesc1 = PluRecord.GetEcrText();	
		strDesc1.MakeUpper();
		
		CString strDesc2 = PluRecord.GetRepText();	
		strDesc2.MakeUpper();
		
		if ( strDesc1.Find ( strSearchText ) != - 1 )  
			bFoundString = TRUE;
		else if ( strDesc2.Find ( strSearchText ) != - 1 )  
			bFoundString = TRUE;
		
		if ( bFoundString == FALSE )
		{
			m_nBuildIndex++;
			return TRUE;
		}
	}

	m_IndexArray.Add ( m_nBuildIndex );
	m_nBuildIndex++;
	return TRUE;
}


/**********************************************************************/

void CSPOSKeyboardPluFilterArray::SetArrayIdx( int nArrayIdx )
{
	if ( nArrayIdx >= 0 && nArrayIdx < GetSize() )
		m_nArrayIdx = nArrayIdx;
	else
		m_nArrayIdx = -1;
}

/**********************************************************************/

void CSPOSKeyboardPluFilterArray::Next()
{
	if ( m_nArrayIdx < GetSize() - 1 )
		m_nArrayIdx++;
}

/**********************************************************************/

void CSPOSKeyboardPluFilterArray::Previous()
{
	if ( m_nArrayIdx > 0 )
		m_nArrayIdx--;
}

/**********************************************************************/

bool CSPOSKeyboardPluFilterArray::HasNext()
{
	if ( m_nArrayIdx < GetSize() - 1 )
		return TRUE; 
	else
		return FALSE;
}

/**********************************************************************/

bool CSPOSKeyboardPluFilterArray::HasPrevious()
{
	if ( m_nArrayIdx > 0 )
		return TRUE;
	else
		return FALSE;
}

/**********************************************************************/

void CSPOSKeyboardPluFilterArray::CopySettingsFrom ( CSPOSKeyboardPluFilterArray& Source )
{
	m_strDescription = Source.GetDescription();
	m_bShowModifiers = Source.GetShowModifiersFlag();
}

/**********************************************************************/

void CSPOSKeyboardPluFilterArray::GetLine ( int nPluIdx )
{
	if ( nPluIdx > SPOSKeyboardManager.GetPluDatabase() -> GetSize() )
	{	
		m_strRecord = "";
		return;
	}

	CSPOSKeyboardImportPluCSVRecord PluRecord;
	SPOSKeyboardManager.GetPluDatabase() -> GetAt ( nPluIdx, PluRecord );
		
	CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );
	csv.Add ( PluRecord.GetFormattedPluNumber() );	
	csv.Add ( PluRecord.GetEcrText() );
	csv.Add ( PluRecord.GetRepText() );
		
	m_strRecord = csv.GetLine();
}

/**********************************************************************/

