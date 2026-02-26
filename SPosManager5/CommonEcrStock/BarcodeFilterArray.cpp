/**********************************************************************/
 
/**********************************************************************/
#include "BarcodeCSVArray.h"
#include "BarcodePluNoTable.h"
//#include "DataManager.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "StatusProgress.h"
#include "SearchTypes.h"
#include "..\SPosStockManager5\StockMonitor.h"
#include "SysInfo.h"
/**********************************************************************/
#include "BarcodeFilterArray.h"
/**********************************************************************/

CBarcodeFilterArray::CBarcodeFilterArray( int nFilterType ) 
{ 
	m_nFilterType = nFilterType;
	m_strDescription = "";
	m_nSearchType = SEARCH_EPOSTEXT_ONLY;
	m_bMatchCase = FALSE;
	m_bAllFlag = TRUE;
	m_arrayBarcodeItemIndex.RemoveAll();
	SetDepartmentFilter( DEPARTMENTSET_ALL );
	m_bEan = FALSE;

#ifdef STOCKMAN_SYSTEM
	m_strStockcode = "";
#endif
} 

/**********************************************************************/

int CBarcodeFilterArray::GetSize()
{
	if ( m_bAllFlag == FALSE )
		return m_arrayBarcodeItemIndex.GetSize();
	else
		return DataManager.Barcode.GetSize();
}

/**********************************************************************/

void CBarcodeFilterArray::SetDescriptionFilter ( const char* szDescription, int nSearchType, bool bMatchCase )
{
	m_strDescription = szDescription;
	m_nSearchType = nSearchType;
	m_bMatchCase = bMatchCase;
}

/**********************************************************************/

void CBarcodeFilterArray::SetDepartmentFilter ( int nDepartmentFilter )
{
	m_nDepartmentFilter = m_DepartmentSet.CreateDepartmentSet ( nDepartmentFilter );
}

/**********************************************************************/

int CBarcodeFilterArray::GetBarcodeItemIndexAt( int nArrayIdx )
{
	if ( nArrayIdx >= GetSize() )
		return -1;

	if ( m_bAllFlag == TRUE )
		return nArrayIdx;
	else
		return m_arrayBarcodeItemIndex.GetAt ( nArrayIdx );
}

/**********************************************************************/

const char* CBarcodeFilterArray::GetDisplayLine ( int nArrayIdx )
{
	m_strRecord ="";
	int nBarcodeIdx;
	
	if ( m_bAllFlag == FALSE )
	{
		if ( nArrayIdx >= m_arrayBarcodeItemIndex.GetSize() )
			return "";

		nBarcodeIdx = m_arrayBarcodeItemIndex.GetAt( nArrayIdx );
	}
	else
		nBarcodeIdx = nArrayIdx;

	GetLine( nBarcodeIdx );
	return m_strRecord;
}

/**********************************************************************/

bool CBarcodeFilterArray::ComparePluDescription( const char* szText, int nArrayIdx, int nSearchType, bool bMatchCase )
{
	CString strSearchText = szText;
	
	if ( bMatchCase == FALSE )
		strSearchText.MakeUpper();

	CBarcodeCSVRecord BarcodeRecord;
	int nBarcodeIdx = GetBarcodeItemIndexAt( nArrayIdx );
	DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );

	int nPluIdx;
	__int64 nPluNo = BarcodeRecord.GetPluNoInt();
	if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == FALSE )
		return FALSE;

	CPluCSVRecord Record;
	DataManager.Plu.GetAt ( nPluIdx, Record );
		
	if ( nSearchType != SEARCH_REPTEXT_ONLY )
	{
		CString strDescription = Record.GetEposText();
			
		if ( bMatchCase == FALSE )
			strDescription.MakeUpper();
		
		if ( strDescription.Find ( strSearchText ) != - 1 )  
			return TRUE;
	}

	if ( nSearchType != SEARCH_EPOSTEXT_ONLY )
	{
		CString strDescription = Record.GetRepText();
			
		if ( bMatchCase == FALSE )
			strDescription.MakeUpper();
		
		if ( strDescription.Find ( strSearchText ) != - 1 )  
			return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CBarcodeFilterArray::FindBarcodeByNumber ( __int64 nBarcodeNo, int& nArrayIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	CBarcodeCSVRecord Record;

	while ( nStart <= nEnd )
	{
		nArrayIdx = ( nStart + nEnd ) / 2;
		int nBarcodeIdx = GetBarcodeItemIndexAt ( nArrayIdx );

		__int64 nArrayBarcodeNo = DataManager.Barcode.GetBarcodeNoInt ( nBarcodeIdx );
		
		if ( nArrayBarcodeNo < nBarcodeNo )
			nStart = nArrayIdx + 1;

		if ( nArrayBarcodeNo > nBarcodeNo )
			nEnd = nArrayIdx - 1;

		if ( nArrayBarcodeNo == nBarcodeNo )
			return TRUE;
	}
		
	nArrayIdx = nStart;
	return FALSE;
}

/**********************************************************************/

bool CBarcodeFilterArray::FindBarcodeByIndex ( int nBarcodeIdx, int& nArrayIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	CBarcodeCSVRecord Record;

	while ( nStart <= nEnd )
	{
		nArrayIdx = ( nStart + nEnd ) / 2;
		int nThisBarcodeIdx = GetBarcodeItemIndexAt ( nArrayIdx );

		if ( nThisBarcodeIdx < nBarcodeIdx )
			nStart = nArrayIdx + 1;

		if ( nThisBarcodeIdx > nBarcodeIdx )
			nEnd = nArrayIdx - 1;

		if ( nThisBarcodeIdx == nBarcodeIdx )
			return TRUE;
	}
		
	nArrayIdx = nStart;
	return FALSE;
}

/**********************************************************************/

bool CBarcodeFilterArray::FindBarcodeByDescription ( const char* szText, int nStartPos, int nSearchType, bool bMatchCase, int& nArrayIdx )
{		
	int nCount = ( GetSize() - nStartPos ) + 1;
	bool bResult = FALSE;

	StatusProgress.Lock( TRUE, "Searching barcode file" );	
	for ( int nOffset = 0; nOffset < nCount; nOffset++ )
	{
		if ( ComparePluDescription ( szText, nStartPos + nOffset, nSearchType, bMatchCase ) == TRUE )
		{	
			nArrayIdx = nStartPos + nOffset;
			bResult = TRUE;
			break;
		}
		StatusProgress.SetPos ( nOffset, nCount );
	}
	StatusProgress.Unlock();
	return bResult;
}

/**********************************************************************/

void CBarcodeFilterArray::BuildRecordList()
{
	CWaitCursor wait;

	if ( BuildFirst() == TRUE )
		while ( BuildNext() == TRUE );
}

/**********************************************************************/

bool CBarcodeFilterArray::BuildFirst()
{
	m_nBuildIndex = 0;
	m_bAllFlag = TRUE;

#ifdef STOCKMAN_SYSTEM

	if ( m_nDepartmentFilter != DEPARTMENTSET_ALL || m_strDescription != "" || m_strStockcode != "" || m_bEan == TRUE ) 
		m_bAllFlag = FALSE;

#else

	if ( m_nDepartmentFilter != DEPARTMENTSET_ALL || m_strDescription != "" || m_bEan == TRUE ) 
		m_bAllFlag = FALSE;

#endif

	if ( m_bAllFlag == TRUE )
		return TRUE;

	m_arrayBarcodeItemIndex.RemoveAll();

	if ( DataManager.Barcode.GetSize() == 0 )
		return FALSE;
	
	return TRUE;
}

/**********************************************************************/
			
bool CBarcodeFilterArray::BuildNext()
{
	if ( m_nBuildIndex >= DataManager.Barcode.GetSize() )
		return FALSE;

	CBarcodeCSVRecord BarcodeRecord;
	DataManager.Barcode.GetAt( m_nBuildIndex, BarcodeRecord );

	if ( m_bEan )
	{
		__int64 nBarcodeNo = BarcodeRecord.GetBarcodeNoInt();
		if ( BarcodePluNoTable.IsValidEan( nBarcodeNo ) == FALSE )
		{
			m_nBuildIndex++;
			return TRUE;
		}
	}

	int nPluIdx;
	__int64 nPluNo = BarcodeRecord.GetPluNoInt();
	if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == FALSE )
	{
		m_nBuildIndex++;
		return TRUE;
	}

	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt ( nPluIdx, PluRecord );

	int nMod = 0;
	if ( EcrmanOptions.GetPluBarcodeModifierFlag() == TRUE )
	{
		nMod = BarcodeRecord.GetModifier();
	}

	if ( m_DepartmentSet.GetDepartmentStatus ( PluRecord.GetModifierDeptNoForReport(nMod) ) == FALSE )
	{
		m_nBuildIndex++;
		return TRUE;
	}

	if ( m_strDescription != "" )
	{
		bool bFoundString = FALSE;
		CString strSearchText = m_strDescription;
	
		if ( m_bMatchCase == FALSE )
			strSearchText.MakeUpper();
		
		if ( m_nSearchType != SEARCH_REPTEXT_ONLY )
		{
			CString strDesc = PluRecord.GetModifierEposText(nMod);
			
			if ( m_bMatchCase == FALSE )
				strDesc.MakeUpper();
		
			if ( strDesc.Find ( strSearchText ) != - 1 )  
				bFoundString = TRUE;
		}

		if ( m_nSearchType != SEARCH_EPOSTEXT_ONLY && bFoundString == FALSE )
		{
			CString strCode = PluRecord.GetRepText();
			
			if ( m_bMatchCase == FALSE )
				strCode.MakeUpper();
		
			if ( strCode.Find ( strSearchText ) != - 1 )   
				bFoundString = TRUE;
		}
	
		if ( bFoundString == FALSE )
		{
			m_nBuildIndex++;
			return TRUE;
		}
	}

#ifdef STOCKMAN_SYSTEM
	if ( m_strStockcode != "" )
	{
		CString strPluStockCode;
		DataManager.Plu.GetPluStockCode ( PluRecord.GetPluNoInt(), strPluStockCode, FALSE );

		if ( strPluStockCode.Find ( m_strStockcode ) == - 1 )  
		{	
			m_nBuildIndex++;
			return TRUE;
		}
	}
#endif

	m_arrayBarcodeItemIndex.Add ( m_nBuildIndex );
	m_nBuildIndex++;
	return TRUE;
}


/**********************************************************************/

void CBarcodeFilterArray::SetArrayIdx( int nArrayIdx )
{
	if ( nArrayIdx >= 0 && nArrayIdx < GetSize() )
		m_nArrayIdx = nArrayIdx;
	else
		m_nArrayIdx = -1;
}

/**********************************************************************/

void CBarcodeFilterArray::Next()
{
	if ( m_nArrayIdx < GetSize() - 1 )
		m_nArrayIdx++;
}

/**********************************************************************/

void CBarcodeFilterArray::Previous()
{
	if ( m_nArrayIdx > 0 )
		m_nArrayIdx--;
}

/**********************************************************************/

bool CBarcodeFilterArray::HasNext()
{
	if ( m_nArrayIdx < GetSize() - 1 )
		return TRUE; 
	else
		return FALSE;
}

/**********************************************************************/

bool CBarcodeFilterArray::HasPrevious()
{
	if ( m_nArrayIdx > 0 )
		return TRUE;
	else
		return FALSE;
}

/**********************************************************************/

void CBarcodeFilterArray::CopySettingsFrom ( CBarcodeFilterArray& Source )
{
	SetDepartmentFilter ( Source.GetDepartmentFilter() );
	m_strDescription = Source.GetDescription();
	m_nSearchType = Source.GetSearchType();
	m_bMatchCase = Source.GetMatchCase();

#ifdef STOCKMAN_SYSTEM
	m_strStockcode = Source.GetStockcodeFilter();
#endif
}

/**********************************************************************/

__int64 CBarcodeFilterArray::GetFirstBarcodeNo()
{
	__int64 nBarcodeNo = 0;

	if ( GetSize() > 0 )
	{
		int nBarcodeIdx = GetBarcodeItemIndexAt ( 0 );
		nBarcodeNo = DataManager.Barcode.GetBarcodeNoInt ( nBarcodeIdx );
	}

	return nBarcodeNo;
}

/**********************************************************************/

__int64 CBarcodeFilterArray::GetLastBarcodeNo()
{
	__int64 nBarcodeNo = 0;

	if ( GetSize() > 0 )
	{
		int nBarcodeIdx = GetBarcodeItemIndexAt ( GetSize() - 1 );
		nBarcodeNo = DataManager.Barcode.GetBarcodeNoInt ( nBarcodeIdx );
	}

	return nBarcodeNo;
}

/**********************************************************************/

void CBarcodeFilterArray::GetDefaultRange ( int nArrayIdx, __int64& nBarcodeFrom, __int64& nBarcodeTo )
{
	if ( nArrayIdx >= 0 && nArrayIdx < GetSize() )
	{
		int nBarcodeIdx = GetBarcodeItemIndexAt ( nArrayIdx );
		nBarcodeFrom = DataManager.Barcode.GetBarcodeNoInt ( nBarcodeIdx ); 
		nBarcodeTo = GetLastBarcodeNo();
	}
	else if ( GetSize() > 0 )
	{
		nBarcodeFrom = GetFirstBarcodeNo();
		nBarcodeTo = GetLastBarcodeNo();
	}
	else
	{
		nBarcodeFrom = 0;
		nBarcodeTo = 0;
	}
}

/**********************************************************************/

void CBarcodeFilterArray::GetLine ( int nBarcodeIdx )
{
	if ( nBarcodeIdx > DataManager.Barcode.GetSize() )
	{	
		m_strRecord = "";
		return;
	}

	int nDPValue = SysInfo.GetDPValue();
	int nDPQty = SysInfo.GetDPQty();
	
	CBarcodeCSVRecord BarcodeRecord;
	DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );

	int nPluIdx;
	__int64 nPluNo = BarcodeRecord.GetPluNoInt();
	if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == FALSE )
	{
		m_strRecord = "";
		return;
	}

	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt ( nPluIdx, PluRecord );
		
	CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );
	csv.Add ( BarcodeRecord.GetBarcodeNoString() );
	csv.Add ( BarcodeRecord.GetPluNoString() );

	int nMod = 0;
	
	if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE )
	{
		nMod = BarcodeRecord.GetModifier();
		csv.Add( DataManager.Modifier.GetDisplayName( nMod ) );
	}
	
#ifdef STOCKMAN_SYSTEM
	CString strStockCode;
	DataManager.Plu.GetPluStockCode ( PluRecord.GetPluNoInt(), strStockCode, TRUE );
	csv.Add ( PluRecord.GetReportText() );
	csv.Add ( strStockCode );
#else
	csv.Add ( PluRecord.GetModifierEposText(nMod) );
	csv.Add ( PluRecord.GetRepText() );
#endif

	csv.Add ( PluRecord.GetModifierDeptNoForReport(nMod) );
	m_strRecord = csv.GetLine();
}

/**********************************************************************/

