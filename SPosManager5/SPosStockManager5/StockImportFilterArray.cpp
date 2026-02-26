/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/
#include "StockImportFilterArray.h"
/**********************************************************************/

CStockImportFilterItem::CStockImportFilterItem()
{
	m_nStockIdx = 0;
	m_nStockSuppIdx = 0;
	m_dCurrentSUQty = 0.0;
	m_dImportDUQty = 0.0;
	m_dImportSUQty = 0.0;
	m_bSelected = TRUE;
	m_bSelectBuffer = TRUE;
}

/**********************************************************************/

double CStockImportFilterItem::GetNewStockLevel( int nAction )
{
	switch( nAction )
	{
	case OVERWRITE_STOCK_LEVELS:
		return m_dImportSUQty;

	case SUBTRACT_STOCK_LEVELS:
		return m_dCurrentSUQty - m_dImportSUQty;

	case ADD_STOCK_LEVELS:
	default:
		return m_dCurrentSUQty + m_dImportSUQty;
	}
}

/**********************************************************************/

CStockImportFilterArray::CStockImportFilterArray(void)
{
	m_nSelectionCount = 0;
	m_bSelectionModified = FALSE;
}

/**********************************************************************/

void CStockImportFilterArray::RemoveAll()
{
	m_arrayItems.RemoveAll();
	m_nSelectionCount = 0;
	m_bSelectionModified = FALSE;
}

/**********************************************************************/

bool CStockImportFilterArray::FindItem( int nStockIdx, int nStockSuppIdx, int& nPos )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while( nStart <= nEnd )
	{
		nPos = ( nStart + nEnd ) / 2;

		if ( m_arrayItems[ nPos ].m_nStockIdx > nStockIdx )
			nEnd = nPos - 1;
		else if ( m_arrayItems[ nPos ].m_nStockIdx < nStockIdx )
			nStart = nPos + 1;
		else if ( m_arrayItems[ nPos ].m_nStockSuppIdx > nStockSuppIdx )
			nEnd = nPos - 1;
		else if ( m_arrayItems[ nPos ].m_nStockSuppIdx < nStockSuppIdx )
			nStart = nPos + 1;
		else
			return TRUE;
	}

	nPos = nStart;
	return FALSE;
}

/**********************************************************************/

void CStockImportFilterArray::AddItem( int nStockIdx, int nStockSuppIdx, double dImportDUQty, double dImportSUQty )
{
	int nPos;
	if ( FindItem( nStockIdx, nStockSuppIdx, nPos ) == TRUE )
	{
		m_arrayItems[ nPos ].m_dImportDUQty += dImportDUQty;
		m_arrayItems[ nPos ].m_dImportSUQty += dImportSUQty;
	}
	else
	{
		CStockImportFilterItem item;
		item.m_nStockIdx = nStockIdx;
		item.m_nStockSuppIdx = nStockSuppIdx;
		item.m_dCurrentSUQty = 0.0;
		item.m_dImportDUQty = dImportDUQty;
		item.m_dImportSUQty = dImportSUQty;
		item.m_bSelected = TRUE;
		m_arrayItems.InsertAt( nPos, item );
		m_nSelectionCount++;
	}
}

/**********************************************************************/

void CStockImportFilterArray::GetItem( int nIndex, CStockImportFilterItem& item )
{
	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
		item = m_arrayItems.GetAt( nIndex );
}

/**********************************************************************/

void CStockImportFilterArray::SetItem( int nIndex, CStockImportFilterItem& item )
{
	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		bool bOldSelect = m_arrayItems[nIndex].m_bSelected;
		bool bNewSelect = item.m_bSelected;

		m_arrayItems.SetAt( nIndex, item );

		if ( bOldSelect != bNewSelect )
		{
			if ( TRUE == bNewSelect )
				m_nSelectionCount++;
			else
				m_nSelectionCount--;

			m_bSelectionModified = TRUE;
		}
	}
}

/**********************************************************************/
