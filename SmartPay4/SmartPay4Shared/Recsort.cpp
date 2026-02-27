#include "RecSort.h"

//**********************************************************************
// put lines with matching labels after NOT BEFORE matching similar labels

CRecordSorter::CRecordSorter()
{
}

//**********************************************************************

int CRecordSorter::GetPercentPosition ( int nIndex )
{
	return  ( nIndex * 100L ) / m_arrayData.GetSize();
}

//**********************************************************************

void CRecordSorter::Add ( const char* szLabel, int nRecNo )
{
	CString strLabIn = szLabel;

	CCSV csvNew;
	csvNew.Add ( strLabIn );
	csvNew.Add ( nRecNo );

	int nIndex = 0;
	int nLow = 0;
	int nLowest = m_arrayData.GetSize();
	int nHigh = nLowest - 1;
	CString strLabel;

	while ( nLow <= nHigh )
	{
		int nMid = ( nLow + nHigh ) / 2;

		CCSV csvArray ( m_arrayData.GetAt(nMid) );
		csvArray.Get ( 0, strLabel);				// date from array

		if ( strLabIn < strLabel )
			nHigh = nMid - 1;						// between low mid point
		else
		{
			if ( strLabIn > strLabel )				// between mid & high point
				nLow = nMid + 1;
			else									// the same
			{
				nLow = nMid + 1;
				if ( nMid > nLowest )
					nLowest = nMid;					// save lowest value 
			}
		}

		if ( nLowest < m_arrayData.GetSize() )	nIndex = nLowest;	// lowest has been modified
		else									nIndex = nLow;
	}

	if ( nIndex < m_arrayData.GetSize() )	m_arrayData.InsertAt ( nIndex, csvNew.GetLine() );	// insert in middle
	else									m_arrayData.Add ( csvNew.GetLine() );				// add onto end
}

//*******************************************************************
// similar to add but consolidates if found otherwise added to array

void CRecordSorter::Consolidate ( const char* szLabel, int nRecNo )
{
	CString strLabIn = szLabel;

	CCSV csvNew;
	csvNew.Add ( strLabIn );
	csvNew.Add ( nRecNo );

	int nIndex = 0;
	int nLow = 0;
	int nLowest = m_arrayData.GetSize();
	int nHigh = nLowest - 1;
	CString strLabel;

	while ( nLow <= nHigh )
	{
		int nMid = ( nLow + nHigh ) / 2;

		CCSV csvArray ( m_arrayData.GetAt(nMid) );
		csvArray.Get ( 0, strLabel);				// date from array

		if ( strLabIn < strLabel )
			nHigh = nMid - 1;						// between low mid point
		else
		{
			if ( strLabIn > strLabel )				// between mid & high point
				nLow = nMid + 1;
			else									//  found it
			{
				int nArrayValue;
				csvArray.Get ( 1, nArrayValue );	// get current value from array

				CCSV csv;
				csv.Add ( strLabel );
				csv.Add ( nArrayValue += nRecNo );			// consolidate with new value

				m_arrayData.SetAt ( nMid, csv.GetLine() );	// save in array data
				return;										// consolidate so exit
			}
		}

		if ( nLowest < m_arrayData.GetSize() )	nIndex = nLowest;	// lowest has been modified 
		else									nIndex = nLow;
	}

	if ( nIndex < m_arrayData.GetSize() )	m_arrayData.InsertAt ( nIndex, csvNew.GetLine() );	// insert in middle
	else									m_arrayData.Add ( csvNew.GetLine() );				// add onto end
}

//*******************************************************************
// similar to consolidates overwrites last entry if found otherwise added to array

void CRecordSorter::Update ( const char* szLabel, int nRecNo, bool bModifyIfFound )
{
	CString strLabIn = szLabel;

	CCSV csvNew;
	csvNew.Add ( strLabIn );
	csvNew.Add ( nRecNo );

	int nIndex = 0;
	int nLow = 0;
	int nLowest = m_arrayData.GetSize();
	int nHigh = nLowest - 1;
	CString strLabel;

	while ( nLow <= nHigh )
	{
		int nMid = ( nLow + nHigh ) / 2;

		CCSV csvArray ( m_arrayData.GetAt(nMid) );
		csvArray.Get ( 0, strLabel);				// date from array

		if ( strLabIn < strLabel )
			nHigh = nMid - 1;						// between low mid point
		else
		{
			if ( strLabIn > strLabel )				// between mid & high point
				nLow = nMid + 1;
			else									//  found it
			{
				if ( bModifyIfFound == TRUE )						// allow overwrite if record pointer not been set
					m_arrayData.SetAt ( nMid, csvNew.GetLine() );	// save in array data
				return;												// overwtitten - so exit
			}
		}

		if ( nLowest < m_arrayData.GetSize() )	nIndex = nLowest;	// lowest has been modified 
		else									nIndex = nLow;
	}

// not found

	if ( nIndex < m_arrayData.GetSize() )		m_arrayData.InsertAt ( nIndex, csvNew.GetLine() );	// insert in middle
	else										m_arrayData.Add ( csvNew.GetLine() );				// add onto end
}

//***************************************************************************

CString CRecordSorter::GetLabel ( int nIndex )
{
	CString strLabel = "";

	if ( nIndex < m_arrayData.GetSize() )
	{
		CCSV csv ( m_arrayData.GetAt(nIndex) );
		strLabel = csv.GetString ( 0 );					// return label
	}

	return strLabel;
}

//***************************************************************************

int CRecordSorter::GetRecordNo ( int nIndex )
{
	if ( nIndex < m_arrayData.GetSize() )
	{
		CCSV csv ( m_arrayData.GetAt(nIndex) );
		return csv.GetInt ( 1 );						// return record number
	}

	return -1;
}

/**********************************************************************/
