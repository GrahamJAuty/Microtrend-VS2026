/**********************************************************************/
#include "FilterTypes.h"
/**********************************************************************/
#include "CategorySetCSVArray.h"
/**********************************************************************/

CCategorySetCSVRecord::CCategorySetCSVRecord()
{
	m_bAllMode = FALSE;
	m_bCreatedEmptyFilter = FALSE;
	m_bConvertedEmptyFilter = FALSE;

	ClearRecord();
	m_strFilterName = "";
}

/**********************************************************************/

void CCategorySetCSVRecord::ClearRecord()
{
	IncludeAllCategories( FALSE );
	m_strName = "";
	m_strFilterName = "";
	m_bReportEnabledSet = FALSE;
}

/**********************************************************************/

int CCategorySetCSVRecord::CreateCategorySet ( int nCategoryFilter )
{
	return CreateCategorySetInternal( DataManager.Category, DataManager.CategorySet, nCategoryFilter );
}

/**********************************************************************/

int CCategorySetCSVRecord::CreateCategorySetSystem( int nCategoryFilter )
{
	return CreateCategorySetInternal( DataManagerNonDb.SystemCategory, DataManagerNonDb.SystemCategorySet, nCategoryFilter );
}

/**********************************************************************/

int CCategorySetCSVRecord::CreateCategorySetInternal( CDefTextCategory& CatArray, CCategorySetCSVArray& CatSetArray, int nCategoryFilter )
{
	m_bReportEnabledSet = FALSE;
	m_bCreatedEmptyFilter = FALSE;

	IncludeAllCategories( FALSE );
	int nCategorySet = nCategoryFilter - FIRST_CATEGORYSET_FILTER;

	if ( nCategoryFilter == 0 )
	{
		IncludeAllCategories ( TRUE );
		m_strFilterName = "All";
	}
	else if ( nCategoryFilter == FIRST_CATEGORYSET_FILTER - 1 )
	{
		CreateReportEnabledCategorySet(CatArray);
		m_strFilterName = "Report enabled";
		m_bReportEnabledSet = TRUE;

	}
	else if ( nCategoryFilter >= 1 && nCategoryFilter <= MAX_CATEGORY )
	{
		IncludeCategory ( nCategoryFilter, TRUE );
		m_strFilterName = CatArray.GetText ( nCategoryFilter );
	}
	else if ( ( nCategorySet >= 0 ) && ( nCategorySet < CatSetArray.GetSize() ) )
	{
		CCSV csv ( CatSetArray.GetAt ( nCategorySet ) );
		ConvertFromCSV ( csv );
		m_strFilterName = m_strName;
		m_bCreatedEmptyFilter = m_bConvertedEmptyFilter;
	}
	else	
	{
		nCategoryFilter = 0;
		IncludeAllCategories ( TRUE );
		m_strFilterName = "All";
	}

	return nCategoryFilter;
}

/**********************************************************************/

int CCategorySetCSVRecord::CreateReportEnabledCategorySet( CDefTextCategory& CatArray )
{
	int nResult = 0;

	for ( int nCategory = 1; nCategory <= MAX_CATEGORY; nCategory++ )
	{
		bool bReportEnable = CatArray.GetReportEnable( nCategory );
		if ( bReportEnable == TRUE ) nResult++;
		IncludeCategory ( nCategory, bReportEnable );
	}
	m_bAllMode = ( MAX_CATEGORY == nResult );
	m_strFilterName = "Report enabled";
	return nResult;
}

/**********************************************************************/

int CCategorySetCSVRecord::GetDefaultCategory()
{
	for ( int nCategory = 1; nCategory <= MAX_CATEGORY; nCategory++ )
		if ( GetCategoryStatus ( nCategory ) == TRUE )
			return nCategory;

	return 1;
}

/**********************************************************************/

void CCategorySetCSVRecord::IncludeAllCategories( bool bFlag )
{
	m_strName = "";
	for ( int nIndex = 0; nIndex <= MAX_CATEGORY; nIndex++ )
		m_bIncluded [ nIndex ] = bFlag;

	m_bAllMode = bFlag;
}

/**********************************************************************/

const char* CCategorySetCSVRecord::GetFilterName( bool bTrailingComma, bool bHideReportEnabled )
{
	m_strFilterNameOut = "";

	if ( ( FALSE == bHideReportEnabled ) || ( FALSE == m_bReportEnabledSet ) )
	{
		if ( TRUE == bTrailingComma )
			m_strFilterNameOut = ", ";
	
		m_strFilterNameOut += m_strFilterName;
	}
	
	return m_strFilterNameOut;
}

/**********************************************************************/

void CCategorySetCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	m_strName = csv.GetString (0);
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CCategorySetCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	int nCount = csv.GetInt(2);
	int nOffset = 3;

	m_bConvertedEmptyFilter = TRUE;

	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		if ( IncludeCategory ( csv.GetInt ( nOffset++ ), TRUE ) == TRUE )
			m_bConvertedEmptyFilter = FALSE;
	}
}

/**********************************************************************/
	
void CCategorySetCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strName );
	csv.Add ( CATEGORYSET_VERSION );				
	
	CArray<int,int> arrayTemp;

	for ( int nIndex = 1; nIndex <= MAX_CATEGORY; nIndex++ )
		if ( GetCategoryStatus ( nIndex ) == TRUE )
			arrayTemp.Add ( nIndex );

	csv.Add ( arrayTemp.GetSize() );

	for ( int nIndex = 0; nIndex < arrayTemp.GetSize(); nIndex++ )
		csv.Add ( arrayTemp.GetAt ( nIndex ) );
}

/**********************************************************************/
			
bool CCategorySetCSVRecord::IncludeCategory ( int nCategory, bool bFlag )
{
	bool bSetCategory = FALSE;

	if ( ( nCategory >= 1 ) && ( nCategory <= MAX_CATEGORY ) )
	{
		m_bIncluded [ nCategory ] = bFlag;
		bSetCategory = bFlag;
	}

	return bSetCategory;
}

/**********************************************************************/

bool CCategorySetCSVRecord::GetCategoryStatus( int nCategory )
{
	if ( nCategory >= 1 && nCategory <= MAX_CATEGORY )
		return m_bIncluded [ nCategory ];

	return FALSE;
}

/**********************************************************************/

CCategorySetCSVArray::CCategorySetCSVArray() : CSharedCSVArray()
{
}

/**********************************************************************/

const char* CCategorySetCSVArray::GetName ( int nIndex )
{
	m_strName = "";
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		m_strName = csv.GetString ( 0 );
	}

	return m_strName;
}

/**********************************************************************/


