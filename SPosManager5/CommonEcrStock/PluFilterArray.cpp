/**********************************************************************/
#include "FilterTypes.h"
#include "SearchTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\SPosEcrManager5\EcrmanSupplierSelector.h"
/**********************************************************************/
#endif
/**********************************************************************/
#include "..\CommonEcrStockTray\BarcodePluNoTable.h"
#include "..\CommonEcrStock\PriceSetItemManager.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

CPluIndexItem::CPluIndexItem()
{
	Reset();
}

/**********************************************************************/

void CPluIndexItem::Reset()
{
	m_strText = "";
	m_strEPOSText = "";
	m_nDeptNo = 0;
	m_nPluNo = 0;
	m_strStockCode = "";
	m_nIndex = 0;
	m_bDeletionFlag = FALSE;
	m_nIndexVersion = 0;
}

/**********************************************************************/

void CPluIndexItem::Add ( CPluIndexItem& source )
{
}

/**********************************************************************/

int CPluIndexItem::Compare( CPluIndexItem& source, int nHint )
{
	switch( nHint )
	{
	case PLUFILTER_SORT_FORWARD_DEPT:
		{
			if ( m_nDeptNo != source.m_nDeptNo )
				return ( m_nDeptNo > source.m_nDeptNo ? 1 : -1 );
		}
		break;

	case PLUFILTER_SORT_REVERSE_DEPT:
		{
			if ( m_nDeptNo != source.m_nDeptNo )
				return ( m_nDeptNo < source.m_nDeptNo ? 1 : -1 );
		}
		break;

	case PLUFILTER_SORT_FORWARD_TEXT:
		{
			if ( m_strText != source.m_strText )
				return ( m_strText > source.m_strText ? 1 : -1 );
		}
		break;

	case PLUFILTER_SORT_REVERSE_TEXT:
		{
			if ( m_strText != source.m_strText )
				return ( m_strText < source.m_strText ? 1 : -1 );
		}
		break;

	case PLUFILTER_SORT_FORWARD_EPOSTEXT:
	{
		if (m_strEPOSText != source.m_strEPOSText)
			return (m_strEPOSText > source.m_strEPOSText ? 1 : -1);
	}
	break;

	case PLUFILTER_SORT_REVERSE_EPOSTEXT:
	{
		if (m_strEPOSText != source.m_strEPOSText)
			return (m_strEPOSText < source.m_strEPOSText ? 1 : -1);
	}
	break;

	case PLUFILTER_SORT_FORWARD_STOCKCODE:
		{
			if ( m_strStockCode != source.m_strStockCode )
				return ( m_strStockCode > source.m_strStockCode ? 1 : -1 );
		}
		break;

	case PLUFILTER_SORT_REVERSE_STOCKCODE:
		{
			if ( m_strStockCode != source.m_strStockCode )
				return ( m_strStockCode < source.m_strStockCode ? 1 : -1 );
		}
		break;
	}

	switch( nHint )
	{
	case PLUFILTER_SORT_REVERSE_PLUNO:
		{
			if ( m_nPluNo != source.m_nPluNo )
				return ( m_nPluNo < source.m_nPluNo ? 1 : -1 );			
		}
		break;

	default:
		{
			if ( m_nPluNo != source.m_nPluNo )
				return ( m_nPluNo > source.m_nPluNo ? 1 : -1 );			
		}
		break;
	}

	return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPluFilterExtraEcrman::CPluFilterExtraEcrman()
{
#ifndef STOCKMAN_SYSTEM
	m_nSupplierFilter = ECRMANSUPPLIER_ALL;
#else
	m_nSupplierFilter = 0;
#endif

	m_bShowNonStockItems = TRUE;	
	m_strSuppRefFilter = "";
	m_bNegativeStock = FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPluFilterExtraStockman::CPluFilterExtraStockman()
{
	m_strStockCode = "";
	m_bShowUnlinkedItems = TRUE;
	m_bShowLinkedItems = TRUE;
	m_bShowRecipeItems = TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPluFilterArray::CPluFilterArray( int nFilterType )
{
	m_nFilterType = nFilterType;
	Reset();
}

/**********************************************************************/

void CPluFilterArray::Reset()
{
	m_nSortType = PLUFILTER_SORT_FORWARD_PLUNO;
	m_bFiltered = FALSE;
	m_nIndexVersion = 0;
	m_strDescription = "";
	m_nSearchType = SEARCH_EPOSTEXT_ONLY;
	m_bMatchCase = FALSE;
	m_bLocalPluLocation = TRUE;
	m_bLocalPluMaster = TRUE;
	m_bLocalPluExclude = TRUE;
	m_bEANOnly = FALSE;
	m_bLiteralOnly = FALSE;
	SetDepartmentFilter( DEPARTMENTSET_ALL );
	m_nAnalysisCategoryFilter = 9999;
	m_bShowActiveItems = TRUE;
	m_bShowInactiveItems = TRUE;
	m_bPluRange = FALSE;
	m_nPluRangeStart = Plu::PluNo.Min;
	m_nPluRangeEnd = Plu::PluNo.Max;
	
	CKeyboardFilterSelectorLoc Loc;
	Loc.m_nNodeType = NODE_SYSTEM;
	Loc.m_nEntityIdx = 0;
	SetKeyboardFilter( Loc, 1 );
	
	m_nArrayIdx = 0;
}

/**********************************************************************/

void CPluFilterArray::SetPluRangeStart(__int64 n)
{
	if ((n >= Plu::PluNo.Min) && (n <= Plu::PluNo.Max))
	{
		m_nPluRangeStart = n;
	}
}

/**********************************************************************/

void CPluFilterArray::SetPluRangeEnd(__int64 n)
{
	if ((n >= Plu::PluNo.Min) && (n <= Plu::PluNo.Max))
	{
		m_nPluRangeEnd = n;
	}
}

/**********************************************************************/

bool CPluFilterArray::CheckFiltered()
{
	if (m_nDepartmentFilter != DEPARTMENTSET_ALL)
		return TRUE;

	if (m_nAnalysisCategoryFilter != 9999)
		return TRUE;

	if (m_strDescription != "")
		return TRUE;

	if (TRUE == m_bEANOnly)
		return TRUE;

	if (TRUE == m_bLiteralOnly)
		return TRUE;

	if (FALSE == m_bShowActiveItems)
		return TRUE;

	if (FALSE == m_bShowInactiveItems)
		return TRUE;

	if (TRUE == m_bPluRange)
		return TRUE;

	if (PriceSetItemManager.IsPluMaster() == FALSE)
	{
		if (m_bLocalPluLocation == FALSE)
			return TRUE;

		if (m_bLocalPluMaster == FALSE)
			return TRUE;

		if (m_bLocalPluExclude == FALSE)
			return TRUE;
	}

	if (m_KeyboardFilterPluArray.IsFiltered() == TRUE)
		return TRUE;

	if (TRUE == CheckFilteredStock())
		return TRUE;

	return FALSE;
}

/**********************************************************************/

bool CPluFilterArray::IsNativeOrder()
{
	return ( ( FALSE == m_bFiltered ) && ( PLUFILTER_SORT_FORWARD_PLUNO == m_nSortType ) );
}

/**********************************************************************/

bool CPluFilterArray::CheckDescription( CPluCSVRecord& PluRecord )
{
	if ( m_strDescription == "" )
		return TRUE;
		
	CString strSearchText = m_strDescription;
	if ( m_bMatchCase == FALSE )
		strSearchText.MakeUpper();
		
	if ( m_nSearchType != SEARCH_REPTEXT_ONLY )
	{
		CString strText = PluRecord.GetEposText();
			
		if ( m_bMatchCase == FALSE )
			strText.MakeUpper();
		
		if ( strText.Find ( strSearchText ) != - 1 )  
			return TRUE;
	}

	if ( m_nSearchType != SEARCH_EPOSTEXT_ONLY )
	{
		CString strText = PluRecord.GetRepText();
			
		if ( m_bMatchCase == FALSE )
			strText.MakeUpper();
		
		if ( strText.Find ( strSearchText ) != - 1 )   
			return TRUE;
	}
	
	return FALSE;
}

/**********************************************************************/

void CPluFilterArray::SetKeyboardFilter( CKeyboardFilterSelectorLoc& Loc, int nKbNo )
{
	m_KeyboardFilterPluArray.SelectKeyboard( Loc, nKbNo );
}

/**********************************************************************/

void CPluFilterArray::GetKeyboardFilter( CKeyboardFilterSelectorLoc& Loc, int& nKbNo )
{
	m_KeyboardFilterPluArray.GetSelection( Loc, nKbNo );
}

/**********************************************************************/

const char* CPluFilterArray::GetStockCodeForSorting( CPluCSVRecord& PluRecord )
{
#ifndef STOCKMAN_SYSTEM
	m_strStockCodeForSorting = PluRecord.GetSupplierRef();
#else
	CString strStockCode;
	DataManager.Plu.GetPluStockCode( PluRecord.GetPluNoInt(), strStockCode, TRUE );

	if ( strStockCode == "" )
		m_strStockCodeForSorting = "C";
	else if ( strStockCode == "Recipe" )
		m_strStockCodeForSorting = "B";
	else
	{
		m_strStockCodeForSorting = "A";
		m_strStockCodeForSorting += strStockCode;
	}
#endif

	return m_strStockCodeForSorting;
}

/**********************************************************************/

void CPluFilterArray::CreateUnfilteredList()
{
	for ( int nPluIdx = DataManager.Plu.GetSize() - 1; nPluIdx >= 0; nPluIdx-- )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		CPluIndexItem item;
		item.m_nPluNo = PluRecord.GetPluNoInt();
		item.m_nDeptNo = PluRecord.GetBaseDeptNo();
		item.m_strText = PluRecord.GetReportText();
		item.m_strText.MakeUpper();
		item.m_strEPOSText = PluRecord.GetEposText();
		item.m_strEPOSText.MakeUpper();
		item.m_strStockCode = GetStockCodeForSorting( PluRecord );
		item.m_nIndex = nPluIdx;
		item.m_nIndexVersion = 0;
		item.m_bDeletionFlag = FALSE;
		m_index.DirectAdd( item );
	}
}

/**********************************************************************/

void CPluFilterArray::BuildRecordList()
{
	m_index.RemoveAll();
	m_nIndexVersion = 0;

	m_bFiltered = CheckFiltered();

	if (FALSE == m_bFiltered)
	{
		switch (m_nSortType)
		{
		case PLUFILTER_SORT_FORWARD_PLUNO:
			break;

		case PLUFILTER_SORT_REVERSE_PLUNO:
			CreateUnfilteredList();
			break;

		default:
			CreateUnfilteredList();
			m_index.QuickSort(m_nSortType);
			break;
		}
	}
	else
	{
		for (int nPluIdx = 0; nPluIdx < DataManager.Plu.GetSize(); nPluIdx++)
		{
			if (PriceSetItemManager.IsPluMaster() == FALSE)
			{
				bool bInclude = TRUE;
				switch (PriceSetItemManager.GetPluStatus(nPluIdx))
				{
				case LOCALPLU_LOCAL:	bInclude = m_bLocalPluLocation;	break;
				case LOCALPLU_MASTER:	bInclude = m_bLocalPluMaster;	break;
				case LOCALPLU_EXCLUDE:	bInclude = m_bLocalPluExclude;	break;
				}

				if (bInclude == FALSE)
					continue;
			}

			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt(nPluIdx, PluRecord);

			if (FALSE == m_bShowActiveItems)
			{
				if (PluRecord.GetActiveItemFlag() == TRUE)
					continue;
			}

			if (FALSE == m_bShowInactiveItems)
			{
				if (PluRecord.GetActiveItemFlag() == FALSE)
					continue;
			}

			if (m_KeyboardFilterPluArray.IsFiltered() == TRUE)
			{
				if (m_KeyboardFilterPluArray.CheckPluNo(PluRecord.GetPluNoInt()) == FALSE)
					continue;
			}

			if (TRUE == m_bLiteralOnly)
			{
				if (SysInfo.IsModifiableSSPluNo(PluRecord.GetPluNoInt()) == TRUE)
					continue;
			}

			if (TRUE == m_bEANOnly)
			{
				if (BarcodePluNoTable.IsValidEan(PluRecord.GetPluNoInt()) == FALSE)
					continue;
			}

			if (m_DepartmentSet.GetDepartmentStatus(PluRecord.GetBaseDeptNo()) == FALSE)
			{
				continue;
			}

			if ((m_nAnalysisCategoryFilter != 9999) && (PluRecord.GetBaseAnalysisCategory() != m_nAnalysisCategoryFilter))
			{
				continue;
			}

			if (TRUE == m_bPluRange )
			{
				__int64 nPluNo = PluRecord.GetPluNoInt();
				if ((nPluNo < m_nPluRangeStart) || (nPluNo > m_nPluRangeEnd))
				{
					continue;
				}
			}

			if (IsExcludedByStockDetail( nPluIdx, PluRecord ) == TRUE)
			{
				continue;
			}

			if (CheckDescription(PluRecord) == TRUE)
			{
				CPluIndexItem item;
				item.m_nPluNo = PluRecord.GetPluNoInt();
				item.m_nDeptNo = PluRecord.GetBaseDeptNo();
				item.m_strText = PluRecord.GetReportText();
				item.m_strText.MakeUpper();
				item.m_strEPOSText = PluRecord.GetEposText();
				item.m_strEPOSText.MakeUpper();
				item.m_strStockCode = GetStockCodeForSorting(PluRecord);
				item.m_nIndex = nPluIdx;
				item.m_nIndexVersion = 0;
				item.m_bDeletionFlag = FALSE;
				m_index.DirectAdd(item);
			}
		}

		if (PLUFILTER_SORT_FORWARD_PLUNO != m_nSortType)
		{
			m_index.QuickSort(m_nSortType);
		}
	}
}

/**********************************************************************/

void CPluFilterArray::RefreshIndexItems()
{
	if ( IsNativeOrder() == FALSE )
	{
		int nPluDbSize = DataManager.Plu.GetSize();

		for ( int n = 0; n < m_index.GetSize(); n++ )
		{
			int nPluIdx = GetPluItemIndexAt(n);

			if ( ( nPluIdx >= 0 ) && ( nPluIdx < nPluDbSize ) )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt( nPluIdx, PluRecord );

				CPluIndexItem item;
				m_index.GetAt( n, item );
				item.m_nPluNo = PluRecord.GetPluNoInt();
				item.m_nDeptNo = PluRecord.GetBaseDeptNo();
				item.m_strText = PluRecord.GetReportText();
				item.m_strText.MakeUpper();
				item.m_strEPOSText = PluRecord.GetEposText();
				item.m_strEPOSText.MakeUpper();
				item.m_strStockCode = GetStockCodeForSorting( PluRecord );
				m_index.SetAt( n, item );
			}
		}
	}
}

/**********************************************************************/

void CPluFilterArray::ChangeSortOrder( int nType, bool bRefreshFields )
{
	switch( nType )
	{
	case PLUFILTER_SORT_FORWARD_PLUNO:
	case PLUFILTER_SORT_FORWARD_TEXT:
	case PLUFILTER_SORT_FORWARD_EPOSTEXT:
	case PLUFILTER_SORT_FORWARD_DEPT:
	case PLUFILTER_SORT_FORWARD_STOCKCODE:
		break;

	default:
		return;
	}

	if ( nType == m_nSortType )
	{
		switch( nType )
		{
		case PLUFILTER_SORT_FORWARD_PLUNO:		nType = PLUFILTER_SORT_REVERSE_PLUNO;		break;
		case PLUFILTER_SORT_FORWARD_TEXT:		nType = PLUFILTER_SORT_REVERSE_TEXT;		break;
		case PLUFILTER_SORT_FORWARD_EPOSTEXT:	nType = PLUFILTER_SORT_REVERSE_EPOSTEXT;	break;
		case PLUFILTER_SORT_FORWARD_DEPT:		nType = PLUFILTER_SORT_REVERSE_DEPT;		break;
		case PLUFILTER_SORT_FORWARD_STOCKCODE:	nType = PLUFILTER_SORT_REVERSE_STOCKCODE;	break;
		}
	}

	if ( IsNativeOrder() == TRUE )
	{
		SetSortType( nType );
		BuildRecordList();
	}
	else
	{
		SetSortType( nType );

		if ( IsNativeOrder() == TRUE )
		{
			m_index.RemoveAll();
			m_nIndexVersion = 0;
		}
		else
		{
			if ( TRUE == bRefreshFields )
				RefreshIndexItems();

			QuickSort();
		}
	}
}

/**********************************************************************/

void CPluFilterArray::QuickSort()
{
	if ( IsNativeOrder() == FALSE )
		m_index.QuickSort( m_nSortType );
}

/**********************************************************************/

void CPluFilterArray::QuickBuild( CByteArray& arrayFlag, bool bOnlyIfActive )
{
	StatusProgress.Lock( TRUE, "Building unsold item list" );
	QuickBuildInternal( arrayFlag, bOnlyIfActive );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CPluFilterArray::QuickBuildInternal( CByteArray& arrayFlag, bool bOnlyIfActive )
{
	Reset();
	m_bFiltered = TRUE;

	m_index.RemoveAll();
	m_nIndexVersion = 0;
	
	int nCount = DataManager.Plu.GetSize();
	if ( arrayFlag.GetSize() == nCount )
	{
		for ( int nPluIdx = 0; nPluIdx < nCount; nPluIdx++ )
		{
			StatusProgress.SetPos( nPluIdx, nCount );

			if ( arrayFlag.GetAt( nPluIdx ) == 0 )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt( nPluIdx, PluRecord );

				if ( ( FALSE == bOnlyIfActive ) || ( PluRecord.GetActiveItemFlag() == TRUE ) )
				{
					CPluIndexItem item;
					item.m_nPluNo = PluRecord.GetPluNoInt();
					item.m_nDeptNo = PluRecord.GetBaseDeptNo();
					item.m_strText = PluRecord.GetReportText();
					item.m_strText.MakeUpper();
					item.m_strEPOSText = PluRecord.GetEposText();
					item.m_strEPOSText.MakeUpper();
					item.m_strStockCode = GetStockCodeForSorting( PluRecord );
					item.m_nIndex = nPluIdx;
					item.m_nIndexVersion = 0;
					item.m_bDeletionFlag = FALSE;
					m_index.DirectAdd( item );
				}
			}
		}
	}
}

/**********************************************************************/

int CPluFilterArray::GetSize()
{
	if ( IsNativeOrder() == FALSE )
		return m_index.GetSize();
	else
		return DataManager.Plu.GetSize();
}

/**********************************************************************/

void CPluFilterArray::SetDescriptionFilter ( const char* szDescription, int nSearchType, bool bMatchCase )
{
	m_strDescription = szDescription;
	m_nSearchType = nSearchType;
	m_bMatchCase = bMatchCase;
}

/**********************************************************************/

void CPluFilterArray::SetDepartmentFilter ( int nDepartmentFilter )
{
	m_nDepartmentFilter = m_DepartmentSet.CreateDepartmentSet ( nDepartmentFilter );
}

/**********************************************************************/

int CPluFilterArray::GetPluItemIndexAt( int nArrayIdx )
{
	if ( nArrayIdx >= GetSize() )
		return -1;

	if ( IsNativeOrder() == TRUE )
		return nArrayIdx;
	
	CPluIndexItem item;
	m_index.GetAt( nArrayIdx, item );

	if ( item.m_nIndexVersion == m_nIndexVersion )
		return item.m_nIndex;

	int nPluIdx;
	if ( DataManager.Plu.FindPluByNumber( item.m_nPluNo, nPluIdx ) == FALSE )
		return -1;

	item.m_nIndex = nPluIdx;
	item.m_nIndexVersion = m_nIndexVersion;
	m_index.SetAt( nArrayIdx, item );

	return nPluIdx;
}

/**********************************************************************/

bool CPluFilterArray::GetDeletionFlagAt( int nArrayIdx )
{
	if ( ( nArrayIdx < 0 ) || ( nArrayIdx >= GetSize() ) )
		return FALSE;

	if ( IsNativeOrder() == TRUE )
		return FALSE;

	CPluIndexItem item;
	m_index.GetAt( nArrayIdx, item );
	
	return item.m_bDeletionFlag;
}

/**********************************************************************/

void CPluFilterArray::SetDeletionFlagAt( int nArrayIdx, bool bFlag )
{
	if ( ( nArrayIdx < 0 ) || ( nArrayIdx >= GetSize() ) )
		return;

	if ( IsNativeOrder() == TRUE )
		return;

	CPluIndexItem item;
	m_index.GetAt( nArrayIdx, item );
	item.m_bDeletionFlag = bFlag;
	m_index.SetAt( nArrayIdx, item );
}

/**********************************************************************/

bool CPluFilterArray::FindPluItemByNumber( __int64 nPluNo, int& nArrayIdx )
{
	bool bResult = FALSE;

	if ( IsNativeOrder() == TRUE )
		bResult = DataManager.Plu.FindPluByNumber( nPluNo, nArrayIdx );
	else if ( ( PLUFILTER_SORT_FORWARD_PLUNO == m_nSortType ) || ( PLUFILTER_SORT_REVERSE_PLUNO == m_nSortType ) )
	{
		CPluIndexItem item;
		item.m_nDeptNo = 0;
		item.m_strText = "";
		item.m_strEPOSText = "";
		item.m_nPluNo = nPluNo;
		bResult = m_index.FindWithHint( item, m_nSortType, nArrayIdx );
	}
	else
	{
		nArrayIdx = 0;
		__int64 nBestMatch = 0;

		for ( int n = 0; n < GetSize(); n++ )
		{
			CPluIndexItem item;
			m_index.GetAt( n, item );

			if ( item.m_nPluNo == nPluNo )
			{
				nArrayIdx = n;
				bResult = TRUE;
				break;
			}

			if ( ( item.m_nPluNo > nBestMatch ) && ( item.m_nPluNo < nPluNo ) )
			{
				nBestMatch = item.m_nPluNo;
				nArrayIdx = n;
			}
		}
	}

	return bResult;
}

/**********************************************************************/

void CPluFilterArray::GetPluItemRange( __int64& nFirstPluNo, __int64& nLastPluNo )
{
	nFirstPluNo = 1;
	nLastPluNo = 1;

	if ( GetSize() > 0 )
	{
		if ( IsNativeOrder() == TRUE )
		{
			nFirstPluNo = DataManager.Plu.GetPluNoInt(0);
			nLastPluNo = DataManager.Plu.GetPluNoInt( GetSize() - 1 );
		}
		else if ( PLUFILTER_SORT_FORWARD_PLUNO == m_nSortType )
		{
			CPluIndexItem itemFirst;
			m_index.GetAt( 0, itemFirst );
			nFirstPluNo = itemFirst.m_nPluNo;
			
			CPluIndexItem itemLast;
			m_index.GetAt( GetSize() - 1, itemLast );
			nLastPluNo = itemLast.m_nPluNo;
		}
		else if ( PLUFILTER_SORT_REVERSE_PLUNO == m_nSortType )
		{
			CPluIndexItem itemFirst;
			m_index.GetAt( 0, itemFirst );
			nLastPluNo = itemFirst.m_nPluNo;
			
			CPluIndexItem itemLast;
			m_index.GetAt( GetSize() - 1, itemLast );
			nFirstPluNo = itemLast.m_nPluNo;
		}
		else
		{
			for ( int n = 0; n < GetSize(); n++ )
			{
				CPluIndexItem item;
				m_index.GetAt( n, item );
				
				if ( ( 0 == n ) || ( item.m_nPluNo < nFirstPluNo ) )
					nFirstPluNo = item.m_nPluNo;

				if ( ( 0 == n ) || ( item.m_nPluNo > nLastPluNo ) )
					nLastPluNo = item.m_nPluNo;
			}
		}
	}
}

/**********************************************************************/

void CPluFilterArray::DeletePluItemRange( __int64 nFirstPluNo, __int64 nLastPluNo )
{
	int nStartIdx, nRange;
	GetArrayIdxRange( nFirstPluNo, nLastPluNo, nStartIdx, nRange );

	if ( IsNativeOrder() == TRUE )
	{
		for ( int nOffset = nRange - 1; nOffset >= 0; nOffset-- )
			DataManager.Plu.RemoveAt( nStartIdx + nOffset );
	}
	else if ( PLUFILTER_SORT_FORWARD_PLUNO == m_nSortType )
	{
		for ( int nOffset = nRange - 1; nOffset >= 0; nOffset-- )
		{
			int nArrayIdx = nStartIdx + nOffset;
			int nPluIdx = GetPluItemIndexAt( nArrayIdx );
			DataManager.Plu.RemoveAt( nPluIdx );
			m_index.RemoveAt( nArrayIdx );
		}

		m_nIndexVersion++;
	}
	else
	{
		for ( int nOffset = nRange - 1; nOffset >= 0; nOffset-- )
		{
			int nArrayIdx = nStartIdx + nOffset;
			
			CPluIndexItem item;
			m_index.GetAt( nArrayIdx, item );

			if ( ( item.m_nPluNo >= nFirstPluNo ) && ( item.m_nPluNo <= nLastPluNo ) )
			{
				int nPluIdx;
				if ( DataManager.Plu.FindPluByNumber( item.m_nPluNo, nPluIdx ) == TRUE )
				{
					DataManager.Plu.RemoveAt( nPluIdx );
					m_index.RemoveAt( nArrayIdx );
				}
			}
		}

		m_nIndexVersion++;
	}
}

/**********************************************************************/

//this is a helper function for the Plu database dlg to use when 
//removing a single item
void CPluFilterArray::RemoveIndex ( int nIndex )
{
	if ( ( IsNativeOrder() == FALSE ) && ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		m_index.RemoveAt( nIndex );
		m_nIndexVersion++;
	}
}

/**********************************************************************/

void CPluFilterArray::SetArrayIdx( int nArrayIdx )
{
	if ( nArrayIdx >= 0 && nArrayIdx < GetSize() )
		m_nArrayIdx = nArrayIdx;
	else
		m_nArrayIdx = -1;
}

/**********************************************************************/

void CPluFilterArray::Next()
{
	if ( m_nArrayIdx < GetSize() - 1 )
		m_nArrayIdx++;
}

/**********************************************************************/

void CPluFilterArray::Previous()
{
	if ( m_nArrayIdx > 0 )
		m_nArrayIdx--;
}

/**********************************************************************/
bool CPluFilterArray::HasNext() { return ( m_nArrayIdx < GetSize() - 1 ); }
bool CPluFilterArray::HasPrevious() { return ( m_nArrayIdx > 0 ); }
/**********************************************************************/

void CPluFilterArray::GetDefaultRange ( int nArrayIdx, __int64& nPluFrom, __int64& nPluTo, bool bNoEan )
{
	switch ( m_nSortType )
	{
	case PLUFILTER_SORT_FORWARD_PLUNO:
	case PLUFILTER_SORT_REVERSE_PLUNO:
		if ( ( nArrayIdx >= 0 ) && ( nArrayIdx < GetSize() ) )
		{
			int nPluIdx = GetPluItemIndexAt ( nArrayIdx );
			
			__int64 nTempFrom, nTempTo;
			GetPluItemRange( nTempFrom, nTempTo );

			if ( PLUFILTER_SORT_FORWARD_PLUNO == m_nSortType )
			{
				nPluFrom = DataManager.Plu.GetPluNoInt ( nPluIdx );
				nPluTo = nTempTo;
			}
			else
			{
				nPluFrom = nTempFrom;
				nPluTo = DataManager.Plu.GetPluNoInt ( nPluIdx );
			}
		}
		else if ( GetSize() > 0 )
		{
			GetPluItemRange( nPluFrom, nPluTo );
		}
		else
		{
			nPluFrom = 0;
			nPluTo = 0;
		}
		break;

	default:
		GetPluItemRange( nPluFrom, nPluTo );
		break;
	}

	if ( TRUE == bNoEan )
	{
		CString strPluFrom;
		strPluFrom.Format( "%I64d", nPluFrom );
		if ( strPluFrom.GetLength() > SysInfo.GetMaxBasePluLen() )
			nPluFrom = 1;

		CString strPluTo;
		strPluTo.Format( "%I64d", nPluTo );
		if ( strPluTo.GetLength() > SysInfo.GetMaxBasePluLen() )
		{
			strPluTo = "";
			::AddLeading( strPluTo, SysInfo.GetMaxBasePluLen(), '9' );
			nPluTo = _atoi64( strPluTo );
		}
	}
}

/**********************************************************************/

void CPluFilterArray::GetArrayIdxRange( __int64 nPluFrom, __int64 nPluTo, int& nStartIdx, int& nRange )
{
	nStartIdx = 0;
	nRange = GetSize();

	if ( GetSortType() == PLUFILTER_SORT_FORWARD_PLUNO )
	{
		FindPluItemByNumber ( nPluFrom, nStartIdx );

		int nEndIdx;
		if ( FindPluItemByNumber( nPluTo, nEndIdx ) == FALSE )
			nEndIdx--;

		nRange = ( nEndIdx - nStartIdx ) + 1;
	}
	if ( GetSortType() == PLUFILTER_SORT_REVERSE_PLUNO )
	{
		FindPluItemByNumber ( nPluTo, nStartIdx );

		int nEndIdx;
		if ( FindPluItemByNumber( nPluFrom, nEndIdx ) == FALSE )
			nEndIdx--;

		nRange = ( nEndIdx - nStartIdx ) + 1;
	}
}

/**********************************************************************/

const char* CPluFilterArray::GetColumnManagerDisplayLine ( CColumnManager& ColumnManager, int nArrayIdx )
{
	m_strDisplayLine ="";
	int nPluIdx = -1;
	
	if ( IsNativeOrder() == TRUE )
	{
		nPluIdx = nArrayIdx;
	}
	else
	{
		if ((nArrayIdx >= 0) && (nArrayIdx < GetSize()))
		{
			CPluIndexItem item;
			m_index.GetAt(nArrayIdx, item);

			if (item.m_nIndexVersion == m_nIndexVersion)
			{
				nPluIdx = item.m_nIndex;
			}
			else if (DataManager.Plu.FindPluByNumber(item.m_nPluNo, nPluIdx) == TRUE)
			{
				item.m_nIndex = nPluIdx;
				item.m_nIndexVersion = m_nIndexVersion;
				m_index.SetAt(nArrayIdx, item);
			}
			else
			{
				nPluIdx = -1;
			}
		}
	}
	
	if (nPluIdx != -1)
	{
		GetColumnManagerLine(ColumnManager, nPluIdx);
	}

	return m_strDisplayLine;
}

/**********************************************************************/

const char* CPluFilterArray::GetDisplayLine ( int nArrayIdx )
{
	m_strDisplayLine ="";
	int nPluIdx = -1;
	
	if ( IsNativeOrder() == TRUE )
	{
		nPluIdx = nArrayIdx;
	}
	else
	{
		if ( ( nArrayIdx >= 0 ) && ( nArrayIdx < GetSize() ) )
		{
			CPluIndexItem item;
			m_index.GetAt( nArrayIdx, item );
			
			if ( item.m_nIndexVersion == m_nIndexVersion )
				nPluIdx = item.m_nIndex;
			else if ( DataManager.Plu.FindPluByNumber( item.m_nPluNo, nPluIdx ) == TRUE )
			{
				item.m_nIndex = nPluIdx;
				item.m_nIndexVersion = m_nIndexVersion;
				m_index.SetAt( nArrayIdx, item );
			}
			else
				nPluIdx = -1;
		}
	}
	
	if ( nPluIdx != -1 )
		GetLine( nPluIdx, nArrayIdx );
	
	return m_strDisplayLine;
}

/**********************************************************************/

int CPluFilterArray::GetListIdxForNewPluIdx(CPluCSVRecord& PluRecord, int nPluIdx)
{
	int nListIdx = nPluIdx;

	if (IsNativeOrder() == FALSE)
	{
		CPluIndexItem item;
		item.m_nPluNo = PluRecord.GetPluNoInt();
		item.m_nDeptNo = PluRecord.GetBaseDeptNo();
		item.m_strText = PluRecord.GetReportText();
		item.m_strText.MakeUpper();
		item.m_strStockCode = GetStockCodeForSorting(PluRecord);
		item.m_nIndex = nPluIdx;
		item.m_nIndexVersion = 0;

		switch (GetSortType())
		{
		case PLUFILTER_SORT_FORWARD_PLUNO:
		case PLUFILTER_SORT_REVERSE_PLUNO:
			m_index.ConsolidateWithHint(item, m_nSortType);
			m_index.FindWithHint(item, m_nSortType, nListIdx);
			break;

		default:
			m_index.DirectAdd(item);
			nListIdx = m_index.GetSize() - 1;
			break;
		}

		m_nIndexVersion++;
	}

	return nListIdx;
}

/**********************************************************************/

int CPluFilterArray::InsertPluRecord(CPluCSVRecord& PluRecord, bool bExportFlag)
{
	int nPluIdx = 0;
	int nListIdx = 0;
	__int64 nPluNo = PluRecord.GetPluNoInt();

	if (DataManager.Plu.FindPluByNumber(nPluNo, nPluIdx) == FALSE)
	{
		InsertPluRecordInternal(PluRecord, bExportFlag);
		nListIdx = GetListIdxForNewPluIdx(PluRecord, nPluIdx);
	}
		
	return nListIdx;
}

/**********************************************************************/

void CPluFilterArray::GetLine(int nPluIdx, int nFilterIdx)
{
	GetLine_Init();

	if (nPluIdx > DataManager.Plu.GetSize())
	{
		m_strDisplayLine = "";
		return;
	}

	int nDPValue = SysInfo.GetDPValue();

	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt(nPluIdx, PluRecord);

	CString strStockCode = "";
	GetLine_GetStockCode(PluRecord, strStockCode);

	CCSV csv('\t', '"', TRUE, FALSE, FALSE);

	switch (m_nFilterType)
	{
	case FILTERTYPE_DATABASE:
		csv.Add(PluRecord.GetPluNoString());
		csv.Add(PluRecord.GetReportText());
		GetLine_AppendStockCode(csv, strStockCode);
		csv.Add(PluRecord.GetBaseDeptNo());
		csv.Add(PluRecord.GetPrice(0), nDPValue);
		csv.Add(PluRecord.GetPrice(1), nDPValue);
		csv.Add(PluRecord.GetPluTypeText());
		csv.Add(PluRecord.GetBaseTaxBandString());

		if (PriceSetItemManager.IsPluMaster() == FALSE)
		{
			switch (PriceSetItemManager.GetPluStatus(nPluIdx))
			{
			case LOCALPLU_MASTER:	csv.Add("Master");	break;
			case LOCALPLU_LOCAL:	csv.Add("Location");	break;
			case LOCALPLU_EXCLUDE:	csv.Add("Excluded");	break;
			}
		}
		else
		{
			GetLine_AppendEcrmanStock(csv, PluRecord);
		}

		break;

	case FILTERTYPE_BROWSE_PLU:
	{
		if (GetLine_BrowseStockCodes() == FALSE)
		{
			CString strDept;
			int nDeptNo = PluRecord.GetBaseDeptNo();

			strDept.Format("D%2.2d %s",
				nDeptNo,
				DataManager.Department.GetReportTextByDeptNo(nDeptNo));

			csv.Add(PluRecord.GetPluNoString());
			csv.Add(PluRecord.GetReportText());
			csv.Add(strDept);
			csv.Add(PluRecord.GetPrice(0), SysInfo.GetDPValue());
		}
		else
		{
			csv.Add(PluRecord.GetPluNoString());
			csv.Add(PluRecord.GetReportText());
			csv.Add(strStockCode);
			csv.Add(PluRecord.GetBaseDeptNo());
		}
	}
	break;

	case FILTERTYPE_PLU_DELETION:
	{
		csv.Add(PluRecord.GetPluNoString());
		csv.Add(PluRecord.GetReportText());
		GetLine_AppendStockCode(csv, strStockCode);
		csv.Add(PluRecord.GetBaseDeptNo());
		csv.Add(PluRecord.GetPrice(0), SysInfo.GetDPValue());

		if (GetDeletionFlagAt(nFilterIdx) == TRUE)
			csv.Add("Yes");
	}
	break;

	default:
		break;
	}
	m_strDisplayLine = csv.GetLine();
}

/**********************************************************************/

void CPluFilterArray::GetColumnManagerLine(CColumnManager& ColumnManager, int nPluIdx)
{
	if (nPluIdx > DataManager.Plu.GetSize())
	{
		m_strDisplayLine = "";
		return;
	}

	int nDPValue = SysInfo.GetDPValue();

	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt(nPluIdx, PluRecord);

	CString strStockCode = "";
	GetLine_GetStockCode(PluRecord, strStockCode);

	CCSV csv('\t', '"', TRUE, FALSE, FALSE);

	for (int n = 0; n < ColumnManager.GetListColumnCount(); n++)
	{
		if (ColumnManager.GetListColumnStatus(n) == COLUMN_STATUS_HIDDEN)
		{
			break;
		}

		int nType = ColumnManager.GetListColumnType(n);

		switch (nType)
		{
		case COLUMN_PLUNO:		csv.Add(PluRecord.GetPluNoString());		break;
		case COLUMN_DESC:		csv.Add(PluRecord.GetReportText());			break;
		case COLUMN_EPOSTEXT:	csv.Add(PluRecord.GetEposText());			break;
		case COLUMN_STOCKCODE:	csv.Add(strStockCode);						break;
		case COLUMN_DEPT:		csv.Add(PluRecord.GetBaseDeptNo());			break;
		case COLUMN_PRICE1:		csv.Add(PluRecord.GetPrice(0), nDPValue);	break;
		case COLUMN_PRICE2:		csv.Add(PluRecord.GetPrice(1), nDPValue);	break;
		case COLUMN_PRICE3:		csv.Add(PluRecord.GetPrice(2), nDPValue);	break;
		case COLUMN_PRICE4:		csv.Add(PluRecord.GetPrice(3), nDPValue);	break;
		case COLUMN_POINTS:		csv.Add(PluRecord.GetPoints());				break;
		case COLUMN_PLUTYPE:	csv.Add(PluRecord.GetPluTypeText());		break;
		case COLUMN_TAX:		csv.Add(PluRecord.GetBaseTaxBandString());	break;
		case COLUMN_COST:		csv.Add(DataManager.Plu.GetPluPurchaseCost(PluRecord), nDPValue); break;

		case COLUMN_PLUSTATUS:
		{
			switch (PriceSetItemManager.GetPluStatus(nPluIdx))
			{
			case LOCALPLU_MASTER:	csv.Add("Master");	break;
			case LOCALPLU_LOCAL:	csv.Add("Location");	break;
			case LOCALPLU_EXCLUDE:	csv.Add("Excluded");	break;
			}
		}
		break;

		case COLUMN_APPARENT:
			GetLine_AppendEcrmanStock(csv, PluRecord);
			break;

		case COLUMN_SUPPLIER:
			GetLine_AppendStockmanSupplier(csv, PluRecord);
			break;

		case COLUMN_PRINTERS:
		{
			CString strPrinters = "";

			int nGroup = PluRecord.GetOrderPrinterGroup();
			if ((nGroup >= 1) && (nGroup <= DataManagerNonDb.OPGroupSystem.GetOPGroupCount()))
			{
				COPGroupMiniInfo infoGroup;
				DataManagerNonDb.OPGroupSystem.GetOPGroup(nGroup - 1, infoGroup);
				strPrinters = infoGroup.m_strName;
			}
			else
			{
				for (int nIdx = 0; nIdx < PLU_ORDER_PRINTERS_NOW; nIdx++)
				{
					if (PluRecord.GetOrderPrinterFlag(nIdx))
					{
						CString strNum;
						strNum.Format("%d", nIdx + 1);
						if (strPrinters != "") strPrinters += ",";
						strPrinters += strNum;
					}
				}
			}

			csv.Add(strPrinters);
		}
		break;
		}
	}

	m_strDisplayLine = csv.GetLine();
}

/**********************************************************************/

void CPluFilterArray::CopyToPreset(CPresetPluFilter& Preset)
{
	Preset.SetPluRangeFlag(m_bPluRange);
	Preset.SetPluRangeStart(m_nPluRangeStart);
	Preset.SetPluRangeEnd(m_nPluRangeEnd);
	Preset.SetDescription(m_strDescription);
	Preset.SetSearchType(m_nSearchType);
	Preset.SetMatchCaseFlag(m_bMatchCase);
	Preset.SetLocalPluLocationFlag(m_bLocalPluLocation);
	Preset.SetLocalPluMasterFlag(m_bLocalPluMaster);
	Preset.SetLocalPluExcludeFlag(m_bLocalPluExclude);
	Preset.SetEANOnlyFlag(m_bEANOnly);
	Preset.SetLiteralOnlyFlag(m_bLiteralOnly);
	Preset.SetDepartmentFilter(m_nDepartmentFilter);
	Preset.SetShowActiveItemsFlag(m_bShowActiveItems);
	Preset.SetShowInactiveItemsFlag(m_bShowInactiveItems);
	Preset.SetStockmanStockCode(m_StockmanFilters.m_strStockCode);
	Preset.SetStockmanShowUnlinkedFlag(m_StockmanFilters.m_bShowUnlinkedItems);
	Preset.SetStockmanShowLinkedFlag(m_StockmanFilters.m_bShowLinkedItems);
	Preset.SetStockmanShowRecipeFlag(m_StockmanFilters.m_bShowRecipeItems);
	Preset.SetEcrmanSupplierFilter(m_EcrmanFilters.m_nSupplierFilter);
	Preset.SetEcrmanSuppRef(m_EcrmanFilters.m_strSuppRefFilter);
	Preset.SetEcrmanShowNonStockFlag(m_EcrmanFilters.m_bShowNonStockItems);
}

/**********************************************************************/

void CPluFilterArray::CopyFromPreset(CPresetPluFilter& Preset)
{
	m_bPluRange = Preset.GetPluRangeFlag();
	m_nPluRangeStart = Preset.GetPluRangeStart();
	m_nPluRangeEnd = Preset.GetPluRangeEnd();
	m_strDescription = Preset.GetDescription();
	m_nSearchType = Preset.GetSearchType();
	m_bMatchCase = Preset.GetMatchCaseFlag();
	m_bLocalPluLocation = Preset.GetLocalPluLocationFlag();
	m_bLocalPluMaster = Preset.GetLocalPluMasterFlag();
	m_bLocalPluExclude = Preset.GetLocalPluExcludeFlag();
	m_bEANOnly = Preset.GetEANOnlyFlag();
	m_bLiteralOnly = Preset.GetLiteralOnlyFlag();
	m_nDepartmentFilter = Preset.GetDepartmentFilter();
	m_bShowActiveItems = Preset.GetShowActiveItemsFlag();
	m_bShowInactiveItems = Preset.GetShowInactiveItemsFlag();
	m_StockmanFilters.m_strStockCode = Preset.GetStockmanStockCode();
	m_StockmanFilters.m_bShowUnlinkedItems = Preset.GetStockmanShowUnlinkedFlag();
	m_StockmanFilters.m_bShowLinkedItems = Preset.GetStockmanShowLinkedFlag();
	m_StockmanFilters.m_bShowRecipeItems = Preset.GetStockmanShowRecipeFlag();
	m_EcrmanFilters.m_nSupplierFilter = Preset.GetEcrmanSupplierFilter();
	m_EcrmanFilters.m_strSuppRefFilter = Preset.GetEcrmanSuppRef();
	m_EcrmanFilters.m_bShowNonStockItems = Preset.GetEcrmanShowNonStockFlag();
}

/**********************************************************************/
