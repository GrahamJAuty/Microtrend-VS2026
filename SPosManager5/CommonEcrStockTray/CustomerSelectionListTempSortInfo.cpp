/**********************************************************************/
#include "CustomerCSVArray.h"
/**********************************************************************/
#include "CustomerSelectionListTempSortInfo.h"
/**********************************************************************/

CCustomerSelectionListTempSortInfo::CCustomerSelectionListTempSortInfo()
{
	Reset();
}

/**********************************************************************/

void CCustomerSelectionListTempSortInfo::Reset()
{
	m_nLocIdx = 0;
	m_nCustomerID = 0;
	m_strName1 = "";
	m_strName2 = "";
	m_strPhone1 = "";
	m_strPhone2 = "";
	m_strPhone3 = "";
}

/**********************************************************************/

int CCustomerSelectionListTempSortInfo::Compare ( CCustomerSelectionListTempSortInfo& source, int nHint )
{
	if ( source.m_nLocIdx != m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );

	if ( CUSTOMER_SORT_ID_DEC == nHint )
	{
		if ( source.m_nCustomerID != m_nCustomerID )
			return ( ( m_nCustomerID < source.m_nCustomerID ) ? 1 : -1 );
		else
			return 0;
	}
	
	switch( nHint )
	{
	case CUSTOMER_SORT_FIRSTNAME_DEC:
		
		if ( source.m_strName1 != m_strName1 )
			return ( ( m_strName1 < source.m_strName1 ) ? 1 : -1 );

		if ( source.m_strName2 != m_strName2 )
			return ( ( m_strName2 < source.m_strName2 ) ? 1 : -1 );
		
		break;

	case CUSTOMER_SORT_FIRSTNAME_INC:
		
		if ( source.m_strName1 != m_strName1 )
			return ( ( m_strName1 > source.m_strName1 ) ? 1 : -1 );

		if ( source.m_strName2 != m_strName2 )
			return ( ( m_strName2 > source.m_strName2 ) ? 1 : -1 );
	
		break;

	case CUSTOMER_SORT_LASTNAME_DEC:

		if ( source.m_strName2 != m_strName2 )
			return ( ( m_strName2 < source.m_strName2 ) ? 1 : -1 );
		
		if ( source.m_strName1 != m_strName1 )
			return ( ( m_strName1 < source.m_strName1 ) ? 1 : -1 );

		break;

	case CUSTOMER_SORT_LASTNAME_INC:

		if ( source.m_strName2 != m_strName2 )
			return ( ( m_strName2 > source.m_strName2 ) ? 1 : -1 );

		if ( source.m_strName1 != m_strName1 )
			return ( ( m_strName1 > source.m_strName1 ) ? 1 : -1 );

		break;
	}

	if ( source.m_nCustomerID != m_nCustomerID )
		return ( ( m_nCustomerID > source.m_nCustomerID ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

void CCustomerSelectionListTempSortInfo::Add ( CCustomerSelectionListTempSortInfo& source )
{
}

/**********************************************************************/
