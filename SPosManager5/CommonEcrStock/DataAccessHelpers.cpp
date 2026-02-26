/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
/**********************************************************************/
#include "DataAccessHelpers.h"
/**********************************************************************/

CDataAccessHelpers::CDataAccessHelpers()
{
	m_pParentWnd = NULL;
}

/**********************************************************************/

int CDataAccessHelpers::GetDbIdx ( int nDbNo )
{
	int nDbIdx = 0;
	if ( ( nDbNo != 0 ) && ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE ) )
		nDbIdx = -1;

	return nDbIdx;
}

/**********************************************************************/

