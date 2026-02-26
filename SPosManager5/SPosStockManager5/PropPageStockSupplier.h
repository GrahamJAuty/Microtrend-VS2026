#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageStockSupplier : public CSSPropertyPage
{
public:
	CPropPageStockSupplier();
	~CPropPageStockSupplier();

	//{{AFX_DATA(CPropPageStockSupplier)
	enum { IDD = IDD_PROPPAGE_STOCK_SUPPLIER };
	CTabCtrl m_TabSupplier;
	CStatic	m_staticSelect;
	CStatic m_staticRetail;
	CButton	m_buttonSelect;
	CEdit	m_editWrapper;
	CButton	m_buttonSupplier;
	CEdit	m_editStockCode;
	CButton	m_buttonDelUnit;
	CButton	m_checkPreferred;
	CButton	m_buttonDelete;
	CSSComboBox	m_comboSupplier;
	CSSComboBox	m_comboDeliveryUnit;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageStockSupplier)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_VIRTUAL

public:
	void SetPluRecord ( CPluCSVRecord* pPluRecord ) { m_pPluRecord = pPluRecord; }
	void AllowChangeSupplier ( bool bFlag ) { m_bAllowChangeSupplier = bFlag; }
	void AllowMakePreferred( bool bFlag ) { m_bAllowMakePreferred = bFlag; }
	void SetStockIdx ( int nStockIdx );
	void SetSuppIdx ( int nIdx, int* pIdx, int nSuppNo ); 
	void Refresh();
	bool UpdateRecord();
	void SetRecordControls();
	void SetInitialDelUnit( const char* szCase, int nDUItems, double dDUItemSize );
	void SetInitialTabOnlyFlag( bool b ){ m_bInitialTabOnly = b; }
	void SelectSupplier();

	bool GetMakePreferredFlag() { return m_bMakePreferred; }

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }

private:
	void SaveRecord();
	void GetRecordData();
	void UpdateFocus();

private:
	void FillDeliveryUnitCombo();
	void FillSupplierCombo();

private:
	CEdit* GetEditCost();
	CEdit* GetEditRetail();
	CEdit* GetEditDUItems();
	CEdit* GetEditDUItemSize();

protected:
	//{{AFX_MSG(CPropPageStockSupplier)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDeliveryUnit();
	afx_msg void OnButtonDeliveryUnit();
	afx_msg void OnButtonSupplier();
	afx_msg void OnButtonDelete();
	afx_msg void OnSelectSupplier();
	afx_msg void OnKillFocusCost();
	afx_msg void OnButtonSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nStockIdx;
	CStockCSVRecord m_StockRecord;
	CPluCSVRecord* m_pPluRecord;
	int m_nSuppIdx;

private:
	int m_nDefaultSuppNo;
	int* m_pLastSupplierTab;
	bool m_bInitialTabOnly;
		
public:
	bool m_bInUse;
	bool m_bAvailable;
	bool m_bMakePreferred;
	bool m_bAllowChangeSupplier;
	bool m_bAllowMakePreferred;
	CString m_strStockCode;
	CString m_strWrapper;
	int m_nSuppNo;
	int m_nDUItems;
	double m_dDUItemSize;
	double m_dCost;
	double m_dRetail;
	double m_dSUCost;
	double m_dTotalSU;

private:
	bool m_bReadOnly;
};

/**********************************************************************/
#endif 
/**********************************************************************/
