/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSBookingCSVArray.h"
/**********************************************************************/
#include "PMSZeroNightStayArray.h"
/**********************************************************************/

CPMSZeroNightStayArray::CPMSZeroNightStayArray(void)
{
}

/**********************************************************************/

void CPMSZeroNightStayArray::RemoveAll()
{
	m_arrayZeroNightStay.RemoveAll();
}

/**********************************************************************/

void CPMSZeroNightStayArray::AddZeroNightStayEntry( int nRoomNo, int nDayNumber, int nID, int nPart )
{
	CString strBookingId;
	strBookingId.Format( "%d", nID );

	CSortedStringByString item;
	item.m_strKey.Format( "%4.4d%6.6d",
		nRoomNo,
		nDayNumber );

	int nInsertPos;
	if ( m_arrayZeroNightStay.Find( item, nInsertPos ) == FALSE )
	{
		CCSV csv;
		csv.Add( nID );
		csv.Add( nPart );
		item.m_strData = csv.GetLine();

		m_arrayZeroNightStay.InsertAt( nInsertPos, item );
	}
	else
	{
		m_arrayZeroNightStay.GetAt( nInsertPos, item );
		CCSV csv( item.m_strData );

		bool bGotID = FALSE;
		for ( int n = 0; n < csv.GetSize(); n += 2 )
		{
			if ( ( csv.GetInt(n) == nID ) && ( csv.GetInt(n+1) == nPart ) )
			{
				bGotID = TRUE;
				break;
			}
		}

		if ( FALSE == bGotID )
		{
			csv.Add( nID );
			csv.Add( nPart );
			item.m_strData = csv.GetLine();
			m_arrayZeroNightStay.SetAt( nInsertPos, item );
		}
	}
}

/**********************************************************************/

void CPMSZeroNightStayArray::GetZeroNightStayArrayRange( int nRoomNo, int nStartDay, int nEndDay, int& nStartIdx, int& nEndIdx )
{
	CSortedStringByString itemStart;
	itemStart.m_strKey.Format( "%4.4d%6.6d", nRoomNo, nStartDay );

	CSortedStringByString itemEnd;
	itemEnd.m_strKey.Format( "%4.4d%6.6d", nRoomNo, nEndDay );

	m_arrayZeroNightStay.Find( itemStart, nStartIdx );

	if ( m_arrayZeroNightStay.Find( itemEnd, nEndIdx ) == FALSE )
		nEndIdx--;
}

/**********************************************************************/

void CPMSZeroNightStayArray::GetZeroNightStayArrayRoom( int nIdx, CString& str )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayZeroNightStay.GetSize() ) )
	{
		CSortedStringByString item;
		m_arrayZeroNightStay.GetAt( nIdx, item );
		str = item.m_strKey;
		str = str.Left(4);
	}
	else
		str = "";
}

/**********************************************************************/

void CPMSZeroNightStayArray::GetZeroNightStayArrayDay( int nIdx, CString& str )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayZeroNightStay.GetSize() ) )
	{
		CSortedStringByString item;
		m_arrayZeroNightStay.GetAt( nIdx, item );
		str = item.m_strKey;
		str = str.Right(6);
	}
	else
		str = "";
}

/**********************************************************************/

void CPMSZeroNightStayArray::GetZeroNightStayArrayIDs( int nIdx, CString& str )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayZeroNightStay.GetSize() ) )
	{
		CSortedStringByString item;
		m_arrayZeroNightStay.GetAt( nIdx, item );
		str = item.m_strData;
	}
	else
		str = "";
}

/**********************************************************************/
#endif
/**********************************************************************/
