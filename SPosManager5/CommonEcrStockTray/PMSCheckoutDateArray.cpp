/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSBookingCSVArray.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PMSCheckOutDateArray.h"
/**********************************************************************/

CPMSCheckoutDateArray::CPMSCheckoutDateArray(void)
{
	m_bGotCheckoutDateArray = FALSE;
}

/**********************************************************************/

void CPMSCheckoutDateArray::InvalidateCheckoutDateArray()
{
	m_arrayActiveID.RemoveAll();
	m_arrayCompleteDate.RemoveAll();
	m_bGotCheckoutDateArray = FALSE;
}

/**********************************************************************/

void CPMSCheckoutDateArray::BuildCheckoutDateArray()
{
	if ( FALSE == m_bGotCheckoutDateArray )
	{
		m_arrayActiveID.RemoveAll();
		m_arrayCompleteDate.RemoveAll();

		StatusProgress.Lock( TRUE, "Building PMS Checkout Table" );

		int nSize = DataManagerNonDb.PMSBooking.GetSize();
		
		for ( int nBookingIdx = 0; nBookingIdx < nSize; nBookingIdx++ )
		{
			StatusProgress.SetPos( nBookingIdx, nSize );

			CPMSBookingCSVRecord BookingRecord;
			DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

			CPMSBookingPartInfo infoPart;
			BookingRecord.GetFinalPartInfo( infoPart );

			switch( BookingRecord.GetBookingStatus() )
			{
			case PMS_BOOKING_STATUS_ACTIVE:
				m_arrayActiveID.Add( BookingRecord.GetBookingId() );
				break;

			case PMS_BOOKING_STATUS_COMPLETE:
				{
					COleDateTime dateCheckout;
					int nDayNumber = infoPart.GetStartDay() + infoPart.GetNights();
					AddCompleteDateEntry( nDayNumber, BookingRecord.GetBookingId() );
				}
				break;
			}
		}

		StatusProgress.Unlock();

		m_bGotCheckoutDateArray = TRUE;
	}
}

/**********************************************************************/

void CPMSCheckoutDateArray::AddCompleteDateEntry( int nDayNumber, int nID )
{
	COleDateTime dateCheckout;
	GetOleDateSince2011( nDayNumber, dateCheckout );

	CString strDate;
	strDate.Format( "%4.4d%2.2d%2.2d",
		dateCheckout.GetYear(),
		dateCheckout.GetMonth(),
		dateCheckout.GetDay() );

	CString strBookingId;
	strBookingId.Format( "%d", nID );

	CSortedStringByString item;
	item.m_strKey = strDate;

	int nInsertPos;
	if ( m_arrayCompleteDate.Find( item, nInsertPos ) == FALSE )
	{
		item.m_strData = strBookingId;
		m_arrayCompleteDate.InsertAt( nInsertPos, item );
	}
	else
	{
		m_arrayCompleteDate.GetAt( nInsertPos, item );
		item.m_strData += ",";
		item.m_strData += strBookingId;
		m_arrayCompleteDate.SetAt( nInsertPos, item );
	}
}

/**********************************************************************/

int CPMSCheckoutDateArray::GetActiveID( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayActiveID.GetSize() ) )
		return m_arrayActiveID.GetAt( nIdx );
	else
		return 0;
}

/**********************************************************************/

void CPMSCheckoutDateArray::GetCompleteDateArrayRange( CString& strStartDay, CString& strEndDay, int& nStartIdx, int& nEndIdx )
{
	CSortedStringByString item;

	item.m_strKey = strStartDay;
	m_arrayCompleteDate.Find( item, nStartIdx );

	item.m_strKey = strEndDay;
	if ( m_arrayCompleteDate.Find( item, nEndIdx ) == FALSE )
		nEndIdx--;
}

/**********************************************************************/

void CPMSCheckoutDateArray::GetCompleteDateArrayDate( int nIdx, CString& str )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayCompleteDate.GetSize() ) )
	{
		CSortedStringByString item;
		m_arrayCompleteDate.GetAt( nIdx, item );
		str = item.m_strKey;
	}
	else
		str = "";
}

/**********************************************************************/

void CPMSCheckoutDateArray::GetCompleteDateArrayIDs( int nIdx, CString& str )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayCompleteDate.GetSize() ) )
	{
		CSortedStringByString item;
		m_arrayCompleteDate.GetAt( nIdx, item );
		str = item.m_strData;
	}
	else
		str = "";
}

/**********************************************************************/
#endif
/**********************************************************************/

