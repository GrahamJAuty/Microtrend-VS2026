#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#define DELUNIT_COLOUR_WHITE 15
#define DELUNIT_COLOUR_GREEN 10
#define DELUNIT_COLOUR_BLUE 9
#define DELUNIT_COLOUR_GREY 8
/**********************************************************************/
#include "OrderDatabase.h"
#include "ProcessedOrderItem.h"
#include "RepCSV.h"
/**********************************************************************/

struct CDelUnitInfo
{
public:
	CDelUnitInfo() {
		m_nComboPos = 0;
		m_nStockSuppIdx = 0;
		m_nSuppNo = 0;
		m_bEditable = FALSE;
		m_dDUCost = 0.0;
		m_dDUYield = 0.0;
		m_nDUItems = 0;
		m_dDUItemSize = 0.0;
		m_nColour = 0;
		m_strCase = "";
		m_dRetail = 0.0;
	};

public:
	int m_nComboPos;
	int m_nStockSuppIdx;
	int m_nSuppNo;
	bool m_bEditable;
	double m_dDUCost;
	double m_dDUYield;
	int m_nDUItems;
	double m_dDUItemSize;
	int m_nColour;
	CString m_strCase;
	double m_dRetail;
};

/**********************************************************************/

class CDelUnitSelector  
{
public:
	CDelUnitSelector();
	
	void FillComboBox( CSSComboBoxEx* pCombo, int nStockIdx, int nSuppNo = -2, int nStockSuppIdxToSelect = -1 );
	void GetDelUnitInfo( CDelUnitInfo& DelUnitInfo );

	void SelectStockSuppIdx( int n );
	bool SelectDelUnit( CCSVMiscStockLevels& csvIn );
	bool SelectDelUnitByInfo( CDelUnitInfo& info );
	void SelectDelUnitFromOrder( COrderCSVRecord& order );
	void SelectDelUnitFromOrder( CProcessedOrderItem& item );

private:
	void AddToCombo( const char* szCase, const char* szCombo, int nStockSuppIdx, int nSuppNo, int nDUItems, double dDUItemSize, double dYield, double dCost, double dRetail, bool bEditable, int nImage );

private:
	CImageList m_ImageList;
	CSSComboBoxEx* m_pDelUnitCombo;

	CArray<CDelUnitInfo,CDelUnitInfo> m_arrayDelUnits;
};

/**********************************************************************/
#endif
/**********************************************************************/
