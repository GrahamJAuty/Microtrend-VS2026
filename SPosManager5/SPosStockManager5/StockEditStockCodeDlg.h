#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/*********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MySSListCtrl.h"
#include "StockFilterArray.h"
/*********************************************************************/

class CStockEditStockCodeDlg : public CDialog
{
public:
	CStockEditStockCodeDlg( CSSListCtrlVirtual& StockList, CStockFilterArray* pFilterArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockEditStockCodeDlg)
	enum { IDD = IDD_STOCK_EDIT_STOCKCODE };
	CEdit	m_editNewStockCode;
	CString	m_strNewStockCode;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockEditStockCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockEditStockCodeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GetRecordData();
	bool ProcessStockCode();
	void SpinPrevious();
	void SpinNext();
	void UpdateTitle();

private:
	CStockFilterArray* m_pFilterArray;

private:
	CSSListCtrlVirtual& m_StockList;
};

/*********************************************************************/
#endif
/*********************************************************************/
