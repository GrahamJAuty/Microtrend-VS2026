/**********************************************************************/
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "CustomerSelectionListOneOff.h"
/**********************************************************************/

CCustomerSelectionListOneOffInfo::CCustomerSelectionListOneOffInfo()
{
	Reset();
}

/**********************************************************************/

void CCustomerSelectionListOneOffInfo::Reset()
{
	m_nLocIdx = 0;
	m_nCustomerID = 0;
	
	m_strName1 = "";
	m_strName2 = "";
	m_strPhone1 = "";
	m_strPhone2 = "";
	m_strPhone3 = "";

	m_nSortIdx = 0;
	m_bIsSelected = TRUE;
	m_bIsSelectedTemp = TRUE;
	m_bIsInFilter = TRUE;
}

/**********************************************************************/

int CCustomerSelectionListOneOffInfo::Compare ( CCustomerSelectionListOneOffInfo& source, int nHint )
{
	if ( source.m_nLocIdx != m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );

	if ( source.m_nCustomerID != m_nCustomerID )
		return ( ( m_nCustomerID > source.m_nCustomerID ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

void CCustomerSelectionListOneOffInfo::Add ( CCustomerSelectionListOneOffInfo& source )
{
	m_nLocIdx = source.m_nLocIdx;
	m_nCustomerID = source.m_nCustomerID;
	m_strName1 = source.m_strName1;
	m_strName2 = source.m_strName2;
	m_strPhone1 = source.m_strPhone1;
	m_strPhone2 = source.m_strPhone2;
	m_strPhone3 = source.m_strPhone3;
	m_nSortIdx = source.m_nSortIdx;
}

/**********************************************************************/

const char* CCustomerSelectionListOneOffInfo::GetReportName()
{
	CString strTemp1 = m_strName1;
	::TrimSpaces( strTemp1, FALSE );

	CString strTemp2 = m_strName2;
	::TrimSpaces( strTemp2, FALSE );

	if ( strTemp1 == "" )
		m_strReportName = strTemp2;
	else
	{
		m_strReportName = strTemp1;

		if ( strTemp2 != "" )
		{
			m_strReportName += " ";
			m_strReportName += strTemp2;
		}
	}
	
	return m_strReportName;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCustomerSelectionListOneOff::CCustomerSelectionListOneOff(void)
{
	m_nSortOrderType = -1;
}

/**********************************************************************/

void CCustomerSelectionListOneOff::BuildList()
{
	ClearList();

	CDataManagerInfo info;
			
	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{
		int nSPOSVersion;
		int nConnectionType;
		dbLocation.GetLocSPOSVersion( nLocIdx, nSPOSVersion, nConnectionType );
		
		if ( CONNECTION_TYPE_STANDARD_NONE == nConnectionType )
		{
			CDataManagerInfo info;
			DataManagerNonDb.CloseCustomerOneOff( info );
			DataManagerNonDb.OpenCustomerOneOff( DB_READONLY, nLocIdx, info );
			DataManagerNonDb.CustomerOneOff.LoadHeaderStartDateRecord();
			DataManagerNonDb.CustomerOneOff.MoveDeletionsToHelperArray();
			BuildListInternal( nLocIdx );
		}
	}

	DataManagerNonDb.CloseCustomerOneOff( info );
}

/**********************************************************************/

void CCustomerSelectionListOneOff::BuildListCurrentDb()
{
	ClearList();
	BuildListInternal(0 );
}

/**********************************************************************/

void CCustomerSelectionListOneOff::BuildListInternal( int nLocIdx )
{
	for ( int n = 0; n < DataManagerNonDb.CustomerOneOff.GetSize(); n++ )
	{
		CCustomerCSVRecord Customer;
		DataManagerNonDb.CustomerOneOff.GetAt( n, Customer );

		CCustomerSelectionListOneOffInfo info;
		info.m_nLocIdx = nLocIdx;
		info.m_nCustomerID = Customer.GetCustomerID();
		info.m_strName1 = Customer.GetDisplayName1();
		info.m_strName2 = Customer.GetDisplayName2();
		info.m_strPhone1 = Customer.GetPhone1();
		info.m_strPhone2 = Customer.GetPhone2();
		info.m_strPhone3 = Customer.GetPhone3();
		
		m_arrayCustomers.Consolidate( info );			
	}
}

/**********************************************************************/

void CCustomerSelectionListOneOff::ClearList()
{
	m_nSortOrderType = -1;
	m_arrayCustomers.RemoveAll();
	m_arraySortOrder.RemoveAll();
}

/**********************************************************************/

bool CCustomerSelectionListOneOff::FindCustomer( CCustomerSelectionListOneOffInfo& info, int& nPos )
{
	return m_arrayCustomers.Find( info, nPos );
}

/**********************************************************************/

bool CCustomerSelectionListOneOff::FindCustomer( CCustomerSelectionListAccountInfo& info, int& nPos )
{
	CCustomerSelectionListOneOffInfo infoOneOff;
	infoOneOff.m_nLocIdx = info.m_nLocIdx;
	infoOneOff.m_nCustomerID = info.m_nCustomerID;
	return m_arrayCustomers.Find( infoOneOff, nPos );
}

/**********************************************************************/

void CCustomerSelectionListOneOff::SetCustomerStatus( int nPos, CCustomerSelectionListOneOffInfo& info )
{
	if ( ( nPos >= 0 ) && ( nPos < m_arrayCustomers.GetSize() ) )
		m_arrayCustomers.SetAt( nPos, info );
}

/**********************************************************************/

void CCustomerSelectionListOneOff::GetCustomerStatus( int nPos, CCustomerSelectionListOneOffInfo& info )
{
	if ( ( nPos >= 0 ) && ( nPos < m_arrayCustomers.GetSize() ) )
		m_arrayCustomers.GetAt( nPos, info );
	else
	{
		info.m_nLocIdx = 0;
		info.m_nCustomerID = 0;
		info.m_strName1 = "";
		info.m_strName2 = "";
		info.m_strPhone1 = "";
		info.m_strPhone2 = "";
		info.m_strPhone3 = "";
		info.m_nSortIdx = 0;
	}
}

/**********************************************************************/

void CCustomerSelectionListOneOff::GetCustomerStatus( int nPos, CCustomerSelectionListAccountInfo& info )
{
	CCustomerSelectionListOneOffInfo infoOneOff;
	GetCustomerStatus( nPos, infoOneOff );
	info.m_nLocIdx = infoOneOff.m_nLocIdx;
	info.m_nCustomerID = infoOneOff.m_nCustomerID;
	info.m_strName1 = infoOneOff.m_strName1;
	info.m_strName2 = infoOneOff.m_strName2;
	info.m_nSortIdx = infoOneOff.m_nSortIdx;
}

/**********************************************************************/

void CCustomerSelectionListOneOff::GetCustomerStatus( CCustomerSelectionListOneOffInfo& info )
{
	int nPos;
	if ( FindCustomer( info, nPos ) == TRUE )
		m_arrayCustomers.GetAt( nPos, info );
	else
	{
		info.m_strName1.Format( "Customer %d", info.m_nCustomerID );
		info.m_strName2 = "";
	}
}

/**********************************************************************/

void CCustomerSelectionListOneOff::GetCustomerStatus( CCustomerSelectionListAccountInfo& info )
{
	int nPos;
	if ( FindCustomer( info, nPos ) == TRUE )
		GetCustomerStatus( nPos, info );
	else
	{
		info.m_strName1.Format( "Customer %d", info.m_nCustomerID );
		info.m_strName2 = "";
	}
}

/**********************************************************************/

void CCustomerSelectionListOneOff::BuildSortOrder( int nSortOrder, bool bWithFilter, CString strID, CString strName, CString strPhone )
{
	if ( -1 == nSortOrder )
	{
		switch( EcrmanOptions.GetReportsCustomerSortType() )
		{
		case 1:		nSortOrder = CUSTOMER_SORT_FIRSTNAME_INC;	break;
		case 2:		nSortOrder = CUSTOMER_SORT_ID_INC;			break;
		case 0:
		default:	nSortOrder = CUSTOMER_SORT_LASTNAME_INC;	break;
		}
	}

	switch( nSortOrder )
	{
	case CUSTOMER_SORT_ID_INC:
	case CUSTOMER_SORT_ID_DEC:
	case CUSTOMER_SORT_FIRSTNAME_INC:
	case CUSTOMER_SORT_FIRSTNAME_DEC:
	case CUSTOMER_SORT_LASTNAME_INC:
	case CUSTOMER_SORT_LASTNAME_DEC:
		break;

	default:
		nSortOrder = CUSTOMER_SORT_LASTNAME_INC;
		break;
	}

	if ( ( nSortOrder != m_nSortOrderType ) || ( TRUE == bWithFilter ) )
	{
		m_arraySortOrder.RemoveAll();
		CReportConsolidationArray<CCustomerSelectionListTempSortInfo> arrayTemp( FALSE, nSortOrder );

		for ( int n = 0; n < GetSize(); n++ )
		{
			CCustomerSelectionListOneOffInfo infoArray;
			m_arrayCustomers.GetAt( n, infoArray );
			
			CCustomerSelectionListTempSortInfo infoTemp;
			infoTemp.m_nLocIdx = infoArray.m_nLocIdx;
			infoTemp.m_strName1 = infoArray.m_strName1;
			infoTemp.m_strName2 = infoArray.m_strName2;
			infoTemp.m_strPhone1 = infoArray.m_strPhone1;
			infoTemp.m_strPhone2 = infoArray.m_strPhone2;
			infoTemp.m_strPhone3 = infoArray.m_strPhone3;
			infoTemp.m_nCustomerID = infoArray.m_nCustomerID;
			infoTemp.m_nIdx = n;
			
			arrayTemp.Consolidate( infoTemp );
		}

		strName.MakeUpper();
		strPhone.MakeUpper();

		for ( int n = 0; n < arrayTemp.GetSize(); n++ )
		{
			CCustomerSelectionListTempSortInfo infoTemp;
			arrayTemp.GetAt( n, infoTemp );

			int nArrayIdx = infoTemp.m_nIdx;

			bool bInclude = TRUE;

			if ( TRUE == bWithFilter )
			{
				bInclude = FALSE;

				if ( strName != "" )
				{
					bInclude |= ( infoTemp.m_strName1.MakeUpper().Find( strName ) >= 0 );
					bInclude |= ( infoTemp.m_strName2.MakeUpper().Find( strName ) >= 0 );
				}

				if ( ( FALSE == bInclude ) && ( strPhone != "" ) )
				{
					bInclude |= ( infoTemp.m_strPhone1.MakeUpper().Find( strPhone ) >= 0 );
					bInclude |= ( infoTemp.m_strPhone2.MakeUpper().Find( strPhone ) >= 0 );
					bInclude |= ( infoTemp.m_strPhone3.MakeUpper().Find( strPhone ) >= 0 );
				}

				if ( ( FALSE == bInclude ) && ( strID != "" ) )
				{
					CString strCustomerID;
					strCustomerID.Format( "%8.8d", infoTemp.m_nCustomerID );
					bInclude = ( strCustomerID.Find( strID ) >= 0 );
				}
			}
			
			if ( TRUE == bInclude )
			{
				m_arraySortOrder.Add( nArrayIdx );
			}

			CCustomerSelectionListOneOffInfo infoArray;
			m_arrayCustomers.GetAt( nArrayIdx, infoArray );
			infoArray.m_nSortIdx = n;
			m_arrayCustomers.SetAt( nArrayIdx, infoArray );
		}

		m_nSortOrderType = nSortOrder;
	}
}

/**********************************************************************/

int CCustomerSelectionListOneOff::GetSortOrderSize()
{
	if ( -1 == m_nSortOrderType )
		return 0;
	else
		return m_arraySortOrder.GetSize();
}

/**********************************************************************/

int CCustomerSelectionListOneOff::GetSortOrderItem( int nIdx )
{
	if ( ( nIdx < 0 ) || ( nIdx >= GetSortOrderSize() ) )
		return 0;
	else
		return m_arraySortOrder.GetAt( nIdx );
}

/**********************************************************************/
