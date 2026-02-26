/**********************************************************************/
#include "EcrmanSupplierCSVArray.h"
/**********************************************************************/

CEcrmanSupplierCSVRecord::CEcrmanSupplierCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CEcrmanSupplierCSVRecord::ClearRecord()
{
	m_nSuppNo = 1;
	m_strSuppName = "";
}

/**********************************************************************/

void CEcrmanSupplierCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nSuppNo );
	csv.Add ( ECRMANSUPPLIER_VERSION_NO );
	csv.Add ( m_strSuppName );
}

/**********************************************************************/

void CEcrmanSupplierCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetSuppNo ( csv.GetInt(0) );
	switch ( csv.GetInt(1) )			/* version number */
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CEcrmanSupplierCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetSuppName( csv.GetString(2) );
}
	
/**********************************************************************/

const char* CEcrmanSupplierCSVRecord::GetDefaultName()
{
	m_strDefaultName.Format ( "Supplier %d", m_nSuppNo );
	return m_strDefaultName;
}

/**********************************************************************/
/*	Set Routines											*/
/**********************************************************************/

void CEcrmanSupplierCSVRecord::SetSuppNo ( int nSuppNo )
{
	Set ( m_nSuppNo, nSuppNo, EcrmanSupplier::SupplierNo );
}

/**********************************************************************/

void CEcrmanSupplierCSVRecord::SetSuppName ( const char* szName )
{
	CString strName = szName;
	if ( strName == "" ) strName = GetDefaultName();

	Set ( m_strSuppName, strName, EcrmanSupplier::SupplierName );
}

/**********************************************************************/

CEcrmanSupplierCSVArray::CEcrmanSupplierCSVArray() : CSharedCSVArray()
{
}

/**********************************************************************/

void CEcrmanSupplierCSVArray::RemoveAll()
{
	while ( GetSize() > 0 )
		RemoveAt( GetSize() - 1 );
}

/**********************************************************************/

int CEcrmanSupplierCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	int nSuppIdx = 0;
	int nLastSuppNo = 0;
	for ( nSuppIdx = 0; nSuppIdx < GetSize(); nSuppIdx++ )
	{
		CEcrmanSupplierCSVRecord Supplier;
		GetAt( nSuppIdx, Supplier );
		
		int nSuppNo = Supplier.GetSuppNo();
		if ( nSuppNo <= nLastSuppNo || nSuppNo > EcrmanSupplier::SupplierNo.Max )
			break;
	
		nLastSuppNo = nSuppNo;
	}

	while ( nSuppIdx < GetSize() )
		RemoveAt( nSuppIdx );

	return nReply;
}

/**********************************************************************/

bool CEcrmanSupplierCSVArray::FindSupplierByNumber ( int nSuppNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	CEcrmanSupplierCSVRecord Record;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;

		GetAt ( nIndex, Record );
		int nArraySuppNo = Record.GetSuppNo();

		if ( nArraySuppNo < nSuppNo )
			nStart = nIndex + 1;

		if ( nArraySuppNo > nSuppNo )
			nEnd = nIndex - 1;

		if ( nArraySuppNo == nSuppNo )
			return TRUE;
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

int CEcrmanSupplierCSVArray::GetNextFreeNumber()
{
	for ( int n = 0; n < GetSize(); n++ )
	{
		CEcrmanSupplierCSVRecord Supplier;
		GetAt( n, Supplier );

		if ( Supplier.GetSuppNo() > n + 1 )
			return n + 1;
	}

	if ( GetSize() < EcrmanSupplier::SupplierNo.Max )
		return GetSize() + 1;

	return -1;
}

/**********************************************************************/

void CEcrmanSupplierCSVArray::CopyFrom( CEcrmanSupplierCSVArray& Source )
{
	RemoveAll();

	for ( int n = 0; n < Source.GetSize(); n++ )
	{
		CEcrmanSupplierCSVRecord Supplier;
		Source.GetAt( n, Supplier );
		Add( Supplier );
	}

}

/**********************************************************************/
