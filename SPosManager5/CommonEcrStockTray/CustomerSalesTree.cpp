/**********************************************************************/
 
/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "CustomerSalesTree.h"
/**********************************************************************/

CCustomerSalesConsolidationInfo::CCustomerSalesConsolidationInfo()
{
	//TREE CONSOLIDATION
	m_nTreeInt = 0;
	m_oleTreeDate = COleDateTime( 2007, 1, 1, 0, 0, 0 );
	m_strTreeString = "";
	m_infoConsol.Reset();
	//INTERNAL
	m_bEndNode = FALSE;
	m_nNextArrayIdx = 0;
	m_nNodeCount = 0;
	m_nNextTransNo = 0;
}

/**********************************************************************/

CCustomerSalesNode::CCustomerSalesNode()
{
	m_infoConsol.Reset();
	m_nNextArrayIdx = 0;
}

/**********************************************************************/

CCustomerSalesArray::CCustomerSalesArray( int nSortType )
{
	m_nSortType = nSortType;
	switch ( m_nSortType )
	{
	case SORT_TYPE_STRING:	m_pArrayKeys = new CArray<CString,CString>;				break;
	case SORT_TYPE_INT:		m_pArrayKeys = new CArray<int,int>;						break;
	case SORT_TYPE_DATE:	m_pArrayKeys = new CArray<COleDateTime,COleDateTime>;	break;
	}
}

/**********************************************************************/

CCustomerSalesArray::~CCustomerSalesArray()
{
	switch ( m_nSortType )
	{
	case SORT_TYPE_STRING:	delete (GetStringKeys() );		break;
	case SORT_TYPE_INT:		delete (GetIntKeys() );			break;
	case SORT_TYPE_DATE:	delete (GetDateKeys() );		break;
	}
}

/**********************************************************************/

void CCustomerSalesArray::Clear()
{
	switch( m_nSortType )
	{
	case SORT_TYPE_STRING:	GetStringKeys() -> RemoveAll();		break;
	case SORT_TYPE_INT:		GetIntKeys() -> RemoveAll();		break;
	case SORT_TYPE_DATE:	GetDateKeys() -> RemoveAll();		break;
	}
	m_arrayNodes.RemoveAll();
}

/**********************************************************************/

bool CCustomerSalesArray::FindNodeString( CString& strLabelToFind, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetStringKeys() -> GetSize() - 1;
	
	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		
		CString strLabel = GetStringKeys() -> GetAt(nIndex);
		
		if ( strLabel == strLabelToFind )
			return TRUE;

		if ( strLabel < strLabelToFind )
			nStart = nIndex + 1;
		else
			nEnd = nIndex - 1;
	}
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

bool CCustomerSalesArray::FindNodeInt( int nLabelToFind, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetIntKeys() -> GetSize() - 1;
	
	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		
		int nLabel = GetIntKeys() -> GetAt(nIndex);
		
		if ( nLabel == nLabelToFind )
			return TRUE;

		if ( nLabel < nLabelToFind )
			nStart = nIndex + 1;
		else
			nEnd = nIndex - 1;
	}
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

bool CCustomerSalesArray::FindNodeDate( COleDateTime oleLabelToFind, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetDateKeys() -> GetSize() - 1;
	
	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		
		COleDateTime oleLabel = GetDateKeys() -> GetAt(nIndex);
		
		if ( oleLabel == oleLabelToFind )
			return TRUE;

		if ( oleLabel < oleLabelToFind )
			nStart = nIndex + 1;
		else
			nEnd = nIndex - 1;
	}
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

int CCustomerSalesArray::ConsolidateString( 
		CCustomerSalesConsolidationInfo& infoNew, 
		CReportConsolidationArray<CEposReportConsolCustomerHistory>& arrayItems,
		CReportConsolidationArray<CEposReportConsolCustomerHistory>& arrayTotals )
{
	int nNodeIdx;
	int nResult = 0;
		
	if ( FindNodeString ( infoNew.m_strTreeString, nNodeIdx ) == FALSE )
	{
		infoNew.m_nNodeCount++;
		infoNew.m_infoConsol.m_nTransactionNo = infoNew.m_nNextTransNo++;

		GetStringKeys() -> InsertAt( nNodeIdx, infoNew.m_strTreeString );
		
		CCustomerSalesNode NewNode;
		NewNode.m_infoConsol = infoNew.m_infoConsol;

		if ( infoNew.m_bEndNode == FALSE )
		{
			NewNode.m_nNextArrayIdx = infoNew.m_nNextArrayIdx;
			m_arrayNodes.InsertAt( nNodeIdx, NewNode );
			nResult = infoNew.m_nNextArrayIdx;
		}
		else
		{
			NewNode.m_nNextArrayIdx = 0;
			m_arrayNodes.InsertAt( nNodeIdx, NewNode );
			nResult = 0;
		}
	}
	else
	{
		m_arrayNodes[nNodeIdx].m_infoConsol.Add( infoNew.m_infoConsol );

		if ( infoNew.m_bEndNode == FALSE )
			nResult = m_arrayNodes[nNodeIdx].m_nNextArrayIdx;
		else
			nResult = 0;

		infoNew.m_infoConsol.m_nTransactionNo = m_arrayNodes[nNodeIdx].m_infoConsol.m_nTransactionNo;
	}

	if ( infoNew.m_infoConsol.m_nReportLineSubType != 0 )
	{
		//ITEM BLOCK
		CEposReportConsolCustomerHistory infoTotal = infoNew.m_infoConsol;
		arrayItems.Consolidate( infoTotal );

		//CUSTOMER SUMMARY
		infoTotal.m_nReportLineSubType = 0;
		arrayTotals.Consolidate( infoTotal );
	}

	return nResult;
}

/**********************************************************************/

int CCustomerSalesArray::ConsolidateInt( 
		CCustomerSalesConsolidationInfo& infoNew, 
		CReportConsolidationArray<CEposReportConsolCustomerHistory>& arrayItems,
		CReportConsolidationArray<CEposReportConsolCustomerHistory>& arrayTotals )
{
	int nNodeIdx;
	int nResult = 0;
		
	if ( FindNodeInt ( infoNew.m_nTreeInt, nNodeIdx ) == FALSE )
	{
		infoNew.m_nNodeCount++;
		infoNew.m_infoConsol.m_nTransactionNo = infoNew.m_nNextTransNo++;

		GetIntKeys() -> InsertAt( nNodeIdx, infoNew.m_nTreeInt );
		
		CCustomerSalesNode NewNode;
		NewNode.m_infoConsol = infoNew.m_infoConsol;
		
		if ( infoNew.m_bEndNode == FALSE )
		{
			NewNode.m_nNextArrayIdx = infoNew.m_nNextArrayIdx;
			m_arrayNodes.InsertAt( nNodeIdx, NewNode );
			nResult = infoNew.m_nNextArrayIdx;
		}
		else
		{
			NewNode.m_nNextArrayIdx = 0;
			m_arrayNodes.InsertAt( nNodeIdx, NewNode );
			nResult = 0;
		}
	}
	else
	{
		m_arrayNodes[nNodeIdx].m_infoConsol.Add( infoNew.m_infoConsol );

		if ( infoNew.m_bEndNode == FALSE )
			nResult = m_arrayNodes[nNodeIdx].m_nNextArrayIdx;
		else
			nResult = 0;

		infoNew.m_infoConsol.m_nTransactionNo = m_arrayNodes[nNodeIdx].m_infoConsol.m_nTransactionNo;
	}

	if ( infoNew.m_infoConsol.m_nReportLineSubType != 0 )
	{
		//ITEM BLOCK
		CEposReportConsolCustomerHistory infoTotal = infoNew.m_infoConsol;
		arrayItems.Consolidate( infoTotal );

		//CUSTOMER SUMMARY
		infoTotal.m_nReportLineSubType = 0;
		arrayTotals.Consolidate( infoTotal );
	}

	return nResult;
}

/**********************************************************************/

int CCustomerSalesArray::ConsolidateDate( 
		CCustomerSalesConsolidationInfo& infoNew, 
		CReportConsolidationArray<CEposReportConsolCustomerHistory>& arrayItems,
		CReportConsolidationArray<CEposReportConsolCustomerHistory>& arrayTotals )
{
	int nNodeIdx;
	int nResult = 0;
		
	if ( FindNodeDate ( infoNew.m_oleTreeDate, nNodeIdx ) == FALSE )
	{
		infoNew.m_nNodeCount++;
		infoNew.m_infoConsol.m_nTransactionNo = infoNew.m_nNextTransNo++;

		GetDateKeys() -> InsertAt( nNodeIdx, infoNew.m_oleTreeDate );
		
		CCustomerSalesNode NewNode;
		NewNode.m_infoConsol = infoNew.m_infoConsol;
		
		if ( infoNew.m_bEndNode == FALSE )
		{
			NewNode.m_nNextArrayIdx = infoNew.m_nNextArrayIdx;
			m_arrayNodes.InsertAt( nNodeIdx, NewNode );
			nResult = infoNew.m_nNextArrayIdx;
		}
		else
		{
			NewNode.m_nNextArrayIdx = 0;
			m_arrayNodes.InsertAt( nNodeIdx, NewNode );
			nResult = 0;
		}
	}
	else
	{
		m_arrayNodes[nNodeIdx].m_infoConsol.Add( infoNew.m_infoConsol );

		if ( infoNew.m_bEndNode == FALSE )
			nResult = m_arrayNodes[nNodeIdx].m_nNextArrayIdx;
		else
			nResult = 0;

		infoNew.m_infoConsol.m_nTransactionNo = m_arrayNodes[nNodeIdx].m_infoConsol.m_nTransactionNo;
	}

	if ( infoNew.m_infoConsol.m_nReportLineSubType != 0 )
	{
		//ITEM BLOCK
		CEposReportConsolCustomerHistory infoTotal = infoNew.m_infoConsol;
		arrayItems.Consolidate( infoTotal );

		//CUSTOMER SUMMARY
		infoTotal.m_nReportLineSubType = 0;
		arrayTotals.Consolidate( infoTotal );
	}

	return nResult;
}

/**********************************************************************/

CCustomerSalesTable::CCustomerSalesTable() : CPtrArray()
{
	m_nNodeCount = 0;
	m_nNextTransNo = 1;
}

/**********************************************************************/

CCustomerSalesTable::~CCustomerSalesTable()
{
	for ( int n = GetSize() - 1; n >= 0; n-- )
		delete ( GetArray( n ) );
}

/**********************************************************************/

void CCustomerSalesTable::ClearConsolidatedLevels()
{
	for ( int n = GetSize() - 1; n >= 0; n-- )
	{
		delete ( GetArray( n ) );
		RemoveAt(n);
	}
	
	m_nNodeCount = 0;
	m_nNextTransNo = 1;

	m_arrayTranLines.RemoveAll();
	m_arrayTranBlocks.RemoveAll();
}

/**********************************************************************/

void CCustomerSalesTable::ClearFieldStructure()
{
	m_arrayFieldTypes.RemoveAll();
	m_bufferString.RemoveAll();
	m_bufferInt.RemoveAll();	
}

/**********************************************************************/

void CCustomerSalesTable::AddField( int nType )
{
	switch ( nType )
	{
	case SORT_TYPE_STRING:
	case SORT_TYPE_INT:
	case SORT_TYPE_DATE:
		m_arrayFieldTypes.Add( nType );
		m_bufferString.Add( "" );
		m_bufferInt.Add(0);
		m_bufferDate.Add ( COleDateTime( 2003, 1, 1, 0, 0, 0 ) );
		break;
	}
}

/**********************************************************************/

void CCustomerSalesTable::SetBufferString( int nPos, CString& string )
{
	m_bufferString.SetAt( nPos, string );
}

/**********************************************************************/

void CCustomerSalesTable::SetBufferInt( int nPos, int n )
{
	m_bufferInt.SetAt( nPos, n );
}

/**********************************************************************/

void CCustomerSalesTable::SetBufferDate( int nPos, COleDateTime date )
{
	m_bufferDate.SetAt( nPos, date );
}

/**********************************************************************/

void CCustomerSalesTable::Consolidate( CEposReportConsolCustomerHistory& infoConsol, bool bSystem )
{
	CCustomerSalesConsolidationInfo infoNew;
	infoNew.m_infoConsol = infoConsol;

	int nArrayToSearch = 0;
	int nFieldCount = m_arrayFieldTypes.GetSize();
	
	if ( ( GetSize() == 0 ) && ( nFieldCount != 0  ) )
	{
		CCustomerSalesArray* pArray = new CCustomerSalesArray( m_arrayFieldTypes.GetAt( 0 ) );
		Add( pArray );
	}

	for ( int nPos = 0; nPos < nFieldCount; nPos++ )
	{
		infoNew.m_bEndNode = ( nPos == nFieldCount - 1 );
		infoNew.m_nNodeCount = m_nNodeCount;
		infoNew.m_nNextTransNo = m_nNextTransNo;
		infoNew.m_nNextArrayIdx = GetSize();

		CCustomerSalesArray* pCustomerSalesArray = GetArray( nArrayToSearch );
		
		switch( m_arrayFieldTypes.GetAt(nPos) )
		{
		case SORT_TYPE_STRING:
			{
				infoNew.m_strTreeString = m_bufferString.GetAt(nPos);
				nArrayToSearch = pCustomerSalesArray -> ConsolidateString( infoNew, m_arrayTranLines, m_arrayTranBlocks );
				m_nNodeCount = infoNew.m_nNodeCount;
				m_nNextTransNo = infoNew.m_nNextTransNo;
			}
			break;

		case SORT_TYPE_INT:
			{
				infoNew.m_nTreeInt = m_bufferInt.GetAt(nPos);
				nArrayToSearch = pCustomerSalesArray -> ConsolidateInt( infoNew, m_arrayTranLines, m_arrayTranBlocks );
				m_nNodeCount = infoNew.m_nNodeCount;
				m_nNextTransNo = infoNew.m_nNextTransNo;
			}
			break;

		case SORT_TYPE_DATE:
			{
				infoNew.m_oleTreeDate = m_bufferDate.GetAt(nPos);
				nArrayToSearch = pCustomerSalesArray -> ConsolidateDate( infoNew, m_arrayTranLines, m_arrayTranBlocks );
				m_nNodeCount = infoNew.m_nNodeCount;
				m_nNextTransNo = infoNew.m_nNextTransNo;
			}
			break;
		}

		if ( nArrayToSearch == GetSize() )
		{
			CCustomerSalesArray* pArray = new CCustomerSalesArray( m_arrayFieldTypes.GetAt( nPos + 1 ) );
			Add( pArray );
		}
	}

	if ( infoNew.m_infoConsol.m_nReportLineSubType != 0 )
	{
		//GRAND TOTAL BLOCK
		CEposReportConsolCustomerHistory infoTotal = infoNew.m_infoConsol;
		infoTotal.m_nTransactionNo = 0;

		if ( TRUE == bSystem )
			infoTotal.m_nReportLineSubType = 0;
		else
			infoTotal.m_nReportLineSubType = infoNew.m_infoConsol.m_nReportLineSubType;
		
		m_arrayTranLines.Consolidate( infoTotal );
	}
}

/**********************************************************************/

int CCustomerSalesTable::GetArraySize( int nArrayIdx )
{
	if ( nArrayIdx >= 0 && nArrayIdx < GetSize() )
		return GetArray( nArrayIdx ) -> m_arrayNodes.GetSize();
	else
		return 0;
}

/**********************************************************************/

const char* CCustomerSalesTable::GetLabelString( int nArrayIdx, int nNodeIdx )
{
	return GetArray( nArrayIdx ) -> GetStringKeys() -> GetAt(nNodeIdx);
}

/**********************************************************************/

int CCustomerSalesTable::GetLabelInt( int nArrayIdx, int nNodeIdx )
{
	return GetArray( nArrayIdx ) -> GetIntKeys() -> GetAt(nNodeIdx);
}

/**********************************************************************/

COleDateTime CCustomerSalesTable::GetLabelDate( int nArrayIdx, int nNodeIdx )
{
	return GetArray( nArrayIdx ) -> GetDateKeys() -> GetAt(nNodeIdx);
}

/**********************************************************************/

void CCustomerSalesTable::GetSalesTotalForNode( int nArrayIdx, int nNodeIdx, CEposReportConsolCustomerHistory& infoConsol )
{
	infoConsol = GetArray( nArrayIdx ) -> m_arrayNodes[ nNodeIdx ].m_infoConsol;
}

/**********************************************************************/

void CCustomerSalesTable::GetSalesTotalForBlock( int nItemIdx, CEposReportConsolCustomerHistory& infoConsol )
{
	m_arrayTranBlocks.GetAt( nItemIdx, infoConsol );
}

/**********************************************************************/

void CCustomerSalesTable::GetSalesTotalForLine( int nItemIdx, CEposReportConsolCustomerHistory& infoConsol )
{
	m_arrayTranLines.GetAt( nItemIdx, infoConsol );
}

/**********************************************************************/

int CCustomerSalesTable::GetNextArrayIdx( int nArrayIdx, int nNodeIdx )
{
	return GetArray( nArrayIdx ) -> m_arrayNodes[nNodeIdx].m_nNextArrayIdx;
}

/**********************************************************************/

void CCustomerSalesTable::FindTransactionItemLines( int nTranArrayNo, int& nStart, int& nEnd )
{
	CEposReportConsolCustomerHistory infoConsol;
	infoConsol.m_nTransactionNo = nTranArrayNo;
	infoConsol.m_nReportLineType = 0;
	infoConsol.m_nReportLineSubType = 0;
	m_arrayTranLines.Find( infoConsol, nStart );

	infoConsol.m_nTransactionNo++;
	m_arrayTranLines.Find( infoConsol, nEnd );
	nEnd--;
}

/**********************************************************************/

void CCustomerSalesTable::FindTransactionBlockLines( int nTranArrayNo, int& nStart, int& nEnd )
{
	CEposReportConsolCustomerHistory infoConsol;
	infoConsol.m_nTransactionNo = nTranArrayNo;
	infoConsol.m_nReportLineType = 0;
	infoConsol.m_nReportLineSubType = 0;
	m_arrayTranBlocks.Find( infoConsol, nStart );

	infoConsol.m_nTransactionNo++;
	m_arrayTranBlocks.Find( infoConsol, nEnd );
	nEnd--;
}

/**********************************************************************/
