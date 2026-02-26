/**********************************************************************/
 
/**********************************************************************/
#include "CustomerCSVArray.h"
//#include "DataManagerNonDb.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "CustomerSelectionListAccount.h"
/**********************************************************************/

CCustomerSelectionListAccountInfo::CCustomerSelectionListAccountInfo()
{
	Reset();
}

/**********************************************************************/

void CCustomerSelectionListAccountInfo::Reset()
{
	m_nLocIdx = 0;
	m_nCustomerID = 0;
	
	m_strName1 = "";
	m_strName2 = "";
	m_strAddress1 = "";
	m_strAddress2 = "";
	m_strAddress3 = "";
	m_strPostcode = "";
	m_strEmail = "";
	m_bEmailRedirect = FALSE;

	m_nSortIdx = 0;
	m_dBalance = 0.0;
	m_dStartBalance = 0.0;
	m_startTime = COleDateTime( 2011, 1, 1, 0, 0, 0 );

	m_bIsSelected = TRUE;
	m_bIsSelectedTemp = TRUE;
	m_bIsInFilter = TRUE;
}

/**********************************************************************/

int CCustomerSelectionListAccountInfo::Compare ( CCustomerSelectionListAccountInfo& source, int nHint )
{
	if ( source.m_nLocIdx != m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );

	if ( source.m_nCustomerID != m_nCustomerID )
		return ( ( m_nCustomerID > source.m_nCustomerID ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

void CCustomerSelectionListAccountInfo::Add ( CCustomerSelectionListAccountInfo& source )
{
	m_nLocIdx = source.m_nLocIdx;
	m_nCustomerID = source.m_nCustomerID;

	m_strName1 = source.m_strName1;
	m_strName2 = source.m_strName2;
	m_strAddress1 = source.m_strAddress1;
	m_strAddress2 = source.m_strAddress2;
	m_strAddress3 = source.m_strAddress3;
	m_strPostcode = source.m_strPostcode;
	m_strEmail = source.m_strEmail;
	m_bEmailRedirect = source.m_bEmailRedirect;

	m_nSortIdx = source.m_nSortIdx;
	m_dBalance = source.m_dBalance;
	m_startTime = source.m_startTime;
}

/**********************************************************************/

const char* CCustomerSelectionListAccountInfo::GetReportName()
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

CCustomerSelectionListAccount::CCustomerSelectionListAccount(void)
{
	m_nSortOrderType = -1;
	m_bOneOffCustomer = TRUE;
	m_bUnknownCustomer = TRUE;
}

/**********************************************************************/

void CCustomerSelectionListAccount::BuildList()
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
			DataManagerNonDb.CloseCustomerAccount( info );
			DataManagerNonDb.OpenCustomerAccount( DB_READONLY, nLocIdx, info );
			DataManagerNonDb.CustomerAccount.LoadHeaderStartDateRecord();
			DataManagerNonDb.CustomerAccount.MoveDeletionsToHelperArray();
			BuildListInternal( nLocIdx );
		}
	}

	DataManagerNonDb.CloseCustomerAccount( info );
}

/**********************************************************************/

void CCustomerSelectionListAccount::BuildListCurrentDb()
{
	ClearList();
	BuildListInternal(0 );
}

/**********************************************************************/

void CCustomerSelectionListAccount::BuildListInternal( int nLocIdx )
{
	for ( int n = 0; n < DataManagerNonDb.CustomerAccount.GetSize(); n++ )
	{
		CCustomerCSVRecord Customer;
		DataManagerNonDb.CustomerAccount.GetAt( n, Customer );

		int nStartDay = 0;
		int nStartHour = 0;
		int nStartMinute = 0;

		if ( Customer.GetSpecifyStartDayFlag() == TRUE )
		{
			nStartDay = Customer.GetStartDay();
			nStartHour = Customer.GetStartHour();
			nStartMinute = Customer.GetStartMinute();
		}
		else
		{
			CCustomerCSVRecord Header;
			DataManagerNonDb.CustomerAccount.GetStartDateRecord( Header );
			nStartDay = Header.GetStartDay();
			nStartHour = Header.GetStartHour();
			nStartMinute = Header.GetStartMinute();
		}

		COleDateTime dateStart;
		GetOleDateSince2011( nStartDay, dateStart );

		CCustomerSelectionListAccountInfo info;
		info.m_nLocIdx = nLocIdx;
		info.m_nCustomerID = Customer.GetCustomerID();
		info.m_strName1 = Customer.GetDisplayName1();
		info.m_strName2 = Customer.GetDisplayName2();
		info.m_strAddress1 = Customer.GetAddress1();
		info.m_strAddress2 = Customer.GetAddress2();
		info.m_strAddress3 = Customer.GetAddress3();
		info.m_strPostcode = Customer.GetPostcode();
		info.m_strEmail = Customer.GetEmail();
		info.m_bEmailRedirect = Customer.GetEmailRedirectFlag();
		info.m_dStartBalance = Customer.GetStartBalance();
		
		info.m_startTime = COleDateTime( 
			dateStart.GetYear(),
			dateStart.GetMonth(),
			dateStart.GetDay(),
			nStartHour,
			nStartMinute,
			0 );

		if ( info.m_startTime.m_status != COleDateTime::valid )
			GetOleDateSince2011( 0, info.m_startTime );

		m_arrayCustomers.Consolidate( info );			
	}
}

/**********************************************************************/

void CCustomerSelectionListAccount::ClearList()
{
	m_nSortOrderType = -1;
	m_arrayCustomers.RemoveAll();
	m_arraySortOrder.RemoveAll();
}

/**********************************************************************/

bool CCustomerSelectionListAccount::FindCustomer( CCustomerSelectionListAccountInfo& info, int& nPos )
{
	return m_arrayCustomers.Find( info, nPos );
}

/**********************************************************************/

void CCustomerSelectionListAccount::SetCustomerStatus( int nPos, CCustomerSelectionListAccountInfo& info )
{
	if ( ( nPos >= 0 ) && ( nPos < m_arrayCustomers.GetSize() ) )
		m_arrayCustomers.SetAt( nPos, info );
}

/**********************************************************************/

void CCustomerSelectionListAccount::GetCustomerStatus( int nPos, CCustomerSelectionListAccountInfo& info )
{
	if ( ( nPos >= 0 ) && ( nPos < m_arrayCustomers.GetSize() ) )
		m_arrayCustomers.GetAt( nPos, info );
	else
	{
		info.m_nLocIdx = 0;
		info.m_nCustomerID = 0;
		info.m_strName1 = "";
		info.m_strName2 = "";
		info.m_strAddress1 = "";
		info.m_strAddress2 = "";
		info.m_strAddress3 = "";
		info.m_strPostcode = "";
		info.m_strEmail = "";
		info.m_bEmailRedirect = FALSE;
		info.m_nSortIdx = 0;
		info.m_dStartBalance = 0.0;
		info.m_startTime = COleDateTime( 2011, 1, 1, 0, 0, 0 );
	}
}

/**********************************************************************/

void CCustomerSelectionListAccount::GetCustomerStatus( CCustomerSelectionListAccountInfo& info )
{
	int nPos;
	if ( FindCustomer( info, nPos ) == TRUE )
		m_arrayCustomers.GetAt( nPos, info );
	else
	{
		info.m_strName1.Format( "Customer %d", info.m_nCustomerID );
		info.m_strName2 = "";
		info.m_strAddress1 = "";
		info.m_strAddress2 = "";
		info.m_strAddress3 = "";
		info.m_strPostcode = "";
		info.m_strEmail = "";
		info.m_bEmailRedirect = FALSE;
	}
}

/**********************************************************************/

void CCustomerSelectionListAccount::GetCustomerListForLocation( int nLocIdx, CReportConsolidationArray<CEposReportConsolCustomerBalanceID>& arrayDest )
{
	CCustomerSelectionListAccountInfo infoSelect;
	infoSelect.m_nLocIdx = nLocIdx;
	infoSelect.m_nCustomerID = 0;

	int nStartIdx;
	FindCustomer( infoSelect, nStartIdx );

	int nEndIdx;
	infoSelect.m_nLocIdx++;
	FindCustomer( infoSelect, nEndIdx );
	nEndIdx--;

	for ( int nIdx = nStartIdx; nIdx <= nEndIdx; nIdx++ )
	{
		GetCustomerStatus( nIdx, infoSelect );

		CEposReportConsolCustomerBalanceID Balance;
		Balance.m_nSortIdx = 0;
		Balance.m_nLocIdx = infoSelect.m_nLocIdx;
		Balance.m_nCustomerID = infoSelect.m_nCustomerID;
		Balance.m_dBalance = infoSelect.m_dStartBalance;
		arrayDest.Consolidate( Balance );
	}
}

/**********************************************************************/

void CCustomerSelectionListAccount::BuildSortOrder( int nSortOrder )
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

	if ( nSortOrder != m_nSortOrderType )
	{
		m_arraySortOrder.RemoveAll();
		CReportConsolidationArray<CCustomerSelectionListTempSortInfo> arrayTemp( FALSE, nSortOrder );

		for ( int n = 0; n < GetSize(); n++ )
		{
			CCustomerSelectionListAccountInfo infoArray;
			m_arrayCustomers.GetAt( n, infoArray );

			CCustomerSelectionListTempSortInfo infoTemp;
			infoTemp.m_nLocIdx = infoArray.m_nLocIdx;
			infoTemp.m_strName1 = infoArray.m_strName1;
			infoTemp.m_strName2 = infoArray.m_strName2;
			infoTemp.m_strPhone1 = "";
			infoTemp.m_strPhone2 = "";
			infoTemp.m_strPhone3 = "";
			infoTemp.m_nCustomerID = infoArray.m_nCustomerID;
			infoTemp.m_nIdx = n;
			
			arrayTemp.Consolidate( infoTemp );
		}

		for ( int n = 0; n < arrayTemp.GetSize(); n++ )
		{
			CCustomerSelectionListTempSortInfo infoTemp;
			arrayTemp.GetAt( n, infoTemp );

			int nArrayIdx = infoTemp.m_nIdx;
			m_arraySortOrder.Add( nArrayIdx );

			CCustomerSelectionListAccountInfo infoArray;
			m_arrayCustomers.GetAt( nArrayIdx, infoArray );
			infoArray.m_nSortIdx = n;
			m_arrayCustomers.SetAt( nArrayIdx, infoArray );
		}

		m_nSortOrderType = nSortOrder;
	}
}

/**********************************************************************/

int CCustomerSelectionListAccount::GetSortOrderSize()
{
	if ( -1 == m_nSortOrderType )
		return 0;
	else
		return m_arraySortOrder.GetSize();
}

/**********************************************************************/

int CCustomerSelectionListAccount::GetSortOrderItem( int nIdx )
{
	if ( ( nIdx < 0 ) || ( nIdx >= GetSortOrderSize() ) )
		return 0;
	else
		return m_arraySortOrder.GetAt( nIdx );
}

/**********************************************************************/
