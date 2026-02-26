/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
#include "ImportDefines.h"
/**********************************************************************/
#include "RecipeCSVArray.h"
/**********************************************************************/

CRecipeCSVArray::CRecipeCSVArray() : CSharedCSVArray ( ',', '"', FALSE, TRUE, TRUE )
{
	m_bInSync = TRUE;
}

/**********************************************************************/

CRecipeCSVArray::~CRecipeCSVArray()
{
}

/**********************************************************************/

bool CRecipeCSVArray::FindPluByNumber ( int nMasterIdx, const char* szPluNumber, int& nIndex )
{
	CString strPluNumberToFind = szPluNumber;
	::AddLeading ( strPluNumberToFind, Recipe::PluNo.Max, '0' );

	if ( ( nMasterIdx >= 0 ) && ( nMasterIdx < GetSize() ) && ( strPluNumberToFind == GetRecipePluNo ( nMasterIdx ) ) )
	{
		nIndex = nMasterIdx;
		return TRUE;
	}
	else if ( nMasterIdx != -1 )
		m_bInSync = FALSE;
	
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;

		CString strPluNumber = GetRecipePluNo ( nIndex );
		
		if ( strPluNumber < strPluNumberToFind )
			nStart = nIndex + 1;

		if ( strPluNumber > strPluNumberToFind )
			nEnd = nIndex - 1;

		if ( strPluNumber == strPluNumberToFind )
			return TRUE;
	}
	nIndex = nStart;
	return FALSE;	
}

/**********************************************************************/

const char* CRecipeCSVArray::GetRecipePluNo ( int nIndex )
{
	CString strTemp;

	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		m_strPluNo = csv.GetString ( 0 );
		::AddLeading ( m_strPluNo, Recipe::PluNo.Max, '0' );
		return m_strPluNo;
	}
	return "";
}

/**********************************************************************/

int CRecipeCSVArray::ImportLine ( CSSImportFile& import, const char* szImportLine, bool& bIncompleteLine )
{
	int nResult = IMPORT_LINE_IGNORED;
	bIncompleteLine = FALSE;

	CCSV csvImport ( szImportLine );
	CString strPluNo = csvImport.GetString(0);
	
	strPluNo = strPluNo.Left ( Recipe::PluNo.Max );
	::TrimSpaces ( strPluNo, FALSE );
	
	if ( strPluNo != "" )
	{
		int nPluIdx;
		bool bFound = FindPluByNumber ( -1, strPluNo, nPluIdx );

		if ( bFound == TRUE )	
		{
			CRecipeCSVRecord NewRecord;
			GetAt ( nPluIdx, NewRecord );
			
			if ( NewRecord.ImportLine ( import, csvImport, bIncompleteLine ) == TRUE )
			{
				SetAt ( nPluIdx, NewRecord );
				nResult = IMPORT_LINE_MODIFIED;
			}
		}	
		else			
		{	
			CRecipeCSVRecord NewRecord;
			NewRecord.SetRecipePluNo( strPluNo );
			
			if ( NewRecord.ImportLine ( import, csvImport, bIncompleteLine ) == TRUE )
			{
				InsertAt ( nPluIdx, NewRecord );
				nResult = IMPORT_LINE_ADDED;
			}
		}
	}

	return nResult;
}

/**********************************************************************/


