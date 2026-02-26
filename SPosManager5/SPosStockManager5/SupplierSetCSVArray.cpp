/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "FilterTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "SupplierSetCSVArray.h"
/**********************************************************************/

CSupplierSetCSVRecord::CSupplierSetCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CSupplierSetCSVRecord::ClearRecord()
{
	m_arraySuppNo.RemoveAll();
	m_strName = "";
}

/**********************************************************************/

void CSupplierSetCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	m_strName = csv.GetString (0);
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CSupplierSetCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	int nCount = csv.GetInt(2);
	int nOffset = 3;

	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
		m_arraySuppNo.Add ( csv.GetInt(nOffset++) );
}

/**********************************************************************/
	
void CSupplierSetCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strName );
	csv.Add ( SUPPLIERSET_VERSION );				
	csv.Add ( m_arraySuppNo.GetSize() );

	for ( int nIndex = 0; nIndex < m_arraySuppNo.GetSize(); nIndex++ )
		csv.Add ( m_arraySuppNo.GetAt ( nIndex ) );
}

/**********************************************************************/

int CSupplierSetCSVRecord::GetSuppNo ( int nIndex )
{
	if ( nIndex >= 0 && nIndex < m_arraySuppNo.GetSize() )
		return m_arraySuppNo.GetAt ( nIndex );

	return 0;
}

/**********************************************************************/

void CSupplierSetCSVRecord::GetSupplierFlags ( CByteArray& SupplierFlags )
{
	CConsolidateArray ConsolArray;
	
	//ALLOW FOR THE UNSPECIFIED SUPPLIER
	SupplierFlags.Add(0);
	ConsolArray.Add ( "00000000" );

	CString strLabel;
		
	for ( int nSuppIdx = 0; nSuppIdx < DataManager.Supplier.GetSize(); nSuppIdx++ )
	{
		strLabel.Format ( "%4.4d%4.4d", DataManager.Supplier.GetSuppNo ( nSuppIdx ), nSuppIdx + 1 );
		ConsolArray.Add ( strLabel );
		SupplierFlags.Add(0);
	}
	
	for ( int nIndex = 0; nIndex < GetSupplierCount(); nIndex++ )
	{
		strLabel.Format ( "%4.4d", GetSuppNo ( nIndex ) );
		ConsolArray.Add ( strLabel );
	}

	CString strSuppNo = "XXXX";

	for ( int nIndex = 0; nIndex < ConsolArray.GetSize(); nIndex++ )
	{
		CString strLabel = ConsolArray.GetLabel ( nIndex );
		
		if ( strLabel.GetLength() == 4 )
			strSuppNo = strLabel;
		else if ( strLabel.GetLength() == 8 )
		{
			if ( strSuppNo == strLabel.Left(4) )
			{
				int nIndex = atoi ( strLabel.Right(4) );

				if ( nIndex >= 0 && nIndex < SupplierFlags.GetSize() )
					SupplierFlags.SetAt ( nIndex, 1 );
			}
			strSuppNo = "XXXX";
		}
	}
}

/**********************************************************************/

void CSupplierSetCSVRecord::PurgeSupplier( int nSuppNo )
{
	for ( int nIndex = GetSupplierCount() - 1; nIndex >= 0; nIndex-- )
	{
		if ( m_arraySuppNo.GetAt ( nIndex ) == nSuppNo )
			m_arraySuppNo.RemoveAt ( nIndex );
	}
}

/**********************************************************************/

CSupplierSetCSVArray::CSupplierSetCSVArray() : CSharedCSVArray()
{
}

/**********************************************************************/

const char* CSupplierSetCSVArray::GetName ( int nIndex )
{
	m_strName = "";
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		m_strName = csv.GetString ( 0 );
	}

	return m_strName;
}

/**********************************************************************/

int CSupplierSetCSVArray::CreateSuppSelectArray ( int nSupplierFilter, CWordArray& SuppSelectArray )
{
	SuppSelectArray.RemoveAll();
	int nSupplierSet = nSupplierFilter - FIRST_SUPPLIERSET_FILTER;
	bool bError = FALSE;

	//UNSPECIFIED SUPPLIER
	if ( nSupplierFilter == -1 )
		SuppSelectArray.Add(0);
	//ALL SUPPLIERS
	else if ( nSupplierFilter == 0 )
	{
		for ( int nIndex = 0; nIndex <= DataManager.Supplier.GetSize(); nIndex++ )
			SuppSelectArray.Add(nIndex);
	}
	//SUPPLIER BY NUMBER
	else if ( nSupplierFilter >= 1 && nSupplierFilter <= MAX_SYSTEM_SUPPLIERS )
	{
		int nSuppIdx;
		if ( DataManager.Supplier.FindSupplierByNumber ( nSupplierFilter, nSuppIdx ) == TRUE )
			SuppSelectArray.Add(nSuppIdx+1);
		else
			bError = TRUE;
	}
	//SUPPLIER SET
	else if ( nSupplierSet >= 0 && nSupplierSet < DataManager.SupplierSet.GetSize() )
	{
		CSupplierSetCSVRecord SupplierSetRecord;
		DataManager.SupplierSet.GetAt ( nSupplierSet, SupplierSetRecord );
		
		CByteArray SupplierFlags;
		SupplierSetRecord.GetSupplierFlags ( SupplierFlags );

		for ( int nIndex = 0; nIndex < SupplierFlags.GetSize(); nIndex++ )
			if ( SupplierFlags.GetAt ( nIndex ) == 1 )
				SuppSelectArray.Add ( nIndex );
	}
	//ERROR
	else
		bError = TRUE;

	if ( bError == TRUE )
		return - 2;

	//SORT ALPHABETICALLY BY SUPPLIER NAME
	if ( SuppSelectArray.GetSize() > 1 )
	{
		CReportConsolidationArray<CSortedStringAndIntItem> SortedArray;
		
		bool bGotUnspecified = FALSE;

		for ( int nIndex = 0; nIndex < SuppSelectArray.GetSize(); nIndex++ )
		{
			int nSuppIdx = SuppSelectArray.GetAt ( nIndex );
			
			if ( nSuppIdx == 0 )
				bGotUnspecified = TRUE;
			else if ( nSuppIdx >= 1 && nSuppIdx <= DataManager.Supplier.GetSize() )
			{
				CSortedStringAndIntItem item;
				item.m_strItem = DataManager.Supplier.GetName ( nSuppIdx - 1 );
				item.m_strItem.MakeUpper();
				item.m_nItem = nSuppIdx;
				SortedArray.Consolidate( item );
			}
		}
			
		SuppSelectArray.RemoveAll();

		for ( int nIndex = 0; nIndex < SortedArray.GetSize(); nIndex++ )
		{
			CSortedStringAndIntItem item;
			SortedArray.GetAt( nIndex, item );
			SuppSelectArray.Add ( item.m_nItem );
		}
		
		if ( bGotUnspecified == TRUE )
			SuppSelectArray.InsertAt ( 0, 0, 1 );
	}

	return nSupplierFilter;
}

/**********************************************************************/

void CSupplierSetCSVArray::PurgeSupplier ( int nSuppNo )
{
	StatusProgress.Lock( TRUE, "Updating supplier sets" );
	for ( int nIndex = 0; nIndex < GetSize(); nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, GetSize() );
		CSupplierSetCSVRecord SupplierSet;
		GetAt ( nIndex, SupplierSet );
		SupplierSet.PurgeSupplier ( nSuppNo );
		SetAt ( nIndex, SupplierSet );
	}
	StatusProgress.Unlock();
}

/**********************************************************************/
