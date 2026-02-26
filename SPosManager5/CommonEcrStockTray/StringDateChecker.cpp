/**********************************************************************/
 
/**********************************************************************/
#include "EposReportBase.h"
//#include "globalfunctions.h"
#include "HistoryFileTypes.h"
/**********************************************************************/
#include "StringDateChecker.h"
/**********************************************************************/

CStringDateChecker::CStringDateChecker()
{
	m_bIsValid = FALSE;
	m_strDateFrom = "";
	m_strDateTo = "";
}

/**********************************************************************/

CStringDateChecker::CStringDateChecker( const char* szDateFrom, const char* szDateTo )
{
	SetRange( szDateFrom, szDateTo );
}

/**********************************************************************/

bool CStringDateChecker::SetRange( const char* szDateFrom, const char* szDateTo )
{
	m_bIsValid = FALSE;
	m_strDateFrom = szDateFrom;
	m_strDateTo = szDateTo;

	COleDateTime oleDate;
	if ( ::GetOleDateFromString( m_strDateFrom, oleDate ) == TRUE )
		if ( ::GetOleDateFromString( m_strDateTo, oleDate ) == TRUE )
			if ( m_strDateTo >= m_strDateFrom )
				m_bIsValid = TRUE;

	return m_bIsValid;
}

/**********************************************************************/

bool CStringDateChecker::CheckDate( CString& strDate, COleDateTime& oleDate )
{
	bool bResult = FALSE;

	if ( TRUE == m_bIsValid )
		if ( ::GetOleDateFromString( strDate, oleDate ) == TRUE )
			if ( ( strDate >= m_strDateFrom ) && ( strDate <= m_strDateTo ) )
				bResult = TRUE;

	return bResult;
}

/**********************************************************************/

bool CStringDateChecker::CheckDate( bool bSaleTime, int nFileType, CString& strDateFolder, CString& strDateTran, CString& strDateSale )
{
	bool bResult = FALSE;
	COleDateTime oleDummy;
	
	bool bCanDoSaleTime = FALSE;
	switch( nFileType )
	{
	case HISTORYFILE_TYPE_SALES:
	case HISTORYFILE_TYPE_REASONS_ALL:
	case HISTORYFILE_TYPE_REASONS_NEW:
		bCanDoSaleTime = TRUE;
		break;
	}
		
	if ( ( TRUE == bSaleTime ) && ( TRUE == bCanDoSaleTime  ) )
	{
		if ( strDateFolder.GetLength() == 17 )
		{
			strDateTran = strDateFolder.Left(8);
			strDateSale = strDateFolder.Right(8);
			bResult = CheckDate( strDateSale, oleDummy );
		}
	}
	else
	{
		switch( strDateFolder.GetLength() )
		{
		case 17:
			strDateTran = strDateFolder.Left(8);
			strDateSale = strDateFolder.Right(8);

			if ((strDateTran == strDateSale) || (TRUE == bCanDoSaleTime))
			{
				bResult = CheckDate(strDateTran, oleDummy);
			}

			break;

		case 8:
			strDateTran = strDateFolder;
			strDateSale = strDateFolder;
			bResult = CheckDate( strDateTran, oleDummy );
			break;
		}
	}

	return bResult;
}

/**********************************************************************/
