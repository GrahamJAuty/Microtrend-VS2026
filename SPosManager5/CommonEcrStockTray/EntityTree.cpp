/**********************************************************************/
#include "SalesHistoryFields.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EntityTree.h"
/**********************************************************************/

CEntityTreeFieldBuffer::CEntityTreeFieldBuffer()
{
	m_nFieldType = 0;
	m_strBufferString = "";
	m_nBufferInt = 0;;
	m_nBufferInt64 = 0;
	m_oleBufferDate = COleDateTime::GetCurrentTime();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeLeaf::CEntityTreeLeaf()
{
	m_nNextBranchIdx = 0;
	m_nEntityIdx = 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeBranch::CEntityTreeBranch( int nSortType )
{
	m_nSortType = nSortType;
	switch ( m_nSortType )
	{
	case SORT_TYPE_STRING:	m_pArrayKeys = new CArray<CString,CString>;				break;
	case SORT_TYPE_INT:		m_pArrayKeys = new CArray<int,int>;						break;
	case SORT_TYPE_INT64:	m_pArrayKeys = new CArray<__int64,__int64>;				break;
	case SORT_TYPE_DATE:	m_pArrayKeys = new CArray<COleDateTime,COleDateTime>;	break;
	}
}

/**********************************************************************/

CEntityTreeBranch::~CEntityTreeBranch()
{
	switch ( m_nSortType )
	{
	case SORT_TYPE_STRING:	delete (GetStringKeys() );	break;
	case SORT_TYPE_INT:		delete (GetIntKeys() );		break;
	case SORT_TYPE_INT64:	delete (GetInt64Keys() );	break;
	case SORT_TYPE_DATE:	delete (GetDateKeys() );	break;
	}
}

/**********************************************************************/

void CEntityTreeBranch::Clear()
{
	switch( m_nSortType )
	{
	case SORT_TYPE_STRING:	GetStringKeys() -> RemoveAll();		break;
	case SORT_TYPE_INT:		GetIntKeys() -> RemoveAll();		break;
	case SORT_TYPE_INT64:	GetInt64Keys() -> RemoveAll();		break;
	case SORT_TYPE_DATE:	GetDateKeys() -> RemoveAll();		break;
	}
	m_arrayLeaves.RemoveAll();
}

/**********************************************************************/

bool CEntityTreeBranch::FindLeafString( CString& strLabelToFind, int& nIndex )
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

bool CEntityTreeBranch::FindLeafInt( int nLabelToFind, int& nIndex )
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

bool CEntityTreeBranch::FindLeafInt64( __int64 nLabelToFind, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetIntKeys() -> GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		
		__int64 nLabel = GetInt64Keys() -> GetAt(nIndex);
		
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

bool CEntityTreeBranch::FindLeafDate( COleDateTime oleLabelToFind, int& nIndex )
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

int CEntityTreeBranch::ConsolidateLeaf( bool bNew, bool bEndBranch, int nLeafIdx, int nNextBranchIdx, CArray<int,int>& arrayEntityIdx, int& nNextEntityIdx )
{
	if ( TRUE == bNew )
	{
		if ( bEndBranch == FALSE )
		{
			CEntityTreeLeaf Leaf;
			Leaf.m_nNextBranchIdx = nNextBranchIdx;
			Leaf.m_nEntityIdx = nNextEntityIdx++;
			m_arrayLeaves.InsertAt( nLeafIdx, Leaf );
			return nNextBranchIdx;
		}
		else
		{
			CEntityTreeLeaf Leaf;
			Leaf.m_nNextBranchIdx = 0;
			Leaf.m_nEntityIdx = nNextEntityIdx++;
			m_arrayLeaves.InsertAt( nLeafIdx, Leaf );
			return 0;
		}
	}
	else
	{
		arrayEntityIdx.Add( m_arrayLeaves[nLeafIdx].m_nEntityIdx );

		if ( bEndBranch == FALSE )
			return m_arrayLeaves[nLeafIdx].m_nNextBranchIdx;
		else
			return 0;
	}
}

/**********************************************************************/

int CEntityTreeBranch::ConsolidateString( CString& str, bool bEndBranch, int nNextBranchIdx, CArray<int,int>& arrayEntityIdx, int& nNextEntityIdx, __int64& nNodeCount )
{
	int nLeafIdx;
	bool bNew = ( FindLeafString( str, nLeafIdx ) == FALSE );
	
	if ( TRUE == bNew )
	{
		GetStringKeys() -> InsertAt( nLeafIdx, str );
		nNodeCount++;
	}

	return ConsolidateLeaf( bNew, bEndBranch, nLeafIdx, nNextBranchIdx, arrayEntityIdx, nNextEntityIdx );
}

/**********************************************************************/

int CEntityTreeBranch::ConsolidateInt( int n, bool bEndBranch, int nNextBranchIdx, CArray<int,int>& arrayEntityIdx, int& nNextEntityIdx, __int64& nNodeCount )
{
	int nLeafIdx;
	bool bNew = ( FindLeafInt( n, nLeafIdx ) == FALSE );
	
	if ( TRUE == bNew )
	{
		GetIntKeys() -> InsertAt( nLeafIdx, n );
		nNodeCount++;
	}

	return ConsolidateLeaf( bNew, bEndBranch, nLeafIdx, nNextBranchIdx, arrayEntityIdx, nNextEntityIdx );
}

/**********************************************************************/

int CEntityTreeBranch::ConsolidateInt64( __int64 n, bool bEndBranch, int nNextBranchIdx, CArray<int,int>& arrayEntityIdx, int& nNextEntityIdx, __int64& nNodeCount )
{
	int nLeafIdx;
	bool bNew = ( FindLeafInt64( n, nLeafIdx ) == FALSE );
	
	if ( TRUE == bNew )
	{
		GetInt64Keys() -> InsertAt( nLeafIdx, n );
		nNodeCount++;
	}

	return ConsolidateLeaf( bNew, bEndBranch, nLeafIdx, nNextBranchIdx, arrayEntityIdx, nNextEntityIdx );
}

/**********************************************************************/

int CEntityTreeBranch::ConsolidateDate( COleDateTime date, bool bEndBranch, int nNextBranchIdx, CArray<int,int>& arrayEntityIdx, int& nNextEntityIdx, __int64& nNodeCount )
{
	int nLeafIdx;
	bool bNew = ( FindLeafDate( date, nLeafIdx ) == FALSE );
	
	if ( TRUE == bNew )
	{
		GetDateKeys() -> InsertAt( nLeafIdx, date );
		nNodeCount++;
	}

	return ConsolidateLeaf( bNew, bEndBranch, nLeafIdx, nNextBranchIdx, arrayEntityIdx, nNextEntityIdx );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTree::CEntityTree() : CPtrArray()
{
	m_nNodeCount = 0;
}

/**********************************************************************/

CEntityTree::~CEntityTree()
{
	for ( int n = GetSize() - 1; n >= 0; n-- )
		delete ( GetBranch( n ) );
}

/**********************************************************************/

void CEntityTree::DestroyTree()
{
	for ( int n = GetSize() - 1; n >= 0; n-- )
	{
		delete ( GetBranch( n ) );
		RemoveAt(n);
	}
	
	m_nNodeCount = 0;
}

/**********************************************************************/

void CEntityTree::ClearFieldStructure()
{
	m_FieldBuffer.RemoveAll();
}

/**********************************************************************/

void CEntityTree::AddField( int nType )
{
	switch ( nType )
	{
	case SORT_TYPE_STRING:
	case SORT_TYPE_INT:
	case SORT_TYPE_INT64:
	case SORT_TYPE_DATE:
		{
			CEntityTreeFieldBuffer buffer;
			buffer.m_nFieldType= nType;
			buffer.m_strBufferString = "";
			buffer.m_nBufferInt = 0;
			buffer.m_nBufferInt64 = 0;
			buffer.m_oleBufferDate = COleDateTime( 2003, 1, 1, 0, 0, 0 );
			m_FieldBuffer.Add( buffer );
		}
		break;
	}
}

/**********************************************************************/

void CEntityTree::SetBufferString(int nPos, const CString& string)
{
	if ((nPos >= 0) && (nPos < m_FieldBuffer.GetSize()))
	{
		m_FieldBuffer[nPos].m_strBufferString = string;
	}
}

/**********************************************************************/

void CEntityTree::SetBufferInt( int nPos, int n )
{
	if ( ( nPos >= 0 ) && ( nPos < m_FieldBuffer.GetSize() ) )
		m_FieldBuffer[ nPos ].m_nBufferInt = n;
}

/**********************************************************************/

void CEntityTree::SetBufferInt64( int nPos, __int64 n )
{
	if ( ( nPos >= 0 ) && ( nPos < m_FieldBuffer.GetSize() ) )
		m_FieldBuffer[ nPos ].m_nBufferInt64 = n;
}

/**********************************************************************/

void CEntityTree::SetBufferDate( int nPos, COleDateTime date )
{
	if ( ( nPos >= 0 ) && ( nPos < m_FieldBuffer.GetSize() ) )
		m_FieldBuffer[ nPos ].m_oleBufferDate = date;
}

/**********************************************************************/

void CEntityTree::Consolidate( CArray<int,int>& arrayEntityIdx, int& nNextEntityIdx )
{
	int nBranchToSearch = 0;
	int nFieldCount = m_FieldBuffer.GetSize();
	
	if ( ( GetSize() == 0 ) && ( nFieldCount != 0  ) )
	{
		CEntityTreeBranch* pArray = new CEntityTreeBranch( m_FieldBuffer[0].m_nFieldType );
		Add( pArray );
	}

	for ( int nPos = 0; nPos < nFieldCount; nPos++ )
	{
		bool bEndBranch = ( nPos == nFieldCount - 1 );
		CEntityTreeBranch* pBranch = GetBranch( nBranchToSearch );
		
		switch( m_FieldBuffer[nPos].m_nFieldType )
		{
		case SORT_TYPE_STRING:
			{
				CString strField = m_FieldBuffer[ nPos ].m_strBufferString;
				nBranchToSearch = pBranch -> ConsolidateString( strField, bEndBranch, GetSize(), arrayEntityIdx, nNextEntityIdx, m_nNodeCount );
			}
			break;

		case SORT_TYPE_INT:
			{
				int nField = m_FieldBuffer[ nPos ].m_nBufferInt;
				nBranchToSearch = pBranch -> ConsolidateInt( nField, bEndBranch, GetSize(), arrayEntityIdx, nNextEntityIdx, m_nNodeCount );
			}
			break;

		case SORT_TYPE_INT64:
			{
				__int64 nField = m_FieldBuffer[ nPos ].m_nBufferInt64;
				nBranchToSearch = pBranch -> ConsolidateInt64( nField, bEndBranch, GetSize(), arrayEntityIdx, nNextEntityIdx, m_nNodeCount );
			}
			break;

		case SORT_TYPE_DATE:
			{
				COleDateTime oleField = m_FieldBuffer[ nPos ].m_oleBufferDate;
				nBranchToSearch = pBranch -> ConsolidateDate( oleField, bEndBranch, GetSize(), arrayEntityIdx, nNextEntityIdx, m_nNodeCount );
			}
			break;
		}

		if ( nBranchToSearch == GetSize() )
		{
			CEntityTreeBranch* pBranch = new CEntityTreeBranch( m_FieldBuffer[ nPos + 1 ].m_nFieldType );
			Add( pBranch );
		}
	}
}

/**********************************************************************/

int CEntityTree::GetBranchSize( int nBranchIdx )
{
	if ( nBranchIdx >= 0 && nBranchIdx < GetSize() )
		return GetBranch( nBranchIdx ) -> m_arrayLeaves.GetSize();
	else
		return 0;
}

/**********************************************************************/

const char* CEntityTree::GetLabelString( int nBranchIdx, int nLeafIdx )
{
	return GetBranch( nBranchIdx ) -> GetStringKeys() -> GetAt(nLeafIdx);
}

/**********************************************************************/

int CEntityTree::GetLabelInt( int nBranchIdx, int nLeafIdx )
{
	return GetBranch( nBranchIdx ) -> GetIntKeys() -> GetAt(nLeafIdx);
}

/**********************************************************************/

__int64 CEntityTree::GetLabelInt64( int nBranchIdx, int nLeafIdx )
{
	return GetBranch( nBranchIdx ) -> GetInt64Keys() -> GetAt(nLeafIdx);
}

/**********************************************************************/

COleDateTime CEntityTree::GetLabelDate( int nBranchIdx, int nLeafIdx )
{
	return GetBranch( nBranchIdx ) -> GetDateKeys() -> GetAt(nLeafIdx);
}

/**********************************************************************/

int CEntityTree::GetNextBranchIdx( int nBranchIdx, int nLeafIdx )
{
	return GetBranch( nBranchIdx ) -> m_arrayLeaves[nLeafIdx].m_nNextBranchIdx;
}

/**********************************************************************/

int CEntityTree::GetEntityIdx( int nBranchIdx, int nLeafIdx )
{
	return GetBranch( nBranchIdx ) -> m_arrayLeaves[nLeafIdx].m_nEntityIdx;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeExportPaymentNode::CEntityTreeExportPaymentNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreeExportPaymentNode::Clear()
{
	m_nPaymentQty = 0;
	m_nInternalTranNo = 0;
	m_nCoversQty = 0;
	m_nTender = 0;
	m_nAmount = 0;
	m_nGratuity = 0;
	m_nCashback = 0;
	m_nSurplus = 0;
}

/**********************************************************************/

CEntityTreeExportPaymentNode& CEntityTreeExportPaymentNode::operator+ ( CEntityTreeExportPaymentNode& source )
{
	m_nPaymentQty += source.m_nPaymentQty;

	if ( source.m_nInternalTranNo != m_nInternalTranNo )
	{
		m_nInternalTranNo = source.m_nInternalTranNo;
		m_nCoversQty += source.m_nCoversQty;
	}

	m_nTender += source.m_nTender;
	m_nAmount += source.m_nAmount;
	m_nGratuity += source.m_nGratuity;
	m_nCashback += source.m_nCashback;
	m_nSurplus += source.m_nSurplus;
	return *this;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeExportPayment::CEntityTreeExportPayment()
{
	CEntityTreeExportPaymentNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreeExportPayment::Consolidate( CEntityTreeExportPaymentNode& NodeToAdd )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdx );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetSize() ) )
			m_arrayNodes[nEntityIdx] = m_arrayNodes[nEntityIdx] + NodeToAdd;
	}

	while ( m_nNextEntityIdx > m_arrayNodes.GetSize() )
		m_arrayNodes.Add( NodeToAdd );
}

/**********************************************************************/

void CEntityTreeExportPayment::ClearConsolidatedLevels()
{
	m_arrayNodes.RemoveAll();
	CEntityTree::DestroyTree();
	CEntityTreeExportPaymentNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreeExportPayment::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeExportPaymentNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetCount() ) )
		{
			Node = m_arrayNodes.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if ( FALSE == bGotNode )
		Node.Clear();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeExportSalesNode::CEntityTreeExportSalesNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreeExportSalesNode::Clear()
{
	m_dBaseQty = 0.0;
	m_dModQty = 0.0;
	m_dVal = 0.0;
	m_dDiscountVal = 0.0;
	m_dPremiumVal = 0.0;
	m_dTaxAmount = 0.0;
	m_dCost = 0.0;
	m_dWasteBaseQty = 0.0;
	m_dWasteModQty = 0.0;
	m_dWasteCost = 0.0;
}

/**********************************************************************/

CEntityTreeExportSalesNode& CEntityTreeExportSalesNode::operator+ ( CEntityTreeExportSalesNode& source )
{
	m_dBaseQty += source.m_dBaseQty;
	m_dModQty += source.m_dModQty;
	m_dVal += source.m_dVal;
	m_dDiscountVal += source.m_dDiscountVal;
	m_dPremiumVal += source.m_dPremiumVal;
	m_dTaxAmount += source.m_dTaxAmount;
	m_dCost += source.m_dCost;
	m_dWasteBaseQty += source.m_dWasteBaseQty;
	m_dWasteModQty += source.m_dWasteModQty;
	m_dWasteCost += source.m_dWasteCost;
	return *this;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeExportSales::CEntityTreeExportSales()
{
	CEntityTreeExportSalesNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreeExportSales::Consolidate( CEntityTreeExportSalesNode& NodeToAdd )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdx );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetSize() ) )
			m_arrayNodes[nEntityIdx] = m_arrayNodes[nEntityIdx] + NodeToAdd;
	}

	while ( m_nNextEntityIdx > m_arrayNodes.GetSize() )
		m_arrayNodes.Add( NodeToAdd );
}

/**********************************************************************/

void CEntityTreeExportSales::ClearConsolidatedLevels()
{
	m_arrayNodes.RemoveAll();
	CEntityTree::DestroyTree();
	CEntityTreeExportSalesNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreeExportSales::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeExportSalesNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetCount() ) )
		{
			Node = m_arrayNodes.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if ( FALSE == bGotNode )
		Node.Clear();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeExportS4LabourNode::CEntityTreeExportS4LabourNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreeExportS4LabourNode::Clear()
{
	m_dVal = 0.0;
	m_dDiscount = 0.0;
	m_dTaxAmount = 0.0;
}

/**********************************************************************/

CEntityTreeExportS4LabourNode& CEntityTreeExportS4LabourNode::operator+ ( CEntityTreeExportS4LabourNode& source )
{
	m_dVal += source.m_dVal;
	m_dDiscount += source.m_dDiscount;
	m_dTaxAmount += source.m_dTaxAmount;
	return *this;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeExportS4Labour::CEntityTreeExportS4Labour()
{
	CEntityTreeExportS4LabourNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreeExportS4Labour::Consolidate( CEntityTreeExportS4LabourNode& NodeToAdd )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdx );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetSize() ) )
			m_arrayNodes[nEntityIdx] = m_arrayNodes[nEntityIdx] + NodeToAdd;
	}

	while ( m_nNextEntityIdx > m_arrayNodes.GetSize() )
		m_arrayNodes.Add( NodeToAdd );
}

/**********************************************************************/

void CEntityTreeExportS4Labour::ClearConsolidatedLevels()
{
	m_arrayNodes.RemoveAll();
	CEntityTree::DestroyTree();
	CEntityTreeExportS4LabourNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreeExportS4Labour::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeExportS4LabourNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetCount() ) )
		{
			Node = m_arrayNodes.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if ( FALSE == bGotNode )
		Node.Clear();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeExportDiscountNode::CEntityTreeExportDiscountNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreeExportDiscountNode::Clear()
{
	m_nLineCount = 0;
	m_dDiscountVal = 0.0;
}

/**********************************************************************/

CEntityTreeExportDiscountNode& CEntityTreeExportDiscountNode::operator+ ( CEntityTreeExportDiscountNode& source )
{
	m_nLineCount += source.m_nLineCount;
	m_dDiscountVal += source.m_dDiscountVal;
	return *this;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeExportDiscount::CEntityTreeExportDiscount()
{
	CEntityTreeExportDiscountNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreeExportDiscount::Consolidate( CEntityTreeExportDiscountNode& NodeToAdd )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdx );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetSize() ) )
			m_arrayNodes[nEntityIdx] = m_arrayNodes[nEntityIdx] + NodeToAdd;
	}

	while ( m_nNextEntityIdx > m_arrayNodes.GetSize() )
		m_arrayNodes.Add( NodeToAdd );
}

/**********************************************************************/

void CEntityTreeExportDiscount::ClearConsolidatedLevels()
{
	m_arrayNodes.RemoveAll();
	CEntityTree::DestroyTree();
	CEntityTreeExportDiscountNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreeExportDiscount::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeExportDiscountNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetCount() ) )
		{
			Node = m_arrayNodes.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if ( FALSE == bGotNode )
		Node.Clear();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeExportVoidNode::CEntityTreeExportVoidNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreeExportVoidNode::Clear()
{
	m_nLineCount = 0;
	m_dBaseQty = 0.0;
	m_dModQty = 0.0;
	m_dVal = 0.0;
}

/**********************************************************************/

CEntityTreeExportVoidNode& CEntityTreeExportVoidNode::operator+ ( CEntityTreeExportVoidNode& source )
{
	m_nLineCount += source.m_nLineCount;
	m_dBaseQty += source.m_dBaseQty;
	m_dModQty += source.m_dModQty;
	m_dVal += source.m_dVal;
	return *this;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeExportVoid::CEntityTreeExportVoid()
{
	CEntityTreeExportVoidNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreeExportVoid::Consolidate( CEntityTreeExportVoidNode& NodeToAdd )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdx );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetSize() ) )
			m_arrayNodes[nEntityIdx] = m_arrayNodes[nEntityIdx] + NodeToAdd;
	}

	while ( m_nNextEntityIdx > m_arrayNodes.GetSize() )
		m_arrayNodes.Add( NodeToAdd );
}

/**********************************************************************/

void CEntityTreeExportVoid::ClearConsolidatedLevels()
{
	m_arrayNodes.RemoveAll();
	CEntityTree::DestroyTree();
	CEntityTreeExportVoidNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreeExportVoid::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeExportVoidNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetCount() ) )
		{
			Node = m_arrayNodes.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if ( FALSE == bGotNode )
		Node.Clear();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeGeneralSalesNode::CEntityTreeGeneralSalesNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreeGeneralSalesNode::Clear()
{
	for ( int n = 0; n < 8; n++ )
	{
		m_dQty[n] = 0.0;
		m_dVal[n] = 0.0;
	}
}

/**********************************************************************/

double CEntityTreeGeneralSalesNode::GetQty( int nWeekday )
{
	if ( ( nWeekday >= 0 ) && ( nWeekday < 8 ) )
		return m_dQty[nWeekday];
	else
		return 0.0;
}

/**********************************************************************/

double CEntityTreeGeneralSalesNode::GetVal( int nWeekday )
{
	if ( ( nWeekday >= 0 ) && ( nWeekday < 8 ) )
		return m_dVal[nWeekday];
	else
		return 0.0;
}

/**********************************************************************/

void CEntityTreeGeneralSalesNode::SetQty( int nWeekday, double dQty )
{
	if ( ( nWeekday >= 0 ) && ( nWeekday < 8 ) )
		m_dQty[nWeekday] = dQty;
}

/**********************************************************************/

void CEntityTreeGeneralSalesNode::SetVal( int nWeekday, double dVal )
{
	if ( ( nWeekday >= 0 ) && ( nWeekday < 8 ) )
		m_dVal[nWeekday] = dVal;
}

/**********************************************************************/

CEntityTreeGeneralSalesNode& CEntityTreeGeneralSalesNode::operator+ ( CEntityTreeGeneralSalesNode& source )
{
	for ( int n = 0; n < 8; n++ )
	{
		m_dQty[n] += source.m_dQty[n];
		m_dVal[n] += source.m_dVal[n];
	}
	return *this;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeGeneralSales::CEntityTreeGeneralSales()
{
	CEntityTreeGeneralSalesNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreeGeneralSales::Consolidate( CEntityTreeGeneralSalesNode& NodeToAdd )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdx );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetSize() ) )
			m_arrayNodes[nEntityIdx] = m_arrayNodes[nEntityIdx] + NodeToAdd;
	}

	while ( m_nNextEntityIdx > m_arrayNodes.GetSize() )
		m_arrayNodes.Add( NodeToAdd );
}

/**********************************************************************/

void CEntityTreeGeneralSales::ClearConsolidatedLevels()
{
	m_arrayNodes.RemoveAll();
	CEntityTree::DestroyTree();
	CEntityTreeGeneralSalesNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreeGeneralSales::GetGrandTotalNode( CEntityTreeGeneralSalesNode& Node )
{
	if ( m_arrayNodes.GetCount() > 0 )
		Node = m_arrayNodes.GetAt(0);
	else
		Node.Clear();
}

/**********************************************************************/

void CEntityTreeGeneralSales::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeGeneralSalesNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetCount() ) )
		{
			Node = m_arrayNodes.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if ( FALSE == bGotNode )
		Node.Clear();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreePluSalesHistoryBlock::CEntityTreePluSalesHistoryBlock()
{
	Clear();
}

/**********************************************************************/

void CEntityTreePluSalesHistoryBlock::Clear()
{
	m_dBaseQty = 0.0;
	m_dModQty = 0.0;
	m_dTotalVal = 0.0;
	m_dDiscountVal = 0.0;
	m_dPremiumVal = 0.0;
	m_dTaxAmount = 0.0;
	m_dCost = 0.0;
	m_dWasteCost = 0.0;
}

/**********************************************************************/

CEntityTreePluSalesHistoryBlock& CEntityTreePluSalesHistoryBlock::operator+ ( CEntityTreePluSalesHistoryBlock& source )
{
	m_dBaseQty += source.m_dBaseQty;
	m_dModQty += source.m_dModQty;
	m_dTotalVal += source.m_dTotalVal;
	m_dDiscountVal += source.m_dDiscountVal;
	m_dPremiumVal += source.m_dPremiumVal;
	m_dTaxAmount += source.m_dTaxAmount;
	m_dCost += source.m_dCost;
	m_dWasteCost += source.m_dWasteCost;
	return *this;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreePluSalesHistoryNormalNode::CEntityTreePluSalesHistoryNormalNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreePluSalesHistoryNormalNode::Clear()
{
	m_SalesBlock.Clear();
}

/**********************************************************************/

void CEntityTreePluSalesHistoryNormalNode::AppendDaily( int nReportMode, CCSV& csv, int& nCol, int nStartOffset, int nEndOffset )
{
}

/**********************************************************************/

CEntityTreePluSalesHistoryNormalNode& CEntityTreePluSalesHistoryNormalNode::operator+ ( CEntityTreePluSalesHistoryNormalNode& source )
{
	m_SalesBlock = m_SalesBlock + source.m_SalesBlock;
	return *this;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreePluSalesHistoryTwoBlockNode::CEntityTreePluSalesHistoryTwoBlockNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreePluSalesHistoryTwoBlockNode::Clear()
{
	m_SalesBlock[0].Clear();
	m_SalesBlock[1].Clear();
}

/**********************************************************************/

CEntityTreePluSalesHistoryTwoBlockNode& CEntityTreePluSalesHistoryTwoBlockNode::operator+ ( CEntityTreePluSalesHistoryTwoBlockNode& source )
{
	m_SalesBlock[0] = m_SalesBlock[0] + source.m_SalesBlock[0];
	m_SalesBlock[1] = m_SalesBlock[1] + source.m_SalesBlock[1];
	return *this;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreePluSalesHistoryWeeklyOneTotalNode::CEntityTreePluSalesHistoryWeeklyOneTotalNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreePluSalesHistoryWeeklyOneTotalNode::Clear()
{
	CEntityTreePluSalesHistoryNormalNode::Clear();

	for (int n = 0; n < 7; n++)
	{
		m_dDailyVal[n] = 0.0;
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistoryWeeklyOneTotalNode::AppendDaily( int nReportMode, CCSV& csv, int& nCol, int nStartOffset, int nEndOffset )
{
	for( int n = 0; n < 7; n++ )
	{
		if ( ( n >= nStartOffset ) && ( 6 - n >= nEndOffset ) ) 
		{
			if (SALESHISTORY_REPORTMODE_WEEKLY_QTY == nReportMode)
			{
				csv.SetAt(nCol++, m_dDailyVal[n], SysInfo.GetDPQty());
			}
			else
			{
				csv.SetAt(nCol++, m_dDailyVal[n], SysInfo.GetDPValue());
			}
		}
		else
		{
			csv.SetAt(nCol++, "");
		}
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistoryWeeklyOneTotalNode::Add(CEntityTreePluSalesHistoryWeeklyOneTotalNode& source, int nHint)
{
	m_SalesBlock = m_SalesBlock + source.m_SalesBlock;

	if ((nHint >= 0) && (nHint < 7))
	{
		m_dDailyVal[nHint] += source.m_dDailyVal[nHint];
	}
	else
	{
		for (int n = 0; n < 7; n++)
		{
			m_dDailyVal[n] += source.m_dDailyVal[n];
		}
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreePluSalesHistoryWeeklyTwoTotalNode::CEntityTreePluSalesHistoryWeeklyTwoTotalNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreePluSalesHistoryWeeklyTwoTotalNode::Clear()
{
	CEntityTreePluSalesHistoryNormalNode::Clear();

	for (int n = 0; n < 7; n++)
	{
		m_dDailyQty[n] = 0.0;
		m_dDailyVal[n] = 0.0;
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistoryWeeklyTwoTotalNode::AppendDaily(int nReportMode, CCSV& csv, int& nCol, int nStartOffset, int nEndOffset)
{
	for (int n = 0; n < 7; n++)
	{
		if ((n >= nStartOffset) && (6 - n >= nEndOffset))
		{
			csv.SetAt(nCol++, m_dDailyQty[n], SysInfo.GetDPQty());
			csv.SetAt(nCol++, m_dDailyVal[n], SysInfo.GetDPValue());
		}
		else
		{
			csv.SetAt(nCol++, "");
			csv.SetAt(nCol++, "");
		}
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistoryWeeklyTwoTotalNode::Add(CEntityTreePluSalesHistoryWeeklyTwoTotalNode& source, int nHint)
{
	m_SalesBlock = m_SalesBlock + source.m_SalesBlock;

	if ((nHint >= 0) && (nHint < 7))
	{
		m_dDailyQty[nHint] += source.m_dDailyQty[nHint];
		m_dDailyVal[nHint] += source.m_dDailyVal[nHint];
	}
	else
	{
		for (int n = 0; n < 7; n++)
		{
			m_dDailyQty[n] += source.m_dDailyQty[n];
			m_dDailyVal[n] += source.m_dDailyVal[n];
		}
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreePluSalesHistoryMonthlyNode::CEntityTreePluSalesHistoryMonthlyNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreePluSalesHistoryMonthlyNode::Clear()
{
	CEntityTreePluSalesHistoryNormalNode::Clear();

	for( int n = 0; n < 31; n++ )
		m_dDailyVal[n] = 0.0;
}

/**********************************************************************/

void CEntityTreePluSalesHistoryMonthlyNode::AppendDaily( int nReportMode, CCSV& csv, int& nCol, int nStartOffset, int nEndOffset )
{
	for( int n = 0; n < 31; n++ )
	{
		if ( SALESHISTORY_REPORTMODE_MONTHLY_QTY == nReportMode )
			csv.SetAt( nCol++, m_dDailyVal[n], SysInfo.GetDPQty() );
		else
			csv.SetAt( nCol++, m_dDailyVal[n], SysInfo.GetDPValue() );
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistoryMonthlyNode::Add( CEntityTreePluSalesHistoryMonthlyNode& source, int nHint )
{
	m_SalesBlock = m_SalesBlock + source.m_SalesBlock;

	if ( ( nHint >= 0 ) && ( nHint < 31 ) )
		m_dDailyVal[nHint] += source.m_dDailyVal[nHint];
	else
	{
		for( int n = 0; n < 31; n++ )
			m_dDailyVal[n] += source.m_dDailyVal[n];
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreePluSalesHistory::CEntityTreePluSalesHistory()
{
	m_bAutoFillDates = FALSE;
	m_autoFillDateFrom = COleDateTime( 2017, 1, 1, 0, 0, 0 );
	m_autoFillDateTo = COleDateTime( 2017, 1, 1, 0, 0, 0 );
	m_nAutoFillStartOffset = 0;
	m_nAutoFillEndOffset = 0;

	{
		CEntityTreePluSalesHistoryNormalNode Node;
		m_arrayNodesNormal.Add( Node );
		m_nNextEntityIdxNormal = 1;
	}
	{
		CEntityTreePluSalesHistoryTwoBlockNode Node;
		m_arrayNodesTwoBlock.Add( Node );
		m_nNextEntityIdxTwoBlock = 1;
	}
	{
		CEntityTreePluSalesHistoryWeeklyOneTotalNode Node;
		m_arrayNodesWeeklyOneTotal.Add( Node );
		m_nNextEntityIdxWeeklyOneTotal = 1;
	}
	{
		CEntityTreePluSalesHistoryWeeklyTwoTotalNode Node;
		m_arrayNodesWeeklyTwoTotal.Add(Node);
		m_nNextEntityIdxWeeklyTwoTotal = 1;
	}
	{
		CEntityTreePluSalesHistoryMonthlyNode Node;
		m_arrayNodesMonthly.Add( Node );
		m_nNextEntityIdxMonthly = 1;
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistory::SetAutoFillDates( COleDateTime& dateFrom, COleDateTime& dateTo, int nStartOffset, int nEndOffset )
{ 
	m_bAutoFillDates = TRUE; 
	m_autoFillDateFrom = dateFrom; 
	m_autoFillDateTo = dateTo; 
	m_nAutoFillStartOffset = nStartOffset;
	m_nAutoFillEndOffset = nEndOffset;
}

/**********************************************************************/

int CEntityTreePluSalesHistory::GetWeeklyStartOffset( COleDateTime& oleWeekStart )
{
	int nResult = 0;
	
	if ( TRUE == m_bAutoFillDates )
		if ( oleWeekStart <= m_autoFillDateFrom )
			nResult = m_nAutoFillStartOffset;

	return nResult;
}

/**********************************************************************/

int CEntityTreePluSalesHistory::GetWeeklyEndOffset( COleDateTime& oleWeekStart )
{
	int nResult = 0;
	
	if (TRUE == m_bAutoFillDates)
	{
		if (oleWeekStart >= m_autoFillDateTo)
		{
			nResult = m_nAutoFillEndOffset;
		}
	}

	return nResult;
}

/**********************************************************************/

void CEntityTreePluSalesHistory::Consolidate( CEntityTreePluSalesHistoryNormalNode& NodeToAdd )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdxNormal );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodesNormal.GetSize() ) )
			m_arrayNodesNormal[nEntityIdx] = m_arrayNodesNormal[nEntityIdx] + NodeToAdd;
	}

	while ( m_nNextEntityIdxNormal > m_arrayNodesNormal.GetSize() )
		m_arrayNodesNormal.Add( NodeToAdd );
}

/**********************************************************************/

void CEntityTreePluSalesHistory::Consolidate( CEntityTreePluSalesHistoryTwoBlockNode& NodeToAdd )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdxTwoBlock );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ((nEntityIdx >= 0) && (nEntityIdx < m_arrayNodesTwoBlock.GetSize()))
		{
			m_arrayNodesTwoBlock[nEntityIdx] = m_arrayNodesTwoBlock[nEntityIdx] + NodeToAdd;
		}
	}

	while (m_nNextEntityIdxTwoBlock > m_arrayNodesTwoBlock.GetSize())
	{
		m_arrayNodesTwoBlock.Add(NodeToAdd);
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistory::Consolidate( CEntityTreePluSalesHistoryWeeklyOneTotalNode& NodeToAdd, int nHint, bool bAllowRecursion )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdxWeeklyOneTotal );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ((nEntityIdx >= 0) && (nEntityIdx < m_arrayNodesWeeklyOneTotal.GetSize()))
		{
			m_arrayNodesWeeklyOneTotal[nEntityIdx].Add(NodeToAdd, nHint);
		}
	}

	bool bNewNode = FALSE;
	while ( m_nNextEntityIdxWeeklyOneTotal > m_arrayNodesWeeklyOneTotal.GetSize() )
	{
		bNewNode = TRUE;
		m_arrayNodesWeeklyOneTotal.Add( NodeToAdd );
	}

	if ( ( TRUE == bNewNode ) && ( TRUE == m_bAutoFillDates ) && ( TRUE == bAllowRecursion ) )
	{
		CEntityTreePluSalesHistoryWeeklyOneTotalNode DummyNode;

		COleDateTime dateFill = m_autoFillDateFrom;
		
		while( dateFill <= m_autoFillDateTo )
		{
			SetBufferDate( m_FieldBuffer.GetSize() - 1, dateFill );
			Consolidate( DummyNode, -1, FALSE );
			dateFill += COleDateTimeSpan( 7, 0, 0, 0 );
		}
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistory::Consolidate(CEntityTreePluSalesHistoryWeeklyTwoTotalNode& NodeToAdd, int nHint, bool bAllowRecursion)
{
	CArray<int, int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate(arrayEntityIdx, m_nNextEntityIdxWeeklyTwoTotal);

	for (int n = 0; n < arrayEntityIdx.GetSize(); n++)
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ((nEntityIdx >= 0) && (nEntityIdx < m_arrayNodesWeeklyTwoTotal.GetSize()))
		{
			m_arrayNodesWeeklyTwoTotal[nEntityIdx].Add(NodeToAdd, nHint);
		}
	}

	bool bNewNode = FALSE;
	while (m_nNextEntityIdxWeeklyTwoTotal > m_arrayNodesWeeklyTwoTotal.GetSize())
	{
		bNewNode = TRUE;
		m_arrayNodesWeeklyTwoTotal.Add(NodeToAdd);
	}

	if ((TRUE == bNewNode) && (TRUE == m_bAutoFillDates) && (TRUE == bAllowRecursion))
	{
		CEntityTreePluSalesHistoryWeeklyTwoTotalNode DummyNode;

		COleDateTime dateFill = m_autoFillDateFrom;

		while (dateFill <= m_autoFillDateTo)
		{
			SetBufferDate(m_FieldBuffer.GetSize() - 1, dateFill);
			Consolidate(DummyNode, -1, FALSE);
			dateFill += COleDateTimeSpan(7, 0, 0, 0);
		}
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistory::Consolidate( CEntityTreePluSalesHistoryMonthlyNode& NodeToAdd, int nHint )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdxMonthly );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ((nEntityIdx >= 0) && (nEntityIdx < m_arrayNodesMonthly.GetSize()))
		{
			m_arrayNodesMonthly[nEntityIdx].Add(NodeToAdd, nHint);
		}
	}

	while (m_nNextEntityIdxMonthly > m_arrayNodesMonthly.GetSize())
	{
		m_arrayNodesMonthly.Add(NodeToAdd);
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistory::ClearConsolidatedLevels()
{
	m_arrayNodesNormal.RemoveAll();
	m_arrayNodesTwoBlock.RemoveAll();
	m_arrayNodesWeeklyOneTotal.RemoveAll();
	m_arrayNodesWeeklyTwoTotal.RemoveAll();
	m_arrayNodesMonthly.RemoveAll();
	
	CEntityTree::DestroyTree();
	
	{
		CEntityTreePluSalesHistoryNormalNode Node;
		m_arrayNodesNormal.Add( Node );
		m_nNextEntityIdxNormal = 1;
	}
	{
		CEntityTreePluSalesHistoryTwoBlockNode Node;
		m_arrayNodesTwoBlock.Add( Node );
		m_nNextEntityIdxTwoBlock = 1;
	}
	{
		CEntityTreePluSalesHistoryWeeklyOneTotalNode Node;
		m_arrayNodesWeeklyOneTotal.Add( Node );
		m_nNextEntityIdxWeeklyOneTotal = 1;
	}
	{
		CEntityTreePluSalesHistoryWeeklyTwoTotalNode Node;
		m_arrayNodesWeeklyTwoTotal.Add(Node);
		m_nNextEntityIdxWeeklyTwoTotal = 1;
	}
	{
		CEntityTreePluSalesHistoryMonthlyNode Node;
		m_arrayNodesMonthly.Add( Node );
		m_nNextEntityIdxMonthly = 1;
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistory::GetGrandTotalNode( CEntityTreePluSalesHistoryNormalNode& Node )
{
	if ( m_arrayNodesNormal.GetCount() > 0 )
		Node = m_arrayNodesNormal.GetAt(0);
	else
		Node.Clear();
}

/**********************************************************************/

void CEntityTreePluSalesHistory::GetGrandTotalNode( CEntityTreePluSalesHistoryTwoBlockNode& Node )
{
	if ( m_arrayNodesTwoBlock.GetCount() > 0 )
		Node = m_arrayNodesTwoBlock.GetAt(0);
	else
		Node.Clear();
}

/**********************************************************************/

void CEntityTreePluSalesHistory::GetGrandTotalNode(CEntityTreePluSalesHistoryWeeklyOneTotalNode& Node)
{
	if (m_arrayNodesWeeklyOneTotal.GetCount() > 0)
	{
		Node = m_arrayNodesWeeklyOneTotal.GetAt(0);
	}
	else
	{
		Node.Clear();
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistory::GetGrandTotalNode(CEntityTreePluSalesHistoryWeeklyTwoTotalNode& Node)
{
	if (m_arrayNodesWeeklyTwoTotal.GetCount() > 0)
	{
		Node = m_arrayNodesWeeklyTwoTotal.GetAt(0);
	}
	else
	{
		Node.Clear();
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistory::GetGrandTotalNode( CEntityTreePluSalesHistoryMonthlyNode& Node )
{
	if (m_arrayNodesMonthly.GetCount() > 0)
	{
		Node = m_arrayNodesMonthly.GetAt(0);
	}
	else
	{
		Node.Clear();
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistory::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreePluSalesHistoryNormalNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodesNormal.GetCount() ) )
		{
			Node = m_arrayNodesNormal.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if ( FALSE == bGotNode )
		Node.Clear();
}

/**********************************************************************/

void CEntityTreePluSalesHistory::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreePluSalesHistoryTwoBlockNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodesTwoBlock.GetCount() ) )
		{
			Node = m_arrayNodesTwoBlock.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if ( FALSE == bGotNode )
		Node.Clear();
}

/**********************************************************************/

void CEntityTreePluSalesHistory::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreePluSalesHistoryWeeklyOneTotalNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodesWeeklyOneTotal.GetCount() ) )
		{
			Node = m_arrayNodesWeeklyOneTotal.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if (FALSE == bGotNode)
	{
		Node.Clear();
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistory::GetNode(int nBranchIdx, int nLeafIdx, CEntityTreePluSalesHistoryWeeklyTwoTotalNode& Node)
{
	bool bGotNode = FALSE;
	if ((nLeafIdx >= 0) && (nLeafIdx < GetBranchSize(nBranchIdx)))
	{
		int nEntityIdx = GetEntityIdx(nBranchIdx, nLeafIdx);

		if ((nEntityIdx >= 0) && (nEntityIdx < m_arrayNodesWeeklyTwoTotal.GetCount()))
		{
			Node = m_arrayNodesWeeklyTwoTotal.GetAt(nEntityIdx);
			bGotNode = TRUE;
		}
	}

	if (FALSE == bGotNode)
	{
		Node.Clear();
	}
}

/**********************************************************************/

void CEntityTreePluSalesHistory::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreePluSalesHistoryMonthlyNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodesMonthly.GetCount() ) )
		{
			Node = m_arrayNodesMonthly.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if ( FALSE == bGotNode )
		Node.Clear();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeStockSalesHistoryNormalNode::CEntityTreeStockSalesHistoryNormalNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreeStockSalesHistoryNormalNode::Clear()
{
	m_dQty = 0.0;
	m_dTotalVal = 0.0;
	m_dTaxAmount = 0.0;
	m_dCost = 0.0;
	m_dWasteCost = 0.0;
}

/**********************************************************************/

void CEntityTreeStockSalesHistoryNormalNode::AppendDaily( int nReportMode, int nSubUnits, CCSV& csv, int nStartOffset, int nEndOffset )
{
}

/**********************************************************************/

CEntityTreeStockSalesHistoryNormalNode& CEntityTreeStockSalesHistoryNormalNode::operator+ ( CEntityTreeStockSalesHistoryNormalNode& source )
{
	m_dQty += source.m_dQty;
	m_dTotalVal += source.m_dTotalVal;
	m_dTaxAmount += source.m_dTaxAmount;
	m_dCost += source.m_dCost;
	m_dWasteCost += source.m_dWasteCost;
	return *this;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeStockSalesHistoryWeeklyNode::CEntityTreeStockSalesHistoryWeeklyNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreeStockSalesHistoryWeeklyNode::Clear()
{
	CEntityTreeStockSalesHistoryNormalNode::Clear();

	for( int n = 0; n < 7; n++ )
		m_dDailyVal[n] = 0.0;
}

/**********************************************************************/

void CEntityTreeStockSalesHistoryWeeklyNode::AppendDaily( int nReportMode, int nSubUnits, CCSV& csv, int nStartOffset, int nEndOffset )
{
	for( int n = 0; n < 7; n++ )
	{
		if ( ( n >= nStartOffset ) && ( 6 - n >= nEndOffset ) ) 
		{
			if ( SALESHISTORY_REPORTMODE_WEEKLY_QTY == nReportMode )
			{
				if ( 1 == nSubUnits )
					csv.Add( m_dDailyVal[n], SysInfo.GetDPQty() );
				else
				{
					CString strQty;
					::GetStockLevelString( m_dDailyVal[n], nSubUnits, strQty );
					csv.Add( strQty );
				}
			}
			else
				csv.Add( m_dDailyVal[n], SysInfo.GetDPValue() );
		}
		else
			csv.Add( "" );
	}
}

/**********************************************************************/

void CEntityTreeStockSalesHistoryWeeklyNode::Add ( CEntityTreeStockSalesHistoryWeeklyNode& source, int nHint )
{
	m_dQty += source.m_dQty;
	m_dTotalVal += source.m_dTotalVal;
	m_dTaxAmount += source.m_dTaxAmount;
	m_dCost += source.m_dCost;
	m_dWasteCost += source.m_dWasteCost;

	if ((nHint >= 0) && (nHint < 7))
	{
		m_dDailyVal[nHint] += source.m_dDailyVal[nHint];
	}
	else
	{
		for (int n = 0; n < 7; n++)
		{
			m_dDailyVal[n] += source.m_dDailyVal[n];
		}
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeStockSalesHistoryMonthlyNode::CEntityTreeStockSalesHistoryMonthlyNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreeStockSalesHistoryMonthlyNode::Clear()
{
	CEntityTreeStockSalesHistoryNormalNode::Clear();

	for( int n = 0; n < 31; n++ )
		m_dDailyVal[n] = 0.0;
}

/**********************************************************************/

void CEntityTreeStockSalesHistoryMonthlyNode::AppendDaily( int nReportMode, int nSubUnits, CCSV& csv, int nStartOffset, int nEndOffset )
{
	for( int n = 0; n < 31; n++ )
	{
		if ( SALESHISTORY_REPORTMODE_MONTHLY_QTY == nReportMode )
		{
			if ( 1 == nSubUnits )
				csv.Add( m_dDailyVal[n], SysInfo.GetDPQty() );
			else
			{
				CString strQty;
				::GetStockLevelString( m_dDailyVal[n], nSubUnits, strQty );
				csv.Add( strQty );
			}
		}
		else
			csv.Add( m_dDailyVal[n], SysInfo.GetDPValue() );
	}
}

/**********************************************************************/

void CEntityTreeStockSalesHistoryMonthlyNode::Add ( CEntityTreeStockSalesHistoryMonthlyNode& source, int nHint )
{
	m_dQty += source.m_dQty;
	m_dTotalVal += source.m_dTotalVal;
	m_dTaxAmount += source.m_dTaxAmount;
	m_dCost += source.m_dCost;
	m_dWasteCost += source.m_dWasteCost;

	if ( ( nHint >= 0 ) && ( nHint < 31 ) )
		m_dDailyVal[nHint] += source.m_dDailyVal[nHint];
	else
	{
		for( int n = 0; n < 31; n++ )
			m_dDailyVal[n] += source.m_dDailyVal[n];
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreeStockSalesHistory::CEntityTreeStockSalesHistory()
{
	m_bAutoFillDates = FALSE;
	m_autoFillDateFrom = COleDateTime( 2017, 1, 1, 0, 0, 0 );
	m_autoFillDateTo = COleDateTime( 2017, 1, 1, 0, 0, 0 );
	m_nAutoFillStartOffset = 0;
	m_nAutoFillEndOffset = 0;

	{
		CEntityTreeStockSalesHistoryNormalNode Node;
		m_arrayNodesNormal.Add( Node );
		m_nNextEntityIdxNormal = 1;
	}
	{
		CEntityTreeStockSalesHistoryWeeklyNode Node;
		m_arrayNodesWeekly.Add( Node );
		m_nNextEntityIdxWeekly = 1;
	}
	{
		CEntityTreeStockSalesHistoryMonthlyNode Node;
		m_arrayNodesMonthly.Add( Node );
		m_nNextEntityIdxMonthly = 1;
	}
}

/**********************************************************************/

void CEntityTreeStockSalesHistory::SetAutoFillDates( COleDateTime& dateFrom, COleDateTime& dateTo, int nStartOffset, int nEndOffset )
{ 
	m_bAutoFillDates = TRUE; 
	m_autoFillDateFrom = dateFrom; 
	m_autoFillDateTo = dateTo; 
	m_nAutoFillStartOffset = nStartOffset;
	m_nAutoFillEndOffset = nEndOffset;
}

/**********************************************************************/

int CEntityTreeStockSalesHistory::GetWeeklyStartOffset( COleDateTime& oleWeekStart )
{
	int nResult = 0;
	
	if ( TRUE == m_bAutoFillDates )
		if ( oleWeekStart <= m_autoFillDateFrom )
			nResult = m_nAutoFillStartOffset;

	return nResult;
}

/**********************************************************************/

int CEntityTreeStockSalesHistory::GetWeeklyEndOffset( COleDateTime& oleWeekStart )
{
	int nResult = 0;
	
	if ( TRUE == m_bAutoFillDates )
		if ( oleWeekStart >= m_autoFillDateTo )
			nResult = m_nAutoFillEndOffset;

	return nResult;
}

/**********************************************************************/

void CEntityTreeStockSalesHistory::Consolidate( CEntityTreeStockSalesHistoryNormalNode& NodeToAdd )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdxNormal );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodesNormal.GetSize() ) )
			m_arrayNodesNormal[nEntityIdx] = m_arrayNodesNormal[nEntityIdx] + NodeToAdd;
	}

	while ( m_nNextEntityIdxNormal > m_arrayNodesNormal.GetSize() )
		m_arrayNodesNormal.Add( NodeToAdd );
}

/**********************************************************************/

void CEntityTreeStockSalesHistory::Consolidate( CEntityTreeStockSalesHistoryWeeklyNode& NodeToAdd, int nHint, bool bAllowRecursion )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdxWeekly );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodesWeekly.GetSize() ) )
			m_arrayNodesWeekly[nEntityIdx].Add( NodeToAdd, nHint );
	}

	bool bNewNode = FALSE;
	while ( m_nNextEntityIdxWeekly > m_arrayNodesWeekly.GetSize() )
	{
		bNewNode = TRUE;
		m_arrayNodesWeekly.Add( NodeToAdd );
	}

	if ( ( TRUE == bNewNode ) && ( TRUE == m_bAutoFillDates ) && ( TRUE == bAllowRecursion ) )
	{
		CEntityTreeStockSalesHistoryWeeklyNode DummyNode;

		COleDateTime dateFill = m_autoFillDateFrom;
		
		while( dateFill <= m_autoFillDateTo )
		{
			SetBufferDate( m_FieldBuffer.GetSize() - 1, dateFill );
			Consolidate( DummyNode, -1, FALSE );
			dateFill += COleDateTimeSpan( 7, 0, 0, 0 );
		}
	}
}

/**********************************************************************/

void CEntityTreeStockSalesHistory::Consolidate( CEntityTreeStockSalesHistoryMonthlyNode& NodeToAdd, int nHint )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdxMonthly );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodesMonthly.GetSize() ) )
			m_arrayNodesMonthly[nEntityIdx].Add( NodeToAdd, nHint );
	}

	while ( m_nNextEntityIdxMonthly > m_arrayNodesMonthly.GetSize() )
		m_arrayNodesMonthly.Add( NodeToAdd );
}

/**********************************************************************/

void CEntityTreeStockSalesHistory::ClearConsolidatedLevels()
{
	m_arrayNodesNormal.RemoveAll();
	m_arrayNodesWeekly.RemoveAll();
	m_arrayNodesMonthly.RemoveAll();
	
	CEntityTree::DestroyTree();
	
	{
		CEntityTreeStockSalesHistoryNormalNode Node;
		m_arrayNodesNormal.Add( Node );
		m_nNextEntityIdxNormal = 1;
	}
	{
		CEntityTreeStockSalesHistoryWeeklyNode Node;
		m_arrayNodesWeekly.Add( Node );
		m_nNextEntityIdxWeekly = 1;
	}
	{
		CEntityTreeStockSalesHistoryMonthlyNode Node;
		m_arrayNodesMonthly.Add( Node );
		m_nNextEntityIdxMonthly = 1;
	}
}

/**********************************************************************/

void CEntityTreeStockSalesHistory::GetGrandTotalNode( CEntityTreeStockSalesHistoryNormalNode& Node )
{
	if ( m_arrayNodesNormal.GetCount() > 0 )
		Node = m_arrayNodesNormal.GetAt(0);
	else
		Node.Clear();
}

/**********************************************************************/

void CEntityTreeStockSalesHistory::GetGrandTotalNode( CEntityTreeStockSalesHistoryWeeklyNode& Node )
{
	if ( m_arrayNodesWeekly.GetCount() > 0 )
		Node = m_arrayNodesWeekly.GetAt(0);
	else
		Node.Clear();
}

/**********************************************************************/

void CEntityTreeStockSalesHistory::GetGrandTotalNode( CEntityTreeStockSalesHistoryMonthlyNode& Node )
{
	if ( m_arrayNodesMonthly.GetCount() > 0 )
		Node = m_arrayNodesMonthly.GetAt(0);
	else
		Node.Clear();
}

/**********************************************************************/

void CEntityTreeStockSalesHistory::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeStockSalesHistoryNormalNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodesNormal.GetCount() ) )
		{
			Node = m_arrayNodesNormal.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if ( FALSE == bGotNode )
		Node.Clear();
}

/**********************************************************************/

void CEntityTreeStockSalesHistory::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeStockSalesHistoryWeeklyNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodesWeekly.GetCount() ) )
		{
			Node = m_arrayNodesWeekly.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if ( FALSE == bGotNode )
		Node.Clear();
}

/**********************************************************************/

void CEntityTreeStockSalesHistory::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeStockSalesHistoryMonthlyNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodesMonthly.GetCount() ) )
		{
			Node = m_arrayNodesMonthly.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if ( FALSE == bGotNode )
		Node.Clear();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreePluStockNode::CEntityTreePluStockNode()
{
	Clear();
}

/**********************************************************************/

void CEntityTreePluStockNode::Clear()
{
	m_dApparentQty = 0.0;
	m_dApparentCost = 0.0;
	m_dApparentRetail = 0.0;
	m_dDeliveryQty = 0.0;
	m_dDeliveryVal = 0.0;
}

/**********************************************************************/

CEntityTreePluStockNode& CEntityTreePluStockNode::operator+ ( CEntityTreePluStockNode& source )
{
	m_dApparentQty += source.m_dApparentQty;
	m_dApparentCost += source.m_dApparentCost;
	m_dApparentRetail += source.m_dApparentRetail;
	m_dDeliveryQty += source.m_dDeliveryQty;
	m_dDeliveryVal += source.m_dDeliveryVal;
	return *this;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEntityTreePluStock::CEntityTreePluStock()
{
	CEntityTreePluStockNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreePluStock::Consolidate( CEntityTreePluStockNode& NodeToAdd )
{
	CArray<int,int> arrayEntityIdx;
	arrayEntityIdx.Add(0);

	CEntityTree::Consolidate( arrayEntityIdx, m_nNextEntityIdx );

	for ( int n = 0; n < arrayEntityIdx.GetSize(); n++ )
	{
		int nEntityIdx = arrayEntityIdx.GetAt(n);
		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetSize() ) )
			m_arrayNodes[nEntityIdx] = m_arrayNodes[nEntityIdx] + NodeToAdd;
	}

	while ( m_nNextEntityIdx > m_arrayNodes.GetSize() )
		m_arrayNodes.Add( NodeToAdd );
}

/**********************************************************************/

void CEntityTreePluStock::ClearConsolidatedLevels()
{
	m_arrayNodes.RemoveAll();
	CEntityTree::DestroyTree();
	CEntityTreePluStockNode Node;
	m_arrayNodes.Add( Node );
	m_nNextEntityIdx = 1;
}

/**********************************************************************/

void CEntityTreePluStock::GetGrandTotalNode( CEntityTreePluStockNode& Node )
{
	if ( m_arrayNodes.GetCount() > 0 )
		Node = m_arrayNodes.GetAt(0);
	else
		Node.Clear();
}

/**********************************************************************/

void CEntityTreePluStock::GetNode( int nBranchIdx, int nLeafIdx, CEntityTreePluStockNode& Node )
{
	bool bGotNode = FALSE;
	if ( ( nLeafIdx >= 0 ) && ( nLeafIdx < GetBranchSize( nBranchIdx ) ) )
	{
		int nEntityIdx = GetEntityIdx( nBranchIdx, nLeafIdx );

		if ( ( nEntityIdx >= 0 ) && ( nEntityIdx < m_arrayNodes.GetCount() ) )
		{
			Node = m_arrayNodes.GetAt( nEntityIdx );
			bGotNode = TRUE;
		}
	}

	if ( FALSE == bGotNode )
		Node.Clear();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/


