/**********************************************************************/
 
/**********************************************************************/
#include "BarcodeCSVArray.h"
//#include "DataManager.h"
//#include "globalfunctions.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PluBarcodeArray.h"
/**********************************************************************/

CPluBarcodeRecord::CPluBarcodeRecord()
{
	m_nPluNo = 0;
}

/**********************************************************************/

void CPluBarcodeRecord::ConvertToCSV ( CCSV& csv )
{
	int nSize = m_arrayBarcodeNo.GetSize();
	
	CString strPluNo;
	strPluNo.Format( "%I64d", m_nPluNo );

	csv.Add ( strPluNo );
	csv.Add ( nSize );

	for ( int nIdx = 0; nIdx < nSize; nIdx++ )
	{
		CString strBarcodeNo;
		strBarcodeNo.Format( "%I64d", m_arrayBarcodeNo.GetAt(nIdx) );
		csv.Add ( strBarcodeNo );
	}
}

/**********************************************************************/

void CPluBarcodeRecord::ConvertFromCSV ( CCSV& csv )
{
	m_arrayBarcodeNo.RemoveAll();
	
	SetPluNo( _atoi64 ( csv.GetString ( 0 ) ) );
	int nCount = csv.GetInt ( 1 );
	
	int nOffset = 2;
	for ( int nIdx = 0; nIdx < nCount; nIdx++ )
		m_arrayBarcodeNo.Add ( _atoi64 ( csv.GetString ( nOffset++ ) ) );
}
	
/**********************************************************************/

__int64 CPluBarcodeRecord::GetBarcodeNo ( int nListPos )
{
	if ( nListPos >= 0 && nListPos < m_arrayBarcodeNo.GetSize() )
		return m_arrayBarcodeNo.GetAt ( nListPos );
	else
		return 0;
}

/**********************************************************************/

bool CPluBarcodeRecord::AddBarcodeNo ( __int64 nBarcodeNo )
{
	for ( int nIdx = 0; nIdx < m_arrayBarcodeNo.GetSize(); nIdx++ )
	{
		__int64 nArrayBarcodeNo = m_arrayBarcodeNo.GetAt ( nIdx );

		if ( nBarcodeNo == nArrayBarcodeNo )
			return FALSE;

		if ( nBarcodeNo < nArrayBarcodeNo )
		{
			m_arrayBarcodeNo.InsertAt ( nIdx, nBarcodeNo );
			return TRUE;
		}
	}
	m_arrayBarcodeNo.Add ( nBarcodeNo );
	return TRUE;
}

/**********************************************************************/

bool CPluBarcodeRecord::RemoveBarcodeNo ( __int64 nBarcodeNo )
{
	for ( int nIdx = 0; nIdx < m_arrayBarcodeNo.GetSize(); nIdx++ )
	{
		__int64 nArrayBarcodeNo = m_arrayBarcodeNo.GetAt ( nIdx );

		if ( nBarcodeNo == nArrayBarcodeNo )
		{
			m_arrayBarcodeNo.RemoveAt ( nIdx );
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/

bool CPluBarcodeArray::FindPluNo ( __int64 nPluNo, int& nIndex ) 
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	CPluBarcodeRecord Record;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;

		__int64 nArrayPluNo = GetPluNo ( nIndex );
		
		if ( nArrayPluNo < nPluNo )
			nStart = nIndex + 1;

		if ( nArrayPluNo > nPluNo )
			nEnd = nIndex - 1;

		if ( nArrayPluNo == nPluNo )
			return TRUE;
	}
		
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

__int64 CPluBarcodeArray::GetPluNo ( int nIndex )
{
	CString strTemp;

	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		return _atoi64( csv.GetString(0) );
	}
	return 0;
}

/**********************************************************************/

int CPluBarcodeArray::GetBarcodeNoCount ( int nIndex )
{
	CPluBarcodeRecord Record;
	GetAt ( nIndex, Record );
	return Record.GetBarcodeNoCount();
}

/**********************************************************************/

__int64 CPluBarcodeArray::GetBarcodeNo ( int nIndex, int nListPos )
{
	CPluBarcodeRecord Record;
	GetAt ( nIndex, Record );
	return Record.GetBarcodeNo ( nListPos );
}

/**********************************************************************/

bool CPluBarcodeArray::AddLink ( __int64 nPluNo, __int64 nBarcodeNo )
{
	int nIndex;
	if ( FindPluNo ( nPluNo, nIndex ) == TRUE )
	{
		CPluBarcodeRecord Record;
		GetAt ( nIndex, Record );
		bool bResult = Record.AddBarcodeNo ( nBarcodeNo );

		if ( bResult == TRUE )
			SetAt ( nIndex, Record );

		return bResult;
	}
	else
	{
		CPluBarcodeRecord Record;
		Record.SetPluNo ( nPluNo );
		Record.AddBarcodeNo ( nBarcodeNo );
		InsertAt ( nIndex, Record );
		return TRUE;
	}
}

/**********************************************************************/

bool CPluBarcodeArray::RemoveLink ( __int64 nPluNo, __int64 nBarcodeNo )
{
	int nIndex;
	if ( FindPluNo ( nPluNo, nIndex ) == FALSE )
		return FALSE;
	else
	{
		CPluBarcodeRecord Record;
		GetAt ( nIndex, Record );
		bool bResult = Record.RemoveBarcodeNo ( nBarcodeNo );

		if ( bResult == TRUE )
			SetAt ( nIndex, Record );

		return bResult;
	}
}

/**********************************************************************/

void CPluBarcodeArray::RemoveAll()
{
	RemoveAt ( 0, GetSize() );
}

/**********************************************************************/

void CPluBarcodeArray::Rebuild()
{
	StatusProgress.Lock( TRUE, "Building barcode table" );
	RebuildInternal();
	StatusProgress.Unlock();
}

/**********************************************************************/

void CPluBarcodeArray::RebuildInternal()
{
	RemoveAll();
	int nSize = DataManager.Barcode.GetSize();

	for ( int nBarcodeIdx = 0; nBarcodeIdx < nSize; nBarcodeIdx++ )
	{
		StatusProgress.SetPos ( nBarcodeIdx, nSize ); 

		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt ( nBarcodeIdx, BarcodeRecord );
		__int64 nPluNo = BarcodeRecord.GetPluNoInt();
		__int64 nBarcodeNo = BarcodeRecord.GetBarcodeNoInt();
		AddLink ( nPluNo, nBarcodeNo );
	}
}

/**********************************************************************/
