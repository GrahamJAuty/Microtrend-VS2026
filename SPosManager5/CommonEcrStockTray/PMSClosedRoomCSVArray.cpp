/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSCalendarTable.h"
#include "PMSRoomCSVArray.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PMSClosedRoomCSVArray.h"
/**********************************************************************/

CPMSClosedRoomCSVRecord::CPMSClosedRoomCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CPMSClosedRoomCSVRecord::ClearRecord()
{
	m_nClosureId = 1;
	m_nRoomNo = 1;
	m_nStartDay = 0;
	m_nNights = 1;
	m_strReason = "";
}

/**********************************************************************/

void CPMSClosedRoomCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetClosureId( csv.GetInt(0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CPMSClosedRoomCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetRoomNo		( csv.GetInt(2) );
	SetStartDay		( csv.GetInt(3) );
	SetNights		( csv.GetInt(4) );
	SetReason		( csv.GetString(5) );
}

/**********************************************************************/
	
void CPMSClosedRoomCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nClosureId );
	csv.Add ( PMS_CLOSEDROOM_VERSION );
	csv.Add ( m_nRoomNo );
	csv.Add ( m_nStartDay );
	csv.Add ( m_nNights );
	csv.Add ( m_strReason );
}

/**********************************************************************/

void CPMSClosedRoomCSVRecord::SetClosureId( int n )
{
	Set ( m_nClosureId, n, PMSClosure::ClosureId );
}

/**********************************************************************/

void CPMSClosedRoomCSVRecord::SetRoomNo( int n )
{
	Set ( m_nRoomNo, n, PMSRoom::RoomNo );
}

/**********************************************************************/

void CPMSClosedRoomCSVRecord::SetStartDay( int n )
{
	Set ( m_nStartDay, n, PMSClosure::StartDay );
}

/**********************************************************************/

void CPMSClosedRoomCSVRecord::SetNights( int n )
{
	Set ( m_nNights, n, PMSClosure::Nights );
}

/**********************************************************************/

void CPMSClosedRoomCSVRecord::SetReason( const char* sz )
{
	Set ( m_strReason, sz, PMSClosure::Reason );
}

/**********************************************************************/

CPMSClosedRoomCSVArray::CPMSClosedRoomCSVArray() 
{
}

/**********************************************************************/

CPMSClosedRoomCSVArray::~CPMSClosedRoomCSVArray() 
{
}

/**********************************************************************/

int CPMSClosedRoomCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}
	
	return nReply;
}

/**********************************************************************/

int CPMSClosedRoomCSVArray::GetClosureId ( int nIdx )
{
	int nId = 1;

	CString strTemp;
	if ( ( strTemp = GetAt ( nIdx ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		nId = csv.GetInt(0);
	}

	return nId;
}

/**********************************************************************/

bool CPMSClosedRoomCSVArray::FindClosureById( int nId, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;

		int nArrayId = GetClosureId( nIndex );

		if ( nId < nArrayId )
			nEnd = nIndex - 1;
		else if ( nId > nArrayId )
			nStart = nIndex + 1;
		else
			return TRUE;
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

int CPMSClosedRoomCSVArray::FindFirstFreeId()
{
	int nResult = 0;

	int nSize = GetSize();
	if ( 0 == nSize )
		nResult = 1;
	else
	{
		int nId = GetClosureId( nSize - 1 );
		nResult = nId + 1;
	}

	if ( nResult < PMSClosure::ClosureId.Min || nResult > PMSClosure::ClosureId.Max )
		nResult = 0;

	return nResult;
}

/**********************************************************************/

void CPMSClosedRoomCSVArray::InsertClosure( CPMSClosedRoomCSVRecord& Closure )
{
	int nPos;
	if ( FindClosureById( Closure.GetClosureId(), nPos ) == TRUE )
		SetAt( nPos, Closure );
	else
		InsertAt( nPos, Closure );
}

/**********************************************************************/

void CPMSClosedRoomCSVArray::RemoveClosureById( int nClosureId )
{
	int nClosureIdx;
	if ( FindClosureById( nClosureId, nClosureIdx ) == TRUE )
		RemoveAt( nClosureIdx );
}

/**********************************************************************/

void CPMSClosedRoomCSVArray::BuildInternalTables()
{
	StatusProgress.Lock( TRUE, "Building PMS Calendar" );

	int nSize = GetSize();
	for ( int nClosureIdx = 0; nClosureIdx < nSize; nClosureIdx++ )
	{
		StatusProgress.SetPos( nClosureIdx, nSize );

		CPMSClosedRoomCSVRecord Closure;
		GetAt( nClosureIdx, Closure );

		PMSCalendarTable.ApplyClosure( Closure );
	}

	StatusProgress.Unlock();
}

/**********************************************************************/
#endif
/**********************************************************************/
