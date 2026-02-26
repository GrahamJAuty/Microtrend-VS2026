#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategorySetCSVArray.h"
#include "StocktakeCSVArray.h"
/**********************************************************************/

class CStocktakeDlg : public CSSDialog
{
public:
	CStocktakeDlg( CArray<int,int>& arrayEntityIdx, CWnd* pParent = NULL); 

	//{{AFX_DATA(CStocktakeDlg)
	enum { IDD = IDD_STOCKTAKE };
	CSSListCtrl m_listStockpoint;
	CSSListCtrlVirtual m_listStocktakes;
	CButton	m_buttonAbandon;
	CButton	m_buttonStockmate;
	CButton m_buttonAction1;	//new
	CButton m_buttonAction2;	//edit
	CButton m_buttonAction3;	//levels
	CButton m_buttonAction4;	//reports
	CButton m_buttonAction5;	//finalise
	CButton m_buttonCancel;		//cancel	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStocktakeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStocktakeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSelectStockpoint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAbandon();
	afx_msg void OnButtonStockmate();
	afx_msg void OnButtonAction1();
	afx_msg void OnButtonAction2();
	afx_msg void OnButtonAction3();
	afx_msg void OnButtonAction4();
	afx_msg void OnButtonAction5();
	afx_msg void OnPaint();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	static int BeginStocktake(int nStocktakeNo, CCategorySetCSVRecord& CategorySet, CWordArray& arraySuppIdx, bool bActiveOnly, bool bClosingStockZero, bool bDoTransfers, int nSpIdx);

private:
	int DatabaseAction(int nAction, CDataManagerInfo& infoDM, bool bLock );
	void HandleNew();
	void HandleEdit();
	void HandleLevels();
	void HandleReports();
	void HandleFinalise();
	void HandleAbandon();
	void HandleClosing();
	void HandleDeliveries();
	void HandleReturns();
	void HandleAdjustmentPlu();
	void HandleAdjustmentStock();
	void HandleSalesPlu();
	void HandleSalesStock();
	void HandleStockmate();
	
private:
	void SelectStockpointLine( int nLineIdx );

private:
	void EditMiscStockLevels( int nType );

private:
	void RebuildStocktakes();

private:
	void UpdateStocktakeList();
	
private:
	int m_nDbIdx;
	int m_nSpIdx;
	int m_nLastUpdateSpIdx;
	int m_nFirstUpdateState;
	CString m_strDate;

private:
	CArray<int,int>& m_arrayEntityIdx;
	CArray<CLocationSelectorEntity,CLocationSelectorEntity> m_arrayStockpoint;
	CStocktakeCSVArray m_StocktakeArray;
	
private:
	bool m_bLogDate;
	int m_nCacheIndex;
	CCSV m_CacheCSV;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
