/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
#include "ImportDefines.h"
/**********************************************************************/
#include "SupplierCSVArray.h"
/**********************************************************************/

CSupplierCSVArray::CSupplierCSVArray() : CSharedCSVArray( ',', '"', FALSE, TRUE, TRUE )
{
	m_bIsValid = TRUE;
	m_nErrorType = 0;
	m_nErrorLine = 0;

	for ( int nIndex = 0; nIndex <= Supplier::SupplierNo.Max; nIndex++ )
		m_arraySuppNoTable.Add ( 0 );
}

/**********************************************************************/

void CSupplierCSVArray::RemoveAll()
{
	for ( int nSuppIdx = GetSize() - 1; nSuppIdx >= 0; nSuppIdx-- )
		RemoveAt ( nSuppIdx );

	for ( int nIndex = 0; nIndex < m_arraySuppNoTable.GetSize(); nIndex++ )
		m_arraySuppNoTable.SetAt ( nIndex, 0 );
}

/**********************************************************************/

int CSupplierCSVArray::GetSuppNo ( int nSuppIdx )
{
	if ( nSuppIdx < 0 || nSuppIdx >= GetSize() )
		return 0;

	CSupplierCSVRecord SupplierRecord;
	GetAt ( nSuppIdx, SupplierRecord );
	return SupplierRecord.GetSuppNo();
}

/**********************************************************************/

const char* CSupplierCSVArray::GetKey ( int nSuppIdx )
{
	m_strTemp = "";
	if ( nSuppIdx >= 0 && nSuppIdx < GetSize() )
	{
		CSupplierCSVRecord SupplierRecord;
		GetAt ( nSuppIdx, SupplierRecord );
		m_strTemp = SupplierRecord.GetKey();
	}
	return m_strTemp;
}

/**********************************************************************/

const char* CSupplierCSVArray::GetName ( int nSuppIdx )
{
	m_strTemp = "";
	if ( nSuppIdx >= 0 && nSuppIdx < GetSize() )
	{
		CSupplierCSVRecord SupplierRecord;
		GetAt ( nSuppIdx, SupplierRecord );
		m_strTemp = SupplierRecord.GetName();
	}
	return m_strTemp;
}

/**********************************************************************/

bool CSupplierCSVArray::FindSupplierByNumber( int nSuppNo, int& nIndex )
{
	if ( nSuppNo <= 0 || nSuppNo >= m_arraySuppNoTable.GetSize() )
	{
		nIndex = -1;
		return FALSE;
	}
		
	nIndex = m_arraySuppNoTable.GetAt ( nSuppNo ) - 1;
	
	if ( nIndex < 0 || nIndex >= GetSize() )
	{
		nIndex = -1;
		return FALSE;
	}
	
	return TRUE;
}

/**********************************************************************/

bool CSupplierCSVArray::FindSupplierByKey( const char* szKey, int& nIndex )
{
	CString strKeyToFind = szKey;

	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		CString strArrayKey = GetKey ( nIndex );
		
		if ( strArrayKey < strKeyToFind )
			nStart = nIndex + 1;

		if ( strArrayKey > strKeyToFind )
			nEnd = nIndex - 1;

		if ( strArrayKey == strKeyToFind )
			return TRUE;
	}
		
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

int CSupplierCSVArray::AddSupplier( CSupplierCSVRecord& SupplierRecord )
{
	CString strKey = SupplierRecord.GetKey();
	int nSuppNo = SupplierRecord.GetSuppNo();

	//ERROR IF SUPPLIER NUMBER IS OUT OF RANGE
	if ( nSuppNo <= 0 || nSuppNo >= m_arraySuppNoTable.GetSize() )
		return -1;

	//ERROR IF SUPPLIER NUMBER IS ALREADY IN USE
	if ( m_arraySuppNoTable.GetAt ( nSuppNo ) != 0 )
		return -1;

	int nKeyIndex;
	if ( FindSupplierByKey ( strKey, nKeyIndex ) == FALSE )
	{
		InsertAt ( nKeyIndex, SupplierRecord );
		m_arraySuppNoTable.SetAt ( nSuppNo, nKeyIndex + 1 );
		return ( nKeyIndex );
	}
	
	return -1;
}

/**********************************************************************/

void CSupplierCSVArray::DeleteSupplier ( int nSuppIdx )
{
	if ( nSuppIdx >= 0 && nSuppIdx < GetSize() )
	{
		//MARK THE SUPPLIER NUMBER AS UNUSED
		int nSuppNo = GetSuppNo ( nSuppIdx );
		if ( nSuppNo >= 0 && nSuppNo < m_arraySuppNoTable.GetSize() )
			m_arraySuppNoTable.SetAt ( nSuppNo, 0 );

		RemoveAt ( nSuppIdx );	
	}
}

/**********************************************************************/

int CSupplierCSVArray::FindFreeNumber()
{
	for ( int nSuppNo = Supplier::SupplierNo.Min; nSuppNo <= Supplier::SupplierNo.Max; nSuppNo++ )
		if ( m_arraySuppNoTable.GetAt ( nSuppNo ) == 0 )
			return nSuppNo;

	return 0;
}

/**********************************************************************/

const char* CSupplierCSVArray::GetOrderFilename ( int nSuppIdx ) 
{
	if ( nSuppIdx >= 0 && nSuppIdx < GetSize() )
	{
		CSupplierCSVRecord SupplierRecord;
		GetAt ( nSuppIdx, SupplierRecord );
		m_strOrderFilename = SupplierRecord.GetOrderFilename();
	}
	else
		m_strOrderFilename = "Supp000.dat";

	return m_strOrderFilename;
}

/**********************************************************************/

const char* CSupplierCSVArray::GetAuditFilename ( int nSuppIdx ) 
{
	if ( nSuppIdx >= 0 && nSuppIdx < GetSize() )
	{
		CSupplierCSVRecord SupplierRecord;
		GetAt ( nSuppIdx, SupplierRecord );
		m_strAuditFilename = SupplierRecord.GetAuditFilename();
	} 
	else
		m_strAuditFilename = "SupAd000.dat";

	return m_strAuditFilename;
}

/**********************************************************************/

const char* CSupplierCSVArray::GetTempFilename ( int nSuppIdx ) 
{
	if ( nSuppIdx >= 0 && nSuppIdx < GetSize() )
	{
		CSupplierCSVRecord SupplierRecord;
		GetAt ( nSuppIdx, SupplierRecord );
		m_strTempFilename = SupplierRecord.GetTempFilename();
	}  
	else
		m_strTempFilename = "SupTm000.dat";

	return m_strTempFilename;
}

/**********************************************************************/

int CSupplierCSVArray::ImportLine ( CSSImportFile& import, const char* szImportLine )
{
	CCSV csvImport ( szImportLine );
	CString strKey = csvImport.GetString(0);
	::TrimSpaces ( strKey, FALSE );

	if ( strKey == "" )	
		return IMPORT_LINE_IGNORED;

	int nSuppIdx;
	if ( FindSupplierByKey ( strKey, nSuppIdx ) == TRUE )	
	{
		CSupplierCSVRecord NewRecord;
		GetAt ( nSuppIdx, NewRecord );
		NewRecord.ImportLine ( import, csvImport, FALSE );
		SetAt ( nSuppIdx, NewRecord );
		return IMPORT_LINE_MODIFIED;
	}	
	else			
	{	
		CSupplierCSVRecord NewRecord;
		NewRecord.SetSuppNo ( FindFreeNumber() );

		if ( NewRecord.GetSuppNo() <= 0 )
			return IMPORT_LINE_IGNORED;
		else
		{
			NewRecord.ImportLine ( import, csvImport, FALSE );
			AddSupplier ( NewRecord );
			return IMPORT_LINE_ADDED;
		}
	}
}

/**********************************************************************/

bool CSupplierCSVArray::ValidateSuppliers()
{
	m_bIsValid = TRUE;
	m_nErrorLine = 0;
	m_nErrorType = 0;

	for ( int nIndex = 0; nIndex < m_arraySuppNoTable.GetSize(); nIndex++ )
		m_arraySuppNoTable.SetAt ( nIndex, 0 );

	if ( GetSize() > MAX_SYSTEM_SUPPLIERS )
	{
		m_bIsValid = FALSE;
		m_nErrorType = 1;
		return FALSE;
	}

	CString strLastKey = "";
	for ( int nSuppIdx = 0; nSuppIdx < GetSize(); nSuppIdx++ )
	{
		CSupplierCSVRecord SupplierRecord;
		GetAt ( nSuppIdx, SupplierRecord );
		CString strKey = SupplierRecord.GetKey();
		int nSuppNo = SupplierRecord.GetSuppNo();

		if ( strKey <= strLastKey )
		{
			m_bIsValid = FALSE;
			m_nErrorLine = nSuppIdx;
			m_nErrorType = 2;
			return FALSE;
		}

		strLastKey = strKey;

		if ( nSuppNo <= 0 || nSuppNo >= m_arraySuppNoTable.GetSize() )
		{
			m_bIsValid = FALSE;
			m_nErrorLine = nSuppIdx;
			m_nErrorType = 3;
			return FALSE;
		}

		if ( m_arraySuppNoTable.GetAt ( nSuppNo ) != 0 )
		{
			m_bIsValid = FALSE;
			m_nErrorLine = nSuppIdx;
			m_nErrorType = 4;
			return FALSE;
		}

		m_arraySuppNoTable.SetAt ( nSuppNo, nSuppIdx + 1 );
	}

	return TRUE;
}

/**********************************************************************/

void CSupplierCSVArray::CopyArray ( CSupplierCSVArray& source )
{
	RemoveAll();
	for ( int nIndex = 0; nIndex < source.GetSize(); nIndex++ )
	{
		CSupplierCSVRecord Supplier;
		source.GetAt ( nIndex, Supplier );
		Add ( Supplier );
	}
	ValidateSuppliers();
}

/**********************************************************************/
