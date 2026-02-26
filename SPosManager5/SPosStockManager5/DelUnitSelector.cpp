/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "PriceHelpers.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DelUnitSelector.h"
/**********************************************************************/
extern CBitmap GlobalBitmap;
extern CBitmap GlobalMask;
/**********************************************************************/

CDelUnitSelector::CDelUnitSelector()
{
	m_pDelUnitCombo = NULL;
	m_ImageList.Create( 31, 16, ILC_COLORDDB | ILC_MASK, 0, 22 );
	m_ImageList.Add( &GlobalBitmap, &GlobalMask );
}

/**********************************************************************/

void CDelUnitSelector::FillComboBox( CSSComboBoxEx* pCombo, int nStockIdx, int nSuppNo, int nStockSuppIdxToSelect )
{
	int nDPQty = SysInfo.GetDPQty();

	m_pDelUnitCombo = pCombo;
	m_pDelUnitCombo -> ResetContent();
	m_pDelUnitCombo -> SetImageList( &m_ImageList );
	m_arrayDelUnits.RemoveAll();

	if ( nStockIdx < 0 || nStockIdx >= DataManager.Stock.GetSize() )
	{
		AddToCombo ( "Stock Unit", "Stock Unit", 0, 0, 1, 1.0, 1.0, 0.0, 0.0, FALSE, DELUNIT_COLOUR_WHITE );
		m_pDelUnitCombo -> SetCurSel(0);
		m_pDelUnitCombo -> EnableWindow( FALSE );
		return;
	}

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt( nStockIdx, StockRecord );

	CString strStockUnit = StockRecord.GetStockUnitName();
	::TrimSpaces ( strStockUnit, FALSE );	
	if ( strStockUnit == "" ) strStockUnit = "Stock Unit";

	if ( StockRecord.GetSupplierCount() >= 1 )
		AddToCombo ( strStockUnit, strStockUnit, 0, 0, 1, 1.0, 1.0, StockRecord.GetUnitCost(0), StockRecord.GetUnitRetail(0), FALSE, DELUNIT_COLOUR_WHITE );
	else
		AddToCombo ( strStockUnit, strStockUnit, 0, 0, 1, 1.0, 1.0, 0.0, 0.0, FALSE, DELUNIT_COLOUR_WHITE );
	
	//ONCE ROUND TO COLLECT ALL THE DELIVERY UNITS THAT ARE LINKED TO THIS SUPPLIER
	for ( int nStockSuppIdx = 0; nStockSuppIdx < StockRecord.GetSupplierCount(); nStockSuppIdx++ )
	{
		int nStockSuppNo = StockRecord.GetSuppNo ( nStockSuppIdx );
		if ( nSuppNo != nStockSuppNo )
			continue;
	
		CString strCase = StockRecord.GetCase( nStockSuppIdx );
		::TrimSpaces ( strCase, FALSE );
		int nDUItems = StockRecord.GetDUItems( nStockSuppIdx );
		double dDUItemSize = StockRecord.GetDUItemSize( nStockSuppIdx );
		double dYield = StockRecord.GetYield( nStockSuppIdx );		
		double dCost = StockRecord.GetCost( nStockSuppIdx );
		double dRetail = StockRecord.GetRetail( nStockSuppIdx );
		
		CString strDisplay;

		if ( strCase == "" )
			strDisplay.Format ( "%d x %.3f (%.*f)", nDUItems, dDUItemSize, nDPQty, dYield );
		else
			strDisplay.Format ( "%s (%.*f)", ( const char* ) strCase, nDPQty, dYield );

		AddToCombo ( strCase, strDisplay, nStockSuppIdx, nStockSuppNo, nDUItems, dDUItemSize, dYield, dCost, dRetail, TRUE, DELUNIT_COLOUR_GREEN );
	}
	
	//ROUND AGAIN FOR THE DELIVERY UNITS THAT ARE LINKED TO OTHER SUPPLIERS
	for ( int nStockSuppIdx = 0; nStockSuppIdx < StockRecord.GetSupplierCount(); nStockSuppIdx++ )
	{
		int nStockSuppNo = StockRecord.GetSuppNo ( nStockSuppIdx );
		if ( ( nSuppNo == nStockSuppNo ) || ( nStockSuppNo <= 0 ) )
			continue;
	
		CString strCase = StockRecord.GetCase( nStockSuppIdx );
		::TrimSpaces ( strCase, FALSE );
		int nDUItems = StockRecord.GetDUItems( nStockSuppIdx );
		double dDUItemSize = StockRecord.GetDUItemSize( nStockSuppIdx );
		double dYield = StockRecord.GetYield( nStockSuppIdx );		
		double dCost = StockRecord.GetCost( nStockSuppIdx );
		double dRetail = StockRecord.GetRetail( nStockSuppIdx );

		CString strDisplay;

		if ( strCase == "" )
			strDisplay.Format ( "%d x %.3f (%.*f)", nDUItems, dDUItemSize, nDPQty, dYield );
		else
			strDisplay.Format ( "%s (%.*f)", ( const char* ) strCase, nDPQty, dYield );

		AddToCombo ( strCase, strDisplay, nStockSuppIdx, nStockSuppNo, nDUItems, dDUItemSize, dYield, dCost, dRetail, FALSE, DELUNIT_COLOUR_BLUE );
	}
	
	//NOW ADD ALL THE ITEMS FROM THE GENERAL ARRAY
	for ( int nIndex = 0; nIndex < DataManager.DelUnit.GetSize(); nIndex++ )
		AddToCombo (
			DataManager.DelUnit.GetText( nIndex ),
			DataManager.DelUnit.GetComboText( nIndex ), 
			-1, 0, 
			DataManager.DelUnit.GetDUItems( nIndex ),
			DataManager.DelUnit.GetDUItemSize( nIndex ),
			DataManager.DelUnit.GetTotalSU( nIndex ), 
			0.0, 0.0, FALSE, DELUNIT_COLOUR_GREY );

	//FIND THE DEFAULT SELECTION AND ENABLE OR DISABLE THE COMBO
	int nSize = m_pDelUnitCombo -> GetCount();

	if ( nSize == 1 || nSuppNo < -1 )
		m_pDelUnitCombo -> SetCurSel(0);
	else if ( nSize > 1 )
	{
		CDelUnitInfo info = m_arrayDelUnits.GetAt(1);
		
		if ( info.m_nSuppNo == nSuppNo )
			m_pDelUnitCombo -> SetCurSel(1);
		else
			m_pDelUnitCombo -> SetCurSel(0);
	}

	if ( nStockSuppIdxToSelect >= 0 )
		SelectStockSuppIdx( nStockSuppIdxToSelect );
	
	m_pDelUnitCombo -> EnableWindow ( nSize > 1 );
}

/**********************************************************************/

void CDelUnitSelector::AddToCombo( const char* szCase, const char* szCombo, int nStockSuppIdx, int nSuppNo, int nDUItems, double dDUItemSize, double dYield, double dCost, double dRetail, bool bEditable, int nImage )
{
	char* p = (char*) szCombo;

	COMBOBOXEXITEM item;
	item.mask = CBEIF_DI_SETITEM | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	item.iItem = m_pDelUnitCombo -> GetCount();
	item.pszText = p;
	item.iImage = nImage;
	item.iSelectedImage = nImage;

	m_pDelUnitCombo -> InsertItem ( &item );

	CDelUnitInfo info;
	info.m_nStockSuppIdx = nStockSuppIdx;
	info.m_nSuppNo = nSuppNo;
	info.m_nDUItems = nDUItems;
	info.m_dDUItemSize = dDUItemSize;
	info.m_dDUYield = dYield;
	info.m_dDUCost = dCost;
	info.m_bEditable = bEditable;
	info.m_nColour = nImage;
	info.m_strCase = szCase;
	info.m_dRetail = dRetail;
	m_arrayDelUnits.Add ( info );
}

/**********************************************************************/

void CDelUnitSelector::GetDelUnitInfo( CDelUnitInfo& DelUnitInfo )
{
	DelUnitInfo.m_bEditable = FALSE;
	DelUnitInfo.m_nComboPos = 0;
	DelUnitInfo.m_nStockSuppIdx = -1;
	DelUnitInfo.m_nSuppNo = 0;
	DelUnitInfo.m_nDUItems = 1;
	DelUnitInfo.m_dDUItemSize = 1.0;
	DelUnitInfo.m_dDUYield = 1.0;
	DelUnitInfo.m_dDUCost = 0.0;
	DelUnitInfo.m_nColour = DELUNIT_COLOUR_WHITE;
	DelUnitInfo.m_strCase = "";
	DelUnitInfo.m_dRetail = 0.0;
	
	if ( m_pDelUnitCombo != NULL )
	{
		int nSel = m_pDelUnitCombo -> GetCurSel();
		if ( nSel >= 0 && nSel < m_arrayDelUnits.GetSize() )
		{
			CDelUnitInfo info = m_arrayDelUnits.GetAt(nSel);
			DelUnitInfo.m_nComboPos = nSel;
			DelUnitInfo.m_bEditable = info.m_bEditable;
			DelUnitInfo.m_nStockSuppIdx = info.m_nStockSuppIdx;
			DelUnitInfo.m_nSuppNo = info.m_nSuppNo;
			DelUnitInfo.m_nDUItems = info.m_nDUItems;
			DelUnitInfo.m_dDUItemSize = info.m_dDUItemSize;
			DelUnitInfo.m_dDUYield = info.m_dDUYield;
			DelUnitInfo.m_dDUCost = info.m_dDUCost;
			DelUnitInfo.m_nColour = info.m_nColour;
			DelUnitInfo.m_strCase = info.m_strCase;
			DelUnitInfo.m_dRetail = info.m_dRetail;
		}
	}
}

/**********************************************************************/

void CDelUnitSelector::SelectStockSuppIdx( int n )
{
	if ( m_pDelUnitCombo != NULL )
	{
		//AVOID SELECTING STOCK UNIT WHEN STOCK SUPP IDX 0 IS PASSED
		for ( int nTest = 1; nTest < m_pDelUnitCombo -> GetCount(); nTest++ )
		{
			if ( m_arrayDelUnits[nTest].m_nStockSuppIdx == n )
			{
				m_pDelUnitCombo -> SetCurSel( nTest );
				return;
			}
		}
	}
}

/**********************************************************************/

//FUNCTION USED WHEN EDITING EXISTING DELIVERY AND RETURN LINES
bool CDelUnitSelector::SelectDelUnit( CCSVMiscStockLevels& csvIn )
{
	if ( m_pDelUnitCombo == NULL )
		return FALSE;

	if ( csvIn.GetUseDUFlag() == FALSE )
	{
		m_pDelUnitCombo -> SetCurSel(0);
		return TRUE;
	}
	else
	{
		int nDUItems = csvIn.GetDUItems();
		double dDUItemSize = csvIn.GetDUItemSize();
		int nSuppNo = csvIn.GetSuppNo();

		for ( int n = 1; n < m_pDelUnitCombo -> GetCount(); n++ )
		{
			CDelUnitInfo info = m_arrayDelUnits.GetAt(n);
 
			if ( ( nDUItems == info.m_nDUItems ) && ( nSuppNo == info.m_nSuppNo ) && ( CPriceHelpers::CompareDoubles( dDUItemSize, info.m_dDUItemSize ,3 ) == 0 ) ) 
			{
				m_pDelUnitCombo -> SetCurSel(n);
				return TRUE;
			}
		}
		
		m_pDelUnitCombo -> SetCurSel(0);
		return FALSE;
	}
}

/**********************************************************************/

bool CDelUnitSelector::SelectDelUnitByInfo( CDelUnitInfo& info )
{
	if ( m_pDelUnitCombo == NULL )
		return FALSE;

	for ( int n = 1; n < m_pDelUnitCombo -> GetCount(); n++ )
	{
		CDelUnitInfo infoArray = m_arrayDelUnits.GetAt(n);
 
		if ( ( info.m_nDUItems == infoArray.m_nDUItems ) && 
			( info.m_nSuppNo == infoArray.m_nSuppNo ) && 
			( info.m_nColour == infoArray.m_nColour ) &&
			( CPriceHelpers::CompareDoubles( info.m_dDUItemSize, infoArray.m_dDUItemSize ,3 ) == 0 ) ) 
		{
			m_pDelUnitCombo -> SetCurSel(n);
			return TRUE;
		}
	}
		
	m_pDelUnitCombo -> SetCurSel(0);
	return FALSE;
}

/**********************************************************************/

void CDelUnitSelector::SelectDelUnitFromOrder( COrderCSVRecord& order )
{
	int nDPQty = SysInfo.GetDPQty();

	int nDUItems = order.GetDUItems();
	double dDUItemSize = order.GetDUItemSize();
	double dDUCost = order.GetDUCost();
	CString strCase = order.GetCase();

	for ( int n = 1; n < m_pDelUnitCombo -> GetCount(); n++ )
	{
		CDelUnitInfo info = m_arrayDelUnits.GetAt(n);
 
		if ( nDUItems == info.m_nDUItems )
		{
			if ( CPriceHelpers::CompareDoubles( dDUItemSize, info.m_dDUItemSize, 3 ) == 0 )
			{
				if ( strCase == info.m_strCase )
				{
					if ( info.m_nColour != DELUNIT_COLOUR_GREEN )
					{
						m_pDelUnitCombo -> SetCurSel(n);
						return;
					}
					else if ( CPriceHelpers::CompareDoubles( dDUCost, info.m_dDUCost , 3 ) == 0 )
					{
						m_pDelUnitCombo -> SetCurSel(n);
						return;
					}
				}
			}
		}
	}

	double dYield = dDUItemSize * nDUItems;
	CString strDisplay;

	if ( strCase == "" )
		strDisplay.Format ( "%d x %.3f (%.*f)", nDUItems, dDUItemSize, nDPQty, dYield );
	else
		strDisplay.Format ( "%s (%.*f)", ( const char* ) strCase, nDPQty, dYield );
	
	AddToCombo (	strCase, 
				strDisplay,
				-1, 0,
				nDUItems,
				dDUItemSize,
				dYield,
				0.0, 0.0, FALSE, DELUNIT_COLOUR_GREY );

	m_pDelUnitCombo -> SetCurSel( m_pDelUnitCombo -> GetCount() - 1 );
}

/**********************************************************************/

void CDelUnitSelector::SelectDelUnitFromOrder( CProcessedOrderItem& item )
{
	int nDPQty = SysInfo.GetDPQty();

	int nDUItems = item.GetDUItems();
	double dDUItemSize = item.GetDUItemSize();
	double dDUCost = item.GetDUCost();
	CString strCase = item.GetCase();

	for ( int n = 1; n < m_pDelUnitCombo -> GetCount(); n++ )
	{
		CDelUnitInfo info = m_arrayDelUnits.GetAt(n);
 
		if ( nDUItems == info.m_nDUItems )
		{
			if ( CPriceHelpers::CompareDoubles( dDUItemSize, info.m_dDUItemSize, 3 ) == 0 )
			{
				if ( strCase == info.m_strCase )
				{
					if ( info.m_nColour != DELUNIT_COLOUR_GREEN )
					{
						m_pDelUnitCombo -> SetCurSel(n);
						return;
					}
					else if ( CPriceHelpers::CompareDoubles( dDUCost, info.m_dDUCost , 3 ) == 0 )
					{
						m_pDelUnitCombo -> SetCurSel(n);
						return;
					}
				}
			}
		}
	}

	double dYield = dDUItemSize * nDUItems;
	CString strDisplay;

	if ( strCase == "" )
		strDisplay.Format ( "%d x %.3f (%.*f)", nDUItems, dDUItemSize, nDPQty, dYield );
	else
		strDisplay.Format ( "%s (%.*f)", ( const char* ) strCase, nDPQty, dYield );
	
	AddToCombo (	strCase, 
				strDisplay,
				-1, 0,
				nDUItems,
				dDUItemSize,
				dYield,
				0.0, 0.0, FALSE, DELUNIT_COLOUR_GREY );

	m_pDelUnitCombo -> SetCurSel( m_pDelUnitCombo -> GetCount() - 1 );
}

/**********************************************************************/
