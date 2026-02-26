/**********************************************************************/
 
/**********************************************************************/
#include "PaidInOutTextCSVArray.h"
/**********************************************************************/

CPaidInOutTextCSVArray::CPaidInOutTextCSVArray() : CSharedCSVArray()
{
	m_bGotFile = FALSE;
}

/**********************************************************************/

void CPaidInOutTextCSVArray::RemoveAll()
{
	while ( GetSize() > 0 )
		RemoveAt( GetSize() - 1 );
}

/**********************************************************************/

int CPaidInOutTextCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	FillArray();

	while ( GetSize() > PAIDIO_TEXT_COUNT )
		RemoveAt( GetSize() - 1 );
	
	return nReply;
}

/**********************************************************************/

void CPaidInOutTextCSVArray::FillArray()
{
	CPaidInOutTextCSVRecord Record;
		
	while ( GetSize() < PAIDIO_TEXT_COUNT )
		Add( Record );
}

/**********************************************************************/

void CPaidInOutTextCSVArray::GetPaidInOutText( int nNo, CPaidInOutTextCSVRecord& Record )
{
	int nIndex = nNo - 1;

	if ( nIndex >= 0 && nIndex < GetSize() )
		GetAt( nIndex, Record );
	else	
		Record.SetEposText( "" );
}

/**********************************************************************/

void CPaidInOutTextCSVArray::GetReportPaidInOutText( int nNo, CPaidInOutTextCSVRecord& Record )
{
	Record.SetEposText( "" );

	int nIndex = nNo - 1;
	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
		GetAt( nIndex, Record );
	
	CString strText = Record.GetEposText();
	if ( strText == "" )
	{
		strText.Format( "Reason %d", nNo );
		Record.SetEposText( strText );
	}
}

/**********************************************************************/

void CPaidInOutTextCSVArray::SetPaidInOutText( int nNo, CPaidInOutTextCSVRecord& Record )
{
	int nIndex = nNo - 1;

	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
		SetAt( nIndex, Record );
}

/**********************************************************************/
