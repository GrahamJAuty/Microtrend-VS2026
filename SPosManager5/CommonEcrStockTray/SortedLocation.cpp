 
/**********************************************************************/
#include "SortedLocation.h"
/**********************************************************************/

CSortedLocation::CSortedLocation()
{
	Reset();
}

/**********************************************************************/

void CSortedLocation::Reset()
{
	m_nDbNo = 0;
	m_nBlock = 0;
	m_strName = "";
	m_nIdx = 0;
	m_bSelected = FALSE;
}

/**********************************************************************/

void CSortedLocation::Add ( CSortedLocation& source )
{
}

/**********************************************************************/

int CSortedLocation::Compare( CSortedLocation& source, int nHint )
{
	if ( m_nDbNo != source.m_nDbNo )
		return ( m_nDbNo > source.m_nDbNo ? 1 : -1 );

	if ( m_nBlock != source.m_nBlock )
		return ( m_nBlock > source.m_nBlock ? 1 : -1 );

	CString strName1 = m_strName;
	CString strName2 = source.m_strName;
	
	strName1.MakeUpper();
	strName2.MakeUpper();

	if ( strName1 != strName2 )
		return ( strName1 > strName2 ? 1 : -1 );

	if ( m_nIdx != source.m_nIdx )
		return ( m_nIdx > source.m_nIdx ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
