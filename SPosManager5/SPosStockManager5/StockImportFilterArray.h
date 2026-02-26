/**********************************************************************/
#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

struct CStockImportFilterItem
{
public:
	CStockImportFilterItem();
	double GetNewStockLevel( int nAction );

public:
	int m_nStockIdx;
	int m_nStockSuppIdx;
	double m_dCurrentSUQty;
	double m_dImportDUQty;
	double m_dImportSUQty;
	bool m_bSelected;
	bool m_bSelectBuffer;
};

/**********************************************************************/

class CStockImportFilterArray
{
public:
	CStockImportFilterArray(void);

public:
	void AddItem( int nStockIdx, int nStockSuppIdx, double dNewDUQty, double dNewSUQty );
	bool FindItem( int nStockIdx, int nStockSuppIdx, int& nPos );

	int GetSize(){ return m_arrayItems.GetSize(); }
	void GetItem( int nIndex, CStockImportFilterItem& item );
	void SetItem( int nIndex, CStockImportFilterItem& item );

	int GetSelectionCount(){ return m_nSelectionCount; }

	void RemoveAll();

	bool IsSelectionModified(){ return m_bSelectionModified; }
	void ClearSelectionModifiedFlag(){ m_bSelectionModified = FALSE; }
	
private:
	CArray<CStockImportFilterItem,CStockImportFilterItem> m_arrayItems;
	int m_nSelectionCount;
	bool m_bSelectionModified;
};

/**********************************************************************/
#endif
/**********************************************************************/
